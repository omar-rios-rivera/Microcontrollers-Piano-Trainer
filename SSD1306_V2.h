/*
 * SSD1306_V2.h
 *
 *  Created on: june 2022
 *      Author: Omar Rios Rivera
 */


/*********************************************************************
LIESE           LIESE           LIESE           LIESE           LIESE
***********************************************************************/

/************************************************
                   SSD1306.h

               Created on: 15 may 2022
       Author: Rosas Garcia Angel Joshua
 **************************************************/

#ifndef SSD1306_V2_H_
#define SSD1306_V2_H_

//REGISTROS DE RELOJ
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608)) //Reloj del puerto
#define SYSCTL_RCGCI2C_R        (*((volatile uint32_t *)0x400FE620)) //Reloj de I2C
#define SYSCTL_PRGPIO_R        (*((volatile uint32_t *)0x400FEA08)) //Bandera de "Peripherial Ready"

//REGISTROS DEL PUERTO B
#define GPIO_PORTB_DATA_R   (*((volatile uint32_t *)0x400593FC)) //Para los datos del puerto
#define GPIO_PORTB_DIR_R    (*((volatile uint32_t *)0x40059400)) //Para seleccionar funciÃƒÂ³n
#define GPIO_PORTB_AFSEL_R  (*((volatile uint32_t *)0x40059420)) //Para seleccionar funciÃƒÂ³n alterna
#define GPIO_PORTB_ODR_R    (*((volatile uint32_t *)0x4005950C)) //Para activar el Open Drain
#define GPIO_PORTB_DEN_R    (*((volatile uint32_t *)0x4005951C)) //Para activar funciÃƒÂ³n digital
#define GPIO_PORTB_PCTL_R   (*((volatile uint32_t *)0x4005952C)) //Para el control del puerto
#define GPIO_PORTB_PUR_R    (*((volatile uint32_t *)0x40059510)) //Registro de pull-up PJ
//REGISTROS DEL MÃƒâ€œUDLO I2C
#define I2C0_MSA_R              (*((volatile uint32_t *)0x40020000)) //I2C Master Slave Adress
#define I2C0_MCS_R              (*((volatile uint32_t *)0x40020004)) //I2C Master Control Status
#define I2C0_MDR_R              (*((volatile uint32_t *)0x40020008)) //I2C Master Data Register
#define I2C0_MTPR_R             (*((volatile uint32_t *)0x4002000C)) //I2C Master Time Period
#define I2C0_MCR_R              (*((volatile uint32_t *)0x40020020)) //I2C Master Congirutation Register


/* El registro I2C Master Control/Status (I2C_MCS_R) tiene:
-Modo READ-ONLY DATUS: los 7 bits menos significativos son:
    7:Clock Time Out Error  6:BUS BUSY      5:IDLE
    4:Arbitration Lost      3:DataAck       2:AdrAck
    1:Error                 0:CONTROLLER BUSY

-Modo WRITE-ONLY CONTROL_ Los 6 bits menos significativos son:
    6:BURST    5:QuickCommand  4:High Speed Enable
    3:ACK      2:STOP          1:START
    0:RUN */

//DEFINICION DE BITS DEL I2C Master Control Status
#define I2C_MCS_ACK 0x00000008 //Transmmitter Acknowledge Enable
#define I2C_MCS_DATACK 0x00000008 // Data Acknowledge Enable
#define I2C_MCS_ADRACK 0x00000004 // Acknowledge Address
#define I2C_MCS_STOP 0x00000004 // Generate STOP
#define I2C_MCS_START 0x00000002 // Generate START
#define I2C_MCS_ERROR 0x00000002 // Error
#define I2C_MCS_RUN 0x00000001 // I2C Master Enable
#define MAXRETRIES 5 // number of receive attempts before giving up
//DEFINICIONES DE SSD1306
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERSEDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet
#define SSD1306_SETPAGE0 0XB0
#define SSD1306_SETPAGE1 0XB1
#define SSD1306_SETPAGE2 0XB2
#define SSD1306_SETPAGE3 0XB3
#define SSD1306_SETPAGE4 0XB4

#define SSD1306_SCROLLINGL_ 0X27
#define SSD1306_SCROLLINGR_ 0X26
//SSD1306
int AdreSSD1306 = 0x3C;
int ControlbC = 0x00;
int ControlbD = 0x40;

/*El cÃƒÂ¡lculo del Time Period Register (TPR) se especifica en la pÃƒÂ¡gina 1284
 Asumiendo un reloj de 16 MHz y un modo de operaciÃƒÂ³n estÃƒÂ¡ndar (100 kbps):*/
