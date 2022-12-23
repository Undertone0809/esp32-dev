/*
 * [ESP32]
 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> IO18/SCK  
 #04 SDA -> IO23/MOSI
 #05 RES -> IO4
 #06 DC  -> IO2
 #07 BLK -> NC
*/
#pragma GCC optimize("O3")
 
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
 
 
TFT_eSPI TFT = TFT_eSPI(); 
TFT_eSprite eSprite = TFT_eSprite(&TFT);
 
void setup() {
    Serial.begin(115200);
    Serial.println("[info] start work");
    TFT.init();
    TFT.setRotation(1);
    TFT.fillScreen(TFT_BLUE);
    TFT.initDMA();
    eSprite.setTextFont(1);
    eSprite.setTextColor(TFT_WHITE);
    eSprite.createSprite(240, 240);
 
}
 
void loop() {
    eSprite.setTextFont(1);
    eSprite.setCursor(20, 20);
    eSprite.println("Hello World\n"); 
    eSprite.setCursor(40, 40);
    eSprite.println("Hello World\n");
    eSprite.pushSprite(0, 0);
    delay(1000);
}