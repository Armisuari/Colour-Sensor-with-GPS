//#include <Arduino.h>
#include <EEPROM.h>

#define s0 D4 // Module pins wiring
#define s1 D3
#define s2 D8
#define s3 D5
#define out D0

#define pin D7
#define ON 0
#define OFF 1
#define button A0
#define toleransi 10

bool red, green, blue, black, yellow, white, unknown;
int Red = 0, Blue = 0, Green = 0; // RGB values
int RED_C[3];
int GREEN_C[3];
int BLUE_C[3];
int BLACK_C[3];
int YELLOW_C[3];
int WHITE_C[3];

String REDstring, GREENstring, BLUEstring, BLACKstring, YELLOWstring, WHITEstring;
int eepromOffset = 0;
int newaddr, newaddr1, newaddr2, newaddr3, newaddr4, newaddr5;
int read_button;

int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}
int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

void pharse(String str, int strs[3])
{
  int StringCount = 0;
  while (str.length() > 0)
  {
    int index = str.indexOf('@');
    if (index == -1) // No space found
    {
      strs[StringCount++] = str.toInt();
      break;
    }
    else
    {
      strs[StringCount++] = (str.substring(0, index)).toInt();
      str = str.substring(index + 1);
    }
  }
}

void GetColors()
{
  digitalWrite(s2, LOW); // S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
  digitalWrite(s3, LOW);
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); // here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);
  digitalWrite(s3, HIGH); // Here we select the other color (set of photodiodes) and measure the other colors value using the same techinque
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);
  digitalWrite(s2, HIGH);
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);

  // delay(2000);
}

