// Rutina para enviar comando arranque y paro ventilador de techo mediante RF.

//Received 2795067714 / 32bit Protocol: 10 / 424
//Received 2774882626 / 32bit Protocol: 10 / 424
//=================
//Arranque ventilador:2795067714
//Paro ventilador: 2774882626

// Para poder ver los codigos cada vez que se envian,  esta rutina tiene en el mismo modulo ESP32 dos modulos: 1 emisor y 1 receptor.

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch(); // Receptor
RCSwitch mySwitch2 = RCSwitch(); // Emisor
bool activado=false;
bool parado=false;

#define RXD2 27 // pin 27 ESP32 utilizado para emisor RF
const int pulsadorGPIO = 13; // interruptor para encender / apagar ventilador
const int ledGPIO = 2; 
bool estadoBoton =  false; 


void setup() {
  Serial.begin(115200);
  Serial.print("Ready to receive.");  
  mySwitch.enableReceive(RXD2); //habilita recepcion
  mySwitch2.enableTransmit(15); // habilita emisor
  mySwitch2.setProtocol(10); // selecciona protocolo 10 para el emisor (el utilizado por el ventilador)
  pinMode(pulsadorGPIO, INPUT_PULLDOWN);    
  pinMode(ledGPIO, OUTPUT);
  
}

void loop() {    
  if (mySwitch.available()) {  
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( mySwitch.getReceivedProtocol() );
    Serial.print(" / ");
    Serial.println( mySwitch.getReceivedDelay() );

    mySwitch.resetAvailable();
  }
  estadoBoton = digitalRead(pulsadorGPIO); //Comprueba si esta activado el interruptor.
  
  if (estadoBoton == HIGH && !activado) {      
       digitalWrite(ledGPIO, HIGH);
       mySwitch2.send(2795067714, 32);
       Serial.println("Enviado Arranque ventilador");
       digitalWrite(ledGPIO, LOW);
       delay(1000);
       digitalWrite(ledGPIO, HIGH);
       mySwitch2.send(2795067714, 32);
       digitalWrite(ledGPIO, LOW);
       activado=true;
       parado=false;   
  }  
  if (estadoBoton == LOW && !parado) {      
       digitalWrite(ledGPIO, HIGH);
       mySwitch2.send(2774882626, 32);
       digitalWrite(ledGPIO, LOW);
       Serial.println("Enviado Paro ventilador");
       delay(1000);
       digitalWrite(ledGPIO, HIGH);
       mySwitch2.send(2774882626, 32);
       digitalWrite(ledGPIO, LOW);
       activado=false;
       parado=true;  
  }  

}
