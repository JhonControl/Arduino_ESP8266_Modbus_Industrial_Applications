/**
 *  Modbus master example 2:
 *  The purpose of this example is to query several sets of data
 *  from an external Modbus slave device. 
 *  The link media can be USB or RS232.
 *
 *  Recommended Modbus slave: 
 *  diagslave http://www.modbusdriver.com/diagslave.html
 *
 *  In a Linux box, run 
 *  "./diagslave /dev/ttyUSB0 -b 19200 -d 8 -s 1 -p none -m rtu -a 1"
 *  This is:
 *   serial port /dev/ttyUSB0 at 19200 baud 8N1
 *  RTU mode and address @1
 *
  */
  
  
/**
*
*    More information about projects PDAControl
*    Mas informacion sobre proyectos PDAControl
*    Blog PDAControl English   http://pdacontrolenglish.blogspot.com.co/   
*    Blog PDAControl Espa?ol   http://pdacontrol.blogspot.com.co/
*    Channel  Youtube          https://www.youtube.com/c/JhonValenciaPDAcontrol/videos   
*
*/

 


#include <OneWire.h>

OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)
float celsius, fahrenheit,celsius_web;

#include <ModbusRtu.h>


/// oled debuging

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);


///  

uint16_t au16data[32]; //!< data array for modbus network sharing  Array
uint8_t u8state; //!< machine state
uint8_t u8query; //!< pointer to message query

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus master(0,0,7); // this is master and RS-232 or USB-FTDI  ///  7 = PIN ENEABLE rs485

/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram[10];

unsigned long u32wait;

void setup() {

  // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(INVERSE); 
  
  pinMode(13, OUTPUT);


  digitalWrite(13, LOW);   
  //////////////////////////////
    
  // telegram 0: read registers
  telegram[0].u8id = 1;                 // slave address
  telegram[0].u8fct = 3;                // function code (registers read multiple  3) 
  telegram[0].u16RegAdd = 0;            // start address in slave -  direccion de Inicio 0
  telegram[0].u16CoilsNo = 10;          // number of elements (coils or registers) to read  0 - 16 
  telegram[0].au16reg = au16data;       // pointer to a memory array in the Arduino - Almacenamiento en Array de memoria de arduino

  
  // telegram 1: write a multiple  register = function 16
  telegram[1].u8id = 1;                 // slave address
  telegram[1].u8fct = 6;               // function code (write multiple registers 16 )
  telegram[1].u16RegAdd = 10;           // start address in slave  -  direccion de Inicio 10
  telegram[1].u16CoilsNo = 1;          // number of elements (coils or registers) to read
  telegram[1].au16reg = au16data+10;    // pointer to a memory array in the Arduino - Almacenamiento en Array de memoria de arduino 10 posiciones despues
  
  
   // telegram 2: write a multiple  register = function 16
  telegram[2].u8id = 1;                 
  telegram[2].u8fct = 6;              
  telegram[2].u16RegAdd = 11;           
  telegram[2].u16CoilsNo = 1;         
  telegram[2].au16reg = au16data+11;   
    
     // telegram 3: write a multiple  register = function 16
  telegram[3].u8id = 1;                 
  telegram[3].u8fct = 6;              
  telegram[3].u16RegAdd = 12;           
  telegram[3].u16CoilsNo = 1;         
  telegram[3].au16reg = au16data+12;   
  
  
     // telegram 4: write a multiple  register = function 16
  telegram[4].u8id = 1;                 
  telegram[4].u8fct = 6;              
  telegram[4].u16RegAdd = 13;           
  telegram[4].u16CoilsNo = 1;         
  telegram[4].au16reg = au16data+13;   
 
     // telegram 5: write a multiple  register = function 16
  telegram[5].u8id = 1;                 
  telegram[5].u8fct = 6;              
  telegram[5].u16RegAdd = 14;           
  telegram[5].u16CoilsNo = 1;         
  telegram[5].au16reg = au16data+14;   
  
     // telegram 6: write a multiple  register = function 16
  telegram[6].u8id = 1;                 
  telegram[6].u8fct = 6;              
  telegram[6].u16RegAdd = 15;           
  telegram[6].u16CoilsNo = 1;         
  telegram[6].au16reg = au16data+15;   
  
     // tel7gram 7: write a multiple  register = function 16
  telegram[7].u8id = 1;                 
  telegram[7].u8fct = 6;              
  telegram[7].u16RegAdd = 16;           
  telegram[7].u16CoilsNo = 1;         
  telegram[7].au16reg = au16data+16;   
  
     // telegram 8: write a multiple  register = function 16
  telegram[8].u8id = 1;                 
  telegram[8].u8fct = 6;              
  telegram[8].u16RegAdd = 17;           
  telegram[8].u16CoilsNo = 1;         
  telegram[8].au16reg = au16data+17;   
  
     // telegram 9: write a multiple  register = function 16
  telegram[9].u8id = 1;                 
  telegram[9].u8fct = 6;              
  telegram[9].u16RegAdd = 18;           
  telegram[9].u16CoilsNo = 1;         
  telegram[9].au16reg = au16data+18;   
  
  master.begin( 19200 ); // baud-rate at 19200
  master.setTimeOut( 2500 ); // if there is no answer in 5000 ms, roll over
  u32wait = millis() + 500;
  u8state = u8query = 0;   
}

void loop() {
  switch( u8state ) {
  case 0: 
    if (millis() > u32wait) u8state++; // wait state
    break;
  case 1: 
    master.query( telegram[u8query] ); // send query (only once)
    u8state++;
    u8query++;
 if (u8query > 9) u8query = 0;
    break;
  case 2:
      master.poll(); // check incoming messages
    if (master.getState() == COM_IDLE) {
      u8state = 0;
      u32wait = millis() + 20; 
    }
    break;
  }  

  ////////////////////////////////////////////////
   
   display.clearDisplay();   
   
   display.setTextSize(2); 
   display.setCursor(15,0); 
   display.print("Pot 100K"); 
   display.setCursor(30,18);   
   au16data[10] = analogRead( 0 ); // Write Holding Register [10]    
   display.print(analogRead( 0 ));     
   display.display();  
   

} 

