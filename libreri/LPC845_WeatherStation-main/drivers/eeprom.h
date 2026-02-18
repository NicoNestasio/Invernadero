#ifndef EEPROM_H_
#define EEPROM_H_


#define EEPROM_ADDRESS	0x50	// Address del AT24C256 (| 1 | 0 | 1 | 0 | 0 | A1 | A0 |)
#define EEPROM_SIZE		32768
#define EEPROM_PAGE_SIZE 64
#define EEPROM_WRITE_DELAY_MS 5  //Tiempo de espera en milisegundos para pasar a la pagina siguiente
#define FLASH_PAGE_SIZE 64 // Tamaño de página en bytes (Flash)
#define FLASH_ADDRESS 0x00007000 // Dirección de la Flash donde se guardarán los datos
#define SECTOR 28 // Sector de la Flash a utilizar

typedef struct {
    uint32_t startAddress;
    uint32_t endAddress;
    uint8_t hours;
    uint8_t minutes;
} datalogger_info;

extern datalogger_info *flash_data;
extern datalogger_info ram_data;

/*!
 * @brief Escribe una página en la EEPROM.
 *
 * Esta función escribe una página de datos en la EEPROM en la subdirección especificada.
 * No puede ser mas de 64 bytes por operación.
 *
 * @param sub_address La subdirección en la EEPROM donde se escribirán los datos.
 * @param data Puntero a los datos que se van a escribir.
 * @param size El número de bytes a escribir.
 */
void eeprom_Page_Write(uint32_t sub_addres, uint8_t *data, size_t size);

/*!
 * @brief Escribe datos en la EEPROM manejando automáticamente el cambio de página.
 *
 * Esta función escribe un bloque de datos en la EEPROM a partir de la dirección de inicio especificada.
 * Si los datos exceden el límite de la página actual, la función continuará escribiendo
 * en la siguiente página hasta que todos los datos hayan sido escritos. Si se alcanza el final de la memoria,
 * la escritura continuará desde el inicio (dirección 0).
 *
 * @param start_address La dirección de inicio en la EEPROM donde se comenzará a escribir.
 * @param data Puntero al buffer de datos que se van a escribir.
 * @param size El número total de bytes a escribir en la EEPROM.
 */
void eeprom_write(uint32_t start_address, uint8_t *data, size_t size);


/*!
 * @brief Lee datos secuencialmente de la EEPROM.
 *
 * Esta función lee datos de la EEPROM comenzando desde la subdirección especificada.
 *
 * @param sub_address La subdirección en la EEPROM donde comenzará la operación de lectura.
 * @param data Puntero al buffer donde se almacenarán los datos leídos.
 * @param size El número de bytes a leer.
 */
void eeprom_Sequential_Random_Read(uint32_t sub_addres, uint8_t *data, size_t size);

/**
 * @brief Guarda la estructura datalogger_info en la memoria Flash.
 *
 * @param data Puntero a la estructura datalogger_info que se desea guardar.
 * @return status_t Retorna el estado de la operación.
 */
status_t guardar_en_flash(datalogger_info *data);

/*!
 * @brief Actualiza las direcciones de inicio y fin en la estructura de RAM considerando el tamaño del buffer escrito y la capacidad total de la EEPROM.
 *
 * Esta función actualiza las direcciones de inicio (`startAddress`) y fin (`endAddress`) dentro de la estructura `datalogger_info` en RAM,
 * basándose en los datos almacenados en la estructura de Flash y en el tamaño del buffer recientemente escrito en la EEPROM.
 * Además, sincroniza los valores de horas y minutos entre ambas estructuras, preservando la información horaria almacenada en Flash.
 *
 * El cálculo de las direcciones se realiza de manera circular, considerando el tamaño total de la EEPROM para evitar desbordamientos.
 *
 * @param ram_data Puntero a la estructura `datalogger_info` almacenada en RAM.
 * @param flash_data Puntero a la estructura `datalogger_info` almacenada en la memoria Flash.
 * @param buffer_size Tamaño del bloque de datos que se ha escrito en la EEPROM.
 * @param eeprom_size Tamaño total de la EEPROM, utilizado para realizar cálculos de direcciones de manera circular.
 */
void actualizar_direcciones(datalogger_info* ram_data, datalogger_info* flash_data, size_t buffer_size, size_t eeprom_size);

#endif /* EEPROM_H_ */
