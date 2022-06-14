/////////////////////////////////////////////////////////////////////////////////////////////////////
// BMP280_DEV - I2C Communications (Alternative Address), Default Configuration, Normal Conversion
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <BMP280_DEV.h>                           // Include the BMP280_DEV.h library
#include <Adafruit_BMP280.h>
#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float temperature, pressure, altitude;            // Create the temperature, pressure and altitude variables
BMP280_DEV bmp280;                                // Instantiate (create) a BMP280_DEV object and set-up for I2C operation
int altitude_data = 0;
String message_bmp = "";
String accelgyromag="";
boolean start = true;
void setup()
{
  Serial.begin(115200);                           // Initialise the serial port
  bmp280.begin(BMP280_I2C_ALT_ADDR);              // Default initialisation with alternative I2C address (0x76), place the BMP280 into SLEEP_MODE
  //bmp280.setPresOversampling(OVERSAMPLING_X4);    // Set the pressure oversampling to X4
  //bmp280.setTempOversampling(OVERSAMPLING_X1);    // Set the temperature oversampling to X1
  //bmp280.setIIRFilter(IIR_FILTER_4);              // Set the IIR filter to setting 4
  bmp280.setTimeStandby(TIME_STANDBY_2000MS);     // Set the standby time to 2 seconds
  bmp280.startNormalConversion();                 // Start BMP280 continuous conversion in NORMAL_MODE
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

}

void loop()
{


  readbmp280();
  readaccelgyromag();

}
void readbmp280() {
  message_bmp = "";
  if (bmp280.getMeasurements(temperature, pressure, altitude))    // Check if the measurement is complete
  {
    if (start) {
      delay(2000);
      altitude_data = altitude;
      String start_message = " Высота на старте= " + String(altitude_data);
      Serial.println(start_message);
      start = false;
    }
    Serial.print(temperature);                    // Display the results
    Serial.print(F("*C   "));
    Serial.print(pressure);
    Serial.print(F("hPa   "));
    Serial.print(altitude);
    Serial.println(F("m"));
    message_bmp = " Температура = " + String(temperature) + " Давление = " + String(pressure) + " Высота = " + String(altitude);
    Serial.println(message_bmp);
  }

}
void readaccelgyromag(){
    if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    Serial.print("accelX: " + String(aX));
    Serial.print("\taccelY: " + String(aY));
    Serial.print("\taccelZ: " + String(aZ));
    Serial.print("\taccelSqrt: " + String(aSqrt));
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.print("\tgyroX: " + String(gX));
    Serial.print("\tgyroY: " + String(gY));
    Serial.print("\tgyroZ: " + String(gZ));
  }

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    Serial.print("\tmagX: " + String(mX));
    Serial.print("\tmaxY: " + String(mY));
    Serial.print("\tmagZ: " + String(mZ));
    Serial.print("\thorizontalDirection: " + String(mDirection));
  }

String accelgyromag="accelX=" + String(aX)+" taccelY=" + String(aY)+" taccelZ=" + String(aZ)+" tgyroX=" + String(gX)+" tgyroY=" + String(gY)+" tgyroZ=" + String(gZ)+ " tmagX=" + String(mX)+" tmaxY=" + String(mY)+" tmagZ=" + String(mZ)+" thorizontalDirection=" + String(mDirection);
Serial.println(accelgyromag);
}
