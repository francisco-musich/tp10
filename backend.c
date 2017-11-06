
//Incluyo librerias necesarias
#include <stdio.h>
#include <stdint.h>   //para los enteros
#include "backend.h"
#include "termlib.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

//Defino macros necesarias para funciones internas
#define MASKSET 0x01
#define MASKCLR 0x01
#define DEBUG 1


 //Definicion de variables globales a utilizar
port16_t puertod;  //Contiene el puerto A y el B.Variable inicializada en backend.c
uint16_t tempMask16;


 //***************DEFINICION FUNCION BITSET*********************
 /*Funcion que recibe un especificador del puerto y el bit especifico que se desea modificar
 para ponerlo en 1 directamente en el puerto*/
 void BitSet (uint8_t flag_puerto,uint8_t bit)
 {
   uint8_t nshifteado;   //creo una variable para cambiar el bit del puerto
   uint8_t bit1 = bit;   //creo una variable para poder shiftear el numero.
                         //por defult es igual que el bit del puerto (8bits)
   uint8_t mascara = 0;   //Creo la mascara para prender el bit deseado
   
   switch (flag_puerto)
   {
     case PUERTOA:
       nshifteado = puertod.portab.porta;
       break;
     case PUERTOB:
       nshifteado = puertod.portab.portb;
       break;
     case PUERTOD:
       //checkeo en realidad a que puerto pertenece y a ese le asigno nshifteado
       if (bit<8)
       {
         nshifteado=puertod.portab.portb;
       }
       else if ((bit>=8)&&(bit<=15))
       {
         bit1=bit-8;   //corro el indice contador para que se corra bien
         nshifteado=puertod.portab.porta;
       }
       else
       {
         printf("El puerto no tiene tantos bits\n");
       }
       break;
     default:
       printf("El puerto ingresado no es valido\n");
   }

   mascara = MASKSET << bit1;
   nshifteado = nshifteado | mascara;   //Prendo el bit correspondiente


   switch (flag_puerto)    //Devuelvo el numero con el bit deseado encendido al puerto
   {
     case PUERTOA:
     if (bit>=8)
     {
       printf("El puerto no tiene tantos bits\n");
     }
     puertod.portab.porta = nshifteado;
       break;
     case PUERTOB:
     if (bit>=8)
     {
       printf("El puerto no tiene tantos bits\n");
     }
       puertod.portab.portb = nshifteado;
       break;
     case PUERTOD:
       //checkeo en realidad a que puerto pertenece y a ese le asigno nshifteado
       if (bit<8)
       {
         puertod.portab.portb = nshifteado;
       }
       else if ((bit>=8)&&(bit<=15))
       {
         puertod.portab.porta = nshifteado;
       }
       break;
   }
   tempMask16 = puertod.port;
   #if DEBUG
   printf("puerto d %u\n", puertod.port );
   #endif

 }



 //***************DEFINICION FUNCION BITCLR *********************
 /*Funcion que recibe un especificador del puerto y el bit especifico que se desea modificar
 para ponerlo en 0 directamente en el puerto*/
 void BitClr (uint8_t flag_puerto,uint8_t bit)
 {
   uint8_t nshifteado;   //creo una variable para cambiar el bit del puerto
   uint8_t bit1 = bit;
   uint8_t mascara = 0;
   
   switch (flag_puerto)
   {
     case PUERTOA:
       nshifteado=puertod.portab.porta;
       break;
     case PUERTOB:
       nshifteado=puertod.portab.portb;
       break;
     case PUERTOD:
       //checkeo en realidad a que puerto pertenece y a ese le asigno nshifteado
       if (bit<8)
       {
         nshifteado=puertod.portab.portb;
       }
       else if ((bit>=8)&&(bit<=15))
       {
         bit1= bit -8;
         nshifteado=puertod.portab.porta;
       }
       else
       {
         printf("El puerto no tiene tantos bits\n");
       }
       break;
     default:
       printf("El puerto ingresado no es valido\n");
   }

   mascara = MASKCLR << bit1;
   nshifteado = nshifteado ^ mascara;

   switch (flag_puerto)    //Devuelvo el numero con el bit deseado encendido al puerto
   {
     case PUERTOA:
     puertod.portab.porta = nshifteado;
       break;
     case PUERTOB:
       puertod.portab.portb = nshifteado;
       break;
     case PUERTOD:
       //checkeo en realidad a que puerto pertenece y a ese le asigno nshifteado
       if (bit<8)
       {
         puertod.portab.portb = nshifteado;
       }
       else if ((bit>=8)&&(bit<=15))
       {
         puertod.portab.porta = nshifteado;
       }
       break;
   }

   #if DEBUG
   printf("puerto d %u\n", puertod.port );
   #endif

 }


char bitGet(uint8_t flag_puerto, uint8_t bit){
    uint8_t nshifteado;   //creo una variable para cambiar el bit del puerto
    uint8_t bit1=bit;
    switch (flag_puerto)
    {
      case PUERTOA:
        nshifteado=puertod.portab.porta;
        break;
      case PUERTOB:
        nshifteado=puertod.portab.portb;
        break;
      case PUERTOD:
        //checkeo en realidad a que puerto pertenece y a ese le asigno nshifteado
        if (bit<8)
        {
          nshifteado=puertod.portab.portb;
        }
        else if ((bit>=8)&&(bit<=15))
        {
          bit1= bit -8;
          nshifteado=puertod.portab.porta;
        }
        else
        {
          printf("El puerto no tiene tantos bits\n");
        }
        break;
      default:
        printf("El puerto ingresado no es valido\n");
    }
    uint8_t temp = 1;
    temp = temp << bit1;
    temp = (nshifteado & temp);
    if(temp){
        return 1;
    }
    else{
        return 0;
    }
}


