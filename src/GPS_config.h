#include "Ublox.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define N_FLOATS 3

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Ublox M8_Gps;
float gpsArray[N_FLOATS] = {0, 0, 0};
//SoftwareSerial mySerial(RX,TX);
SoftwareSerial mySerial(12, 13);

char Buf[50];
char * Messagepad = "     GPS Signal Not Detected, Please Wait...     ";
bool gps_state = false;
unsigned long prevMillis;

void readGPS(void){

  if (!mySerial.available())
    return;
    
  while (mySerial.available())
  {
    char c = mySerial.read();
    if (M8_Gps.encode(c))
    {
      //gpsArray[0] = M8_Gps.altitude;
      gpsArray[0] = M8_Gps.latitude;
      gpsArray[1] = M8_Gps.longitude;
      gpsArray[2] = M8_Gps.sats_in_use;
    }
  }
  
//   for (byte i = 0; i < 3; i++)
//   {
//     if (i == 2)
//     {
//       Serial.print(gpsArray[i], 0); Serial.println("");
//     }
//     else
//     {
//       Serial.print(gpsArray[i], 6); Serial.print("_");
//     }
//   }
}

void showLetters(int printStart, int startLetter)
  {
    //char Buf[50];
    lcd.setCursor(printStart, 1);
    for (int letter = startLetter; letter <= startLetter + 15; letter++) // Print only 16 chars in Line #2 starting 'startLetter'
    {
      lcd.print(Messagepad[letter]);
    }
    lcd.print(" ");
    delay(250);
  }

void display(){

//  if (millis() - prevMillis >= 15000){
//      prevMillis = millis();
//      if (gps_state == false) {
//        gps_state = true;
//        Serial.println("TRUE");
//      }
//    }

    if (gpsArray[2] == 0){
      lcd.setCursor(0,1);
      lcd.print(" Scan Signal... ");
     // delay(1000);
//      if (gps_state == true){
//        for (int letter = 0; letter <= strlen(Messagepad) - 16; letter++) //From 0 to upto n-16 characters supply to below function
//          {
//            showLetters(0, letter);
//          }
//      }
//      gps_state = false;
    }
    else {
      lcd.setCursor(0,1);
      lcd.print(" GPS on Signal! ");
      //delay(1000);
    }
}
