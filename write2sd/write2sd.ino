
#include <SPI.h>
#include <SD.h>

const int PIN_CHIP_SELECT = 4;

void setup() {
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
 // Этот пин обязательно должен быть определен как OUTPUT
  pinMode(10, OUTPUT);

  // Пытаемся проинициализировать модуль
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  // Строка с данными, которые мы поместим в файл:
  String logStringData = "";

  // Считываем данные с портов и записываем в строку
  for (int i = 0; i < 5; i++) {
    int sensor = analogRead(i);
    logStringData += String(sensor);
    if (i < 4) {
      logStringData += ",";
    }
  }

  // Открываем файл, но помним, что одновременно можно работать только с одним файлом.
  // Если файла с таким именем не будет, ардуино создаст его.
  File dataFile = SD.open("cansat.csv", FILE_WRITE);

  // Если все хорошо, то записываем строку:
  if (dataFile) {
    dataFile.println(logStringData);
    dataFile.close();
    // Публикуем в мониторе порта для отладки
    Serial.println(logStringData);
  }
  else {
  // Сообщаем об ошибке, если все плохо
    Serial.println("cansat.csv");
  }
}
