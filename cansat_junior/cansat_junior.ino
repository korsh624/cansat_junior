#include <Servo.h>//подключаем библиотеку серво
#include <SPI.h>//SPI для работы с интерфейсом модулея карт памяти
#include <SD.h>//для работы с самим сд модулем
#include <BMP280_DEV.h> // библиотка для баромета
#include <Adafruit_BMP280.h>// для него же
#include <MPU9250_asukiaaa.h>// для гироскопа акселерометра и компаса
const int PIN_CHIP_SELECT = 4;// пин для включения sd модуля
MPU9250_asukiaaa mySensor; //создаем объект акселерометра
Servo servo;// создаем объект привода
BMP280_DEV bmp280;//создаем объект барометра
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;// создаем переменные в которые будем писать показания с акселерометра
float temperature, pressure, altitude;//создаем переменные для показаний с барометра
int altitude_data = 0; //создаем переменную для сохранения стартовой высоты с баромета
String message_bmp = "";// создаем строковую переменную для хранения значений с барометра для записи на карту
String accelgyromag = "";// создаем строковую переменную для хранения значений с гироскопа бля запси на карту
boolean start = true; //логическая переменная на старте для первых измерений
void setup()
{
  Serial.begin(115200);                           // инициализируем порт
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);// Этот пин обязательно должен быть определен как OUTPUT
  pinMode(3, OUTPUT);// Этот пин включет радиопередатчик, описываем как выход
  digitalWrite(3, 1); // Включаем этот пин
  pinMode(2, OUTPUT);//Этот пин отвечает за режим работы передатчика 1 - рабочий режим, 2 - режим настройки
  digitalWrite(2, 1); // переводим в рабочий режим
  servo.attach(9);// сервопривод подключен к 9 пину
  Serial.println("card initialized.");// карта вкл
  bmp280.begin(BMP280_I2C_ALT_ADDR);//Инициализируем работу барометра
  bmp280.setTimeStandby(TIME_STANDBY_2000MS);// таймаут работы барометра
  bmp280.startNormalConversion();// выставляем нормальный режим работы барометра
  mySensor.beginAccel();//инициализируем работу акселерометра
  mySensor.beginGyro();//инициализируем работу гироскопа
  mySensor.beginMag();//инициализируем работу компаса
  servo.write(30);//шевелим сервоприводом для теста работает или нет
  delay(1000);
  servo.write(90);

}

void loop()
{
  readbmp280(); // эта функция читает данные с барометра
  readaccelgyromag(); //эта функция читает с гироскопа акселерометра и компаса
//  TeamID;Time;Altitude;Ax;Ay;Az;Gy;Gx;Gz;Mx;My;Mz;Pressure;Temperature;Start point;Separate point;Recovery point;Landing point \n 
}
void write2sd(String data, String filname) {// создаем функцию записи на вход подаем данные и имя файла в строковом формате
  File telemetry = SD.open("filname", FILE_WRITE);// открываем файл для записи, если файла нет то он автоматом создается
  if (telemetry) {// если файл удалось открыть
    telemetry.println(data);//записываем данные в строку и курсор на следущую строку
    Serial.println(data);// печатаем даннык для отладки
    telemetry.close();//закрывам файл
  }
  else {// если не открыли файл, какие то проблемы
    // Сообщаем об ошибке, если все плохо
    Serial.println("error opening datalog.csv.csv");//ошибка открытия файла
  }
}
void readbmp280() {// создаем функцию для чтения данных с барометра
  message_bmp = ""; //очищаем строку для записи данных 
  if (bmp280.getMeasurements(temperature, pressure, altitude))    // проверяем на связи ли барометра
  {
    if (start) {// если мы на старте то 
      delay(2000);// ждем 2 секунды (можно убрать)
      altitude_data = altitude;//сохраняем данные выстоты на старте
      String start_message = " Высота на старте= " + String(altitude_data);//формируем сообщение о стартовой выстоте
      Serial.println(start_message); //печатаем сообщение
      start = false;//выключем флаг старта
    }
    Serial.print(temperature);// печатаем инфу с баромета 
    Serial.print(F("*C   "));
    Serial.print(pressure);
    Serial.print(F("hPa   "));
    Serial.print(altitude);
    Serial.println(F("m"));
    message_bmp = " Температура = " + String(temperature) + " Давление = " + String(pressure) + " Высота = " + String(altitude);// формируем сообщение от барометра
//    Serial.println(message_bmp);//печатаем сообщение 
  }

}
void readaccelgyromag() {//создаем функцию для чтения с гироскопа акселерометра и компаса
  if (mySensor.accelUpdate() == 0) {// если акселерометр на связи 
    aX = mySensor.accelX();//читаем данные с акселерометра
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    Serial.print("accelX: " + String(aX));// печатаем инфу с акселерометра
    Serial.print("\taccelY: " + String(aY));
    Serial.print("\taccelZ: " + String(aZ));
    Serial.print("\taccelSqrt: " + String(aSqrt));

  }

  if (mySensor.gyroUpdate() == 0) {//если гироскоп на связи 
    gX = mySensor.gyroX();//читаем данные с гироскопа
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.print("\tgyroX: " + String(gX));//печатаем данные с гироскопа
    Serial.print("\tgyroY: " + String(gY));
    Serial.print("\tgyroZ: " + String(gZ));
  }

  if (mySensor.magUpdate() == 0) {// если компас на связи(магнитометр)
    mX = mySensor.magX();//читаем данные с компаса
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    Serial.print("\tmagX: " + String(mX));// печатаем данные с компаса
    Serial.print("\tmaxY: " + String(mY));
    Serial.print("\tmagZ: " + String(mZ));
    Serial.print("\thorizontalDirection: " + String(mDirection));
  }
  //формируем сообщение с суммой инфы
  String accelgyromag = "accelX=" + String(aX) + " taccelY=" + String(aY) + " taccelZ=" + String(aZ) + " tgyroX=" + String(gX) + " tgyroY=" + String(gY) + " tgyroZ=" + String(gZ) + " tmagX=" + String(mX) + " tmaxY=" + String(mY) + " tmagZ=" + String(mZ) + " thorizontalDirection=" + String(mDirection);
//  Serial.println(accelgyromag);// печатаем сообщение с инфой
}
