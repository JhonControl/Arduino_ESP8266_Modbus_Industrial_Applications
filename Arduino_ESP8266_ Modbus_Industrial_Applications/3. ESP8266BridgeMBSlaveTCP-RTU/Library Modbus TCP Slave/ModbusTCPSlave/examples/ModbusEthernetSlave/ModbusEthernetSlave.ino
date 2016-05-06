#include <SPI.h>
#include <Ethernet.h>
#include <ModbusTCPSlave.h>

// Ethernet settings (depending on MAC and Local network)
byte ip[]      = { 192, 168, 1, 127};
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[]  = { 255, 255, 255, 0 };

unsigned long timer = 0;

ModbusTCPSlave Mb;
//Port 502 (defined in ModbusTCPS.h) MB_PORT

void setup()
{
  Mb.begin(ip, gateway, subnet);

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

  if (millis() - timer >= 1000) {
    timer = millis();
    Mb.MBInputRegister[1]++;
  }
}


