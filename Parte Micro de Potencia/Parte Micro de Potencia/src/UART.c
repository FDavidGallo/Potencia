#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "FuncionesEeprom.h"
#include "UART.h"
/*
    _____               _   __              _            _       _
   / ____|             (_) /_/             | |          | |     | |
  | (___   ___  ___ ___ _  ___  _ __     __| | ___    __| | __ _| |_ ___  ___    ___ _ __
   \___ \ / _ \/ __/ __| |/ _ \| '_ \   / _` |/ _ \  / _` |/ _` | __/ _ \/ __|  / _ \ '_ \
   ____) |  __/ (_| (__| | (_) | | | | | (_| |  __/ | (_| | (_| | || (_) \__ \ |  __/ | | |
  |_____/ \___|\___\___|_|\___/|_| |_|  \__,_|\___|  \__,_|\__,_|\__\___/|___/  \___|_| |_|
  |  \/  |                         (_)           | |    | |
  | \  / | ___ _ __ ___   ___  _ __ _  __ _    __| | ___| |  _ __  _ __ ___   __ _ _ __ __ _ _ __ ___   __ _
  | |\/| |/ _ \ '_ ` _ \ / _ \| '__| |/ _` |  / _` |/ _ \ | | '_ \| '__/ _ \ / _` | '__/ _` | '_ ` _ \ / _` |
  | |  | |  __/ | | | | | (_) | |  | | (_| | | (_| |  __/ | | |_) | | | (_) | (_| | | | (_| | | | | | | (_| |
  |_|  |_|\___|_| |_| |_|\___/|_|  |_|\__,_|  \__,_|\___|_| | .__/|_|  \___/ \__, |_|  \__,_|_| |_| |_|\__,_|
                                                            | |               __/ |
                                                            |_|              |___/
*/  

const char TextoElegir[] PROGMEM = ">> Seleccione alguna de las anteriores opciones ";
const char TextoRegresar[] PROGMEM = ">> Aprete X para volver al menu inicial";
const char TextoExito[] PROGMEM = ">>¡Cambio exitoso!<<";
const char TextoFracaso[] PROGMEM = ">>ERROR: LA CANTIDAD PROPUESTA ESTÁ FUERA DE RANGO O EN FORMATO ERRONEO";
const char TextoFracaso1[] PROGMEM = ">>Por favor, intentelo nuevamente más tarde. ACEPTAR: Aprete X";
// TEXTOS PARA LAS MEDICIONES
const char TextoTemperatura[] PROGMEM ="   >>Temperatura actual del agua: ";
const char TextoTemperatura0[] PROGMEM ="*C";
const char TextoPWM0[] PROGMEM ="   >>El calentador de agua esta al ";
const char TextoPWM1[] PROGMEM ="% de su potencia nominal";
const char TextoSetPoint0[] PROGMEM ="   >>El setpoint esta situado en: ";
const char TextoSetPoint1[] PROGMEM ="*C";
const char TextoVentana1[] PROGMEM ="   >>La ventana esta situada en: +/- ";
const char TextoVentana2[] PROGMEM ="*C";

// TEXTO DE LOS MENUS
const char Bienvenida0[] PROGMEM ="          .oooooo.         .o.       ooooo        oooooooooooo  ooooo      ooo  ooooooooooooo       .o.       oooooooooo.     .oooooo.    oooooooo";
const char Bienvenida1[] PROGMEM ="         d8P'  `Y8b       .888.      `888'        `888'     `8  `888b.     `8'  8'   888   '8      .888.      `888'   `Y8b   d8P    Y8b   888    Y88";
const char Bienvenida2[] PROGMEM ="        888              .8 888.      888          888           8 `88b.    8        888          .8 888.      888      888 888      888  888    d88'";
const char Bienvenida3[] PROGMEM ="        888             .8   888.     888          888oooo8      8   `88b.  8        888         .8   888.     888      888 888      888  888ooo88P";
const char Bienvenida4[] PROGMEM ="        888            .88ooo8888.    888          888           8     `88b.8        888        .88ooo8888.    888      888 888      888  888`88b.";
const char Bienvenida5[] PROGMEM ="        `88b    ooo   .8'     `888.   888       o  888       o   8       `888        888       .8'     `888.   888     d88' `88b    d88'  888   88b";
const char Bienvenida6[] PROGMEM ="         `Y8bood8P   o88o     o8888o o888ooooood8 o888ooooood8  o8o        `8       o888o     o88o     o8888o o888bood8P'    `Y8bood8P'  o888o  o888o ";

