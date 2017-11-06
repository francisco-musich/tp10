#include <allegro5/allegro.h>   //Manejo de display y eventos
#include <allegro5/allegro_image.h> //Manejo de imagenes
#include <stdio.h>      //Printf
#include <stdint.h> 
#include "backend.h"
#include "termlib.h"


//Defines necesarios
#define CANT_LEDS 16
#define WIDTH_DIS (HEIGHT_LED)*CANT_LEDS       //ANcho del display
#define HEIGHT_DIS (HEIGHT_LED)*2      //Alto del display
#define HEIGHT_LED 50      //Alto del display
#define WIDTH_LED 50      //Alto del display
#define true 1          //lo hago como defines en vez de bool porque no toma la libreria stdbool
#define false 0

void decideLed(uint8_t portFlag, uint8_t bitFlag,ALLEGRO_BITMAP *ledverde, ALLEGRO_BITMAP *ledrojo ); //Funcion que decide e imprime los bits sellecionados por teclado

int main (void)
{
    //Declaracion de variables necesrias
    uint8_t close_display = false;
    uint8_t cantleds =16;   //como el puerto inicial es D, inicializo en 16bits
    uint8_t puerto_actual = PUERTOD;    //Por default estoy en el puerto D
    uint8_t maskTemp8;
    //Declaracion de variables necesarias para allegro
    ALLEGRO_DISPLAY * display = NULL; //Creo un puntero a mi display
    ALLEGRO_BITMAP *ledverde = NULL;
    ALLEGRO_BITMAP *ledrojo = NULL; //Creo punteros a imagenes
    ALLEGRO_BITMAP *a = NULL; //Creo punteros a imagenes
    ALLEGRO_BITMAP *b = NULL; //Creo punteros a imagenes
    ALLEGRO_BITMAP *d = NULL; //Creo punteros a imagenes
    ALLEGRO_EVENT_QUEUE  *event_queue = NULL;
    uint8_t id = 0;
    
    //Inicializo Allegro
    if(!al_init()) 
    { 
	printf("failed to initialize allegro!\n");
	return -1;
    }
    
    //Inicializo cola de eventos
    event_queue = al_create_event_queue();
    if(!event_queue) {
		printf("failed to create event_queue!\n");
		return -1;
	}
    //Inicializo teclado
    if(!al_install_keyboard()) {
		printf("failed to initialize the keyboard!\n");
                al_destroy_event_queue(event_queue);
		return -1;
	}
    
    //Inicializo imagenes
    if(!al_init_image_addon()) { // ADDON necesario para manejo de imagenes 
		printf("failed to initialize image addon !\n");
                al_destroy_event_queue(event_queue);
		return -1;
	}
    
    ledverde = al_load_bitmap("led verde.jpeg");
    if(!ledverde)
    {
        printf ("failed to load led verde!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        return -1;
    }
    
    ledrojo = al_load_bitmap ("led rojo.png");
    if(!ledrojo)
    {
        printf ("failed to create led rojo!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        return -1;
    }
    
    a= al_load_bitmap("a.jpg");
       if (!a)
    {
        printf ("failed to create led rojo!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        al_destroy_bitmap(a);
        return -1;
    }
    
    b= al_load_bitmap("b.jpeg");
       if (!b)
    {
        printf ("failed to create led rojo!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        al_destroy_bitmap(a);
        
        return -1;
    }
    
    d= al_load_bitmap("d.jpeg");
       if (!d)
    {
        printf ("failed to create led rojo!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        al_destroy_bitmap(a);
        al_destroy_bitmap(b);
        
        return -1;
    }
    
    
    //Inicializo Mouse
    
    if (!al_install_mouse())
    {
        printf ("failed to create mouse!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        al_destroy_bitmap(a);
        al_destroy_bitmap(b);
        al_destroy_bitmap(d);
        
        return -1;
    }
    
    //Creo el display
    
    display = al_create_display(WIDTH_DIS,HEIGHT_DIS);
    
   if(!display) 
   {
	printf("failed to create display!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(ledrojo);
        al_destroy_bitmap(ledverde);
        al_destroy_bitmap(display);
	return -1;
    }
    
    //Incluyo los eventos a la cola de eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    
    //Pongo cosas en el display
    al_clear_to_color(al_map_rgb(255,255,255)); //Hace clear del backbuffer del diplay al color RGB 255,255,255 (blanco)
    
    uint16_t i,k; //i= led, k=posicion 
    al_draw_scaled_bitmap(a,
            0,0, al_get_bitmap_width(a),al_get_bitmap_height(a),
            0,0, WIDTH_LED,HEIGHT_LED,
            0);
    al_draw_scaled_bitmap(b,
            0,0, al_get_bitmap_width(b),al_get_bitmap_height(b),
            WIDTH_LED*8,0, WIDTH_LED,HEIGHT_LED,        //WIDTH_LED*8 porque quiero que lo ponga en el 8vo bit
            0);
    
    al_draw_scaled_bitmap(d,
            0,0, al_get_bitmap_width(d),al_get_bitmap_height(d),
            WIDTH_LED*15,0, WIDTH_LED,HEIGHT_LED,        //WIDTH_LED*8 porque quiero que lo ponga en el 8vo bit
            0);
    
    
    for (i=0,k=0 ; i < cantleds ; (i++, k+=50)) //inicializo los leds
    {
        al_draw_scaled_bitmap(ledrojo,
            0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
            k,HEIGHT_LED ,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
            0);
    }
    
    
    al_flip_display(); //Flip del backbuffer, pasa a verse a la pantalla
    
    while (!close_display)
    {
        ALLEGRO_EVENT evento;
        if (al_get_next_event(event_queue, &evento))    
        {
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //verifico si debo cerrar el display
                close_display = true;
            
            if (evento.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch (evento.keyboard.keycode)
                {
                    case ALLEGRO_KEY_S:
                        if (puerto_actual == PUERTOB)
                        {
                            maskOn (puerto_actual,0xFF);
                        for (i=0,k= (WIDTH_LED*8) ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                        al_flip_display();
                        break;
                        }
                        
                        else
                        {
                        maskOn (puerto_actual,0xFFFF);
                        for (i=0,k=0 ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                        al_flip_display();
                        break;
                        }
                        
                    case ALLEGRO_KEY_C:
                        if (puerto_actual == PUERTOB)
                            {
                            maskOff (puerto_actual,0xFF);
                            for (i=0,k= (WIDTH_LED*8) ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                            al_flip_display();
                            break;
                            }
                        else
                            {
                            maskOff (puerto_actual,0xFFFF);
                            for (i=0,k= 0 ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                            al_flip_display();
                            break;
                            }
                        
                    case ALLEGRO_KEY_B:
                        if (puerto_actual == PUERTOB)
                            {
                            maskTemp8 = puertod.portab.portb;
                            maskOff (puerto_actual,0xFF);
                            for (i=0,k= (WIDTH_LED*8) ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            if (bitGet(PUERTOB,i) == false) 
                             {al_draw_scaled_bitmap(ledrojo,
                                            0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                            k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                            0);
                             al_flip_display();
                             }
                             else{
                            
                            al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            
                            }
                            al_flip_display();
                            al_rest(1.0);
                            maskOn (puerto_actual,maskTemp8);
                        for (i=0,k= (WIDTH_LED*8) ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            if (bitGet(PUERTOB,i) == false) 
                             {al_draw_scaled_bitmap(ledrojo,
                                            0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                            k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                            0);
                             al_flip_display();
                             }
                             else{
                            
                            al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            
                            }
                        }}
                        else if(puerto_actual = PUERTOA){
                            
                                
                            maskTemp8 = puertod.portab.porta;
                            maskOff (puerto_actual,0xFF);
                            for (i=0,k= 0 ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                             if (bitGet(PUERTOA,i) == false)    
                                    {
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
                                    
                                    else 
                                    {
                                       
                                     al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();  
                                    }
                            }
                            al_flip_display();
                            al_rest(1.0);
                            maskOn (puerto_actual,maskTemp8);
                        for (i=0,k=0 ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                             if (bitGet(PUERTOA,i) == false)    
                                    {
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
                                    
                                    else 
                                    {
                                       
                                     al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();  
                                    }
                            }
                        al_flip_display();
                            
                        }
                            
                        
                        else{
                        for (i=0,k= (WIDTH_LED*8) ; i < cantleds ; (i++, k+=50)) //inicializo los leds
                            {
                            al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                        
                        for (i=0; i < cantleds ; (i++)) //inicializo los leds
                            {
                            
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
          
                        al_rest(2.0);
                                                         
                        for (i=0,k=0 ; i < 8 ; (i++, k+=50)) //inicializo los leds
                            {
                            if (bitGet(PUERTOA,i) == false)    
                                    {
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
                                    
                                    else 
                                    {
                                       
                                     al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        (WIDTH_LED*i),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();  
                                    }
                            
                            }
                        for (i=0,k= (WIDTH_LED*8) ; i < 8 ; (i++, k+=50)) //inicializo los leds
                            {
                             if (bitGet(PUERTOB,i) == false) 
                             {al_draw_scaled_bitmap(ledrojo,
                                            0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                            k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                            0);
                             al_flip_display();
                             }
                             else{
                            
                            al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        k,HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                            }
                        al_flip_display();  
                        }}
                            
                            
                        break;
                   
                    case ALLEGRO_KEY_ESCAPE:        //Si se oprime escape, el programa cierra
                        close_display = true;
                        
                    case ALLEGRO_KEY_0:         // Dependiendo del numero que se toque, se prendera cierto led
                        id = 0;
                        decideLed(puerto_actual,id,ledverde,ledrojo );
                        break;
                    case ALLEGRO_KEY_1:
                      id = 1;
                      decideLed(puerto_actual,id,ledverde,ledrojo );
                      break;
                            
                        
                    case ALLEGRO_KEY_2:
                        id = 2;
                        decideLed(puerto_actual,id,ledverde,ledrojo );
                        break;
                case ALLEGRO_KEY_3:
                    id = 3;
                    decideLed(puerto_actual,id,ledverde,ledrojo );
                    break;
                case ALLEGRO_KEY_4:
                    id = 4;
                    decideLed(puerto_actual,id,ledverde,ledrojo );
                    break;
                case ALLEGRO_KEY_5:
                    id = 5;
                    decideLed(puerto_actual,id,ledverde,ledrojo );
                    break;
                case ALLEGRO_KEY_6:
                    id = 6;
                    decideLed(puerto_actual,id,ledverde,ledrojo );
                    break;
                case ALLEGRO_KEY_7:
                    id = 7;
                    decideLed(puerto_actual,id,ledverde,ledrojo );
                    break;
                
                     
                 
                  

                        
                    
                        
                }
            }
         if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if ((evento.mouse.x <= WIDTH_LED) && (evento.mouse.y <= HEIGHT_LED)) //Tocaron a
                    {
                        puerto_actual = PUERTOA;
                        cantleds = 8;
                    }
                    
                    if ((evento.mouse.x >= (WIDTH_LED*8)) && (evento.mouse.x <= ((WIDTH_LED*8)+WIDTH_LED)) && (evento.mouse.y <= HEIGHT_LED)) //Tocaron b 
                    {   
                        puerto_actual = PUERTOB;
                        cantleds = 8;
                    }
                    
                    if ((evento.mouse.x >= (WIDTH_LED*15)) && (evento.mouse.x <= ((WIDTH_LED*15)+WIDTH_LED)) && (evento.mouse.y <= HEIGHT_LED)) //Tocaron b 
                    {   
                        puerto_actual = PUERTOD;
                        cantleds = 16;
                    }
                    
                    if ((evento.mouse.y >= HEIGHT_LED) && (evento.mouse.y <= 2*HEIGHT_LED))
                    {
                        for (k= 0,i=0 ; i <= cantleds ; k+=WIDTH_LED) //inicializo los leds
                            {
                            if (evento.mouse.x <= k)    //Me fijo si estoy en el led que debo cambiar
                                { 
                                    if (bitGet(PUERTOD,k/WIDTH_LED) == true)    
                                    {
                                    BitClr (PUERTOD,k/WIDTH_LED);
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (k-WIDTH_LED),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
                                    
                                    else 
                                    {
                                     BitSet (PUERTOD,k/WIDTH_LED);  
                                     al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        (k-WIDTH_LED),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();  
                                    }
                                    break;
                                }
                            }
                            
                           
                    }
                    
                    
                    
                }
                    
                
            }
        }
        
        
    
     
    //Destruyo lo que cree
    al_destroy_event_queue(event_queue);
    al_destroy_display(display); 
    al_destroy_bitmap(ledrojo);
    al_destroy_bitmap(ledverde);
    al_destroy_bitmap(a);
    al_destroy_bitmap(b);
    
    
    return 0;
}
void decideLed(uint8_t portFlag, uint8_t bitFlag,ALLEGRO_BITMAP *ledverde, ALLEGRO_BITMAP *ledrojo ){
   
                        switch(portFlag)
                        {
                        case PUERTOB:
                            if (bitGet(PUERTOB,bitFlag) == true)    
                                    {
                                    BitClr (PUERTOB,bitFlag);
                                    al_draw_scaled_bitmap(ledrojo,
                                        0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                        (WIDTH_LED*(bitFlag+8)),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();
                                    }
                                    
                                    else 
                                    {
                                     BitSet (PUERTOB,bitFlag);  
                                     al_draw_scaled_bitmap(ledverde,
                                        0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                        (WIDTH_LED*(8+bitFlag)),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                        0);
                                    al_flip_display();  
                                    }
                        break;
                        default:
                                if (bitGet(PUERTOA,bitFlag) == true)    
                                            {
                                            BitClr (PUERTOA,bitFlag);
                                            al_draw_scaled_bitmap(ledrojo,
                                                0,0, al_get_bitmap_width(ledrojo),al_get_bitmap_height(ledrojo),
                                                (WIDTH_LED*bitFlag),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                                0);
                                            al_flip_display();
                                            }

                                            else 
                                            {
                                             BitSet (PUERTOA,bitFlag);  
                                             al_draw_scaled_bitmap(ledverde,
                                                0,0, al_get_bitmap_width(ledverde),al_get_bitmap_height(ledverde),
                                                (WIDTH_LED*bitFlag),HEIGHT_LED,WIDTH_LED,HEIGHT_LED,  //tamano que quiero que se imprima la imagen
                                                0);
                                            al_flip_display();  
                                            }
                                break;

                            
                        }
                      
}

