
//-----------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- BIBLIOTECAS ------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<math.h>
#include "inc/tm4c1294ncpdt.h"
#include "SSD1306_V2.h"

//-----------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- PUERTOS ------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
 /*I2C
       I2C0SCL conectado a PB2
       I2C0SDA conectado a PB3

   UART
       U7Rx (receptor) conectado a PC4
       U7Tx (transmisor) conectado a PC5 (no se usa)

   SPI (DAC)
       SPI SDO conectado a PA5 (MISO) (no se usa)
       SPI SDI conectado a PA4 (MOSI)
       SPI CS conectado a PA3
       SPI SCK conectado a PA2

   ADC (MICROFONO)
       AI9 conectado a PE4

   TECLADO
       PD0-7 y PL0-3
       BLANCAS: PD0-6   ->  NEGRAS PD7 Y PL0-3
 */

//-----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------- DECLARACIONES ----------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------


void UARTInit(void);
void I2CInit(void);
void displayInit(void);
void vozInit(void);
void pianoInit(void);
void ADCInit(void);
void SSI0_init(void);
void TimersInit(void);
void sequenceInit(void);
void DatosDisplay(void);
void GPIO_Init(void);

int uartinterr,adc0interr,muestrasADC,periodo,frecuencia,adcflag,sumafrec,frecprom,frecsel,modo,samples,nota,octava,semi,inter,pdint,plint,a;

volatile uint32_t data[4]; // Arreglo donde se guardan los bytes recibidos por UART
const int seno[32] ={2048,2447,2831,3185,3495,3750,3939,4056,4095,4056,3939,3750,3495,3185,2831,
2447,2048,1648,1264,910,600,345,156,39,0,39,156,345,600,910,1264,1648}; // Señal seno muestreada de 0 a 4095

//-----------------------------------------------------------------------------------------------------------------------
//------------------------------------------------- PROGRAMA PRINCIPAL --------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
void main(void)

{
    UARTInit(); // Inicializador del UART
    I2CInit();  // Inicializador del I2C
    SSI0_init();    // Inicializador del SPI
    displayInit();  // Inicializador de interfaz del display
    ADCInit();  // Inicializador del ADC
    TimersInit();   // Inicializador de los contadores utilizados
    GPIO_Init();
    //frecsel=nota*octava*/semi;
    while(1);   // Espera del procesador
}

//-----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- FUNCIONES -------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------- INICIALIZACIONES ---------------------------------------------------

//-------------------------------------------------------- UART ---------------------------------------------------------

void UARTInit(void){
    // Initializa UART7 para 115,200 bauds (reloj de 16 MHz del PIOSC),
    // palabra de 8 bit, no hay bits de paridad, un stop bit, los FIFOs habilitados,
    // no hay interrupciones
      SYSCTL_RCGCUART_R |=  0x00000080; // Activa el reloj para el UART7 (p.388)
      SYSCTL_RCGCGPIO_R |=  0x00000004;  // Activa el reloj para el Puerto C (p.382)

      while((SYSCTL_PRUART_R&SYSCTL_PRUART_R7) == 0){}; // Se espera a que el reloj se estabilice (p.505)
      UART7_CTL_R &= ~0x00000001; // Deshabilita el UART (p.1188)
      UART7_IBRD_R = 8;  // Parte entera de Baudrt = int(16,000,000 / (16 * 115,200)) = int(8.681) (p.1184)
      UART7_FBRD_R = 44; // Parte fraccionaria de Baudrt = round(0.6806 * 64) = 44 (p. 1185)
      // Configuracion de trama: Palabra de 8 bits (sin bits de paridad, un bit stop, FIFOs) (p. 1186)
      UART7_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
      // UART toma su reloj del la fuente alterna como se define por SYSCTL_ALTCLKCFG_R (p. 1213)
      UART7_CC_R = (UART7_CC_R&~UART_CC_CS_M)+UART_CC_CS_PIOSC;
      // La fuente de reloj alterna es el PIOSC (default)(P. 280)
      SYSCTL_ALTCLKCFG_R = (SYSCTL_ALTCLKCFG_R&~SYSCTL_ALTCLKCFG_ALTCLK_M)+SYSCTL_ALTCLKCFG_ALTCLK_PIOSC;
      //Bloque que configura la interrupcion y la desenmascara
      UART7_IFLS_R &= ~0x0000003F;   // Limpia el registro longitud de FIFO de TX y RX (p.1191)
      UART7_IFLS_R += (UART_IFLS_TX2_8|UART_IFLS_RX2_8);
      // Habilita las interrupciones de la FIFO de TX y RX, y la interrupcion de 1/4 de FIFO (4 bytes)
      UART7_ICR_R = 0x10;
      //Limpia la bandera interrupcion de FIFO RX (En este programa solo manejaremos esta interrupcion)
      UART7_IM_R |= 0x00000010;    // Desenmascara la interrupcion para UART7
      //Fin de configuracion de interrupcion

      // Alta velocidad deshabilitada;divide el reloj por 16 en lugar de 8 (default)(1188)
      UART7_CTL_R &= ~0x00000020;
      UART7_CTL_R |= 0x00000001;  // Habilita el UART (p.1188)
      // Se espera a que el reloj se estabilice
      while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R2) == 0){};
      GPIO_PORTC_AHB_AFSEL_R |= 0x30; // Habilita funcion alterna en PC5,4
      GPIO_PORTC_AHB_DEN_R |= 0x30;  // Habilita digital I/O en PC5,4
      // Configura PC5-4 como UART
      GPIO_PORTC_AHB_PCTL_R = (GPIO_PORTC_AHB_PCTL_R&0xFF00FFFF)+0x00110000;
      GPIO_PORTC_AHB_AMSEL_R &= ~0x30; // deshabilita la funcionabilidad analogica de PC
      // Configuracion de prioridad y hablitacion de interrupcion en NVIC
      NVIC_PRI15_R = (NVIC_PRI15_R&0xFFFFFF00)|0x00000000; // Prioridad 0 (p.152)
      NVIC_EN1_R = 1<<(60-32); // Habilita la interrupcion 60 en NVIC (p. 154)
}

