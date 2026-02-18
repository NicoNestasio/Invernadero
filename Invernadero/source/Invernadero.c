#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_Systick_Delay.h"
#include "fsl_i2c.h"
#include "fsl_swm.h"
#include "fsl_SSD1306_I2C.h"
#include "teclado.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_sctimer.h"
#include "fsl_iap.h"

// --- Configuración de Sensores (CANALES CORREGIDOS) ---
#define ADC_LUZ_CANAL    2    // PIO0_14
#define ADC_AGUA_CANAL   0    // PIO0_7
#define ADC_HUM_CANAL    10   // PIO0_13
#define ADC_TEMP_CANAL   1    // PIO0_06

// --- Configuración de Actuadores ---
#define MOTOR_PIN        19   // Ventilador
#define BOMBA_PIN        16   // Bomba
#define SERVO_PIN        15   // Servo ventana
#define LED_LUZ_PIN      1    // LED

// --- Parámetros de Calibración ---
#define RESISTENCIA_FIJA     10000.0f
#define TERMISTOR_NOMINAL    100000.0f
#define BETA_COEFFICIENT     3950.0f
#define CORRECCION_OFFSET    33.0f

#define LIMITE_AGUA_CRITICO 1000
#define BLINK_INTERVAL      400
#define MSG_INTERVAL        3000
#define SENSOR_INTERVAL     3000

// --- CONFIGURACIÓN DE MEMORIA FLASH ---
#define FLASH_SECTOR_STORAGE        30
#define FLASH_ADDRESS_STORAGE       0x00007800
#define FRECUENCIA_CORE_FLASH       24000000
#define FLASH_MAGIC_KEY             0xA1B2C3D4

// Estructura para guardar configuración
typedef struct {
    uint32_t saved_luz;
    uint32_t saved_hum;
    float    saved_temp;
    uint32_t magic_key;
} GreenhouseConfig;

// --- Variables Globales ---
uint32_t limite_luz   = 500;
uint32_t limite_hum   = 30;
float    limite_temp  = 30.0f;

uint32_t val_luz_raw = 0, val_hum_raw = 0, val_agua_raw = 0, val_temp_raw = 0;
uint32_t luz_lumens = 0, hum_porcentaje = 0;
float temperatura_c = 0;
bool blink_state = false;
bool alerta_agua = false;
uint32_t event_servo;
uint32_t last_sensor = 0;

// Flag de estado de la ventana
uint8_t v = 0;

// --- Prototipos ---
void InicializarHardware(void);
void ADC_Configuration(uint32_t chs_mask);
void LeerSensores(void);
void ProcesarAlertas(uint32_t now, uint32_t *last_msg);
void ActualizarPantallaPrincipal(void);
uint32_t MenuIngresoNumerico(const char* titulo, const char* unidad, uint8_t max_p, uint32_t max_v);
float CalcularTemperatura(uint32_t adc_val);
void MoverServo(uint8_t grados);
void GestionarMenu(void);
void CargarConfiguracionFlash(void);
void GuardarConfiguracionFlash(void);

// ================= MAIN =================

int main(void) {
    InicializarHardware();

    CargarConfiguracionFlash();
    MoverServo(0);
    v = 0;

    uint32_t last_blink = 0;
    uint32_t last_msg = 0;
    last_sensor = 0;

    PRINTF("SISTEMA INVERNADERO v5.1 - CANALES INVERTIDOS\r\n");

    while (1) {
        GestionarMenu();
        uint32_t now = Return_Tick_Value();

        if (now - last_sensor >= SENSOR_INTERVAL) {
            LeerSensores();
            ProcesarAlertas(now, &last_msg);
            ActualizarPantallaPrincipal();
            last_sensor = now;
        }

        if (now - last_blink >= BLINK_INTERVAL) {
            blink_state = !blink_state;
            last_blink = now;
            if (alerta_agua) ActualizarPantallaPrincipal();
        }

        Ticks_Delay(10);
    }
}

// ================= FUNCIONES =================

