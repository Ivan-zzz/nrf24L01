#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8); // можно использовать любые (порты 15-19 CSN CE MOSI MISO SCK)

const uint32_t pipe1 = 123; // адрес в канале 2
const uint32_t pipe2 = 789; // адрес в канале 2



void setup()
{
  Serial.begin(115200);
  radio.begin();                // старт 
//  radio.setAutoAck(false);       // - автоответ.
   delay(2000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_16); // длинна контрольной суммы 8-bit or 16-bit
  radio.setPALevel(RF24_PA_MIN); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
                                 // соответствует уровням:    -18dBm,      -12dBm,      -6dBM,           0dBm
  radio.setChannel(121);         // установка канала
  radio.powerUp();               // включение или пониженное потребление powerDown - powerUp
  radio.openWritingPipe(pipe1);   // открыть канал 1 на отправку
  radio.openReadingPipe(1,pipe2); // открыть канал 2 на приём
//  radio.startListening(); // приём

// void loop()   
// {
//  data = data + 1;
//  radio.write(&data, 1);
//  delay(1000);
//  Serial.println(data);

//  radio.read(data2, 1);
//   Serial.println(data2[0]);
//  radio.read(data2, 1);
//  Serial.println(data2[0]);
//  delay(1000); 
//  if(radio.available())
//    {
//      radio.read(data2, 1);
//      Serial.println(data2[0]);
//      delay(1000);   
//   }

}

// byte data = 0;
byte data2[4], CRC;   // входящие цифровые значения
char msg_from_B[25];                             // Grab the response, compare, and send to debugging spew
byte X[50];                             

void loop() {

byte data[6] ;   // в скобках количество байт в передаваемом массиве
// Для примера будем отправлять 5 произвольных данных + 1 CRC (сумма)
data[0] = random(256);
data[1] = random(256);
data[2] = random(256);
data[3] = random(256);
data[4] = random(256);
    
//    data[0] = 1;
//    data[1] = 2;
//    data[2] = 3;
//    data[3] = 4;
//    data[4] = 5;
    data[5] = data[0] + data[1] + data[2] + data[3] + data[4]; // CRC

//  data = data + 1;
  radio.stopListening(); 
    radio.write(&data, sizeof(data));  // отправляем массив  

//  radio.write(&data, 1)  ;             // отправляем значение
//  Serial.print(data);
//  Serial.print(F(" - "));
//  Serial.println(F("Sent to B"));
  radio.startListening();                                    // Now, continue listening
 delay(2000);

  if ( radio.available()) {
    while (radio.available()) {                                   // While there is data ready
    radio.read(&msg_from_B, sizeof(msg_from_B));    
    radio.read(&X, sizeof(X));    
//    radio.read(data2, 1);      // входящие сообщения в числовом формате


// Spew it
        Serial.print("SENT to B : ");
        Serial.print(sizeof(data));
        Serial.print("): ");
        Serial.print(data[0]);
        Serial.print(" , ");
        Serial.print(data[1]);
        Serial.print(" , ");
        Serial.print(data[2]);
        Serial.print(" , ");
        Serial.print(data[3]); 
        Serial.print(" , ");
        Serial.print(data[4]);
        Serial.print(" = ");
        Serial.print(data[5]);
        Serial.print(" (");
        Serial.print("CRC");
        Serial.println(")");

    Serial.print(F(" Got following response from B: ("));
    Serial.print(sizeof(msg_from_B));
    Serial.print(F(")"));
    Serial.println(msg_from_B);
    Serial.println(" ");
    Serial.print(F(" CRC value  = "));
    Serial.print(X[0]);
    Serial.print("(");    
    Serial.print(sizeof(X));
    Serial.println(")");
    Serial.println(" =================================================================== ");

   
//    Serial.print(data2[0]);
    Serial.println(F(" "));


//    Serial.print(F("', Round-trip delay "));
//    Serial.print(end_time - start_time);
//    Serial.println(F(" microseconds"));
 
}
} else {
          Serial.print("SENT to B : ");
        Serial.print(sizeof(data));
        Serial.print("): ");
        Serial.print(data[0]);
        Serial.print(" , ");
        Serial.print(data[1]);
        Serial.print(" , ");
        Serial.print(data[2]);
        Serial.print(" , ");
        Serial.print(data[3]); 
        Serial.print(" , ");
        Serial.print(data[4]);
        Serial.print(" = ");
        Serial.print(data[5]);
        Serial.print(" (");
        Serial.print("CRC");
        Serial.println(")");

  Serial.println(F("no answer"));
      Serial.println(" =================================================================== ");
  }
//  delay(500); }
}
