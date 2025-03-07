
#ifndef UART_H_
#define UART_H_

//#define F_CPU 16000000
#include <avr/io.h>
;;;;;;;
void USART_Transmit(uint8_t data);
unsigned char UART_receive_non_blocking();
//void MenuUart();
void uart_init(void);
void uart_send_char(char c);
void Bienvenida(void);
void MenuInicial(void);
void uart_send_string(const char* str);
void uart_send_newline();
char echo_serial();
char uart_receive_char();
void Carpy();
char echo_serialMenu();
void MenuMediciones(int TemperaturaActualEntero,int TemperaturaActualDecimal,int SetPointEntero,int SetPointDecimal,int Ventana, int PorcentajePWMActual);
void MenuCambiarVolumenBidon(void);
void MenuConfiguraciones(void);
void MandarStringdesdePrograma(const char *str);
void MenuCambioDeNombre(void);
void Fracaso(void);
void CambioDeNumero(void);
char uart_receive_charNoBloqueante(void);
char echo_serialNobloqueante(int Bandera1,int Bandera3,int Bandera2);
void MenuTamanho(void);
void uart_clear_screen();
#include <util/delay.h>
#include <stdio.h>

#endif /* UART_H_ */