//-------------------------------------------------------- SPI ---------------------------------------------------------

void SSI0_init (void) {
    SYSCTL_RCGCSSI_R = SYSCTL_RCGCSSI_R0; // Activa reloj al SSI0
    while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R0) == 0); // Espera a que este listo
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // Activa reloj del GPIO A
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0) == 0); // Espera a que este listo

    GPIO_PORTA_AHB_AFSEL_R |= 0x3C; // Seleciona la función alterna de PA[2:5].
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & 0XFF0000FF) | 0x00FFFF00; // Configura las terminales de PA a su función de SSI0.
    GPIO_PORTA_AHB_AMSEL_R = 0x00; // Deshabilita la función analogica
    GPIO_PORTA_AHB_DIR_R = (GPIO_PORTA_AHB_DIR_R & ~0x3C) | 0x1C; // Configura al puerto como salida
    GPIO_PORTA_AHB_DEN_R |= 0x3C; // Habilita la función digital del puerto

    SSI0_CR1_R = 0x00; // Selecciona modo maestro/deshabilita SSI0. (p. 1247)
    SSI0_CC_R=0x05;//usa reloj de 16 MHz
    SSI0_CPSR_R = 0x02; // preescalador (CPSDVSR) del reloj SSI (p. 1252)
    // configura para Freescale SPI; 16bit; 4 Mbps; SPO = 0; SPH = 0 (p. 1245)
    SSI0_CR0_R = (0x0100 | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_16) & ~(SSI_CR0_SPO | SSI_CR0_SPH);
    //0 en estado de espera del bus
    SSI0_CR1_R |= SSI_CR1_SSE; // Habilita SSI0.
}


/*Funcion para organizar la forma de envio de los datos al DAC
 * considerando dos bits que configura para tener ganancia unitaria en la salida
 * y permitir la salida del mismo
 * */

void SSI0_sendData (uint16_t dat) {
    // Envia dato de 16-bit

    SSI0_DR_R = (dat&0xFFF)|0x3000; // envia dato. con bits de enable y ganancia unitaria activados.
    while ((SSI0_SR_R & SSI_SR_BSY) != 0); // espero si el bus está ocupado
}

//--------------------------------------------------------- I2C ---------------------------------------------------------

void I2CInit(void){

    I2C_Init(); // Inicialización de relojes y puertos de I2C
    while(I2C0_MCS_R&0x00000001){}; // I2C listo?
    SSD1306_Init(); // Configura display
    SSD1306_Clear();    // Limpia display
}

//--------------------------------------------------------- ADC ---------------------------------------------------------