int TPR = 1;
uint8_t error;
uint32_t i;
/*********************************************
        DECLARACION DE FUNCIONES
**********************************************/
void SSD1306_Init(void);
void I2C_Init(void);
void SSD1306_Command(int comand);
void SSD1306_Data(int dat);
void SSD1306_SCROLLINGL(void);
void SSD1306_WriteString(unsigned char *cadena);

//Letras
void Letra_A(void);
void Letra_B(void);
void Letra_C(void);
void Letra_D(void);
void Letra_E(void);
void Letra_F(void);
void Letra_G(void);
void Letra_H(void);
void Letra_I(void);
void Letra_J(void);
void Letra_K(void);
void Letra_L(void);
void Letra_M(void);
void Letra_O(void);
void Letra_P(void);
void Letra_Q(void);
void Letra_R(void);
void Letra_S(void);
void Letra_T(void);
void Letra_U(void);
void Letra_V(void);
void Letra_W(void);
void Letra_X(void);
void Letra_Y(void);
void Letra_Z(void);

//Caracteres Especiales
void Espacio (void);
void Grado(void);
void Punto(void);
void dosPuntos(void);
void DosPuntos(void);

//Notas
void Nota_A(int page,int coll,int colh);
void Nota_B(int page,int coll,int colh);
void Nota_C(int page,int coll,int colh);
void Nota_D(int page,int coll,int colh);
void Nota_E(int page,int coll,int colh);
void Nota_F(int page,int coll,int colh);
void Nota_G(int page,int coll,int colh);
void Nota_bemol(int page,int coll,int colh);
void Nota_sostenido(int page,int coll,int colh);
void Letra_PZOTA(void);

//Figuras
void Figura_redonda(int page,int coll,int colh);
void Figura_blanca(int page,int coll,int colh);
void Figura_blancacp(int page,int coll,int colh);
void Figura_negra(int page,int coll,int colh);
void Figura_negracp(int page,int coll,int colh);
void Figura_corchea(int page,int coll,int colh);
void Figura_semicorchea(int page,int coll,int colh);

//Numeros
void Letra_0(void);
void Letra_1(void);
void Letra_2(void);
void Letra_3(void);
void Letra_4(void);
void Letra_5(void);
void Letra_6(void);
void Letra_7(void);
void Letra_8(void);
void Letra_9(void);

//
void SSD1306_WriteChar(char C);
void SSD1306_WriteString(unsigned char *cadena);

