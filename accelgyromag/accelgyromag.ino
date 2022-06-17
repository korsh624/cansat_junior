
#include <Adafruit_BMP280.h>
#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
String accelgyromag = "";
void setup()
{
  Serial.begin(115200);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void loop()
{
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
  String accelgyromag = "accelX=" + String(aX) + " taccelY=" + String(aY) + " taccelZ=" + String(aZ) + " tgyroX=" + String(gX) + " tgyroY=" + String(gY) + " tgyroZ=" + String(gZ) + " tmagX=" + String(mX) + " tmaxY=" + String(mY) + " tmagZ=" + String(mZ) + " thorizontalDirection=" + String(mDirection);
  Serial.println(accelgyromag);
}