void ADCInit(void){

      SYSCTL_RCGCGPIO_R |= 0x10; // 1) Habilita reloj para Puerto E

      while((SYSCTL_PRGPIO_R & 0x10) == 0); // Se espera a que el reloj se estabilice (p.499)

      GPIO_PORTE_AHB_DIR_R = 0x00;    // 2) PE4 entradas (analógica)
      GPIO_PORTE_AHB_AFSEL_R |= 0x10; // 3) Habilita Función Alterna en PE4 (p. 770)
      GPIO_PORTE_AHB_DEN_R &= ~0x10;  // 4) Deshabilita Función Digital en PE4 (p 781)
      GPIO_PORTE_AHB_AMSEL_R |= 0x10; // 5) Habilita Función Analógica de PE4 (p. 786)

      SYSCTL_RCGCADC_R |= 0x01;       // 6) Habilita reloj para ADC0(p. 396)

      while((SYSCTL_PRADC_R & 0x01) == 0); // Se espera a que el reloj se estabilice

      ADC0_PC_R = 0x01;         // 7)Configura para 125Ksamp/s (p.1159)
      ADC0_SSPRI_R = 0x0123;    // 8)SS3 con la más alta prioridad

      ADC0_ACTSS_R &= ~0x0004;  // 9) Deshabilita SS2 antes de cambiar configuración de registros (p. 1076)

      ADC0_EMUX_R = 0x0000;     // 10) Se configura SS2 para disparar muestreo por software (default) (p.1091)

      ADC0_SAC_R = 0x0;         // 11) Se configura para no tener sobremuestreo por hardware(default)(p. 1105)

      ADC0_CTL_R = 0x0;         //12) Se configura con referencias internas (default VDDA and GNDA) (p. 1107)

      ADC0_SSOP2_R |= 0x0001;    // 13) Se configure para salvar los resultados del ADC en DC (default)(p. 1134)
      ADC0_SSDC2_R = 0x0000;    // 14) Se selecciona el DC0 para guardar los resultados

      ADC0_SSTSH2_R = 0x000;    // 15) Se configure el ADC para un periodo de 4  S&H (default) (p. 1139)

      ADC0_SSMUX2_R |= 0x0009;    // 16) Se configura entrada 1°muestra=AIN9  (p.1128)
      ADC0_SSEMUX2_R &= ~0x0001; // 17) Canales del SS2 para 1°muestra  (p.1137)
      ADC0_SSCTL2_R |= 0x0006;    // 18) SI: INT 2SAMPLE, SAMPLE DOS ES FIN; No:muestra diferencial (p.1142)


      ADC0_DCCMP0_R |= 0x0746064E; // 19) Umbral de comparacion COM0=1241(1.3V), COM1=1366(1.5V)
      ADC0_DCCTL0_R |= 0x1F;      // 20) Habilitacion en modo de interrupcion por banda alta

      ADC0_IM_R |= 0x00040000;      // 21) Habilita interrupción SS2 (p. 1081)
      ADC0_ACTSS_R |= 0x0004;    // 22) Habilita SS2 (p. 1076)

      SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;  // 23) Encender PLL
      while((SYSCTL_PLLSTAT_R&0x01)==0);            // 24) Espera a que el PLL fije su frecuencia
      SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR; // 25) Apagar PLL

      ADC0_ISC_R = 0x00040004;  //  26) Se recomienda limpiar la bandera RIS del ADC0 y DC0
      ADC0_DCISC_R = 0x01;       // 27) Limpia bandera de interrupcion de CD0

      // Configuracion de prioridad y hablitacion de interrupcion en NVIC (Interrupcion 16)
      NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFFFF00)|0x00000040; // Prioridad 2 (p.152)
      NVIC_EN0_R = 1<<(16-0); // Habilita la interrupcion 16 en NVIC (p. 154)

}

//-------------------------------------------------------- HMI ---------------------------------------------------------


