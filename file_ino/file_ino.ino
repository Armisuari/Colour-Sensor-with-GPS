////////////////////////Author by Armisuari//////////////////////
///////////////////////client: Hendra///////////////////////////

#include "GPS_config.h"
#include "FB_setup.h"
#include "Colour_sens.h"
#include <Thread.h>
#include <ThreadController.h>

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

Thread Thread1 = Thread();
Thread Thread2 = Thread();

unsigned long prevMillis2;
unsigned long prevMillis3;

int data_calibrate = 0;
String hasil_online;

void setup(void)
{
  Serial.begin(115200);
  mySerial.begin(9600);
  EEPROM.begin(512);

  lcd.init();
  // lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(" Loading Data.. ");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("  Please Wait!  ");

  /////////////////////////////////Load Red/////////////////////////////////////////////////////
  int new_addr = readStringFromEEPROM(eepromOffset, &REDstring);
  pharse(REDstring, RED_C);
  delay(1000);
  printf("\n\nRED DATA >> R: %i G: %i B: %i\n", RED_C[0], RED_C[1], RED_C[2]);

  ////////////////////////////////Load Green///////////////////////////////////////////////////
  int new_addr1 = readStringFromEEPROM(new_addr, &GREENstring);
  pharse(GREENstring, GREEN_C);
  delay(1000);
  printf("\n\nGREEN DATA >> R: %i G: %i B: %i\n", GREEN_C[0], GREEN_C[1], GREEN_C[2]);

  ////////////////////////////////Load Blue////////////////////////////////////////////////////
  int new_addr2 = readStringFromEEPROM(new_addr1, &BLUEstring);
  pharse(BLUEstring, BLUE_C);
  delay(1000);
  printf("\n\nBLUE DATA >> R: %i G: %i B: %i\n", BLUE_C[0], BLUE_C[1], BLUE_C[2]);

  ////////////////////////////////Load Black//////////////////////////////////////////////////
  int new_addr3 = readStringFromEEPROM(new_addr2, &BLACKstring);
  pharse(BLACKstring, BLACK_C);
  delay(1000);
  printf("\n\nBLACK DATA >> R: %i G: %i B: %i\n", BLACK_C[0], BLACK_C[1], BLACK_C[2]);

  ////////////////////////////////Load Yellow////////////////////////////////////////////////
  int new_addr4 = readStringFromEEPROM(new_addr3, &YELLOWstring);
  pharse(YELLOWstring, YELLOW_C);
  delay(1000);
  printf("\n\nYELLOW DATA >> R: %i G: %i B: %i\n", YELLOW_C[0], YELLOW_C[1], YELLOW_C[2]);

  ////////////////////////////////Load White//////////////////////////////////////////////////
  int new_addr5 = readStringFromEEPROM(new_addr2, &WHITEstring);
  pharse(WHITEstring, WHITE_C);
  delay(1000);
  printf("\n\nWHITE DATA >> R: %i G: %i B: %i\n", WHITE_C[0], WHITE_C[1], WHITE_C[2]);

  delay(2000);

  IoT_cloud();

  //  Thread1.onRun(readGPS);
  //  Thread1.setInterval(10);

  Thread2.onRun(GetColors);
  Thread2.setInterval(100);

  controll.add(&Thread1);
  controll.add(&Thread2);

  delay(2000);

  while (true)
  {
    controll.run();
    readGPS();
    // delay(10);
    display();

    read_button = analogRead(button);

    if (online == true)
    {
      printf(">>>>>>>>>>>>>>>>>DEVICE ONLINE MODE<<<<<<<<<<<<<<<<<<");

      if (data_calibrate == 1 || read_button == 1024) // if button pressed
      {
        Calibrate();
        Firebase.setInt(firebaseData, "/Colour/Calibrate/state", 0);
      }

      if (millis() - prevMillis2 >= 1000)
      {
        prevMillis2 = millis();

        if (WiFi.status() != WL_CONNECTED)
        {
          connectwifi();
        }

        // printf("\nread_button: %i\n", read_button);
        // printf("data calibrate: %i\n", data_calibrate);
        printf("\nGPS >> Lt: %f Lg: %f\n", gpsArray[0], gpsArray[1]);
        printf("Red: %i Green: %i Blue: %i\n", Red, Green, Blue);
        // colour_result();
        String colour_read = String(Red) + "@" + String(Green) + "@" + String(Blue);
        Firebase.setString(firebaseData, "/Colour/read", colour_read) ? printf("Send data RGB Succes\n") : printf("Failed send data RGB\n");
        // Firebase.getString(firebaseData, "/Colour/hasil_online") ? hasil_online = firebaseData.stringData() : Serial.println(firebaseData.errorReason());

        if (Firebase.getString(firebaseData, "/Colour/hasil_online"))
        {
          hasil_online = firebaseData.stringData();
        }
        else
        {
          Serial.println(firebaseData.errorReason());
        }

        // printf("Result ==> %s\n", String(hasil_online));
        Serial.print("Result ==> ");
        Serial.println(hasil_online);
        // lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  >> " + hasil_online + " <<   ");

        Firebase.getInt(firebaseData, "/Colour/Calibrate/state") ? data_calibrate = firebaseData.intData() : Serial.println(firebaseData.errorReason());
      }

      if (millis() - prevMillis3 >= 5000)
      {
        prevMillis3 = millis();
        Firebase.setFloat(firebaseData, "/GPS/Latitude", gpsArray[0]) ? printf("Send data Latitude Succes\n") : printf("Failed send data Latitude\n");
        Firebase.setFloat(firebaseData, "/GPS/Longitude", gpsArray[1]) ? printf("Send data Longitude Succes\n") : printf("Failed send data Longitude\n");
        Firebase.setFloat(firebaseData, "/GPS/Num_Satelite", gpsArray[2]) ? printf("Send data Satelite Succes\n") : printf("Failed send data Satelite\n");
      }
    }
    else
    {
      printf(">>>>>>>>>>>>>>>>>DEVICE OFFLINE MODE<<<<<<<<<<<<<<<<<<");

      if (/*data_calibrate == 1 ||*/ read_button == 1024) // if button pressed
      {
        Calibrate();
      }

      if (millis() - prevMillis2 >= 2000)
      {
        prevMillis2 = millis();

        // printf("\nread_button: %i\n", read_button);
        // printf("data calibrate: %i\n", data_calibrate);
        printf("\nGPS >> Lt: %f Lg: %f\n", gpsArray[0], gpsArray[1]);
        printf("Red: %i Green: %i Blue: %i\n", Red, Green, Blue);
        colour_result();
      }
    }
  }
}

void loop() {}