void Calibrate()
{
  printf("Calibrating...\n");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Calibraitng... ");
  delay(1000);

  printf("For RED...\n");
  lcd.setCursor(0, 0);
  lcd.print("   Get RED...   ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  RED_C[0] = Red;
  RED_C[1] = Green;
  RED_C[2] = Blue;
  REDstring = String(RED_C[0]) + "@" + String(RED_C[1]) + "@" + String(RED_C[2]);
  newaddr = writeStringToEEPROM(eepromOffset, REDstring);
  Serial.println(REDstring);
  delay(3000);

  printf("For GREEN...\n");
  lcd.setCursor(0, 0);
  lcd.print("  Get GREEN...  ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  GREEN_C[0] = Red;
  GREEN_C[1] = Green;
  GREEN_C[2] = Blue;
  GREENstring = String(GREEN_C[0]) + "@" + String(GREEN_C[1]) + "@" + String(GREEN_C[2]);
  newaddr1 = writeStringToEEPROM(newaddr, GREENstring);
  Serial.println(GREENstring);
  delay(3000);

  printf("For BLUE...\n");
  lcd.setCursor(0, 0);
  lcd.print("  Get BLUE...  ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  BLUE_C[0] = Red;
  BLUE_C[1] = Green;
  BLUE_C[2] = Blue;
  BLUEstring = String(BLUE_C[0]) + "@" + String(BLUE_C[1]) + "@" + String(BLUE_C[2]);
  newaddr2 = writeStringToEEPROM(newaddr1, BLUEstring);
  Serial.println(BLUEstring);
  delay(3000);

  printf("For BLACK...\n");
  lcd.setCursor(0, 0);
  lcd.print("  Get BLACK...  ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  BLACK_C[0] = Red;
  BLACK_C[1] = Green;
  BLACK_C[2] = Blue;
  BLACKstring = String(BLACK_C[0]) + "@" + String(BLACK_C[1]) + "@" + String(BLACK_C[2]);
  newaddr3 = writeStringToEEPROM(newaddr2, BLACKstring);
  Serial.println(BLACKstring);
  delay(3000);

  printf("For YELLOW...\n");
  lcd.setCursor(0, 0);
  lcd.print("  Get YELLOW...  ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  YELLOW_C[0] = Red;
  YELLOW_C[1] = Green;
  YELLOW_C[2] = Blue;
  YELLOWstring = String(YELLOW_C[0]) + "@" + String(YELLOW_C[1]) + "@" + String(YELLOW_C[2]);
  newaddr4 = writeStringToEEPROM(newaddr3, YELLOWstring);
  Serial.println(YELLOWstring);
  delay(3000);

  printf("For WHITE...\n");
  lcd.setCursor(0, 0);
  lcd.print("  Get WHITE...  ");
  delay(3000);
  GetColors();
  printf("Get Colours...\n");
  delay(3000);
  WHITE_C[0] = Red;
  WHITE_C[1] = Green;
  WHITE_C[2] = Blue;
  WHITEstring = String(WHITE_C[0]) + "@" + String(WHITE_C[1]) + "@" + String(WHITE_C[2]);
  newaddr5 = writeStringToEEPROM(newaddr4, WHITEstring);
  Serial.println(WHITEstring);
  delay(3000);

  EEPROM.commit();
  printf("Save data to EEPROM...\n");
  delay(2000);
  printf("Sendig Cakibrated data to Database...");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", REDstring) ? printf("Send data RED Succes\n") : printf("Failed send data colour\n");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", GREENstring) ? printf("Send data GREEN Succes\n") : printf("Failed send data colour\n");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", BLUEstring) ? printf("Send data GREEN Succes\n") : printf("Failed send data colour\n");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", WHITEstring) ? printf("Send data GREEN Succes\n") : printf("Failed send data colour\n");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", BLACKstring) ? printf("Send data GREEN Succes\n") : printf("Failed send data colour\n");
  Firebase.setString(firebaseData, "/Colour/Calibrate/RED", YELLOWstring) ? printf("Send data GREEN Succes\n") : printf("Failed send data colour\n");
  delay(2000);
  printf("done.\n");
  delay(1000);
}

void colour_result()
{
  if ((Red >= RED_C[0] - toleransi && Red <= RED_C[0] + toleransi) && (Green >= RED_C[1] - toleransi && Green <= RED_C[1] + toleransi) && (Blue >= RED_C[2] - toleransi && Blue <= RED_C[2] + toleransi)) // if Red value is the lowest one and smaller thant 23 it's likely Red
  {
    printf("Result ==> RED\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   >> RED <<");
    red = true;
    green = false;
    blue = false;
    black = false;
    yellow = false;
    white = false;
    // Firebase.setString(firebaseData, "/Colour/result", "red") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else if ((Red >= GREEN_C[0] - toleransi && Red <= GREEN_C[0] + toleransi) && (Green >= GREEN_C[1] - toleransi && Green <= GREEN_C[1] + toleransi) && (Blue >= GREEN_C[2] - toleransi && Blue <= GREEN_C[2] + toleransi)) // Same thing for Blue
  {
    printf("Result ==> GREEN\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  >> GREEN <<   ");
    red = false;
    green = true;
    blue = false;
    black = false;
    yellow = false;
    white = false;
    // Firebase.setString(firebaseData, "/Colour/result", "green") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else if ((Red >= BLUE_C[0] - toleransi && Red <= BLUE_C[0] + toleransi) && (Green >= BLUE_C[1] - toleransi && Green <= BLUE_C[1] + toleransi) && (Blue >= BLUE_C[2] - toleransi && Blue <= BLUE_C[2] + toleransi)) // Green it was a little tricky, you can do it using the same method as above (the lowest), but here I used a reflective object
  {
    printf("Result ==> BLUE\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   >> BLUE <<   ");
    // Firebase.setString(firebaseData, "/Colour/result", "blue") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else if ((Red >= BLACK_C[0] - toleransi && Red <= BLACK_C[0] + toleransi) && (Green >= BLACK_C[1] - toleransi && Green <= BLACK_C[1] + toleransi) && (Blue >= BLACK_C[2] - toleransi && Blue <= BLACK_C[2] + toleransi)) // Green it was a little tricky, you can do it using the same method as above (the lowest), but here I used a reflective object
  {
    printf("Result ==> BLACK\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  >> BLACK <<   ");
    // Firebase.setString(firebaseData, "/Colour/result", "black") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else if ((Red >= YELLOW_C[0] - toleransi && Red <= YELLOW_C[0] + toleransi) && (Green >= YELLOW_C[1] - toleransi && Green <= YELLOW_C[1] + toleransi) && (Blue >= YELLOW_C[2] - toleransi && Blue <= YELLOW_C[2] + toleransi)) // Green it was a little tricky, you can do it using the same method as above (the lowest), but here I used a reflective object
  {
    printf("Result ==> YELLOW\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  >> YELLOW <<  ");
    // Firebase.setString(firebaseData, "/Colour/result", "yellow") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else if ((Red >= WHITE_C[0] - toleransi && Red <= WHITE_C[0] + toleransi) && (Green >= WHITE_C[1] - toleransi && Green <= WHITE_C[1] + toleransi) && (Blue >= WHITE_C[2] - toleransi && Blue <= WHITE_C[2] + toleransi)) // Green it was a little tricky, you can do it using the same method as above (the lowest), but here I used a reflective object
  {
    printf("Result ==> WHITE\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  >> WHITE <<   ");
    // Firebase.setString(firebaseData, "/Colour/result", "white") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }

  else
  {
    printf("Result ==> UNKNOWN\n");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" >> UNKNOWN <<  ");
    // Firebase.setString(firebaseData, "/Colour/result", "unknown") ? printf("Send data color Succes\n") : printf("Failed send data colour\n");
  }
}