void displayInit(void){
    // Primer renglon
    SSD1306_Command(SSD1306_SETPAGE0);  // Selecciona pagina
    SSD1306_Command(0x01);  // Selecciona columna en nibble menos significativo
    SSD1306_Command(0x10);  // Selecciona columna en nibble mas significativo
    SSD1306_WriteString("MUSIC TRAINER");   // Cadena a escribir

    // Segundo renglon
    SSD1306_Command(SSD1306_SETPAGE1);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("BIENVENIDO");

    // Tercer renglon
    SSD1306_Command(SSD1306_SETPAGE2);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("SELECCIONE UN MODO");

    // Cuarto renglon
    Nota_sostenido(3,0x01,0x10);
    SSD1306_WriteString(" ");
    Nota_bemol(3,0x07,0x10);
    Nota_sostenido(3,0x0F,0x10);
    SSD1306_WriteString(" ");
    Nota_bemol(3,0x05,0x11);
    Nota_sostenido(3,0x0D,0x11);
    SSD1306_WriteString(" ");
    Nota_bemol(3,0x03,0x12);
    Nota_sostenido(3,0x0B,0x12);
    SSD1306_WriteString(" ");
    Nota_bemol(3,0x02,0x13);
}


//----------------------------------------------------- VOZ INIT ------------------------------------------------------

void vozInit(void){
    // Primer renglon
    SSD1306_Command(SSD1306_SETPAGE0);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("VOICE TRAINER");

    // Segundo renglon
    SSD1306_Command(SSD1306_SETPAGE1);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("MODO VOZ SELECCIONADO");

    // Tercer renglon
    SSD1306_Command(SSD1306_SETPAGE2);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("NOTA:");
}

//----------------------------------------------------- PIANO INIT ------------------------------------------------------

void pianoInit(void){
    // Primer renglon
    SSD1306_Command(SSD1306_SETPAGE0);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("PIANO TRAINER");

    // Segundo renglon
    SSD1306_Command(SSD1306_SETPAGE1);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("MODO PIANO SELECCIONADO");

    // Tercer renglon
    SSD1306_Command(SSD1306_SETPAGE2);
    SSD1306_Command(0x01);
    SSD1306_Command(0x10);
    SSD1306_WriteString("NOTA:");

    GPIO_PORTD_AHB_IM_R |= 0xFF;
    GPIO_PORTL_IM_R |= 0x0F;
}


//----------------------------------------------------- TIMERS INIT ------------------------------------------------------

void TimersInit(void){
//-------------------------------------------------- TIMER 0 ---------------------------------------------------
// Medicion de frecuencia de ADC
    SYSCTL_RCGCTIMER_R |= 0x01;        //Habilita reloj al timer0
    while((SYSCTL_PRTIMER_R & 0x01)==0);    //Espera para que se habilite el timer 0
    TIMER0_CTL_R = 0X00000000;      //Deshabilita el timer 0
    TIMER0_CFG_R = 0X00000000;      //Habilita el timer en 32 bits
    TIMER0_TAMR_R = 0X00000012;     //Timer en modo peródico en cuenta hacia arriba
    TIMER0_TAILR_R = 0XFFFFFFFF;    //Tope de cuenta
    TIMER0_TAPR_R = 0X00;           //Valor actual de la cuenta (Empieza en 0)

//-------------------------------------------------- TIMER 2 ---------------------------------------------------
// Tiempo entre mediciones de ADC
    SYSCTL_RCGCTIMER_R |= 0X04; //HABILITA TIMER 2
    while((SYSCTL_PRTIMER_R & 0x04)==0);    //Espera para que se habilite el timer 2
    TIMER2_CTL_R=0X00000000; //DESHABILITA TIMER EN LA CONFIGURACION (p.986)
    TIMER2_CC_R=1;  //Reloj interno de 16MHz
    TIMER2_CFG_R= 0X00000004; //CONFIGURAR PARA 16 BITS (p. 976)
    TIMER2_TAMR_R= 0X00000002; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA ABAJO (p. 977)
    TIMER2_TAILR_R= 2528; // VALOR DE RECARGA (p.1004)

    TIMER2_ICR_R= 0X00000001 ; //LIMPIA POSIBLE BANDERA PENDIENTE DE TIMER2 (p.1002)
    TIMER2_IMR_R |= 0X00000001; //ACTIVA INTRRUPCION DE TIMEOUT (p.993)

    NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x60000000; // Prioridad 3 (p.152)
    NVIC_EN0_R = 1<<(23-0); // Habilita la interrupcion 23 en NVIC (p. 154)


//-------------------------------------------------- TIMER 3 ---------------------------------------------------
// Retardo en DAC para modificar frecuencia
    SYSCTL_RCGCTIMER_R |= 0X08; //HABILITA TIMER 3
    while((SYSCTL_PRTIMER_R & 0x08)==0);    //Espera para que se habilite el timer 3
    TIMER3_CTL_R=0X00000000; //DESHABILITA TIMER EN LA CONFIGURACION (p.986)
    TIMER3_CC_R=1;//reloj interno de 16MHz
    TIMER3_CFG_R= 0X00000004; //CONFIGURAR PARA 16 BITS (p. 976)
    TIMER3_TAMR_R= 0X00000002; //CONFIGURAR PARA MODO PERIODICO CUENTA HACIA ABAJO (p. 977)
    TIMER3_TAILR_R=  100904 + 21; // VALOR DE RECARGA (p.1004)
    // Para frecuencia de señal de salida(fsin) TIMER3_TAILR_R==fclk/(32*fsin)
    TIMER3_ICR_R= 0X00000001 ; //LIMPIA POSIBLE BANDERA PENDIENTE DE TIMER3 (p.1002)
    TIMER3_IMR_R |= 0X00000001; //ACTIVA INTRRUPCION DE TIMEOUT (p.993)
    NVIC_PRI8_R= (NVIC_PRI8_R&0x00FFFFFF)|0x20000000; // Prioridad 1 (p.152)
    NVIC_EN1_R|= 1<<(35-32); //HABILITA LA INTERRUPCION 35 DE  TIMER3 A
}