const char Bienvenida7[] PROGMEM ="                                oooooooooo.   oooooooooooo                 .o.         .oooooo.    ooooo     ooo       .o.";
const char Bienvenida8[] PROGMEM ="                                 888     Y8b  `888'     `8                .888.       d8P'  `Y8b    888       8       .888.";
const char Bienvenida9[] PROGMEM ="                                 888      888  888                       .8 888.     888             88       8      .8 888.";
const char BienvenidaA[] PROGMEM ="                                 888      888  888oooo8                 .8   888.    888             88       8     .8   888.";
const char BienvenidaB[] PROGMEM ="                                 888      888  888                     .88ooo8888.   888    oooooo   88       8    .88ooo8888.";
const char BienvenidaC[] PROGMEM ="                                 888     d88'  888       o            .8'     `888.  `88.     88      8      8'   .8'     `888.";
const char BienvenidaD[] PROGMEM ="                                o888bood8P'   o888ooooood8           o88o     o8888o  `Y8bood8P       'YbodP'    o88o     o8888o";



const char Bienvenida14[] PROGMEM ="                                       7 ";
const char Bienvenida15[] PROGMEM ="                          348000005  393                       TRABAJO PRACTICO FINAL INTEGRADOR: ";
const char Bienvenida16[] PROGMEM ="                     3900000000002  404  23                                >>CALENTADOR DE AGUA  ";
const char Bienvenida17[] PROGMEM ="                  300000004317    78007 500003";
const char Bienvenida18[] PROGMEM ="                20000047         30002  75000002               CATEDRA: >>ELECTRONICA DE POTENCIA     ";
const char Bienvenida19[] PROGMEM ="              7000001           40009      1800001             CICLO LECTIVO: >>2025     ";
const char Bienvenida20[] PROGMEM ="             200001            800003        100005            ALUMNO:     ";
const char Bienvenida21[] PROGMEM ="            50006            2000006           40004                     >>FABRICIO DAVID GALLO ";
const char Bienvenida22[] PROGMEM ="           30005            60000007            50002          PROFESORES:     ";
const char Bienvenida23[] PROGMEM ="           0009           700000005              60007                  >>ING PATRICIO BELLINGERI ";
const char Bienvenida24[] PROGMEM ="          6000           2000000001               0009                  >>LIC JULIAN MINATEL ";
const char Bienvenida25[] PROGMEM ="          0006          900000000000000000003     4000";
const char Bienvenida26[] PROGMEM ="          0002        1000000000000000000001      30007";
const char Bienvenida27[] PROGMEM ="          0003       500000000000000000006        30001        EL SIGUIENTE TRABAJO TIENE POR OBJETO EL";
const char Bienvenida28[] PROGMEM ="          0005      900000000000000000003         50007        REGULAR LA TEMPERATURA DE UN TANQUE DE AGUA. ";
const char Bienvenida29[] PROGMEM ="          8000     400000000000000000007          8000         EL AGUA ES CALENTADA MEDIANTE UNA RESISTENCIA";
const char Bienvenida30[] PROGMEM ="          30002              100000004           30003         QUE ES REGULADA MEDIANTE UN CONTROL DE PWM;  ";
const char Bienvenida31[] PROGMEM ="           9000              60000003            0009          CONECTADO A LA RED TRIFASICA.   ";
const char Bienvenida32[] PROGMEM ="            00007           2000008             0000";
const char Bienvenida33[] PROGMEM ="             00005          800005            50000            >> 1 Ver Mediciones y configuraciones";
const char Bienvenida34[] PROGMEM ="              500003       500003           300004             >> 2 Configurar Temperatura deseada (SetPoint)";
const char Bienvenida35[] PROGMEM ="               7000005    10008           5000001              >> 3 Configurar Ventana";
const char Bienvenida36[] PROGMEM ="                 1900004  6005        290000081                >> 4 Datos Extras del Proyecto";
const char Bienvenida37[] PROGMEM ="                    5007 3001  4000000000005                   >> Seleccione alguna de las anteriores opciones";
const char Bienvenida38[] PROGMEM ="                         99  100000000847";
const char Bienvenida39[] PROGMEM ="                        32";

   
   
 const char Menu0_0[] PROGMEM =">> 1 Ver Mediciones y configuraciones";
 const char Menu0_1[] PROGMEM =">> 2 Configurar Temperatura deseada (SetPoint)";
 const char Menu0_2[] PROGMEM =">> 3 Configurar Ventana";
 const char Menu0_3[] PROGMEM =">> 4 Datos Extras del Proyecto";
 
 const char TextoRefrescar[] PROGMEM ="   >>Aprete '1' nuevamente para actualizar la medicion"; 
 