/* Creo una estructura usando el typedef de los puertos, asi puedo recibir una mascara de 16 bits, y dependiendo
del puerto seleccionado, utilizar 8 o 16 de la misma. Despues asigno y opero en cada puerto de la forma necesaria y guardo los resultados*/
void maskOn(uint8_t flag_puerto,uint16_t mask)
{
    port16_t tempMask;
    tempMask.port = mask;
    uint8_t mask8bit = tempMask.portab.portb; //Busco la mascara en la union, LITTLE ENDIAN (CASO PUERTO 8 BITS)
    uint8_t nshifteado1;    //bytes auxiliares en caso de puertoD
    uint8_t nshifteado2;
    switch (flag_puerto)
    {
      case PUERTOA:
        nshifteado1=puertod.portab.porta;
        nshifteado1 = (nshifteado1 | mask8bit);     //utilizo mascara de 8bits con un or bitwise y guardo en el puerto el resultado
        puertod.portab.porta = nshifteado1;
        break;
      case PUERTOB:
        nshifteado1=puertod.portab.portb;
        nshifteado1 = (nshifteado1 | mask8bit);
        puertod.portab.portb = nshifteado1;
        break;
      case PUERTOD:
      {
         
          
          nshifteado1=puertod.portab.portb;                             //SImilar a los puertos de 8 bits, pero ahora uso ambos bytes de la mascara contenida en la union
          nshifteado1 = (nshifteado1 | (tempMask.portab.porta));
          puertod.portab.portb = nshifteado1;
          nshifteado2=puertod.portab.porta;
          nshifteado2 = (nshifteado2 | tempMask.portab.portb);
          puertod.portab.porta = nshifteado2;
      }
        break;
      default:
        printf("El puerto ingresado no es valido\n");

    }
}

void maskOff(uint8_t flag_puerto,uint16_t mask)
{
    port16_t tempMask;
    tempMask.port = mask;
    uint8_t mask8bit = tempMask.portab.portb; //Busco la mascara en la union, LITTLE ENDIAN (CASO PUERTO 8 BITS)
    mask8bit = ~(mask8bit); //niego la mascara de 8 bits para poder hacer un NAND con el puerto
    uint8_t nshifteado1;    //bytes auxiliares en caso de puertoD
    uint8_t nshifteado2;
    switch (flag_puerto)
    {
      case PUERTOA:
        nshifteado1=puertod.portab.porta;
        nshifteado1 = (nshifteado1 & mask8bit);     //utilizo mascara de 8bits con un or bitwise y guardo en el puerto el resultado
        puertod.portab.porta = nshifteado1;
        break;
      case PUERTOB:
        nshifteado1=puertod.portab.portb;
        nshifteado1 = (nshifteado1 & mask8bit);
        puertod.portab.portb = nshifteado1;
        break;
      case PUERTOD:
      {
          nshifteado1=puertod.portab.portb;                             //SImilar a los puertos de 8 bits, pero ahora uso ambos bytes de la mascara contenida en la union
          nshifteado1 = (nshifteado1 &(~ (tempMask.portab.porta)));
          puertod.portab.portb = nshifteado1;
          nshifteado2=puertod.portab.porta;
          nshifteado2 = (nshifteado2 & (~tempMask.portab.portb));
          puertod.portab.porta = nshifteado2;
      }
        break;
      default:
        printf("El puerto ingresado no es valido\n");

    }
}


void Simulacion(void){

  puertod.portab.porta = 0x0; //Inicializa el puerto a 0x00

  printf("\n---------------\n\n");
  printf("%02X \n", puertod.portab.porta); //Imprime el puerto inicial
  printf("\n---------------\n\n");

  char c=getch(); //Input
  (void) getch(); //Asi evitamos el getch correspondiendo a la validacion del input
  
  while (c!=27){ //El programa anda hasta que el usuario entraga ESC

    if (c>=(0+'0') && c<=(7+'0')){ //Si la entrada esta un numero de 0 a 7, se prende el LED correspondiendo
      BitSet(PUERTOA, c-'0');
    }

    else if (c=='b'){ //Si la entrada esta un b, los LEDs prendidos parpadean
      port16_t tempMask;
      tempMask.portab.porta=puertod.portab.porta; //Guardamos el estado del puerto en una variable temporal
      maskOff(PUERTOA, tempMask.portab.porta); //Se apagan los LEDs prendidos
      printf("%02X \n", puertod.portab.porta); //Se imprime el puerto
      printf("\n---------------\n\n");
      maskOn(PUERTOA, tempMask.portab.porta); //El puerto vuelve a su estado anterior
    }

    else if (c=='c'){ //Si la entrada es un c, se apagan todos los LEDs...
      maskOff(PUERTOA, 0xFF); //...utilizando la funcion maskOff con una mascara total
    }
    
    else if(c=='s'){ //Si la entrada es un s, se prenden todos los LEDs...
      maskOn(PUERTOA, 0xFF); //...utilizando la funcion maskOn con una mascara total
    } 

    else{
      printf("\nLa entraga debe ser un numero de '0' a '7', 'b', 'c' o 's'\n\n"); //Imprime un mensaje de error si a entrada no esta correcta
    }

    printf("%02X \n", puertod.portab.porta); //Imprime el puerto
    printf("\n---------------\n\n");

    c=getch();
    (void) getch();    //Asi evitamos el getch correspondiendo a la validacion del input
  }
}