//----------------------------------------------------- GPIO INIT ------------------------------------------------------

void GPIO_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000408; // (a) activa el reloj para el puerto D y L

  //******* Puerto D de entrada *******
    GPIO_PORTD_AHB_DIR_R &= ~0xFF;       // (c) PD como entrada
    GPIO_PORTD_AHB_DEN_R |= 0xFF;        //     PD se habilita
    GPIO_PORTD_AHB_PUR_R |= 0xFF;        //     habilita weak pull-up on PD
    GPIO_PORTD_AHB_IS_R &= ~0xFF;        // (d) PD es sensible por flancos
    GPIO_PORTD_AHB_IBE_R &= ~0xFF;       //     PD no es sensible a dos flancos
    GPIO_PORTD_AHB_IEV_R &= ~0xFF;       //     PD detecta eventos de flanco de bajada
    GPIO_PORTD_AHB_ICR_R = 0xFF;         // (e) limpia la bandera
    GPIO_PORTD_AHB_IM_R &= ~0xFF;         // (f) Enmascara la interrupcion PD y se envia al controlador de interrupciones
    //PORTD tiene la interrupcion 3
    NVIC_PRI1_R = (NVIC_PRI1_R&0x00FFFFFF)|0x80000000; // (g) prioridad 4  (pag 159)
    NVIC_EN0_R= 1<<(3-0);          //(h) habilita la interrupción 3 en NVIC (Pag. 154)

    //******* Puerto L de entrada *******
      GPIO_PORTL_DIR_R &= ~0x0F;       // (c) PL como entrada
      GPIO_PORTL_DEN_R |= 0x0F;        //     PL se habilita
      GPIO_PORTL_PUR_R |= 0x0F;        //     habilita weak pull-up on PL
      GPIO_PORTL_IS_R &= ~0x0F;        // (d) PL es sensible por flancqs
      GPIO_PORTL_IBE_R &= ~ 0x0F;       //     PL no es sensible a dos flancos
      GPIO_PORTL_IEV_R &= ~0x0F;       //     PL detecta eventos de flanco de bajada
      GPIO_PORTL_ICR_R = 0x0F;         // (e) limpia la bandera
      GPIO_PORTL_IM_R &= ~0x0F;         // (f) Enmascara la interrupcion PL y se envia al controlador de interrupciones
      //PORTD tiene la interrupcion 53
      NVIC_PRI13_R = (NVIC_PRI13_R&0xFFFF00FF)|0x00008000; // (g) prioridad 4  (pag 159)
      NVIC_EN1_R= 1<<(53-32);          //(h) habilita la interrupción 53 en NVIC (Pag. 154)
}


//-------------------------------------------------- Inicio de secuencia ADC ---------------------------------------------------
void sequenceInit(void){
    muestrasADC=muestrasADC+1;
    ADC0_PSSI_R = 0x0004;            //Inicia conversión del SS2
    while((ADC0_RIS_R&0x04)==0);     // Espera a que SS2 termine conversión (polling)
    ADC0_ISC_R = 0x00000004;   //Limpia bandera de interrupcion de SS2
}

//-------------------------------------------------- INTERRUPCIONES ---------------------------------------------------

