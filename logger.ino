#include "HX711.h"
#include <SPI.h>
#include <SD.h>

/* El lector SD card conectado como sigue:
 *  MOSI - pin 11
 *  MISO - pin 12
 *  CLK - pin 13
 *  CS - pin 4
*/

/* Conectar el pulsador de inicio y el buzzer como sigue:
 *  Botón activación - pin 10
 *  LED - pin 5
 *  Buzzer - pin 6
  */

#define buttonPin 4   //  Pin para el botón activación.
#define ledPin 5
#define buzzPin 6
#define igniterPin 7   //  OJO!!! pin que va a la etapa de potencia del ignitor!!!
#define frecBuzz 440
#define powerPin 8

//Parte sensor empuje HX711
//Pines  2, 3 y 8
#define CLK  2    // Pin para el reloj
#define DOUT  3   // Pin para salida de datos
long dato; //Variable que recoge la lectura del peso
//Se inicializa el HX711
HX711 scale(DOUT, CLK);

//Parte SD pines 
#define SD_CS_PIN 10

File myFile;

void setup() {
  Serial.begin(38400); 
  while (!Serial) {
    ; // Esperamos conexión por USB.
  }
  Serial.print("Inicializando txartela SD ...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Fallo en la inicialización! NO GO!");
    return;
  }
  Serial.println("Txartela inicializada! Cinco Segundos de retardo");
  delay(5000);

  pinMode(buttonPin, INPUT); 
  pinMode(igniterPin, OUTPUT); 
  pinMode(ledPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin,HIGH);
  Serial.println("HX711 Rocket Motor Dynamometer, V.5"); 
  Serial.println("Poner la tobera mirando arriba ;) Introducir el ignitor el la tobera. Fuera de la zoana de peligro!!!"); 
  delay(2000); 
  Serial.println("Procediendo a calibrar el stand. Tomando lecturas de peso en reposo"); 
  scale.set_scale(); 
  scale.tare(); //Reset the scale to 0 
  long zero_factor = scale.read_average(); //Get a baseline reading 
  Serial.print("Este valor es de referencia: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects. 
  Serial.println(zero_factor);
 /* while (digitalRead(buttonPin==HIGH)){
    Serial.print("Pulsar el botón RUN para comenzar la secuencia de 15 tonos para comenzar a leer valores durante 10 segundos:");
    
  }
  */
  for (int f=0;f<10;f++) {
    digitalWrite(ledPin,HIGH);
    tone(buzzPin,frecBuzz,500);
    digitalWrite(ledPin,LOW);
    noTone(buzzPin);
    delay(500);
  }
  Serial.println();
  Serial.println("Abriendo el fichero Kohete.txt para logging...");
  
  myFile= SD.open("Kohete.txt", FILE_WRITE);
  myFile.print("FactMor de cero: ");
  myFile.println(zero_factor);
  myFile.print("Timestamp inicio: ");
  myFile.println(millis());
  myFile.println("Desde esta posicion lekturas:");
 
  Serial.println("Hasi!!");
  for (int f=0;f<800;f++){
    dato=scale.read();
    myFile.print(dato);
    myFile.print(":");
//    Serial.print(dato);
//    Serial.print(":");
    }
 // closes the file:
  myFile.print("Timestamp final: ");
  myFile.println(millis());
  myFile.close(); 
  Serial.print("Pograma finalizado.Pueden retirar la txartela SD");  
} 

void loop() { 
} 
