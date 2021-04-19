// Dispositivo:    PIC 16F887
// Hadward:        Leds, displays y botones
// Autor:          Dylan Ixcayau    18486
// Fecha Creadad:  13 de Abril 2021
// Ultima Actualizacion:    18 de Abril del 2021

// ----------------- Laboratorio No.7 ----------------------------------------

#pragma config FOSC=INTRC_NOCLKOUT //Oscilador interno sin salidas
#pragma config WDTE=OFF	           //WDT disabled (reinicio repetitivo del pic)
#pragma config PWRTE=ON	           //PWRT enabled (espera de 72ms al iniciar
#pragma config MCLRE=OFF	       //pin MCLR se utiliza como I/O
#pragma config CP=OFF		       //sin protección de código
#pragma config CPD=OFF		       //sin protección de datos
#pragma config BOREN=OFF	       //sin reinicio cuando el voltaje baja de 4v
#pragma config IESO=OFF	           //Reinicio sin cambio de reloj de interno a externo
#pragma config FCMEN=OFF	       //Cambio de reloj externo a interno en caso de falla
#pragma config LVP=OFF


 
//------------------ Configuración word2 --------------------------------------
#pragma config WRT=OFF	//Protección de autoescritura 
#pragma config BOR4V=BOR40V	//Reinicio abajo de 4V 
  


#include<xc.h>
#include<stdint.h>

//------------------------Variables --------------------------------------------
uint8_t cont = 0;   //Declaramos las variables de 8 bits y sin signo
uint8_t unidades = 0;
uint8_t decenas =  0;
uint8_t centenas=  0;
uint8_t banderas = 0;
uint8_t v1 = 0;
//----------------------funciones-----------------------------------------------
void setup(void);   //llamamos a las funciones desde el principio
int tabla(int v1);  //llamamamos a la tabla que regresara valores enteros
void Decimal(void);

//-------------------Interrupciones---------------------------------------------
void __interrupt() interrupciones(void){        //funcion de interrupcion
    if(T0IF == 1) {          //Revisamos si la bandera del timer0 esta levantada
        INTCONbits.T0IF = 0;         //Bajamos la bandera del timer0
        TMR0 = 236;                  //Le asignamos un valor al TMR0
        PORTE = 0;                   //Dejamos todo el puerto E apagado
        if (banderas == 0){          //Cambio entre displays
            v1 = unidades;          // El valor de unidades pasa a v1
            PORTC = tabla(v1);      //PORTC muestra el contador
            RE0 = 1;                //Encendemos el display correspondiente
            banderas = 1;           //Nos movemos un valor en banderas
            return;                 //regresamos
        }
        if (banderas == 1){
            v1 = decenas; 
            PORTC = tabla(v1);
            RE1 = 1;
            banderas = 2; 
            return; 
        }
        if (banderas == 2){
            v1 = centenas;
            PORTC = tabla(v1); 
            RE2 = 1;
            banderas = 0; 
            return; 
        }
    }
    
    if(RBIF == 1){          //Interrupciones de los botones
        if(PORTBbits.RB0 == 0){     //Si el boton de RB0 esta apagado
            PORTA ++;      //Incrementamos el contador
        }
        if(PORTBbits.RB1 == 0){    //Si el boton de RB1 esta apagado
            PORTA --;       //Decrementamos el contador
        }
        INTCONbits.RBIF = 0;        //Apagamos la bandera del puerto B
    }
}

//---------------- LOOP Principal ----------------------------------------------
void main(void){
    setup();            //Llamamos a las configuraciones, parecido a un call
    while(1){           //Que se repita por siempre
    cont = PORTA;       //El valor de PORTA se pasa a cont
    Decimal();       //Llamamos a la funcion para conversiones
    }
}
//------------------Funciones---------------------------------------------------
int tabla (int v1){     //La tabla
 int w;                 //Declaramos w de tipo int
 switch (v1)   {         //Entramos a un tipo de "Menu" que se navega con v1
 
     case 0 :           //Opción 0
         w = 0b00111111;    //Numero 0
         break; 
     case 1 :           //Opción 1
         w = 0b00000110;    //Numero 1
         break;         
     case 2 :           //Opción 2
         w = 0b01011011;    //Numero 2
         break;             
     case 3 :           //Opción 3
         w = 0b01001111;    //Numero 3
         break; 
     case 4 :           //Opción 4
         w = 0b01100110;    //Numero 4
         break;
     case 5 :           //Opción 5
         w = 0b01101101;    //Numero 5
         break;
     case 6 :           //Opción 6
         w = 0b01111101;    //Numero 6
         break;
     case 7 :           //Opción 7
         w = 0b00000111;    //Numero 7
         break;
     case 8 :           //Opción 8
         w = 0b01111111;    //Numero 8
         break;
     case 9 :           //Opción 9
         w = 0b01101111;    //Numero 9
            break;              //Rompemos el menu
        default:;
 }
 return w;
}
void Decimal(void){      //Funcion para conversion en decimal
    centenas = cont/100;            //Operacin para centenas
    decenas  = (cont - 100*centenas)/10;     //operacion para decenas
    unidades = (cont - 100*centenas - 10*decenas)/1;  //Operacion para unidades
}
//---------------- Configuraciones Generales ----------------------------------
void setup(void){
//-------  Asigancion de Pines Didigitales
    ANSEL  = 0x00;
    ANSELH = 0x00;
   
    TRISBbits.TRISB0 = 1;       //activamos los botones
    TRISBbits.TRISB1 = 1;
    TRISC = 0x00;         //Dejamos los los demas puertos Como salidas digitales
    TRISA = 0x00;
    TRISD = 0x00;
    TRISE = 0x0000;
    
    
    PORTB = 0x00;        //Limpiamos todos los puertos
    PORTC = 0x00;
    PORTA = 0x00;
    PORTE = 0x00;
    PORTD = 0x00;
    
//-------------------- Configuraciones del Reloj -------------------------------
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;       //Reloj a 8MHz
    OSCCONbits.SCS   = 1;       //Oscilador interno
//--------------------- Configuraciones del IoC --------------------------------
    OPTION_REGbits.nRBPU = 0;   //Pull up encendido
    WPUB = 0b00000011;          //Habilitamos las resistencias de los botones
    IOCBbits.IOCB0 = 1;         //Interrupciones de cada boton encendidos
    IOCBbits.IOCB1 = 1;
    
//------------------ Configuraciones de TMR0 ------------------------------------
    OPTION_REGbits.T0CS = 0;    //Ciclo de reloj interno
    OPTION_REGbits.PSA  = 0;    //Prescaler para el modulo de timer 0
    OPTION_REGbits.PS2  = 1;
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 1;    //Prescaler a 256
  
   
//-------------------- Configuracion del las interrupciones --------------------
    INTCONbits.GIE  = 1 ;       //Habilitamos las interrupciones globales
    INTCONbits.RBIF = 1 ;       //Dejamos la  bandera levantada
    INTCONbits.RBIE = 1 ;       //Encendemos las interrupciones del puerto B
    INTCONbits.T0IE = 1 ;       //Encendemos la interrupcion del timer0
}