//-------------------------------------------------- UART7_Handler ---------------------------------------------------


void UART7_Handler(void)
{
    uartinterr = uartinterr + 1;

    data[0] = UART7_DR_R&0xFF;// Se guarda el primer valor de FIFO
    data[1] = UART7_DR_R&0xFF;// Se guarda el segundo valor de FIFO
    data[2] = UART7_DR_R&0xFF;// Se guarda el tercer valor de FIFO
    data[3] = UART7_DR_R&0xFF;// Se guarda el cuarto valor de FIFO

    if(modo==1 && data[0]==31){    // Desactiva modo bocina
        TIMER3_CTL_R &= ~0X00000001; // Deshabilita Timer 3 (p.986)
        TIMER2_CTL_R |= 0X00000001;  // Habilita Timer 2 (p.986)
    }

    if(modo==1 && data[0]==32){     // Activa modo bocina
        TIMER3_TAILR_R= 16000000/(frecsel*32)-65;  // Carga del contador de frecuencia del dato de SPI. (DAC)
        TIMER2_CTL_R &= ~0X00000001;  // Deshabilita Timer 2 (p.986)
        TIMER3_CTL_R |= 0X00000001; // Habilita Timer 3 (p.986)
    }

    switch(data[0]){    // Selección de modo

    case 0x01:  // Modo voz
        modo=1; // Bandera de modo 1
        SSD1306_Clear();    // Limpieza de pantalla
        vozInit();  // Interfaz de modo voz
        nota=32;    // Nota seleccionada
        octava=16;   // Octava seleccionada
        semi=12;    // Semitono seleccionado
        DatosDisplay(); // Escritura en pantalla de nota seleccionada
        TIMER2_CTL_R |= 0X00000001; // Habilita Timer 2(p.986)
        break;

    case 0x02:  // Modo piano
        modo=2; // Bandera modo piano
        SSD1306_Clear();    // Limpieza de pantalla
        pianoInit();    // Interfaz de modo piano
        nota=55;    // Nota seleccionada
        octava=5;   // Octava seleccionada
        semi=13;    // Semitono seleccionado
        DatosDisplay(); // Escritura en pantalla de nota seleccionada
        break;

    case 0x03:  // Regreso al menu principal
        modo=0; // Bandera de regreso al menú
        SSD1306_Clear();    // Limpieza de pantalla
        //UARTInit();
        //I2CInit();
        displayInit();
        //ADCInit();
        TimersInit();
        GPIO_Init();

        break;
    }

    UART7_ICR_R = 0x00000010;      // Limpia bandera de interrupcion
}


//-------------------------------------------------- ADCSequence2_Handler ---------------------------------------------------

// Se activa cuando el comparador digital recibe muestras mayores al umbral de histéresis (1.3-1.5 [V])
void ADCSequence2_Handler(void){
    // Switch para conocer el primer cruce por cero y el segundo para obtener frecuencia de señal recibida por microfono
    switch (adc0interr){
    case 0: // Primer cruce por cero
        adc0interr=adc0interr+1;
        TIMER0_TAV_R = 0X00000000;  // Inicio de cuenta de Timer 0 igual a cero
        TIMER0_CTL_R = 0X00000001;  // Habilita Timer 0
    break;

    default:    // Segundo cruce por cero
        periodo = TIMER0_TAV_R; // Cuenta del Timer 0 en el tiempo de un periodo
        TIMER0_CTL_R = 0X00000000;  // Deshabilita Timer 0
        if((16000000/80)>=periodo & periodo >= (16000000/1350)){    // Filtro de mediciones dentro del intervalo esperado
            adcflag=adcflag+1;
            frecuencia=16000000/periodo;    // Obtiene frecuencia a partir de la cuenta y la frecuencia de reloj.
            sumafrec=sumafrec+frecuencia;   // Suma las frecuencias para obtener promedio
        }

        adc0interr=0;

        if  (adcflag==100){ // Se obtienen 100 muestras de frecuencia
            frecprom=sumafrec/adcflag;  // Se obtiene el promedio
            sumafrec=0; // Reinicia variables de promedio
            adcflag=0;

            // Escritura en pantalla a partir de frecuencia recibida y esperada
            if(frecprom<(frecsel-10)){
                SSD1306_Command(SSD1306_SETPAGE2);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MAS ALTO   ");
            }

            else if(frecprom<=(frecsel+10) & frecprom>=(frecsel-10)){
                SSD1306_Command(SSD1306_SETPAGE2);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("EXCELENTE");
            }

            else if(frecprom>(frecsel+10)){
                SSD1306_Command(SSD1306_SETPAGE2);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MAS BAJO   ");
            }
        }

    }
    ADC0_DCRIC_R |= 0x01;       // Condiciones iniciales de CD0
    ADC0_ISC_R = 0x00040000;    // Limpia bandera de interrupcion de SS2
    ADC0_DCISC_R |= 0x01;       // Limpia bandera de interrupcion de CD0
}