const char Mediciones0[] PROGMEM ="   888b     d888               888  d8b          d8b                                                d8888             888                        888   ";
const char Mediciones1[] PROGMEM ="   8888b   d8888               888  Y8P          Y8P                                               d88888             888                        888       ";
const char Mediciones2[] PROGMEM ="   88888b.d88888               888                                                                d88P888             888                        888           ";
const char Mediciones3[] PROGMEM ="   888Y88888P888  .d88b.   .d88888  888  .d8888b 888  .d88b.  88888b.   .d88b.   .d8888b         d88P 888  .d8888b 888888888  888  888   8888b.  888   .d88b.   .d8888b  ";
const char Mediciones4[] PROGMEM ="   888 Y888P 888 d8P  Y8b d88  888  888 d88P     888 d88  88b 888  88b d8P  Y8b  88K            d88P  888 d88P        888     888  888       88b 888  d8P  Y8b  88K          ";
const char Mediciones5[] PROGMEM ="   888  Y8P  888 88888888 888  888  888 888      888 888  888 888  888 88888888  'Y888b.       d88P   888 888         888     888  888  .d888888 888  88888888 'Y888b.   ";
const char Mediciones6[] PROGMEM ="   888       888 Y8b.     Y88b 888  888 Y88b     888 Y88  88P 888  888 Y8b           X88      d8888888888 Y88b.       Y88b.   Y88b 888  888  888 888  Y8b.          X88  ";
const char Mediciones7[] PROGMEM ="   888       888  'Y8888   'Y88888  888  'Y8888P 888  'Y88P'  888  888  'Y8888   88888P'     d88P     888  'Y8888P     'Y888   'Y88888  'Y888888 888   'Y8888   88888P'   ";


// TEXTO PARA LAS CONFIGURACIONES
const char MenuConfiguracion0[] PROGMEM=" A-Temperatura Deseada (60-95)*C =";

// MENU TAMAÑO BIDON
const char MenuTamagno[] PROGMEM=">>Seleccione el nuevo volumen presionando una de estas letras:";
const char MenuTamagno0[] PROGMEM=" V- 10 L";
const char MenuTamagno1[] PROGMEM=" W- 20 L";