void GestionarMenu(void) {
    uint8_t tecla = Teclado_Scan();

    if (tecla == '#') {
        while(Teclado_Scan() != 0);

        OLED_Clear();
        OLED_Set_Text(15, 0, kOLED_Pixel_Set, (char *)"CONFIGURACION", 1);
        OLED_Set_Text(0, 15, kOLED_Pixel_Set, (char *)"1) Lim. Luz (Lm)", 1);
        OLED_Set_Text(0, 25, kOLED_Pixel_Set, (char *)"2) Lim. Hum (%)", 1);
        OLED_Set_Text(0, 35, kOLED_Pixel_Set, (char *)"3) Lim. Temp (C)", 1);
        OLED_Refresh();

        uint8_t opcion = 0;
        while((opcion = Teclado_Scan()) == 0);
        while(Teclado_Scan() != 0);

        bool cambio_realizado = false;

        if (opcion == '1') {
            uint32_t val = MenuIngresoNumerico("LIMITE LUZ", "Lm", 4, 1000);
            if(val > 0) { limite_luz = val; cambio_realizado = true; }
        } else if (opcion == '2') {
            uint32_t val = MenuIngresoNumerico("LIMITE HUMEDAD", "%", 3, 100);
            if(val > 0) { limite_hum = val; cambio_realizado = true; }
        } else if (opcion == '3') {
            uint32_t val = MenuIngresoNumerico("LIMITE TEMP", "C", 2, 50);
            if (val > 0) { limite_temp = (float)val; cambio_realizado = true; }
        }

        if (cambio_realizado) {
            GuardarConfiguracionFlash();
            OLED_Clear();
            OLED_Set_Text(10, 20, kOLED_Pixel_Set, (char*)"GUARDADO OK!", 1);
            OLED_Refresh();
            Ticks_Delay(1000);
        }

        last_sensor = Return_Tick_Value();
        OLED_Clear();
    }
}

uint32_t MenuIngresoNumerico(const char* titulo, const char* unidad, uint8_t max_p, uint32_t max_v) {
    char input_buf[11] = "";
    uint8_t p = 0;
    char display[20];
    OLED_Clear();
    OLED_Set_Text(0, 0, kOLED_Pixel_Set, (char*)titulo, 1);
    OLED_Refresh();

    while(1) {
        uint8_t t = Teclado_Scan();
        if (t != 0) {
            if (t >= '0' && t <= '9' && p < max_p) {
                input_buf[p++] = t;
                input_buf[p] = '\0';
                OLED_Clear();
                OLED_Set_Text(0, 0, kOLED_Pixel_Set, (char*)titulo, 1);
                sprintf(display, "%s %s", input_buf, unidad);
                OLED_Set_Text(0, 16, kOLED_Pixel_Set, display, 2);
                OLED_Refresh();
                while(Teclado_Scan() != 0);
            }
            else if (t == '#') {
                while(Teclado_Scan() != 0);
                if (p == 0) return 0;
                uint32_t res = (uint32_t)atoi(input_buf);
                return (res > max_v) ? max_v : res;
            }
            else if (t == '*') {
                if (p > 0) {
                    input_buf[--p] = '\0';
                    OLED_Clear();
                    OLED_Set_Text(0, 0, kOLED_Pixel_Set, (char*)titulo, 1);
                    sprintf(display, "%s %s", input_buf, unidad);
                    OLED_Set_Text(0, 16, kOLED_Pixel_Set, display, 2);
                    OLED_Refresh();
                    while(Teclado_Scan() != 0);
                }
            }
        }
        Ticks_Delay(50);
    }
}

