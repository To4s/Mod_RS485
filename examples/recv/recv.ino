#include <SPI.h>
#include <mcp_canbus.h>

#define SPI_CS_PIN  9 
int scaling_factor = 0.01;          // scaling factor for obtained data  
MCP_CAN CAN(SPI_CS_PIN);                                  // Set CS pin

void setup()
{
    Serial.begin(115200); 
    while (CAN_OK != CAN.begin(CAN_500KBPS))  // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS FAIL!");
        delay(100);
    }
    Serial.println("CAN BUS OK!");
}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8]; // len: data length, buf: data buf


    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data
        unsigned long canId = CAN.getCanId();
        
        Serial.println("-----------------------------");
        if (canId == 0x100)  // CAN ID for battery data
        {
            uint16_t voltageRaw = (buf[0] << 8) | buf[1]; // Combining two bytes of data for voltage
            float voltage = voltageRaw * scaling_factor;   
            Serial.print("Battery Voltage: ");
            Serial.print(voltage);
            Serial.println(" V");
            Serial.println("-----------------------------");
        }
        else
        {
            Serial.println("Unknown CAN ID");
        }
    }
}

