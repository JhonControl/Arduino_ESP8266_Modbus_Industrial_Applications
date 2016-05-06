#include <Arduino.h>
#include "ModbusTCPSlave.h"


ModbusTCPSlave::ModbusTCPSlave(void):
#ifdef MB_ETHERNET
    MBServer(MB_PORT)
    #endif
#ifdef MB_CC3000
    MBClient(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER),
    MBServer(MB_PORT)
    #endif
#ifdef MB_ESP8266
    MBServer(MB_PORT)
    #endif
{
}
#ifdef MB_ETHERNET
void ModbusTCPSlave::begin(){
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    #ifdef MBDebug
    Serial1.begin(9600);
    Serial1.print(F("Requesting DHCP ..."));
    #endif
    if (Ethernet.begin(mac) == 0) {
        #ifdef MBDebug
        Serial1.println("failed");
        #endif
        while (1);
    }
    #ifdef MBDebug
    else
        Serial1.println();
    
    Serial1.print(F("Listening on "));
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        Serial1.print(Ethernet.localIP()[thisByte], DEC);
        Serial1.print(thisByte < 3 ? "." : "");
    }
    Serial1.print(":");
    Serial1.print(MB_PORT);
    Serial1.println(" ...");
    #endif
    digitalWrite(LED_PIN, HIGH);
}
void ModbusTCPSlave::begin(byte ip[4],uint8_t gateway[4],uint8_t subnet[4]){
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    Ethernet.begin(mac,ip,gateway,subnet);
    #ifdef MBDebug
    Serial1.begin(9600);
    Serial1.print(F("Listening on "));
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        Serial1.print(ip[thisByte], DEC);
        Serial1.print(thisByte < 3 ? "." : "");
    }
    Serial1.print(F(":"));
    Serial1.print(MB_PORT);
    Serial1.println(" ...");
    #endif
    digitalWrite(LED_PIN, HIGH);
}
#endif
#ifdef MB_CC3000
void ModbusTCPSlave::begin(const char *ssid, const char *key, uint8_t secmode){
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    MBClient.begin();
    #ifdef MBDebug
    Serial1.print(F("Connecting to ")); Serial1.print(ssid); Serial1.println(F(" ..."));
    #endif
    MBClient.connectToAP(ssid, key, secmode);
    while (!MBClient.checkDHCP())
        delay(100);
    uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
    MBClient.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv);
    #ifdef MBDebug
    Serial1.print(F("Listening on "));
    MBClient.printIPdotsRev(ipAddress);
    Serial1.print(F(":"));
    Serial1.println(MB_PORT);
    #endif
    digitalWrite(LED_PIN, HIGH);
    MBServer.begin();
}
#endif
#ifdef MB_ESP8266
void ModbusTCPSlave::begin(const char *ssid, const char *key){
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledPinStatus = LOW;
    #ifdef MBDebug
    Serial1.begin(115200);
    // Connect to WiFi network
    Serial1.println();
    Serial1.println();
    Serial1.print("Connecting to ");
    Serial1.println(ssid);
    #endif
    
    WiFi.begin(ssid, key);
    
    while (WiFi.status() != WL_CONNECTED) {
        // Blink the LED
        digitalWrite(LED_PIN, ledPinStatus); // Write LED high/low
        ledPinStatus = (ledPinStatus == HIGH) ? LOW : HIGH;
        delay(100);
        #ifdef MBDebug
        Serial1.print(".");
        #endif
    }
    #ifdef MBDebug
    Serial1.println("");
    Serial1.println("WiFi connected");
    #endif
    
    #ifdef MBDebug
    Serial1.print("IP address: ");
    Serial1.println(WiFi.localIP());
    Serial1.print("WiFi signal");
    Serial1.println(WiFi.RSSI());
    #endif
    
    // Start the server
    digitalWrite(LED_PIN, HIGH);
    MBServer.begin();
    #ifdef MBDebug
    Serial1.print(F("Listening on "));
    Serial1.print(MB_PORT);
    Serial1.println(" ...");
    #endif
}
void ModbusTCPSlave::begin(const char *ssid, const char *key,uint8_t ip[4],uint8_t gateway[4],uint8_t subnet[4]){
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledPinStatus = LOW;
#ifdef MBDebug
    Serial1.begin(115200);
    // Connect to WiFi network
    Serial1.println();
    Serial1.println();
    Serial1.print("Connecting to ");
    Serial1.println(ssid);
#endif
    
    WiFi.config(IPAddress(ip), IPAddress(gateway), IPAddress(subnet));
    
    WiFi.begin(ssid, key);
    
    while (WiFi.status() != WL_CONNECTED) {
        // Blink the LED
        digitalWrite(LED_PIN, ledPinStatus); // Write LED high/low
        ledPinStatus = (ledPinStatus == HIGH) ? LOW : HIGH;
        delay(100);
#ifdef MBDebug
        Serial1.print(".");
#endif
    }
#ifdef MBDebug
    Serial1.println("");
    Serial1.println("WiFi connected");
#endif
    
#ifdef MBDebug
    Serial1.print("IP address: ");
    Serial1.println(WiFi.localIP());
    Serial1.print("WiFi signal");
    Serial1.println(WiFi.RSSI());
#endif
    
    // Start the server
    digitalWrite(LED_PIN, HIGH);
    MBServer.begin();
#ifdef MBDebug
    Serial1.print(F("Listening on "));
    Serial1.print(MB_PORT);
    Serial1.println(" ...");
#endif
}
#endif
/*
void ModbusTCPSlave::Stop(){

	MBClient.close();

}

*/