/***************************************************************
        FUNCION QUE INICIALIZA LOS RELOJES, EL GPIO E I2C
***************************************************************/
void I2C_Init(void){
    //CONFIGURACIÃƒâ€œN DE LOS RELOJ
    SYSCTL_RCGCI2C_R |= 0x0001; // Activamos el reloj de I2C0 [I2C9 I2C8 I2C7 ... I2C0]<--Mapa de RCGCI2C
    SYSCTL_RCGCGPIO_R |= 0x0002; // Activamos el reloj GPIO_PORTB mientras se activa el reloj de I2C0
    while((SYSCTL_PRGPIO_R&0x0002) == 0){};//Espero a que se active el reloj del puerto B

    /* Acorde con la tabla "Signals by function" de la p. 1808:
     el PIN 2 del puerto B (PB2) es el I2C0SCL del I2C0, y
     el PIN 3 del pue+3) es el I2C0SDA del I2C0 */

    //CONFIGURACIÃƒâ€œN DE LOS GPIOS
    GPIO_PORTB_AFSEL_R |= 0x0C; // Activo la funciÃƒÂ³n alterna del PB2 y PB3
    GPIO_PORTB_ODR_R |= 0x08;   // Activo el OPEN DRAIN para el PB3, ya que el PB2 ya tiene uno por preconfig.
    GPIO_PORTB_DIR_R |= 0x0C;   //Activo al PB2 y al PB3 como OUTPUT
    GPIO_PORTB_DEN_R |= 0x0C;   //Activo la funciÃƒÂ³n digital de PB3 y PB2

   // GPIO_PORTB_PUR_R |= 0x0C;        //     habilita weak pull-up on PB3 Y PB2
    /*AsÃƒÂ­ como el registro AFSEL indica que se ejecutarÃƒÂ¡ una funciÃƒÂ³n externa, en el registro PCTL
    debemos indicar QUÃƒâ€° funciÃƒÂ³n alterna se realizarÃƒÂ¡ acorde con la tabla 26-5 de la p.1808 e indicarlo
     en el correspondiente PCMn (uno por cada bit del puerto) del registro PCTL*/

    GPIO_PORTB_PCTL_R|=0x00002200;

    //CONFIGURACIÃƒâ€œN DEL MODULO I2C0
    I2C0_MCR_R = 0x00000010; // Habilitar funciÃƒÂ³n MASTER para el I2C0
    I2C0_MTPR_R = TPR; // Se establece una velocidad estÃƒÂ¡ndar de 100kbps
}
/***************************************************************
      FUNCION PARA INICIALIZAR SSD1306
***************************************************************/
void SSD1306_Init(void){

    for(i=0;i<400;i++){}
SSD1306_Command(SSD1306_DISPLAYOFF);
SSD1306_Command(SSD1306_SETMULTIPLEX);
SSD1306_Command(0x1F);
SSD1306_Command(SSD1306_SETDISPLAYOFFSET);
SSD1306_Command(0x00);
SSD1306_Command(SSD1306_SETSTARTLINE);
SSD1306_Command(SSD1306_SEGREMAP|0x1);
SSD1306_Command(SSD1306_COMSCANDEC);
SSD1306_Command(SSD1306_SETCOMPINS);
SSD1306_Command(0x02);
SSD1306_Command(SSD1306_SETCONTRAST);
SSD1306_Command(0x8F);
SSD1306_Command(SSD1306_DISPLAYALLON_RESUME);
SSD1306_Command(SSD1306_INVERSEDISPLAY);
SSD1306_Command(SSD1306_SETDISPLAYCLOCKDIV);
SSD1306_Command(0x80);
SSD1306_Command(SSD1306_CHARGEPUMP);
SSD1306_Command(0x14);
SSD1306_Command(SSD1306_DISPLAYON);

}
/***************************************************************
      FUNCION ESPERAR, MONITOREO DE EL BIT BUSY
***************************************************************/
int esperar(){
    while(I2C0_MCS_R&0x00000001){}; //Espero a que la transmisiÃƒÂ³n acabe
      if(I2C0_MCS_R&0x00000002==1){ //Ã‚Â¿Hubo error?
          error=1;
          return error;
      };
      return 0;
}
/***************************************************************
      FUNCION COMANDO, RECIBE UN ENTERO
Se crea una trama de datos correspondiente a cada comando
***************************************************************/
void SSD1306_Command(int command){

    while(I2C0_MCS_R&0x00000001){};//i2C RADY?
    //Para Transmitir
    I2C0_MSA_R=(AdreSSD1306<<1)&0xFE;
    I2C0_MDR_R=ControlbC&0x0FF;
    I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_START); // CondiciÃƒÂ³n de START y corro

    for(i=0;i<300;i++){} //Delay
    esperar();

    I2C0_MDR_R=command;
    I2C0_MCS_R=(I2C_MCS_STOP|I2C_MCS_RUN); //Inicio la ultima transmisiÃƒÂ³n y STOP
    for(i=0;i<300;i++){} //Delay
    esperar();
}


void SSD1306_Data(int dat){
    while(I2C0_MCS_R&0x00000001){};//i2C RADY?
        //Para Transmitir
        I2C0_MSA_R=(AdreSSD1306<<1)&0xFE; //Cargo la direcciÃƒÂ³n del DS1307 e indico "SEND", es decir, el Slave va a recibir
        I2C0_MDR_R=ControlbD&0x0FF;
        I2C0_MCS_R=(I2C_MCS_RUN|I2C_MCS_START); // CondiciÃƒÂ³n de START y corro

        for(i=0;i<300;i++){} //Delay
        esperar();

        I2C0_MDR_R=dat;
        I2C0_MCS_R=(I2C_MCS_STOP|I2C_MCS_RUN); //Inicio la ultima transmisiÃƒÂ³n y STOP
        for(i=0;i<300;i++){} //Delay
        esperar();

}
//LETRAS
void Letra_A(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x12);
    SSD1306_Data(0x12);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_B(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_C(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x42);
    SSD1306_Data(0x42);
    SSD1306_Data(0x00);
}
void Letra_D(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x42);
    SSD1306_Data(0x3C);
    SSD1306_Data(0x00);
}