//-------------------------------------------------- Timer 2A Handler ---------------------------------------------------

// Se activa con modo voz (UART data[0]=1) y timeout
void Timer02AIntHandler(void){
    // La frecuencia reciiba se encuentra dentro de un intervalo esperado
    sequenceInit(); // Inicia secuencia de muestreo
    if(frecprom>=frecsel-10 && frecprom<=frecsel+10){
        TIMER2_CTL_R &= ~0X00000001; //Deshabilita Timer 2 (Detiene secuencia de muestreo)(p.986)
        for(i=0;i<5000000;i++);     // Retardo de 5 seg.
        frecprom=0;             // Reinicia valor de frecuencia esperada
        modo=0;
        SSD1306_Clear();    // Limpieza de pantalla
        //UARTInit();
        //I2CInit();
        displayInit();  // Inicializacion de menu principal en pantalla
        //ADCInit();
        TimersInit();   // Reincio y deshabilitacion de Timers
        //prueba=prueba+1;
    }

    TIMER2_ICR_R= 0X00000001 ; //Limpia bandera de Timer 2
}

//-------------------------------------------------- Timer 3A Handler ---------------------------------------------------

// Se activa con modo bocina (UART data[0]=32) y timeout
void Timer03AIntHandler(void){
    inter=inter+1;
    if(samples==32){    // Reinicia arreglo de muestras de señal seno
        samples=0;
    }
    SSI0_sendData((int)seno[samples]); // Se envían coeficientes a SPI para iniciar proceso DAC
    samples=samples+1;  // Recorre el arreglo de muestrasa de señal seno
    TIMER3_ICR_R= 0X00000001 ; //LIMPIA BANDERA DE TIMER3
}


//-------------------------------------------------- Port D Handler ---------------------------------------------------

