/**
*
*    More information about projects PDAControl
*    Mas informacion sobre proyectos PDAControl
*    Blog PDAControl English   http://pdacontrolenglish.blogspot.com.co/   
*    Blog PDAControl Espa?ol   http://pdacontrol.blogspot.com.co/
*    Channel  Youtube          https://www.youtube.com/c/JhonValenciaPDAcontrol/videos   
*
*/

#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

//Setup the brewtrollers register bank
//All of the data accumulated will be stored here
modbusDevice regBank;
//Create the modbus slave protocol handler
modbusSlave slave;

  

#include <ESP8266WiFi.h>
#include <ModbusTCPSlave.h>
ModbusTCPSlave Mb;
unsigned long timer;
unsigned long checkRSSIMillis;
 
  
void setup()
{   
    
/// Config Modbus TCP Slave  
 Mb.begin("1503523", "D2E7D32DBC883");

// Config Modbus RTU Slave
//Assign the modbus device ID.  
  regBank.setId(1);
// Añade Salida analógica registra 40.001-40.020 al banco de registros

  regBank.add(40001);  
  regBank.add(40002);  
  regBank.add(40003);  
  regBank.add(40004);  
  regBank.add(40005);  
  regBank.add(40006);  
  regBank.add(40007);  
  regBank.add(40008);  
  regBank.add(40009);  
  regBank.add(40010);  
  regBank.add(40011);  
  regBank.add(40012);  
  regBank.add(40013);  
  regBank.add(40014);  
  regBank.add(40015);  
  regBank.add(40016);  
  regBank.add(40017);  
  regBank.add(40018);  
  regBank.add(40019);  
  regBank.add(40020);  

  slave._device = &regBank;  
  
/// speed 19200 None
  slave.setBaud(19200);   
}

void loop()
{    
 while(1)
  {

  
  ////  Write  Modbus TCP in Modbus RTU   10 Holding Registers
  ////  Escritura de Modbus TCP en  Registros 10  Modbus RTU  
    
  regBank.set(40001,  Mb.MBHoldingRegister[0]);   
  regBank.set(40002,  Mb.MBHoldingRegister[1]);     
  regBank.set(40003,  Mb.MBHoldingRegister[2]);   
  regBank.set(40004,  Mb.MBHoldingRegister[3]); 
  regBank.set(40005,  Mb.MBHoldingRegister[4]); 
  regBank.set(40006,  Mb.MBHoldingRegister[5]); 
  regBank.set(40007,  Mb.MBHoldingRegister[6]); 
  regBank.set(40008,  Mb.MBHoldingRegister[7]);   
  regBank.set(40009,  Mb.MBHoldingRegister[8]);     
  regBank.set(40010,  Mb.MBHoldingRegister[9]);   
  
  ////  Write Modbus RTU in Modbus TCP  10 Holding Registers
  ////  Escritura de Modbus RTU en  Registros 10  Modbus TCP  

  Mb.MBHoldingRegister[10] =  regBank.get(40011);
  Mb.MBHoldingRegister[11] =  regBank.get(40012);
  Mb.MBHoldingRegister[12] =  regBank.get(40013);
  Mb.MBHoldingRegister[13] =  regBank.get(40014);
  Mb.MBHoldingRegister[14] =  regBank.get(40015);
  Mb.MBHoldingRegister[15] =  regBank.get(40016);
  Mb.MBHoldingRegister[16] =  regBank.get(40017);
  Mb.MBHoldingRegister[17] =  regBank.get(40018);
  Mb.MBHoldingRegister[18] =  regBank.get(40019);
  Mb.MBHoldingRegister[19] =  regBank.get(40020);
  

     delay(10);
   
     slave.run();  /// Run  Slave Modbus RTU
     Mb.Run();     /// Run Slave  Modbus TCP
  }
}

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