void Letra_E(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x00);
}
void Letra_F(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x32);
    SSD1306_Data(0x32);
    SSD1306_Data(0x02);
    SSD1306_Data(0x00);
}
void Letra_G(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x52);
    SSD1306_Data(0x72);
    SSD1306_Data(0x00);
}
void Letra_H(void){
       SSD1306_Data(0x7E);
       SSD1306_Data(0x08);
       SSD1306_Data(0x08);
       SSD1306_Data(0x7E);
       SSD1306_Data(0x00);
}
void Letra_I(void){
    SSD1306_Data(0x42);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x00);
}
void Letra_J(void){
    SSD1306_Data(0x42);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x02);
    SSD1306_Data(0x00);
}
void Letra_K(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x18);
    SSD1306_Data(0x24);
    SSD1306_Data(0x42);
    SSD1306_Data(0x00);
}

void Letra_L(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x40);
    SSD1306_Data(0x40);
    SSD1306_Data(0x40);
    SSD1306_Data(0x00);
}
void Letra_M(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x04);
    SSD1306_Data(0x08);
    SSD1306_Data(0x04);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
    //6 julio

}
void Letra_N(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x18);
    SSD1306_Data(0x30);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);

}
void Letra_O(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x42);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_P(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x0A);
    SSD1306_Data(0x0A);
    SSD1306_Data(0x0E);
    SSD1306_Data(0x00);
}
void Letra_Q(void){
    SSD1306_Data(0x3E);
    SSD1306_Data(0x22);
    SSD1306_Data(0x22);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_R(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x1A);
    SSD1306_Data(0x2A);
    SSD1306_Data(0x4E);
    SSD1306_Data(0x00);
}
void Letra_S(void){
    SSD1306_Data(0x4E);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x7A);
    SSD1306_Data(0x00);
}
void Letra_T(void){
    SSD1306_Data(0x02);
    SSD1306_Data(0x02);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x02);
    SSD1306_Data(0x02);
    SSD1306_Data(0x00);
}
void Letra_U(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x40);
    SSD1306_Data(0x40);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_V(void){

    SSD1306_Data(0x1E);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x1E);
    SSD1306_Data(0x00);

}
void Letra_W(void){
    SSD1306_Data(0x7E);
    SSD1306_Data(0x20);
    SSD1306_Data(0x10);
    SSD1306_Data(0x20);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}
void Letra_X(void){
SSD1306_Data(0x66);
SSD1306_Data(0x18);
SSD1306_Data(0x18);
SSD1306_Data(0x66);
SSD1306_Data(0x00);
}
void Letra_Y(void){
    SSD1306_Data(0x0E);
    SSD1306_Data(0x70);
    SSD1306_Data(0x0E);
    SSD1306_Data(0x00);
}

void Letra_Z(void){
    SSD1306_Data(0x62);
    SSD1306_Data(0x52);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x46);
    SSD1306_Data(0x00);
}

void Numero_0(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x42);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Numero_1(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x02);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Numero_2(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x7A);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x4E);
    SSD1306_Data(0x00);
}

void Numero_3(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x42);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Numero_4(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x0E);
    SSD1306_Data(0x08);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Numero_5(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x4E);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x7A);
    SSD1306_Data(0x00);
}

void Numero_6(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x4A);
    SSD1306_Data(0x7A);
    SSD1306_Data(0x00);
}

void Numero_7(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x02);
    SSD1306_Data(0x0A);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Numero_8(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x5A);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Numero_9(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x0E);
    SSD1306_Data(0x0A);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x00);
}

void Nota_A(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFC);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0xFC);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x03);
    SSD1306_Data(0x03);
    SSD1306_Data(0x03);
    SSD1306_Data(0x03);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x00);
}

void Nota_B(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFC);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0xFC);
    SSD1306_Data(0xF8);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x63);
    SSD1306_Data(0x63);
    SSD1306_Data(0x63);
    SSD1306_Data(0x63);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x3E);
    SSD1306_Data(0x00);
}

void Nota_C(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFC);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x3F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x00);
}

void Nota_D(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x3F);
    SSD1306_Data(0x00);
}

void Nota_E(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0x06);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x00);
}

void Nota_F(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0xC6);
    SSD1306_Data(0x06);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Nota_G(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x67);
    SSD1306_Data(0x63);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x3F);
    SSD1306_Data(0x00);
}

void Nota_sostenido(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x48);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x24);
    SSD1306_Data(0x7E);
    SSD1306_Data(0x12);
}

void Nota_bemol(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x7F);
    SSD1306_Data(0x24);
    SSD1306_Data(0x14);
    SSD1306_Data(0x08);
    SSD1306_Data(0x00);
}

