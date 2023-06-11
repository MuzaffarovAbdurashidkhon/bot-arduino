#include <SPI.h> //RF24 by TMRh20
#include <nRF24L01.h>
#include <RF24.h>

#define VRx A0
#define VRy A1

RF24 radio(8, 10);
int data[2];

void setup(){
   radio.begin();
  radio.setChannel(4);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(0x1234567890LL);
  radio.stopListening();
}

void loop(){

   int valx = analogRead (VRx);
   int valy = analogRead (VRy);
   data[0] = valx;
   data[1] = valy;

  radio.write(&data, sizeof(data));
  delay(100);
}