/*
Arduino:

MODULE..........ARDUINO
GND.............GND
VCC.............3.3V
D0 (CLK)........D13
D1 (DATA).......D11
RST (RESET).....D8
DC..............D9
CS (SS).........D10
*/

#include "HCuOLED.h"
#include "SPI.h"
#include "DHT.h"                               // Подключаем библиотеку DHT 
#define DHTPIN 2                               // Пин к которому подключен датчик
#define DHTTYPE DHT22                          // Используемый датчик DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);                      // Инициализируем датчик

/* Example bitmap */
const PROGMEM byte Tiny_Logo_Resistor[] =
{
  0xC0, 0xE0, 0x30, 0x30, 0xF0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x38, 0x0E, 0x07, 0x1E, 0x78, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x1C, 0x07, 0x0F, 0x3C, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF0, 0x3C, 0x0F, 0x07, 0x1C, 0x70, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0x78, 0x1E, 0x07, 0x0E, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xF0, 0x30, 0x30, 0xE0, 0xC0, 
  0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x3C, 0x38, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1E, 0x38, 0x1C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0x38, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1C, 0x38, 0x1E, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x00, 
};

/* Digital pin number for the displays chip select pin */
#define CS_DI 10
/* Digital pin number for the displays data/command pin */
#define DC_DI 9
/* Digital pin number for the displays reset pin */
#define RST_DI 8


/* Create an instance of the library (uncomment one of the lines below) */
//HCuOLED HCuOLED(SSD1307, SS_DI, DC_DI, RST_DI); // For SSD1307 displays (HCMODU0050 & HCMODU0052)
HCuOLED HCuOLED(SH1106, CS_DI, DC_DI, RST_DI); // For SH1106 displays (HCMODU0058 & HCMODU0059)


void temp_func()
{
  delay(1000);                                  // Задежка в 2 с
  HCuOLED.Erase(50,45,80,64);
  HCuOLED.Erase(50,18,100,30);
  float h = dht.readHumidity();                 // Считывание влажности в процентах
  float t = dht.readTemperature();              // Считывание температуры в градусах цельсия
  if (isnan(h) || isnan(t))                     // Проверяем, получилось считать данные 
    {
      Serial.println("Read error DHT22");    // Выводим текст
      return;                                  
    }
  float hic = dht.computeHeatIndex(t, h, false);// Расчет типла по целсию
  Serial.print(t); // Выводим текст
  
  HCuOLED.Cursor(50,18);
  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Print(t);
  HCuOLED.Print("*C");
 
  HCuOLED.Cursor(50,45);
  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Print(h);
  HCuOLED.Print("%");

  Serial.print("Humidity: ");                   // Выводим текст
  Serial.print(h);                              // Отображаем влажность
  Serial.print(" %\t");                         // Выводим текст
  
  Serial.print(h);                              // Отображаем влажность
  Serial.print(" %\t");                         // Выводим текст
  Serial.print("Temperature: ");                // Выводим текст
  Serial.print(t);                              // Отображаем температуру
  Serial.println(" *C ");                       // Выводим текст
  
    /* Draw an outlined box around the screen */
  HCuOLED.Rect(0,0,125,63, OUTLINE);
  
  /* Output the display buffer to the screen */
  HCuOLED.Refresh();
}


void setup() 
{
  /* Reset the display */
  HCuOLED.Reset();
  Serial.begin(9600);                          // Задаем скорость передачи данных
  Serial.println("DHT22 TEST!");               // Выводим текст
  dht.begin();
}


void loop() 
{
  /* Display some text using a medium 2 line proportional font */  
  HCuOLED.Cursor(15,4);
  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Print("Temperature");
  HCuOLED.Cursor(35,31);
  HCuOLED.SetFont(MedProp_11pt);
  HCuOLED.Print("Humidity");
  /* Draw an outlined box around the screen */
  HCuOLED.Rect(0,0,125,63, OUTLINE);
  /* Output the display buffer to the screen */
  HCuOLED.Refresh();
  /* Nothing more to do */
  while(1)
  {
    temp_func();
  }
}