void Letra_PZOTA(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0xFE);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x66);
    SSD1306_Data(0x66);
    SSD1306_Data(0x66);
    SSD1306_Data(0x66);
    SSD1306_Data(0x66);
    SSD1306_Data(0x3C);
    SSD1306_Command(SSD1306_SETPAGE1);
    SSD1306_Command(0x00);
    SSD1306_Command(0x10);
    SSD1306_Data(0x00);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

//Figuras
void Figura_redonda(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xC0);
    SSD1306_Data(0x20);
    SSD1306_Data(0x10);
    SSD1306_Data(0x08);
    SSD1306_Data(0x08);
    SSD1306_Data(0x08);
    SSD1306_Data(0x10);
    SSD1306_Data(0x20);
    SSD1306_Data(0xC0);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x01);
    SSD1306_Data(0x01);
    SSD1306_Data(0x07);
    SSD1306_Data(0x09);
    SSD1306_Data(0x11);
    SSD1306_Data(0x21);
    SSD1306_Data(0x21);
    SSD1306_Data(0x21);
    SSD1306_Data(0x11);
    SSD1306_Data(0x09);
    SSD1306_Data(0x07);
    SSD1306_Data(0x01);
    SSD1306_Data(0x01);
}

void Figura_blanca(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x60);
    SSD1306_Data(0x50);
    SSD1306_Data(0x90);
    SSD1306_Data(0x88);
    SSD1306_Data(0x48);
    SSD1306_Data(0x68);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Figura_blancacp(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x60);
    SSD1306_Data(0x50);
    SSD1306_Data(0x90);
    SSD1306_Data(0x88);
    SSD1306_Data(0x48);
    SSD1306_Data(0x68);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Figura_negra(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x60);
    SSD1306_Data(0x70);
    SSD1306_Data(0xF0);
    SSD1306_Data(0xF8);
    SSD1306_Data(0x78);
    SSD1306_Data(0x78);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Figura_negracp(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFC);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);

    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x60);
    SSD1306_Data(0x70);
    SSD1306_Data(0xF0);
    SSD1306_Data(0xF8);
    SSD1306_Data(0x78);
    SSD1306_Data(0x78);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x0C);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Figura_corchea(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x04);
    SSD1306_Data(0x08);
    SSD1306_Data(0x10);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x60);
    SSD1306_Data(0x70);
    SSD1306_Data(0xF0);
    SSD1306_Data(0xF8);
    SSD1306_Data(0x78);
    SSD1306_Data(0x78);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}

void Figura_semicorchea(int page,int coll,int colh){
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0xFE);
    SSD1306_Data(0x24);
    SSD1306_Data(0x48);
    SSD1306_Data(0x90);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    switch (page){
    case 0:
        SSD1306_Command(SSD1306_SETPAGE1);
        break;
    case 1:
        SSD1306_Command(SSD1306_SETPAGE2);
        break;
    case 2:
        SSD1306_Command(SSD1306_SETPAGE3);
        break;
    case 3:
        SSD1306_Command(SSD1306_SETPAGE0);
        break;
    }
    SSD1306_Command(coll);
    SSD1306_Command(colh);
    SSD1306_Data(0x00);
    SSD1306_Data(0x60);
    SSD1306_Data(0x70);
    SSD1306_Data(0xF0);
    SSD1306_Data(0xF8);
    SSD1306_Data(0x78);
    SSD1306_Data(0x78);
    SSD1306_Data(0x1F);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}



void Grado(void){
    SSD1306_Data(0x07);
    SSD1306_Data(0x05);
    SSD1306_Data(0x07);
    SSD1306_Data(0x00);
}

void Punto(void){
    SSD1306_Data(0x60);
    SSD1306_Data(0x60);
    SSD1306_Data(0x00);
}

void dosPuntos(void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x66);
    SSD1306_Data(0x66);
    SSD1306_Data(0x00);
}

void DosPuntos(void){
    SSD1306_Data(0x06);
    SSD1306_Data(0x06);
    SSD1306_Data(0x00);
}

void Espacio (void){
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
    SSD1306_Data(0x00);
}
void SSD1306_Clear(void){
    int c;
    SSD1306_Command(SSD1306_SETPAGE0);
for(c=0;c<128;c++){
    SSD1306_Data(0x00);
}
SSD1306_Command(SSD1306_SETPAGE1);
for(c=0;c<128;c++){
    SSD1306_Data(0x00);
}
SSD1306_Command(SSD1306_SETPAGE2);
for(c=0;c<128;c++){
    SSD1306_Data(0x00);
}
SSD1306_Command(SSD1306_SETPAGE3);
for(c=0;c<128;c++){
    SSD1306_Data(0x00);}
SSD1306_Command(SSD1306_SETPAGE0);
}