const char MenuCambioDeNumero0[] PROGMEM=" Para cambiar la variable siga los siguiente pasos:";
const char MenuCambioDeNumero1[] PROGMEM="1)- Coloque cifra por cifra segun sea el maximo. Por ejemplo:";
const char MenuCambioDeNumero2[] PROGMEM=" si el numero máximo es 100 y queremos poner 25-> '0' '2' '5' ";
const char MenuCambioDeNumero3[] PROGMEM="2)- Aprete '2' hasta ver su cambio reflejado";
const char MenuCambioDeNumero4[] PROGMEM="===========================================================";
const char MenuCambioDeNumero5[] PROGMEM="|  ¡ATENCION!: COLOQUE NUMEROS DENTRO DEL RANGO PERMITIDO |";
const char MenuCambioDeNumero6[] PROGMEM="===========================================================";
/***
 *                                                                                                                             
 *     #    #   ##   #####  #   ##   #####  #      ######  ####    ###### #    #     ###### ###### #####  #####   ####  #    # 
 *     #    #  #  #  #    # #  #  #  #    # #      #      #        #      ##   #     #      #      #    # #    # #    # ##  ## 
 *     #    # #    # #    # # #    # #####  #      #####   ####    #####  # #  #     #####  #####  #    # #    # #    # # ## # 
 *     #    # ###### #####  # ###### #    # #      #           #   #      #  # #     #      #      #####  #####  #    # #    # 
 *      #  #  #    # #   #  # #    # #    # #      #      #    #   #      #   ##     #      #      #      #   #  #    # #    # 
 *       ##   #    # #    # # #    # #####  ###### ######  ####    ###### #    #     ###### ###### #      #    #  ####  #    # 
 *                                                                                                                             
 */ 
// Atención: B1=Bebida1 ; B2=Bebida2; B3=Bebida3;B 4=Bebida4. CAgua=Cantidad de Agua caliente. 
//========== PorcDescarga=Porcentaje del tiempo de descarga extra al que es igual al "tiempo de dosificación" (ver documentación para mayor detalle)
// Si tienen una P al final es porque pertenecen al programa, no a la eprom...
// Las direcciones son:
	#define NombreB1 10
	#define NombreB2 25
	#define NombreB3 40
	#define NombreB4 55
	#define DosificacionB1 280
	#define DosificacionB2 70
	#define DosificacionB3 80
	#define DosificacionB4 90
    #define CAguaB1 100
	#define CAguaB2 110
	#define CAguaB3 120
	#define CAguaB4 130
	#define PorcDescargaB1 140
	#define PorcDescargaB2 145
	#define PorcDescargaB3 150
	#define PorcDescargaB4 155
	#define TamagnoBidon 160
	#define KpEprom 170
	#define TemperaturaDeseada 180


/*
   ____                _   __              _
  / ___|  ___  ___ ___(_) /_/  _ __     __| | ___
  \___ \ / _ \/ __/ __| |/ _ \| '_ \   / _` |/ _ \
   ___) |  __/ (_| (__| | (_) | | | | | (_| |  __/
  |____/ \___|\___\___|_|\___/|_| |_|  \__,_|\___|
   _____                 _                             _        _
  |  ___|   _ _ __   ___(_) ___  _ __   ___  ___    __| | ___  | | __ _
  | |_ | | | | '_ \ / __| |/ _ \| '_ \ / _ \/ __|  / _` |/ _ \ | |/ _` |
  |  _|| |_| | | | | (__| | (_) | | | |  __/\__ \ | (_| |  __/ | | (_| |
  |_|   \__,_|_| |_|\___|_|\___/|_| |_|\___||___/  \__,_|\___| |_|\__,_|
   _   _            _
  | | | | __ _ _ __| |_
  | | | |/ _` | '__| __|
  | |_| | (_| | |  | |_
   \___/ \__,_|_|   \__|   */
  
