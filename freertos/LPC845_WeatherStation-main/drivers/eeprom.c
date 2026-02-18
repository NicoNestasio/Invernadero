#include "fsl_common.h"
#include "fsl_i2c.h"
#include "eeprom.h"
#include "fsl_iap.h"
#include "fsl_Systick_Delay.h"

datalogger_info *flash_data = (datalogger_info *)FLASH_ADDRESS;
datalogger_info ram_data;


void eeprom_Page_Write(uint32_t sub_addres, uint8_t *data, size_t size){
	i2c_master_transfer_t xfer = {0};

	xfer.data = data;
	xfer.dataSize = size;
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = EEPROM_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = sub_addres;
	xfer.subaddressSize = 2;

	I2C_MasterTransferBlocking(I2C1, &xfer);
}

void eeprom_Sequential_Random_Read(uint32_t sub_addres, uint8_t *data, size_t size){
	i2c_master_transfer_t xfer = {0};

	xfer.data = data;
	xfer.dataSize = size;
	xfer.flags = kI2C_TransferRepeatedStartFlag;
	xfer.slaveAddress = EEPROM_ADDRESS;
	xfer.direction = kI2C_Read;
	xfer.subaddress = sub_addres;
	xfer.subaddressSize = 2;

	I2C_MasterTransferBlocking(I2C1, &xfer);
}


void eeprom_write(uint32_t start_address, uint8_t *data, size_t size) {
    size_t bytes_written = 0;       // Cantidad de bytes que ya se han escrito
    size_t bytes_to_write = size;   // Cantidad total de bytes que se deben escribir

    while (bytes_to_write > 0) {
        // Comprobar si la dirección actual excede el tamaño total de la EEPROM
        if (start_address >= EEPROM_SIZE) {
            start_address = 0;  // Reiniciar al comienzo de la EEPROM
        }

        // Calcular la dirección actual y el límite de la página
        uint32_t offset_within_page = start_address % EEPROM_PAGE_SIZE;
        size_t available_space_in_page = EEPROM_PAGE_SIZE - offset_within_page;

        // Calcular el tamaño de la escritura actual
        size_t current_write_size = (bytes_to_write < available_space_in_page) ? bytes_to_write : available_space_in_page;

        // Escribir los datos en la EEPROM utilizando la función existente
        eeprom_Page_Write(start_address, &data[bytes_written], current_write_size);

        // Actualizar contadores y dirección de inicio
        bytes_written += current_write_size;
        bytes_to_write -= current_write_size;
        start_address += current_write_size;  // Incrementar la dirección de inicio para la próxima escritura

        if (current_write_size == available_space_in_page && bytes_to_write > 0) {
            Ticks_Delay(EEPROM_WRITE_DELAY_MS);  // Delay para poder hacer una nueva escritura en la pagina siguiente
        }
    }
}

status_t guardar_en_flash(datalogger_info *data) {
    uint8_t buffer[FLASH_PAGE_SIZE]; // Buffer para almacenar los datos antes de escribir en la Flash

    // Copia los datos de la estructura al buffer
    memcpy(buffer, data, sizeof(datalogger_info));

    IAP_PrepareSectorForWrite(SECTOR, SECTOR);
    IAP_EraseSector(SECTOR, SECTOR, SystemCoreClock);

    IAP_PrepareSectorForWrite(SECTOR, SECTOR);

    // Escribir los datos del buffer en la memoria Flash
    status_t status = IAP_CopyRamToFlash(FLASH_ADDRESS, (uint32_t *)buffer, FLASH_PAGE_SIZE, SystemCoreClock);
    return status;
}

void actualizar_direcciones(datalogger_info* ram_data, datalogger_info* flash_data, size_t buffer_size, size_t eeprom_size) {
    // Actualizar endAddress en ram_data
    ram_data->endAddress = (flash_data->endAddress + buffer_size) % eeprom_size;

    //Posible nuevo start address
    uint32_t nextStartAddress = (flash_data->startAddress + buffer_size) % eeprom_size;


    // Verificar si endAddress ha alcanzado o superado startAddress
    if (nextStartAddress > ram_data->endAddress) {
        ram_data->startAddress = nextStartAddress;		// Mover startAddress
    } else {
        ram_data->startAddress = flash_data->startAddress;
    }

    // Actualizar horas y minutos
    ram_data->hours = flash_data->hours;
    ram_data->minutes = flash_data->minutes;
}