void SSD1306_SCROLLINGL(void){

SSD1306_Command(SSD1306_SCROLLINGL_);
SSD1306_Command(0x00);
SSD1306_Command(0x00);
SSD1306_Command(0x00);
SSD1306_Command(0x03);
SSD1306_Command(0x00);
SSD1306_Command(0xFF);
SSD1306_Command(0x2F);


}


void SSD1306_SCROLLINGR(void){
SSD1306_Command(SSD1306_SCROLLINGR_);
SSD1306_Command(0x00);
SSD1306_Command(0x00);
SSD1306_Command(0x00);
SSD1306_Command(0x03);
SSD1306_Command(0x00);
SSD1306_Command(0xFF);
SSD1306_Command(0x2F);
}

void SSD1306_WriteChar(char C){

    switch (C){

    //Letras
    case 'A':
Letra_A();
    break;
    case 'B':
Letra_B();
    break;
    case 'C':
Letra_C();
    break;
    case 'D':
Letra_D();
    break;
    case 'E':
Letra_E();
    break;
    case 'F':
Letra_F();
    break;
    case 'G':
Letra_G();
    break;
    case 'H':
Letra_H();
    break;
    case 'I':
Letra_I();
    break;
    case 'J':
Letra_J();
    break;
    case 'K':
Letra_K();
    break;
    case 'L':
Letra_L();
    break;
    case 'M':
Letra_M();
    break;
    case 'N':
Letra_N();
    break;
    case 'O':
Letra_O();
    break;
    case 'P':
Letra_P();
    break;
    case 'Q':
Letra_Q();
    break;
    case 'R':
Letra_R();
    break;
    case 'S':
Letra_S();
    break;
    case 'T':
Letra_T();
    break;
    case 'U':
Letra_U();
    break;
    case 'V':
Letra_V();
    break;
    case 'W':
Letra_W();
    break;
    case 'X':
Letra_X();
    break;
    case 'Y':
Letra_Y();
    break;
    case 'Z':
Letra_Z();
    break;

    //Números
    case '1':
Numero_1();
    break;
    case '2':
Numero_2();
    break;
    case '3':
Numero_3();
    break;
    case '4':
Numero_4();
    break;
    case '5':
Numero_5();
    break;
    case '6':
Numero_6();
    break;
    case '7':
Numero_7();
    break;
    case '8':
Numero_8();
    break;
    case '9':
Numero_9();
    break;
    case '0':
Numero_0();
    break;

    //Caracteres Especiales
    case '.':
Punto();
    break;
    case '*':
DosPuntos();
    break;
    case ':':
    dosPuntos();
    break;
    case '$':
Grado();
    break;
    case ' ':
Espacio();
    break;

    //Notas
    case 'a':
Nota_A(0,0x01,0x10);
    break;
    case 'b':
Nota_B(0,0x01,0x10);
    break;
    case 'c':
Nota_C(0,0x01,0x10);
    break;
    case 'd':
Nota_D(0,0x01,0x10);
    break;
    case 'e':
Nota_E(0,0x01,0x10);
    break;
    case 'f':
Nota_F(0,0x01,0x10);
    break;
    case 'g':
Nota_G(0,0x01,0x10);
    break;
    case '#':
Nota_sostenido(0,0x01,0x10);
    break;
    case '&':
Nota_bemol(0,0x01,0x10);
    break;

//Figuras
    case 'r':
Figura_redonda(0,0x01,0x10);
    break;
    case 'v':
Figura_blanca(0,0x01,0x10);
    break;
    case 'w':
Figura_blancacp(0,0x01,0x10);
    break;
    case 'n':
Figura_negra(0,0x01,0x10);
    break;
    case 'm':
Figura_negracp(0,0x01,0x10);
    break;
    case 'k':
Figura_corchea(0,0x01,0x10);
    break;
    case 's':
Figura_semicorchea(0,0x01,0x10);
    break;
    }
}


void SSD1306_WriteString(unsigned char *cadena)
{
    unsigned int i = 0;
    unsigned int l = 0;

    l = cadena[i];
    while ((l) != 0)
    {
SSD1306_WriteChar(l);
        i++;
        l = cadena[i];
    }
}




/*********************************************************************
LIESE           LIESE           LIESE           LIESE           LIESE
***********************************************************************/

#endif /* SSD1306_V2_H_ */
