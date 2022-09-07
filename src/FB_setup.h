#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://ta-hendra-f9f06-default-rtdb.firebaseio.com/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "baV0cFwJF5aG62RHvG8o64B8pkYKgH1xk9aICxSU"
// #define WIFI_SSID "Kost oliv lt.3"
// #define WIFI_PASSWORD "tarianraya"
#define WIFI_SSID  "Delta"
#define WIFI_PASSWORD "sandidelta"
bool online = false;

FirebaseData firebaseData;
FirebaseJson json;

void lcd_clear_line(int row, int line){
  lcd.setCursor(row,line);
  lcd.print("                ");
}

void connectwifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    lcd.setCursor(0,0);
    lcd.print("Connect to Wi-Fi");
    lcd_clear_line(0,1);
    int i = 0;
    int j = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        lcd.setCursor(i,1);
        lcd.print(".");
        i++;
        
        if (i>=16){
          delay(300);
          lcd_clear_line(0,1);
          i = 0;
          j++;
        }

        if (j>=3){
          //printf("Gagal terhubung WIFI..\n");
          Serial.println("\nGagal terhubung WIFI...");
          delay(2000);
          lcd.setCursor(0,0);
          lcd.print("  OFFLINE MODE  ");
          delay(2000);
//          Serial.println(">>>>>> Mode Offline <<<<<<<");
          break;
        }
        
        delay(300);
    }
    
    if (WiFi.status() == WL_CONNECTED){

      online = true;
      Serial.println();
      Serial.print("Connected with IP: ");
      Serial.println(WiFi.localIP());
      Serial.println();

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Connected to  ");
      lcd.setCursor(0,1);
      lcd.print(WiFi.localIP());
      delay(1000);
      lcd.clear();

      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      Firebase.reconnectWiFi(true);

      // Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
      firebaseData.setBSSLBufferSize(1024, 1024);

      // Set the size of HTTP response buffers in the case where we want to work with large data.
      firebaseData.setResponseSize(1024);

      // Set database read timeout to 1 minute (max 15 minutes)
      Firebase.setReadTimeout(firebaseData, 1000 * 60);
      // tiny, small, medium, large and unlimited.
      // Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
      Firebase.setwriteSizeLimit(firebaseData, "tiny");
    
    } else{
      online = false;
    }
}

void IoT_cloud()
{
    connectwifi();
}

void printResult(FirebaseData &data)
{

    if (data.dataType() == "int")
        Serial.println(data.intData());
    else if (data.dataType() == "float")
        Serial.println(data.floatData(), 5);
    else if (data.dataType() == "double")
        printf("%.9lf\n", data.doubleData());
    else if (data.dataType() == "boolean")
        Serial.println(data.boolData() == 1 ? "true" : "false");
    else if (data.dataType() == "string")
        Serial.println(data.stringData());
    else if (data.dataType() == "json")
    {
        Serial.println();
        FirebaseJson &json = data.jsonObject();
        //Print all object data
        Serial.println("Pretty printed JSON data:");
        String jsonStr;
        json.toString(jsonStr,true);
        Serial.println(jsonStr);
        Serial.println();
        Serial.println("Iterate JSON data:");
        Serial.println();
        size_t len = json.iteratorBegin();
        String key, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++)
        {
            json.iteratorGet(i, type, key, value);
            Serial.print(i);
            Serial.print(", ");
            Serial.print("Type: ");
            Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
            if (type == FirebaseJson::JSON_OBJECT)
            {
                Serial.print(", Key: ");
                Serial.print(key);
            }
            Serial.print(", Value: ");
            Serial.println(value);
        }
        json.iteratorEnd();
    }
    else if (data.dataType() == "array")
    {
        Serial.println();
        //get array data from FirebaseData using FirebaseJsonArray object
        FirebaseJsonArray &arr = data.jsonArray();
        //Print all array values
        Serial.println("Pretty printed Array:");
        String arrStr;
        arr.toString(arrStr,true);
        Serial.println(arrStr);
        Serial.println();
        Serial.println("Iterate array values:");
        Serial.println();
        for (size_t i = 0; i < arr.size(); i++)
        {
            Serial.print(i);
            Serial.print(", Value: ");

            FirebaseJsonData &jsonData = data.jsonData();
            //Get the result data from FirebaseJsonArray object
            arr.get(jsonData, i);
            if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
                Serial.println(jsonData.boolValue ? "true" : "false");
            else if (jsonData.typeNum == FirebaseJson::JSON_INT)
                Serial.println(jsonData.intValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
              Serial.println(jsonData.floatValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
                printf("%.9lf\n", jsonData.doubleValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
                     jsonData.typeNum == FirebaseJson::JSON_NULL ||
                     jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
                     jsonData.typeNum == FirebaseJson::JSON_ARRAY)
                Serial.println(jsonData.stringValue);
        }
    }
    else
    {
      Serial.println(data.payload());
    }
}