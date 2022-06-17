
#include <BMP280_DEV.h> 
#include <Adafruit_BMP280.h>
BMP280_DEV bmp280;
float temperature, pressure, altitude;
int altitude_data = 0;
String message_bmp = "";
boolean start = true;
void setup()
{
  Serial.begin(115200);          
  bmp280.begin(BMP280_I2C_ALT_ADDR);
  bmp280.setTimeStandby(TIME_STANDBY_2000MS);
  bmp280.startNormalConversion();
}

void loop()
{
  message_bmp = ""; 
  if (bmp280.getMeasurements(temperature, pressure, altitude))    
  {
    if (start) {
      delay(2000);
      altitude_data = altitude;
      String start_message = " Высота на старте= " + String(altitude_data);
      Serial.println(start_message); 
      start = false;
    }
    Serial.print(temperature);
    Serial.print(F("*C   "));
    Serial.print(pressure);
    Serial.print(F("hPa   "));
    Serial.print(altitude);
    Serial.println(F("m"));
    message_bmp = " Температура = " + String(temperature) + " Давление = " + String(pressure) + " Высота = " + String(altitude);
    Serial.println(message_bmp);
  }
}
