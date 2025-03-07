// El PWM funciona con Fast PWM en Timer0 y tiene una frecuencia de aproximadamente 7.8 kHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include "FuncionesEeprom.h"
#include "UART.h"
#include <stdio.h>
#define PWM_MAX 255   // Máximo valor(255 es 100%)
#define PWM_MIN 0     // 0%

volatile int Disparos = 0;
volatile long TemperaturaActual = 0; //En mili°C
volatile long SetPoint = 75000; // SetPoint en mili°C, por defecto 75°C   >> Por ahora, luego si hay tiempo haremos que sea permanente la configuración
volatile long Ventana = 1000; // Ventana en mili°C, por defecto 1°C		  >> Por ahora, luego si hay tiempo haremos que sea permanente la configuración
volatile int  PorcentajePWM=0;
volatile int  Refrescar=0;   // esto es una bandera de refrescar pantalla de mediciones automaticamente
volatile   int  Contador=0;
void setupPWM() {
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);  // Fast PWM, no inversión
	TCCR0B |= (1 << CS01);  // Prescaler 8 para una frecuencia adecuada
	OCR0A = PWM_MIN;        // Inicio en 0%
	DDRD |= (1 << PD6);     // Salida PWM en OC0A (PD6)
}

void setupTimer1() {
	TCCR1B |= (1 << WGM12) | (1 << CS11); // CTC, Prescaler 8
	OCR1A = 3333;  // 3.333 ms (ajustar según frecuencia de reloj)
	TIMSK1 |= (1 << OCIE1A); // Habilitar interrupción
}

void setupINT0() {
	EICRA |= (1 << ISC00);  // Interrupción en cualquier cambio
	EIMSK |= (1 << INT0);   // Habilitar INT0
}

void setupADC() {
	ADMUX |= (1 << REFS0); // Referencia AVcc, ADC0 seleccionado
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1); // Habilitar ADC y prescaler 64
}

long leerTemperatura() {
	long adcValue = ADC; // Leer ADC
	return ((long)adcValue * 120000) / 1023; // Convertir a mili°C
}

void timer2_init() {  // REFRESCAR AUTOMATICAMENTE LUEGO DE 2 PI SEGUNDOS (ESTE TIEMPO ES ARBITRARIO XD)
	// Configuración del Timer 2 en modo CTC
	TCCR2A = 0;           // Normal mode
	TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS20);  // CTC mode, prescaler = 256
	OCR2A = 255;
	TIMSK2 = (1 << OCIE2A);
}

void ajustarPWM() {
	TemperaturaActual = leerTemperatura();
	if (TemperaturaActual < SetPoint - Ventana) {
		OCR0A = PWM_MAX;  // 100% PWM
		} else if (TemperaturaActual > SetPoint + Ventana) {
		OCR0A = PWM_MIN;  // 0% PWM
		} else {
		// Ajuste lineal entre 0% y 100%
		uint32_t diferencia = SetPoint - TemperaturaActual;
		OCR0A = (PWM_MAX * diferencia) / (2 * Ventana);
	}
	 // Convertimos el valor de OCR0A a un porcentaje de 0 a 100
	PorcentajePWM = (int)(((int)OCR0A * 100) / (int)PWM_MAX);
}
// Función para realizar la conversión de las variables
void ConvertirYEnviar() {
	// Convertir TemperaturaActual
	int TemperaturaActualEntero = (int)(TemperaturaActual / 1000);
	int TemperaturaActualDecimal = (int)(((TemperaturaActual - (long)(TemperaturaActualEntero) * 1000)) / 1);
	if (TemperaturaActualDecimal<100) // si la diferencia es minima, que no la muestre, para que no se genere no error de medición
	{  TemperaturaActualDecimal=0;
	}
	

	// Convertir SetPoint
	int SetPointEntero = (int)(SetPoint / 1000);
	int SetPointDecimal = (int)((SetPoint - (long)(SetPointEntero) * 1000) / 1); // División por 100 para obtener las décimas

	// Convertir Ventana
	int Ventana1 = (int)(Ventana / 1000);

	// Asignar el valor de PorcentajePWM
	int PorcentajePWMActual = PorcentajePWM;

	// Llamar a la función MenuMediciones con los valores convertidos
	MenuMediciones(TemperaturaActualEntero, TemperaturaActualDecimal, SetPointEntero, SetPointDecimal, Ventana1, PorcentajePWMActual);
}
void RecuperarMedicionesParaElMonitor(){
	 ConvertirYEnviar();
	 Refrescar=0;
	 Contador=0;
	};
void SelectorMenu(){
	char CaracterRecibido=uart_receive_char();
	switch (CaracterRecibido) {
		
		case '1': RecuperarMedicionesParaElMonitor(); break;        //<< Muestra o refresca las Mediciones Actuales, falta que se les de los valores de las mismas, podes "terciarizarlo" en otra función.
		//case '2': ConfigurarSetPoint(); break;  //<< Esta función aún no existe
		//case '3': ConfigurarVentana(); break;   // <<Función de selección entre dos opciones (aún no existe)
		//case '4': menu4(); break;               // << Acá poner Boludeces, tales como la resistencia utilizada,lenguaje utilizado, micro utilizado, etc (No olvidar poner que con la X se vuelve a este menú)
		case 'x': MenuInicial(); break;           // Si aprieta 'X' se debe mostrar el menu principal, hará falta colocarlo en otras partes del programa
		case 'X': MenuInicial(); break;           // Si aprieta 'X' se debe mostrar el menu principal, hará falta colocarlo en otras partes del programa
		default:  /*Por defecto no se hace nada*/ break;
	    /*
		A recordar: El menú principal es este:
		const char Menu0_0[] PROGMEM =">> 1 Ver Mediciones y configuraciones";
		const char Menu0_1[] PROGMEM =">> 2 Configurar Temperatura deseada (SetPoint)";
		const char Menu0_2[] PROGMEM =">> 3 Configurar Ventana";
		const char Menu0_3[] PROGMEM =">> 4 Datos Extras del Proyecto";
		A lo que se le suma la X para volver a mostrarlo*/
	}
}
ISR(TIMER1_COMPA_vect) {
	OCR0A = PWM_MIN;  // Apagar PWM
	Disparos = 0;      // Reiniciar conteo de disparos
	ajustarPWM();      // Ajustar PWM según la temperatura
}

ISR(INT0_vect) {
	if (Disparos < 50) {
		OCR0A = PWM_MAX;  // Encender PWM con límite
		Disparos++;
	}
}
ISR(TIMER2_COMPA_vect) {
	Refrescar=1;
	Contador++;
}
int main() {
	cli();  // Desactivar interrupciones globales
	uart_init();
	Bienvenida();
	setupPWM();
	setupTimer1();
	setupINT0();
	setupADC();
	sei();  // Activar interrupciones globales
	
	while (1) {
		ajustarPWM(); // Ajustar continuamente el PWM según la temperatura
		SelectorMenu();
	}
}
