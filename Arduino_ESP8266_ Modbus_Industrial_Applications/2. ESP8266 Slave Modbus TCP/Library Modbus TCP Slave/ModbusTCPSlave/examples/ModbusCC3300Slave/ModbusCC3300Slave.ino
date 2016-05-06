#include <SPI.h>
#include <Adafruit_CC3000.h>
#include <ModbusTCP.h>

unsigned long timer = 0;

ModbusTCP Mb;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in ModbusTCPSlave.h) MB_PORT

void setup()
{

  Mb.begin("myssid", "mypass", WLAN_SEC_WPA2);

  delay(5000);

}

void loop()
{

}


