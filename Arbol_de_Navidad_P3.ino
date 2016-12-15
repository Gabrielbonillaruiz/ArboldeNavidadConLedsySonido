/*************************************************************************
*                                                                        *                             
* ARBOL DE NAVIDAD CON LEDS Y SONIDOS USANDO TIMERS                      *                              
*                                                                        *
* ************************************************************************
*                                                                        *
* Realizado por:                                                         *
*                                                                        *
* Gabriel Bonilla Ruiz                                                   *
* Borja Gordillo Ramajo                                                  *
* Sergio Jímenez Salmerón                                                *
*                                                                        *
**************************************************************************                                                      
*                                                                        *
* COMENTARIOS:                                                           *                                                
*                                                                        *
* Este programa solo funciona en Arduino Uno.                            *
* Si queremos usarlo en un Arduino diferente, debemos                    *
* asignar a la constante buttonPin el valor 3, que                       *     
* es el pin asociado a las interrupciones y es                           *
* diferente en cada modelo de Arduino.                                   *
*                                                                        *
* Los LEDs azules se encienden por una condición HW                      *
* (están conectados con el Speaker y se encienden cuando este            *
* emite sonido), y simulan la base del arbol.                            *
*                                                                        *
*************************************************************************/

 
#include "TimerOne.h"                                                        //Libreria para el timerOne de Arudino.
#include <TimerFreeTone.h>                                                   //Esta es una libreria adicional, que utilizar para usar un timer asociado a una nota.
                                                                             //TimerFreeTone(Pin del Speaker, frecuencia[], duracion[] [,volumen*]). 
                                                                             //*opcional, volumen asociado al tono (1-10, 10 sería el valor por defecto).

#define TONE_PIN 12                                                          //Pin al que se conecta el Speaker. 
const int buttonPin = 2;                                                     //Pin al que se conecta el Pulsador

int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262, 0 };                  //Es la frecuencia de las notas de la primera melodia.
int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250, 250 };            //Duracion de las notas de la primera melodia.
int melody2[] = { 262, 262, 196, 196,0, 247,0, 247,220, };                   //Es la frecuencia de las notas de la segunda melodia.
int duration2[] = { 250, 250, 150, 250, 125, 125, 250, 250, 280 };           //Duracion de las notas de la segunda melodia.
int buttonState = 0;                                                         //Variable usada para leer el estado del Pulsador.
int bandera = 1;                                                             

unsigned long rebote1 = 0;                                                   //Variable usada para el antirrebote de la interrupción. 
unsigned long rebote2 = 0;                                                   //Variable usada para el antirrebote de la interrupción.


void setup()
{  
  pinMode(3,OUTPUT);                                                         //Pin 3, salida. LED blanco que simula la estrella del arbol.
  pinMode(10, OUTPUT);                                                       //Pin 10, salida. LED rojo que simula la parte mas alta del arbol.
  pinMode(buttonPin, INPUT);                                                 //Pin 2, que es el que usamos para las interrupciones.
  Timer1.initialize(500000);                                                 //Inicializa el timer1 y le asigna un periodo de 1/2 segundos.
  Timer1.attachInterrupt(callback);                                          //Adjunta callback() como una interrupcion de desbordamiento del timer.
  buttonState = digitalRead(buttonPin);                                      //Asignamos un nombre a la lectura del estado del Pulsador.
  attachInterrupt(digitalPinToInterrupt(2), boton, FALLING);                 //Usaremos la interrupción del pin 2 asociado al Pulsador que llama a la rutina boton, 
                                                                             //y usaremos el modo FALLING para el flanco de bajada del Pulsador
  rebote2 = millis();                                                        //La variable rebote2 se inicia nada más empezar.
}
 
void callback()
{
  digitalWrite(10, digitalRead(10) ^ 1);                                     //Callback para cambiar el LED del puerto 10 (LED rojo) asociado al timerOne. 
                                                                             //El parpadeo ira a la velocidad indicada en Timer1.initialize(vel); 
}

 void boton()
{
  rebote1 = millis();                                                        //En este momento se inicia la variable rebote1.
  if((rebote1 - rebote2) > 100) {                                            //Proteccion antirrebote.
    bandera = bandera * (-1);
    rebote2 = rebote1;
  }
}

void loop()
{
    if (bandera == 1) {                                                      //Accion que realiza si el botón está en HIGH.
      for (int thisNote = 0; thisNote < 9; thisNote++) {                     //Bucle que recorre las notas en el array.
        TimerFreeTone(TONE_PIN, melody[thisNote], duration[thisNote]);       
        if(duration[thisNote] == 250) {                                      //Si la nota es una negra, asina un pwm en el pin 9 al 100% del ciclo de trabajo
                                                                             //(pin asociado a los dos leds verdes, que simulan la parte intermedia de nuestro arbol).
                                                                             //Además, apaga el led blanco (que simula una estrella).
          Timer1.pwm(9,1024);                                                
          digitalWrite(3,LOW);                                               
        }
        if(duration[thisNote] == 125) {                                       //Si la nota es una corchea (mitad de una negra), asigna un pwm en el pin 9 al 50% del ciclo de trabajo
                                                                             //(pin asociado a los dos leds verdes, que simulan la parte intermedia de nuestro arbol).
                                                                             //Además, apaga el led blanco (que simula una estrella).
          Timer1.pwm(9,512);                                                
          digitalWrite(3,HIGH);
        }
      }
    }else if (bandera == -1) {                                               //Accion que realiza si el botón está en LOW.
      for (int thisNote2 = 0; thisNote2 < 9; thisNote2++) {                 
        TimerFreeTone(TONE_PIN, melody2[thisNote2], duration2[thisNote2], 6);  
        if(duration2[thisNote2] == 250) {
          Timer1.pwm(9,1024);
          digitalWrite(3,LOW);
        }
        if(duration2[thisNote2]==125) {
          Timer1.pwm(9,512);
          digitalWrite(3,HIGH);
        }
      }
    } 
}
