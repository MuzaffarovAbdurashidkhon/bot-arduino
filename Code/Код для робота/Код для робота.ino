#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define MATRIX_PIN    0
#define LED_COUNT 12

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

RF24 radio(8, 10);
int data[2];

 
void setup() {
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
    Serial.begin(9600);

  radio.begin();
  radio.setChannel(4);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, 0x1234567890LL);
  radio.startListening();
} 
 
void loop() {

  if (radio.available()){
    radio.read(&data, sizeof(data));

   for (int i = 0; i < matrix.numPixels(); i++) {
      matrix.setPixelColor(i, 255, 255, 255);
      matrix.show();  
    }


    if (data[0] > 900 ) {
      digitalWrite (DIR_1, LOW);
      analogWrite(SPEED_1, 200);  
      digitalWrite (DIR_2, HIGH);
      analogWrite(SPEED_2, 200);  
    }
    
    else if (data[0] < 200) {
      digitalWrite(DIR_1, HIGH);
      analogWrite(SPEED_1, 200);
      digitalWrite(DIR_2, LOW);
      analogWrite(SPEED_2, 200);
    }
    else if (data[1] < 200) {
      digitalWrite(DIR_2, HIGH);
      analogWrite(SPEED_2, 200);
      digitalWrite(DIR_1, HIGH);
      analogWrite(SPEED_1, 200);
    }

    else if (data[1] > 900) {
      digitalWrite(DIR_2, LOW);
      analogWrite(SPEED_2, 200);  
      digitalWrite(DIR_1, LOW);
      analogWrite(SPEED_1, 200); 
    }
    else {  
      digitalWrite(DIR_1, HIGH);
      analogWrite(SPEED_1, 0);
      digitalWrite(DIR_2, HIGH);
      analogWrite(SPEED_2, 0);  
    }

  }
}