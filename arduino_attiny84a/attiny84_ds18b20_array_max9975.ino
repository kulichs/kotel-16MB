#include <SoftwareSerial.h>
#include <GyverMAX6675.h>  //https://github.com/GyverLibs/GyverMAX6675
#include <GyverDS18Array.h>

#define RX_PIN 2            // Pin pro příjem (UART, TX z ESP)
#define TX_PIN 1            // Pin pro vysílání (UART, RX z ESP)
#define CLK_PIN   4        // Pin SCK
#define DATA_PIN  5        // Pin SO (MISO)
#define CS_PIN    7        // Pin CS
#define BUS0 0             // Pin 1-Wire druha sbernice
#define BUS1 3             // Pin 1-Wire prvni sbernice


// UART pro komunikaci s ESP
SoftwareSerial mySerial(RX_PIN, TX_PIN);

// Definice pinu pro komunikaci MAX6675
GyverMAX6675<CLK_PIN, DATA_PIN, CS_PIN> sens;

// Max 12 cidel, pak jsou odesilany postupne 0 az 11
uint64_t arrayBus3[] = {
  0x940000006a2b6d28,
  0xdf0000006a195528,
  0x7300000069fc1628,
  0xbe00000069e8e228,
  0xc500000069ff1d28,
  0xd900000069f9b928,
  0xef0000006a177a28,
  0x420000006b485828,
};

//Max 12 cidel, pak jsou odesilany postupne 20 az 29 a 210, 211 
uint64_t arrayBus0[] = {
  0x630000006bc33b28,
  0x733de1e380d63d28,
  0x200000006bc0d428,
  0x360000006bbd5828,
  0xea0000006b13e528,
  0x400000006ae1e528,
  0xfd0000006a1a4428,
  0x4f3de1e380119328,
};

GyverDS18Array dsBus0(BUS0, arrayBus3, sizeof(arrayBus3) / sizeof(arrayBus3[0]));
GyverDS18Array dsBus1(BUS1, arrayBus0, sizeof(arrayBus0) / sizeof(arrayBus0[0]));
//GyverDS18Array dsBus0(3, arrayBus0, 8);
//GyverDS18Array dsBus1(0, arrayBus0, 8);

void setup() {
  Serial.begin(9600);
  dsBus0.setResolution(10);
  delay(1000);
  dsBus1.setResolution(10);
  delay(1000);
  dsBus0.requestTemp();
  delay(1000);
  dsBus1.requestTemp();
  delay(1000);
  

}

void loop() {
    if (dsBus0.ready()) {
    //Serial.println("DSReady");
        for (int i = 0; i < dsBus0.amount(); i++) {
            Serial.print(i);
            Serial.print(":");
            if (dsBus0.readTemp(i)) Serial.print(dsBus0.getTemp());
            else Serial.print("nan");
            Serial.println();
            delay(1500); //Pauza 1.5 sekundy mezi cidlama nechceme zahltit ESP32
        }
        Serial.println();

        dsBus0.requestTemp();
    }
    
    if (dsBus1.ready()) {
    //Serial.println("DSReady");
        for (int i = 0; i < dsBus1.amount(); i++) {
            Serial.print("2"); //Prida pred hodnotu i cislo 2 at vime ze to je z druhe zbernice
            Serial.print(i);
            Serial.print(":");
            if (dsBus1.readTemp(i)) Serial.print(dsBus1.getTemp());
            else Serial.print("nan");
            Serial.println();
            delay(1500); //Pauza 1.5 sekundy mezi cidlama nechceme zahltit ESP32
        }
        Serial.println();

        dsBus1.requestTemp();
    }

    //Serial.print("99");
    if (sens.readTemp()) {  // Nacteni a odeslani teploty spalin MAX6675
    Serial.print("99");  //musi mit 15 znaku kvuli parsovani v ESPHome
    Serial.print(":");
    Serial.println(sens.getTemp());
   }
   else; //kdyz neni cidlo neposle nic

   delay(1500); //A po 1.5 skekundach znovu
}