void InicializarHardware(void) {
    BOARD_InitDebugConsole();
    Systick_Init(SystemCoreClock / 1000);

    i2c_master_config_t i2config;
    CLOCK_Select(kI2C1_Clk_From_MainClk);
    CLOCK_EnableClock(kCLOCK_Swm);

    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_23);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_22);
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT0, kSWM_PortPin_P0_15);

    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN2, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN10, true);
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN1, true);

    SYSCON->SYSAHBCLKCTRL0 |= (1 << 13);
    IOCON->PIO[30] &= ~(3 << 3);

    CLOCK_DisableClock(kCLOCK_Swm);

    I2C_MasterGetDefaultConfig(&i2config);
    i2config.baudRate_Bps = 400000;
    I2C_MasterInit(I2C1, &i2config, 12000000);

    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    ADC_DoSelfCalibration(ADC0, CLOCK_GetFreq(kCLOCK_Fro));
    ADC_Configuration((1 << ADC_LUZ_CANAL) | (1 << ADC_HUM_CANAL) | (1 << ADC_AGUA_CANAL) | (1 << ADC_TEMP_CANAL));

    GPIO_PortInit(GPIO, 0);
    gpio_pin_config_t out_cfg = {kGPIO_DigitalOutput, 0};
    GPIO_PinInit(GPIO, 0, MOTOR_PIN, &out_cfg);
    GPIO_PinInit(GPIO, 0, BOMBA_PIN, &out_cfg);
    GPIO_PinInit(GPIO, 0, LED_LUZ_PIN, &out_cfg);

    sctimer_config_t sct_cfg;
    SCTIMER_GetDefaultConfig(&sct_cfg);
    SCTIMER_Init(SCT0, &sct_cfg);
    sctimer_pwm_signal_param_t pwm_p = {kSCTIMER_Out_0, kSCTIMER_HighTrue, 7};
    SCTIMER_SetupPwm(SCT0, &pwm_p, kSCTIMER_EdgeAlignedPwm, 50, CLOCK_GetFreq(kCLOCK_Fro), &event_servo);
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);

    OLED_Init();
    Teclado_Init();
}

void LeerSensores(void) {
    adc_result_info_t res;
    ADC_DoSoftwareTriggerConvSeqA(ADC0);

    while (!ADC_GetChannelConversionResult(ADC0, ADC_LUZ_CANAL, &res));
    val_luz_raw = res.result;
    while (!ADC_GetChannelConversionResult(ADC0, ADC_HUM_CANAL, &res));
    val_hum_raw = res.result;
    while (!ADC_GetChannelConversionResult(ADC0, ADC_AGUA_CANAL, &res));
    val_agua_raw = res.result;
    while (!ADC_GetChannelConversionResult(ADC0, ADC_TEMP_CANAL, &res));
    val_temp_raw = res.result;

    luz_lumens = (4095 - val_luz_raw) * 1000 / 4095;
    hum_porcentaje = (4095 - val_hum_raw) * 100 / 4095;
    temperatura_c = CalcularTemperatura(val_temp_raw);
    alerta_agua = (val_agua_raw < LIMITE_AGUA_CRITICO);
}

float CalcularTemperatura(uint32_t adc_val) {
    if (adc_val < 10) return -99.9f;
    float r = RESISTENCIA_FIJA * ((4095.0f / (float)adc_val) - 1.0f);
    float t = 1.0f / (logf(r / TERMISTOR_NOMINAL) / BETA_COEFFICIENT + 1.0f / 298.15f) - 273.15f;
    return t - CORRECCION_OFFSET;
}

void MoverServo(uint8_t grados) {
    if (grados > 180) grados = 180;
    uint32_t duty = 3 + ((180 - (uint32_t)grados) * 9 / 180);
    SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_0, duty, event_servo);
}

void ProcesarAlertas(uint32_t now, uint32_t *last_msg) {
    GPIO_PinWrite(GPIO, 0, LED_LUZ_PIN, (luz_lumens < limite_luz) ? 1 : 0);

    bool riego_on = (hum_porcentaje < limite_hum && !alerta_agua);
    GPIO_PinWrite(GPIO, 0, BOMBA_PIN, riego_on);

    bool calor = (temperatura_c > limite_temp);
    bool humedad_alta = (hum_porcentaje > 80);

    if (calor || humedad_alta) {
        if (v == 0) {
            MoverServo(180);
            v = 1;
        }
        GPIO_PinWrite(GPIO, 0, MOTOR_PIN, 1);
    } else {
        if (v == 1) {
            MoverServo(0);
            v = 0;
        }
        GPIO_PinWrite(GPIO, 0, MOTOR_PIN, 0);
    }

    if (now - *last_msg > MSG_INTERVAL) {
        int t_i = (int)temperatura_c, t_d = abs((int)((temperatura_c - (float)t_i) * 10));
        PRINTF("LOG -> T:%d.%d C | L:%lu | H:%lu%% | A:%lu | V:%d\r\n", t_i, t_d, luz_lumens, hum_porcentaje, val_agua_raw, v);
        *last_msg = now;
    }
}

