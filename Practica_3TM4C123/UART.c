#include "lib/include.h"

extern void Configurar_UART7(void)
{
    SYSCTL->RCGCUART  = (1<<7);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port E
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOE->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PE0-> U0Rx PE1-> U0Tx pag.688
    GPIOE->PCTL =  0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN = (1<<0) | (1<<1);//PE1 PE0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART7->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
    UART7->IBRD = 54;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART7->FBRD = 17;
    //  UART Line Control (UARTLCRH) pag.916
    UART7->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART7->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART7->CTL = (1<<0) | (1<<8) | (1<<9);



}

/*
extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART0->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    
    BRD = 25,000,000 / (16*57600) = 27.1267
    UARTFBRD[DIVFRAC] = integer(.1267 * 64 + 0.5)
    */
   /*
    UART0->IBRD = 162;//27;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 49;//7;
    //  UART Line Control (UARTLCRH) pag.916
    UART0->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);



}
*/

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART7->FR & (1<<4)) != 0 );
    v = UART7->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART7->FR & (1<<5)) != 0 );
    UART7->DR = c;
}
extern void printString(char* string)
{
    int i=0;
    while(string[i]>47)
    {
        printChar(string[i]);
        i++;
    }
}

extern int readString(char delimitador, char *string)
{

   int i=0;
   //char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       string[i] = c;
       i++;
       c = readChar();
   }

    return i;

}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 
//FUNCION DEL VIDEO

/*
extern char * invertir_String(char* str)  //FUNCION PARA INVERTIR LA CADENA
{  
    int n = strlen(str);
    int i = 0;
    char *inv = (char*)calloc(10,sizeof(char));
    while(i<n)
    {
        *(inv + (n-i-1)) = str[i];
        i++;
        if(i%10==0)
        {
            inv = realloc(inv, (1+10)*sizeof(char));
        }
    }

    return inv;
}
*/


extern void NomInvert(char *PAL, int tamstring) //FUNCION PARA INVERTIR UNA CADENA
{
int i = 1;       //CONTADOR PARA RECORRER LOS ARREGLOS
int x=1;         //"CONTADOR" PARA AGREGAR LOS NUMEROS A LA CADENA 
int y=0;         //"CONTADOR" PARA POSICIONAR EL NUEVO ORDEN DENTRO DE UN ARREGLO
int banderin = 0; //PERMITE ENTRAR AL CONDICIONAL QUE CAMBIARA LA POSICION DE CADENA
int Size;          //PARA GUARDAR EL TAMAÑO DE LA CADENA
Size=tamstring;
   

//ES NECESARIO ESTABLECER EL TAMAÑO DE LA MEMORIA QUE SE UTILIZARA PARA LA CADENA
if (tamstring < 10) //SEGUN EL TAMAÑO DE LA CADENA INICIAL ESTABLECEMOS LA MEMORIA NECESARIA
    
        tamstring = tamstring + (tamstring-1);
        
    
    else
    {
        tamstring = 18 + ((tamstring - 9)*3);
    }    
        char MON[tamstring]; //CON EL TAMAÑO ESTABLECIDO ENTONCES DECLARAMOS EL ARRECLO DONDE SE GUARDARA LA INVERSION
    
//______________

//char invert = (char)calloc(10,sizeof(char));
while(i<tamstring) 
{
if (banderin == 0 )
{
	MON[y]=PAL[Size-i];  //SE CAMBIA LA POSICION DE LAS LETRAS EN ORDEN INVERSO A LA POSICION ORIGINAL Y SE GUARDAN EN EL NUEVO ARREGLO
	i=i+1;
	y=y+1;
    banderin=1;       //SE CAMBIA EL VALOS DE BANDERIN PARA QUE ENTRE AL ELSE

}
else
{
	banderin=0;
	MON[y]=x +'0'; //SE COLOCAN LOS NUMEROS ALTERNANDO LETRA Y NUMERO, SE RRECORRE LA POSICION DEL ARREGLO CON AYUDA DEL "CONTADOR y"
	x=x+1;
	y=y+1;
}

}


    for(int j=0; j<=(tamstring-1); j++)
{
    PAL[j]=MON[j]; //SE REASIGNA LOS VALORES DENTRO DE MON A EL ARREGLO PAL PARA DESPUES SER LLAMADA EN EL MAIN
} 
  for(int n=tamstring ;n <= 100;n++) //ESTE FOR SE OCUPA PAR "RELLENAR" LOS ESPACIOS BACIOS EN EL ARREGLO YA QUE ES DE UN TOTAL DE 100

    PAL[n] = 0;
return;

}