#include "UART.h"
char Buffer[10];
void uart_init() {
	// Configurar la velocidad de transmisión a 57600 baudios,
	UBRR0H = 0;
	UBRR0L = 16;

	// Habilitar el transmisor y el receptor
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// Configurar el formato del trama: 8 bits de datos, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Enviar un solo carácter por la UART
void uart_send_char(char c) {
	while (!(UCSR0A & (1 << UDRE0))); // Esperar hasta que el buffer esté vacío
	UDR0 = c;
}

// Enviar una cadena (texto) terminada en nulo por la UART
void uart_send_string(const char* str) {
	while (*str) {
		uart_send_char(*str);
		str++;
	}
}

// Enviar un carácter de salto de línea por la UART
void uart_send_newline() {
	uart_send_char(13);
	uart_send_char(10);
}

// Recibir un carácter desde la UART
char uart_receive_char()  {
	while (!(UCSR0A & (1 << RXC0))); // Esperar hasta que se reciba un dato
	char recibido = UDR0;
	if (recibido=='_'){ // Si envia un gui?n bajo devuelve un 0
		recibido='\0';  // esto es para que se pueda poner fin a un nuevo nombre
	}
	return recibido;
}
char echo_serialNobloqueante(int Bandera1,int Bandera3,int Bandera2) {// Lo que lo gace no bloqueante es que si alguna de las banderas está
	char recibido;													  // en alto, no se espera que se reciba un dato
	int BanderaBloqueo=((Bandera1)||(Bandera2)||(Bandera3));
	if (BanderaBloqueo){                                              // En C, todo valor distinto de 0 es verdadero
		recibido=';';
		return recibido;
	}else {
	char input = uart_receive_char(); // TENGO QUE PREGUNTARLE AL EDU PORQUE SI LLAMO AL NO BLOQUEANTE SE ROMPE TODO
	uart_send_char(input);
	recibido=input;
	return recibido;}
}
// Función para el eco serial  y recuperar el dato mandado vía uart

char echo_serial() {
	char input = uart_receive_char();
	uart_send_char(input);
	uart_send_newline();
	char recibido=input;
	return recibido;
}

void MandarStringdesdePrograma(const char *str) {
	char c;
	while ((c = pgm_read_byte(str++))) { // Leemos de la memoria del programa
		uart_send_char(c); // Enviamos un caracter
	}
}
unsigned char UART_receive_non_blocking() {
	// Si hay datos disponibles para leer
	if (UCSR0A & (1<<RXC0)) {
		unsigned char Char = UDR0; // Lee el dato del registro UDR0
		return Char;
		} else {
		return ';'; // No hay datos disponibles
	}
}
void EnviarTextoSeleccionarOpcion(){
	uart_send_newline();
	MandarStringdesdePrograma(TextoElegir);
	
}
void uart_clear_screen() {
	uart_send_string("\033[2J"); // Código ANSI para borrar la pantalla
	uart_send_string("\033[H");  // Código ANSI para mover el cursor a la posición inicial
}

/*  ____                _   __              _
   / ___|  ___  ___ ___(_) /_/  _ __     __| | ___             /////    Las siguientes funciones sson solo menús
   \___ \ / _ \/ __/ __| |/ _ \| '_ \   / _` |/ _ \            /////     de la interfaz gráfica de la UART
    ___) |  __/ (_| (__| | (_) | | | | | (_| |  __/            /////    las funciones de acá en adelante son sólo 
   |____/ \___|\___\___|_|\___/|_| |_|  \__,_|\___|            /////    combinaciones de las anteriores...
   __  __              __
  |  \/  | ___ _ __  _/_/_ ___
  | |\/| |/ _ \ '_ \| | | / __|
  | |  | |  __/ | | | |_| \__ \
  |_|  |_|\___|_| |_|\__,_|___/
                                    */
void Bienvenida(void) {
        uart_send_newline();
        uart_clear_screen();
        MandarStringdesdePrograma(Bienvenida0);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida1);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida2);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida3);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida4);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida5);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida6);
        uart_send_newline();
		uart_send_newline();
        MandarStringdesdePrograma(Bienvenida7);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida8);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida9);
        uart_send_newline();
        MandarStringdesdePrograma(BienvenidaA);
        uart_send_newline();
        MandarStringdesdePrograma(BienvenidaB);
        uart_send_newline();
        MandarStringdesdePrograma(BienvenidaC);
        uart_send_newline();
        MandarStringdesdePrograma(BienvenidaD);
		uart_send_newline();
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida14);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida15);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida16);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida17);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida18);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida19);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida20);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida21);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida22);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida23);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida24);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida25);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida26);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida27);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida28);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida29);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida30);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida31);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida32);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida33);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida34);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida35);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida36);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida37);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida38);
        uart_send_newline();
        MandarStringdesdePrograma(Bienvenida39);
        uart_send_newline();
}

	
void MenuInicial(void){
	uart_send_newline();
	uart_clear_screen();
	MandarStringdesdePrograma(Menu0_0);
	uart_send_newline();
	MandarStringdesdePrograma(Menu0_1);
	uart_send_newline();
	MandarStringdesdePrograma(Menu0_2);
	uart_send_newline();
	MandarStringdesdePrograma(Menu0_3);
	uart_send_newline();
	MandarStringdesdePrograma(TextoElegir);
}
	