void GPIOPortD_Handler(void){
    //for(i=0;i<0xFFFFF;i++){}

    pdint=pdint+1;

    switch(nota){
    // Selección de nota (frecuencia fundamental)
        case 32:
            if(GPIO_PORTD_AHB_DATA_R==0x7E && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN      ");
                TIMER3_TAILR_R= 16000000/(frecsel*32)-65;  // Carga del contador de frecuencia del dato de SPI. (DAC)
                TIMER3_CTL_R |= 0X00000001; // Habilita Timer 3 (p.986)
                for(i=0;i<6000000;i++);
                TIMER3_CTL_R &= ~0X00000001; // Deshabilita Timer 3 (p.986)
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();

            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

            break;

        case 36:
            if(GPIO_PORTD_AHB_DATA_R==0x02 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

            break;

        case 41:
            if(GPIO_PORTD_AHB_DATA_R==0x04 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

            break;

        case 43:
            if(GPIO_PORTD_AHB_DATA_R==0x08 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

            break;

        case 48:
            if(GPIO_PORTD_AHB_DATA_R==0x10 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 55:
            a=a+1;
            if(GPIO_PORTD_AHB_DATA_R == 0x5F && GPIO_PORTL_DATA_R == 0x0F){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN      ");
                TIMER3_TAILR_R= 16000000/(frecsel*32)-65;  // Carga del contador de frecuencia del dato de SPI. (DAC)
                TIMER3_CTL_R |= 0X00000001; // Habilita Timer 3 (p.986)
                for(i=0;i<5000000;i++);
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
                TIMER3_CTL_R &= ~0X00000001; // Deshabilita Timer 3 (p.986)
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

            break;

        case 61:
            if(GPIO_PORTD_AHB_DATA_R==0x40 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }
            break;

        case 62:
            if(GPIO_PORTD_AHB_DATA_R==0x80 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

        break;

        case 63:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x01){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

        break;

        case 64:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x02){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

        break;

        case 65:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x04){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

        break;

        case 66:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x08){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");
            }

        break;
    }


    GPIO_PORTD_AHB_ICR_R = 0xFF;
}


//-------------------------------------------------- Port L Handler ---------------------------------------------------

void GPIOPortL_Handler(void){
    for(i=0;i<0xFFFFF;i++){}

    plint=plint+1;

    switch(nota){
        // Selección de nota (frecuencia fundamental)
        case 32:
            if(GPIO_PORTD_AHB_DATA_R==0x01 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 36:
            if(GPIO_PORTD_AHB_DATA_R==0x02 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 41:
            if(GPIO_PORTD_AHB_DATA_R==0x04 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 43:
            if(GPIO_PORTD_AHB_DATA_R==0x08 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 48:
            if(GPIO_PORTD_AHB_DATA_R==0x10 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 55:
            if(GPIO_PORTD_AHB_DATA_R==0x20 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

            break;

        case 61:
            if(GPIO_PORTD_AHB_DATA_R==0x40 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }
            break;

        case 62:
            if(GPIO_PORTD_AHB_DATA_R==0x80 && GPIO_PORTL_DATA_R==0x00){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

        break;

        case 63:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x01){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

        break;

        case 64:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x02){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

        break;

        case 65:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x04){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

        break;

        case 66:
            if(GPIO_PORTD_AHB_DATA_R==0x00 && GPIO_PORTL_DATA_R==0x08){
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("MUY BIEN");
                modo=0;
                SSD1306_Clear();    // Limpieza de pantalla
                displayInit();  // Inicializacion de menu principal en pantalla
                //TimersInit();   // Reincio y deshabilitacion de Timers
                GPIO_Init();
            } else {
                SSD1306_Command(SSD1306_SETPAGE3);
                SSD1306_Command(0x01);
                SSD1306_Command(0x14);
                SSD1306_WriteString("INCORRECTO");

            }

        break;
    }


    GPIO_PORTL_ICR_R = 0x0F;
}



//-------------------------------------------------- OTRAS FUNCIONES ---------------------------------------------------


void DatosDisplay(void)
{
    // Datos para pantalla a partir de nota seleccionada
    switch(nota){
    // Selección de nota (frecuencia fundamental)
        case 32:
            Nota_C(2,0x01, 0x12);   // Escritura de nota en display
            frecsel=nota+0.7032;    // Aumento de parte decimal para mayor precision
            break;
        case 36:
            Nota_D(2,0x01, 0x12);
            frecsel=nota+0.7081;
            break;
        case 41:
            Nota_E(2,0x01, 0x12);
            frecsel=nota+0.2035;
            break;
        case 43:
            Nota_F(2,0x01, 0x12);
            frecsel=nota+0.6536;
            break;
        case 48:
            Nota_G(2,0x01, 0x12);
            frecsel=nota+0.9995;
            break;
        case 55:
            Nota_A(2,0x01, 0x12);
            frecsel=nota;
            break;
        case 61:
            Nota_B(2,0x01, 0x12);
            frecsel=nota+0.7354;
        break;
    }
    // Selección de octava
    switch(octava){
        case 1: // Octava 1
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("1");   // Escritura de octava en display
            break;
        case 2: // Octava 2
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("2");
            break;
        case 4: // Octava 3
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("3");
            break;
        case 8: // Octava 4
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("4");
            break;
        case 16: // Octava 5
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("5");
            break;
        case 32: // Octava 6
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("6");
            break;
        case 64: // Octava 7
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("7");
            break;
        case 128: // Octava 8
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("8");
            break;
        case 256: // Octava 9
            SSD1306_Command(SSD1306_SETPAGE3);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("9");
            break;
    }
    // Seleccion de aumento o disminucion de semitono
    switch(semi){
        case 11:    // Caso sostenido
            Nota_sostenido(2,0x0B, 0x12);
            frecsel=frecsel*1.059463094;    // Aumento de semitono
            break;
        case 12:    // Caso bemol
            Nota_bemol(2,0x0B, 0x12);
            frecsel=frecsel*0.9438743127;   // Disminucion de semitono
            break;
        case 13:    // Caso natural
            SSD1306_Command(SSD1306_SETPAGE2);
            SSD1306_Command(0x0B);
            SSD1306_Command(0x12);
            SSD1306_WriteString("  ");  // Se mantiene el tono
            break;
    }
    frecsel=round(frecsel*octava);  // Obtencion de nota en su resspectiva octava
}