void ActualizarPantallaPrincipal(void) {
    OLED_Clear();
    char line[21];
    int t_i = (int)temperatura_c, t_d = abs((int)((temperatura_c - (float)t_i) * 10));

    OLED_Set_Text(10, 0, kOLED_Pixel_Set, (char*)"--- MONITOR ---", 1);
    sprintf(line, "T: %d.%d C / %d C", t_i, t_d, (int)limite_temp);
    OLED_Set_Text(0, 11, kOLED_Pixel_Set, line, 1);
    sprintf(line, "Luz: %4lu / %lu Lm", luz_lumens, limite_luz);
    OLED_Set_Text(0, 20, kOLED_Pixel_Set, line, 1);
    sprintf(line, "Hum: %3lu%% / %lu%%", hum_porcentaje, limite_hum);
    OLED_Set_Text(0, 29, kOLED_Pixel_Set, line, 1);

    if (alerta_agua && blink_state) {
        OLED_Set_Text(5, 38, kOLED_Pixel_Set, (char*)"!! TANQUE VACIO !!", 1);
    } else if (!alerta_agua) {
        OLED_Set_Text(20, 38, kOLED_Pixel_Set, (char*)"Nivel Agua OK", 1);
    }
    OLED_Refresh();
}

void ADC_Configuration(uint32_t chs_mask) {
    adc_config_t cfg;
    adc_conv_seq_config_t seq;
    ADC_GetDefaultConfig(&cfg);
    cfg.clockMode = kADC_ClockSynchronousMode;
    cfg.clockDividerNumber = 1;
    cfg.enableLowPowerMode = false;
    cfg.voltageRange = kADC_HighVoltageRange;
    ADC_Init(ADC0, &cfg);
    seq.channelMask = chs_mask;
    seq.triggerMask = 0;
    seq.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
    seq.enableSingleStep = false;
    seq.enableSyncBypass = false;
    seq.interruptMode = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(ADC0, &seq);
    ADC_EnableConvSeqA(ADC0, true);
}

// ================= MEMORIA FLASH =================

void GuardarConfiguracionFlash(void) {
    GreenhouseConfig config;
    config.saved_luz = limite_luz;
    config.saved_hum = limite_hum;
    config.saved_temp = limite_temp;
    config.magic_key = FLASH_MAGIC_KEY;

    uint32_t buffer_escritura[16];
    memset(buffer_escritura, 0, sizeof(buffer_escritura));
    memcpy(buffer_escritura, &config, sizeof(GreenhouseConfig));

    PRINTF("Guardando en Flash...\r\n");
    __disable_irq();

    if (IAP_PrepareSectorForWrite(FLASH_SECTOR_STORAGE, FLASH_SECTOR_STORAGE) == kStatus_IAP_Success) {
        if (IAP_EraseSector(FLASH_SECTOR_STORAGE, FLASH_SECTOR_STORAGE, FRECUENCIA_CORE_FLASH) == kStatus_IAP_Success) {
            IAP_PrepareSectorForWrite(FLASH_SECTOR_STORAGE, FLASH_SECTOR_STORAGE);
            IAP_CopyRamToFlash(FLASH_ADDRESS_STORAGE, (uint32_t *)buffer_escritura, 64, FRECUENCIA_CORE_FLASH);
        }
    }

    __enable_irq();
}

void CargarConfiguracionFlash(void) {
    GreenhouseConfig *config_leida = (GreenhouseConfig *)FLASH_ADDRESS_STORAGE;
    if (config_leida->magic_key == FLASH_MAGIC_KEY) {
        limite_luz = config_leida->saved_luz;
        limite_hum = config_leida->saved_hum;
        limite_temp = config_leida->saved_temp;
        PRINTF("Flash OK. L:%lu H:%lu T:%.1f\r\n", limite_luz, limite_hum, limite_temp);
    } else {
        PRINTF("Flash vacia. Usando defaults.\r\n");
    }
}
