#ifndef FuncionesEeprom_H
#define FuncionesEeprom_H

#include <avr/io.h>
void EPROM_Write(uint16_t uiAddress, uint8_t ucData);
void EPROM_Write_String(uint16_t uiAddress, const char* string);
void EPROM_Read_String(uint16_t uiAddress, char* buffer, uint16_t length);

#endif 