void ModbusTCPSlave::Run(){
    boolean flagClientConnected = 0;
    byte byteFN = MB_FC_NONE;
    int Start;
    int WordDataLength;
    int ByteDataLength;
    int MessageLength;
    
    //****************** Read from socket ****************
    #ifdef MB_ETHERNET
        EthernetClient client = MBServer.available();
    #endif
    #ifdef MB_CC3000
        Adafruit_CC3000_ClientRef client = MBServer.available();
    #endif
    #ifdef MB_ESP8266
        WiFiClient client = MBServer.available();
    #endif
    
    while (client.connected()) {
    if(client.available())
    {
        flagClientConnected = 1;
        int i = 0;
        while(client.available())
        {
            ByteArray[i] = client.read();
            i++;
        }
        #ifdef MB_ESP8266
            client.flush();
        #endif
        byteFN = ByteArray[MB_TCP_FUNC];
        Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
        WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
        #ifdef MBDebug
        Serial1.println();
        Serial1.print("RX: ");
        for (byte thisByte = 0; thisByte < 20; thisByte++) {
            Serial1.print(ByteArray[thisByte], DEC);
            Serial1.print("-");
        }
        Serial1.println();
        Serial1.print("Ricevuta Funzione: ");
        Serial1.println(byteFN);

        #endif
    }
    
    // Handle request

    switch(byteFN) {
        case MB_FC_NONE:
            break;
        
        case MB_FC_READ_REGISTERS:  // 03 Read Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            for(int i = 0; i < WordDataLength; i++)
            {
                ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
                ByteArray[10 + i * 2] =  lowByte(MBHoldingRegister[Start + i]);
            }
            MessageLength = ByteDataLength + 9;
            client.write((const uint8_t *)ByteArray,MessageLength);
            #ifdef MBDebug
                Serial1.print("TX: ");
                for (byte thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    Serial1.print(ByteArray[thisByte], DEC);
                    Serial1.print("-");
                }
                Serial1.println();
            #endif
			client.stop();
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_READ_INPUT_REGISTERS:  // 04 Read Input Registers
            Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
            WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            for(int i = 0; i < WordDataLength; i++)
            {
                ByteArray[ 9 + i * 2] = highByte(MBInputRegister[Start + i]);
                ByteArray[10 + i * 2] =  lowByte(MBInputRegister[Start + i]);
            }
            MessageLength = ByteDataLength + 9;
            client.write((const uint8_t *)ByteArray,MessageLength);
            #ifdef MBDebug
                Serial1.print("TX: ");
                for (byte thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    Serial1.print(ByteArray[thisByte], DEC);
                    Serial1.print("-");
                }
                Serial1.println();
            #endif
			client.stop();
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_WRITE_REGISTER:  // 06 Write Holding Register
            MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
            ByteArray[5] = 6; //Number of bytes after this one.
            MessageLength = 12;
            client.write((const uint8_t *)ByteArray,MessageLength);
            #ifdef MBDebug
                Serial1.print("TX: ");
                for (byte thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    Serial1.print(ByteArray[thisByte], DEC);
                    Serial1.print("-");
                }
                Serial1.println();
                Serial1.print("Write Holding Register: ");
                Serial1.print(Start);
                Serial1.print("=");
                Serial1.println(MBHoldingRegister[Start]);
            #endif
			client.stop();
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_WRITE_MULTIPLE_REGISTERS:    //16 Write Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            for(int i = 0; i < WordDataLength; i++)
            {
                MBHoldingRegister[Start + i] =  word(ByteArray[ 13 + i * 2],ByteArray[14 + i * 2]);
            }
            MessageLength = 12;
            client.write((const uint8_t *)ByteArray,MessageLength);
         
		 #ifdef MBDebug
                Serial1.print("TX: ");
                for (byte thisByte = 0; thisByte <= MessageLength; thisByte++) {
                    Serial1.print(ByteArray[thisByte], DEC);
                    Serial1.print("-");
                }
                Serial1.println();
                Serial1.print("Write Holding Registers from: ");
                Serial1.print(Start);
                Serial1.print("=");
                Serial1.println(WordDataLength);
            #endif
			
			client.stop();
            byteFN = MB_FC_NONE;

            break;
    }
	
    }
	
    client.stop();
    
    if (flagClientConnected == 1){
        #ifdef MBDebug
        Serial1.println("client disonnected");
        #endif
        flagClientConnected = 0;
    }
}

