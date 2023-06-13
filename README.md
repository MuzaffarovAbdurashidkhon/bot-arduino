Робот на ардуино
Бот был протестирован на трассе. Время прохождения трассы - 5 минута 36 секунды. Пройдены 3/4 испытаний.

Робот на Arduino Leonardo с радиомодулем NRF24. Для лучшего сцепления с гладкой поверхностью на гусиницы был нанесен термоклей в небольшом количестве.

![photo_2023-06-12_00-51-14](https://github.com/MuzaffarovAbdurashidkhon/bot-arduino/assets/116067226/35b40e4d-948f-4b3d-b6ed-71afb111970d)

![IMG_20230612_004057](https://github.com/MuzaffarovAbdurashidkhon/bot-arduino/assets/116067226/a6a16a98-555e-4fbb-aad7-74bb36f6d34f)


В роботе использовалось:

Motor Shield,

Arduino Leonardo/Amperka Iskra Neo,

NRF,

NRF+,

акумуляторы 18650 x2,

DC motor x2,

RGB LED  (12 светодиода),

servo 360 x1.



https://github.com/MuzaffarovAbdurashidkhon/bot-arduino/assets/116067226/fda593c7-4616-4aea-8c3c-866edd7bf1e3

Для контроллера использовалась плата Arduino Pro Micro. Джойствик отвечал за движение по оси Y,X

![photo_2023-06-12_00-59-01](https://github.com/MuzaffarovAbdurashidkhon/bot-arduino/assets/116067226/f066537f-93ec-4936-9b58-ac70c2b8e2c0)
![photo_2023-06-12_00-58-55](https://github.com/MuzaffarovAbdurashidkhon/bot-arduino/assets/116067226/2297a1e0-3a49-45dd-8b5c-c804429b4c0d)

В контроллере использовалось:

Arduino Pro Micro,

NRF,

NRF+,

повербанк,

потенциометр,

джойстик.

Использованные библиотеки:

Servo by Michael Margolis (управление сервоприводами),

Adafruit NeoPixel by Adafruit (настройка светодиодной ленты),

RF24 by TMRh20 (управление радиомодулем).

1. Код для контроллера:

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



2. Код для робота:

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

