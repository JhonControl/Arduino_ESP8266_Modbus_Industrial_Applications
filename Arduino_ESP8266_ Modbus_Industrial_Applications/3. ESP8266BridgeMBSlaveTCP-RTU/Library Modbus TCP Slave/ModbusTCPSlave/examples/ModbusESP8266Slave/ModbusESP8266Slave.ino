#include <ESP8266WiFi.h>
#include <ModbusTCPSlave.h>

////https://forums.adafruit.com/viewtopic.php?f=31&t=61774&sid=06d9885dcf852259d1f568fa96e40265&start=15

////WIFI Settings
///*
//byte ip[]      = { 192, 168, 1, 126};
//byte gateway[] = { 192, 168, 1, 1 };
//byte subnet[]  = { 255, 255, 255, 0 };
//*/
ModbusTCPSlave Mb;

unsigned long timer;
unsigned long checkRSSIMillis;

void setup()
{

  //Mb.begin("Telecom-28778737", "passwordwificasa47893000", ip, gateway, subnet);
  Mb.begin("1503523", "D2E7D32DBC883");

  delay(1000);
  Mb.MBInputRegister[0] = 100;
  Mb.MBInputRegister[1] = 65500;
  Mb.MBInputRegister[2] = 300;
  Mb.MBInputRegister[3] = 400;
  Mb.MBInputRegister[4] = 500;

  Mb.MBHoldingRegister[0] = 1;
  Mb.MBHoldingRegister[1] = 2;
  Mb.MBHoldingRegister[2] = 3;
  Mb.MBHoldingRegister[3] = 4;
  Mb.MBHoldingRegister[4] = 5;

}

void loop()
{
  Mb.Run();
  delay(10);

  if (millis() - timer >= 1000) {
    timer = millis();
    Mb.MBInputRegister[1]++;
  }

  if (millis() - checkRSSIMillis >= 10000) {
    checkRSSIMillis = millis();
    Mb.MBInputRegister[0] = checkRSSI();
  }
}

/*****FUNZIONI*****/

byte checkRSSI() {
  byte quality;
  long rssi = WiFi.RSSI();
  if (rssi <= -100)
    quality = 0;
  else if (rssi >= -50)
    quality = 100;
  else
    rssi = rssi + 100;
  quality = byte(rssi * 2);

  return quality;
}


