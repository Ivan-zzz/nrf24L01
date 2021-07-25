#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8);

const uint32_t pipe1 = 123; // адрес в канале 2
const uint32_t pipe2 = 789; // адрес в канале 2




void setup()   
{
  Serial.begin(115200);
  radio.begin();
//  radio.setAutoAck(false);       // - автоответ.
  delay(2000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_16); // длинна контрольной суммы 8-bit or 16-bit
  radio.setPALevel(RF24_PA_MIN); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
                                 // соответствует уровням:    -18dBm,      -12dBm,      -6dBM,           0dBm
  radio.setChannel(121);         // уствновка канала
  radio.powerUp();               // включение или пониженное потребление powerDown - powerUp
  radio.openReadingPipe(1,pipe1); // открыть канал 1 на приём
  radio.openWritingPipe(pipe2);   // открыть канал 2 на отправку
 
}

//  byte data[4];
//  byte msg_from_A[4];   // - числа

  char msg_to_A1[40] = " Received, CRC=OK";
  char msg_to_A2[40] = " Received, WRONG CRC";
  byte X = 0;    // числовая переменная (позже приравняем её к контрольной сумме)
//  ????  как сделать??? char Y = "all together"; // текстовая переменная совмещённая позже с цифрой

void loop()  {
  // {
//   if(radio.available())
//     {
//       radio.read(data, 1);
//      Serial.println(data[0]);
//      radio.write(&data, 1);
//      delay(100);   
//   }
//}


//  char msg_from_A[40];  // - символы
 radio.startListening(); // приём
delay(2000);
byte data[10],ABC;

  if ( radio.available()) {
    while (radio.available()) {                                   // While there is data ready
      radio.read(&data, sizeof(data));
      // расчитываем CRC принятых данных
   ABC = data[0] + data[1] + data[2] + data[3] + data[4];
//   ABC = 100;
X=ABC;
// Y= msg_to_A1 + X;
      

// выводим в порт данные
        Serial.print("Received (");
        Serial.print(sizeof(data));
        Serial.println("): ");
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

   
    
//    radio.read( &msg_from_A, sizeof(msg_from_A) );             // Get the payload
//    Serial.print(F("Got message from A = "));
//    Serial.print(msg_from_A[0]);
    
    radio.stopListening();                                        // First, stop listening so we can talk
//    radio.write(msg_from_A[0], 1);                             // повторяем что мы получили от А на В
      // проверяем CRC
if (data[5] == ABC)
{    

    radio.write( &msg_to_A1, sizeof(msg_to_A1) );              // Send the final one back.
    radio.write(&X, sizeof(X) );              // отправка контрольной суммы.
///    radio.write(&Y, sizeof(Y) );              // 
    Serial.println(F("Sent POSITIVE response to A"));
    Serial.println(" =================================================================== ");
// Serial.println(msg_to_A1);

} else {

    radio.write( &msg_to_A2, sizeof(msg_to_A2) );              // Send the final one back.
    radio.write(&X, sizeof(X) );              // отправка контрольной суммы.
    Serial.print(F("Sent NEGATIVE response to A"));
Serial.println(msg_to_A2);
    Serial.println(" =================================================================== ");
    Serial.println(F("'"));  
//    delay(100);}
}
}    
} else {
    Serial.println(F(" nothing received "));
    Serial.println(" =================================================================== ");
//    delay(1000);
    Serial.println(F(" ")); 
}
}