void MenuMediciones(int TemperaturaActualEntero,int TemperaturaActualDecimal,int SetPointEntero,int SetPointDecimal,int Ventana, int PorcentajePWMActual) {
	uart_clear_screen();
	MandarStringdesdePrograma(Mediciones0);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones1);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones2);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones3);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones4);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones5);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones6);
	uart_send_newline();
	MandarStringdesdePrograma(Mediciones7);
	uart_send_newline();
	uart_send_newline();
	MandarStringdesdePrograma(TextoTemperatura);
	sprintf(Buffer, "%d", TemperaturaActualEntero);
	uart_send_string(Buffer);
	uart_send_string(",");
	sprintf(Buffer, "%d", TemperaturaActualDecimal);
	uart_send_string(Buffer);
	MandarStringdesdePrograma(TextoTemperatura0);
	uart_send_newline();
	MandarStringdesdePrograma(TextoSetPoint0);
	sprintf(Buffer, "%d", SetPointEntero);
	uart_send_string(Buffer);
	if (SetPointDecimal>0)
	{
	
	uart_send_string(",");
	sprintf(Buffer, "%d", SetPointDecimal);
	if ((SetPointDecimal<100)&&(SetPointDecimal>10))
	{ uart_send_string("0");
	}
	if ((SetPointDecimal<10))
	{ uart_send_string("00");
	}uart_send_string(Buffer);
	}
	MandarStringdesdePrograma(TextoSetPoint1);
	uart_send_newline();
	MandarStringdesdePrograma(TextoVentana1);
	sprintf(Buffer, "%d", Ventana);
	uart_send_string(Buffer);
	MandarStringdesdePrograma(TextoVentana2);
	uart_send_newline();
	MandarStringdesdePrograma(TextoPWM0);
	sprintf(Buffer, "%d", PorcentajePWMActual);
	uart_send_string(Buffer);
	MandarStringdesdePrograma(TextoPWM1);
	uart_send_newline();
	MandarStringdesdePrograma(TextoRefrescar);
	uart_send_newline();
	uart_send_string("   >>Aprete X para volver al menu inicial");}
void MenuCambiarVolumenBidon(void) {
	
}
void MenuConfiguraciones(void) {
	uart_clear_screen();
	MandarStringdesdePrograma(TextoRegresar);
}

void MenuCambioDeNombre(void){
	}
	
void Fracaso(void){
	uart_send_newline();
	MandarStringdesdePrograma(TextoFracaso);
	uart_send_newline();
	MandarStringdesdePrograma(TextoFracaso1);
	uart_send_newline();
	}
	
void CambioDeNumero(void){
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero0);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero1);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero2);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero3);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero4);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero5);
	uart_send_newline();
	MandarStringdesdePrograma(MenuCambioDeNumero6);
	uart_send_newline();
}

