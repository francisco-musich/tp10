#ifndef BACKEND_H
#define BACKEND_H

//Defino macros a utilizar tambien en el main
#define PUERTOA 1
#define PUERTOB 2
#define PUERTOD 3

//Incluyo librerias necesarias
#include <stdio.h>
#include <stdint.h>
#include "backend.h"
#include "termlib.h"


//Defino typedefs a utilizar
/*ACLARACION: la estructura esta definida para plataformas little endian*/
typedef struct
{
  uint8_t portb;
  uint8_t porta;
}ports8_t;    //LITTLE ENDIAN!

typedef union
{
  ports8_t portab;
  uint16_t port;
}port16_t;



//Prototipos de funciones
void BitSet (uint8_t flag_puerto,uint8_t bit);
void BitClr (uint8_t flag_puerto,uint8_t bit);
char bitGet (uint8_t flag_puerto,uint8_t bit);
void maskOn(uint8_t flag_puerto,uint16_t mask);
void maskOff(uint8_t flag_puerto,uint16_t mask);

//Declaracion de variables globales
extern port16_t puertod;
extern uint16_t tempMask16;

//Prototipos de simulaciones
void Simulacion(void);
#endif
