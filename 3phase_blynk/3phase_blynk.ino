/*
   -PZEM004T-
   5V - 5V
   GND - GND

   D3 - TX(PZEM1)
   D4 - RX(PZEM1)

   D5 - TX(PZEM2)
   D6 - RX(PZEM2)

   D7 - TX(PZEM3)
   D0 - RX(PZEM3)
   
*/

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLnx5ptZE1"
#define BLYNK_DEVICE_NAME "POWERMERTER"
#define BLYNK_AUTH_TOKEN "Did9U8FssPk-GBxXcy65IT6cl4doO3SZ"

#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <BlynkSimpleEsp8266.h>

#define PIN_RESET -1
#define DC_JUMPER 0


MicroOLED oled(PIN_RESET, DC_JUMPER);
PZEM004Tv30 pzem1(D3, D4); //rx,tx pin for phase1
PZEM004Tv30 pzem2(D5, D6); //rx,tx pin for phase2
PZEM004Tv30 pzem3(D7, D0); //rx,tx pin for phase3

unsigned long previousMillis = 0;
float voltage1, current1, power1, energy1, frequency1, pf1;
float voltage2, current2, power2, energy2, frequency2, pf2;
float voltage3, current3, power3, energy3, frequency3, pf3;

char ssid[] = "G6PD_2.4G";
char pass[] = "570610193";
char auth[] = BLYNK_AUTH_TOKEN;

uint8_t logo_bmp[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xC0, 0xF0, 0xE0, 0x78, 0x38, 0x78, 0x3C, 0x1C, 0x3C, 0x1C, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1C, 0x3C, 0x1C, 0x3C, 0x78, 0x38, 0xF0, 0xE0, 0xF0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0xF0, 0xF8, 0x70, 0x3C, 0x3C, 0x1C, 0x1E, 0x1E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x1E, 0x1E, 0x1E, 0x3C, 0x1C, 0x7C, 0x70, 0xF0, 0x70, 0x20, 0x01, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x1E, 0x0F, 0x0F, 0x07, 0x87, 0x87, 0x07, 0x0F, 0x0F, 0x1E, 0x3E, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1F, 0x1F, 0x3F, 0x3F, 0x1F, 0x1F, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//for reset energy
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt();
  if (pinValue == HIGH) {
    Serial.println("Reset energy1");
    pzem1.resetEnergy();
  }
}
BLYNK_WRITE(V14)
{
  int pinValue = param.asInt();
  if (pinValue == HIGH) {
    Serial.println("Reset energy2");
    pzem2.resetEnergy();
  }
}
BLYNK_WRITE(V21)
{
  int pinValue = param.asInt();
  if (pinValue == HIGH) {
    Serial.println("Reset energy3");
    pzem3.resetEnergy();
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  //------Display LOGO at start------
  oled.begin();
  oled.clear(PAGE);
  oled.clear(ALL);
  oled.drawBitmap(logo_bmp);//call the drawBitmap function and pass it the array from above
  oled.setFontType(0);
  oled.setCursor(0, 36);
  oled.print(" IoTbundle");
  oled.display();

  //  pzem.resetEnergy(); //reset energy

  Blynk.begin(auth, ssid, pass);

}

void loop() {
  Blynk.run();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) { //run every 1 second
    previousMillis = currentMillis;
    displayValue(); //update OLED

    //------sent data to blynk------
    //phase1 (V7 for reset energy)
    if (!isnan(voltage1)) {
      Blynk.virtualWrite(V1, voltage1);
      Blynk.virtualWrite(V2, current1);
      Blynk.virtualWrite(V3, power1);
      Blynk.virtualWrite(V4, energy1);
      Blynk.virtualWrite(V5, frequency1);
      Blynk.virtualWrite(V6, pf1);
    }
    //phase2 (V14 for reset energy)
    if (!isnan(voltage2)) {
      Blynk.virtualWrite(V8, voltage2);
      Blynk.virtualWrite(V9, current2);
      Blynk.virtualWrite(V10, power2);
      Blynk.virtualWrite(V11, energy2);
      Blynk.virtualWrite(V12, frequency2);
      Blynk.virtualWrite(V13, pf2);
    }
    //phase3 (V21 for reset energy)
    if (!isnan(voltage3)) {
      Blynk.virtualWrite(V15, voltage3);
      Blynk.virtualWrite(V16, current3);
      Blynk.virtualWrite(V17, power3);
      Blynk.virtualWrite(V18, energy3);
      Blynk.virtualWrite(V19, frequency3);
      Blynk.virtualWrite(V20, pf3);
    }
  }
}

void displayValue() {

  //------read data------
  //phase1
  voltage1 = pzem1.voltage();
  current1 = pzem1.current();
  power1 = pzem1.power();
  energy1 = pzem1.energy();
  frequency1 = pzem1.frequency();
  pf1 = pzem1.pf();
  //phase2
  voltage2 = pzem2.voltage();
  current2 = pzem2.current();
  power2 = pzem2.power();
  energy2 = pzem2.energy();
  frequency2 = pzem2.frequency();
  pf2 = pzem2.pf();
  //phase3
  voltage3 = pzem3.voltage();
  current3 = pzem3.current();
  power3 = pzem3.power();
  energy3 = pzem3.energy();
  frequency3 = pzem3.frequency();
  pf3 = pzem3.pf();


  voltage1 = 200+random(10);
  current1 = 1;
  power1 = voltage1;
  energy1 = 10;
  frequency1 = 50;
  pf1 = 1;
  //phase2
  voltage2 = 210+random(10);
  current2 = 1.2;
  power2 = voltage2*current2;
  energy2 = 11;
  frequency2 = 50;
  pf2 = 1.0;
  //phase3
  voltage3 = 220+random(10);
  current3 = 1.1;
  power3 = voltage3*current3;
  energy3 = 12;
  frequency3 = 50;
  pf3 = 1.0;
  

  //------Update OLED display------
  oled.clear(PAGE);
  oled.setFontType(0);

  //display power
  //phase1
  if (power1 < 10) oled.setCursor(26, 0);
  else if (power1 < 100)  oled.setCursor(20, 0);
  else if (power1 < 1000)  oled.setCursor(14, 0);
  else if (power1 < 10000)  oled.setCursor(8, 0);
  else  oled.setCursor(2, 0);
  oled.print(power1, 0); oled.setCursor(46, 0); oled.println("W");
  //phase1
  if (power2 < 10) oled.setCursor(26, 16);
  else if (power2 < 100)  oled.setCursor(20, 16);
  else if (power2 < 1000)  oled.setCursor(14, 16);
  else if (power2 < 10000)  oled.setCursor(8, 16);
  else  oled.setCursor(2, 16);
  oled.print(power2, 0); oled.setCursor(46, 16); oled.println("W");
  //phase1
  if (power3 < 10) oled.setCursor(26, 32);
  else if (power3 < 100)  oled.setCursor(20, 32);
  else if (power3 < 1000)  oled.setCursor(14, 32);
  else if (power3 < 10000)  oled.setCursor(8, 32);
  else  oled.setCursor(2, 32);
  oled.print(power3, 0); oled.setCursor(46, 32); oled.println("W");

  //on error
  if (isnan(voltage1) && isnan(voltage2) && isnan(voltage3)) {
    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.printf("Please\n\nConnect\n\nPZEM004T");
  }
  oled.display();

  //------Serial display------
  //phase1
  Serial.print("Voltage1: "); Serial.print(voltage1); Serial.println("V");
  Serial.print("Current1: "); Serial.print(current1); Serial.println("A");
  Serial.print("Power1: "); Serial.print(power1); Serial.println("W");
  Serial.print("Energy1: "); Serial.print(energy1, 3); Serial.println("kWh");
  Serial.print("Frequency1: "); Serial.print(frequency1, 1); Serial.println("Hz");
  Serial.print("PF1: "); Serial.println(pf1); Serial.println("");
  //phase2
  Serial.print("Voltage2: "); Serial.print(voltage2); Serial.println("V");
  Serial.print("Current2: "); Serial.print(current2); Serial.println("A");
  Serial.print("Power2: "); Serial.print(power2); Serial.println("W");
  Serial.print("Energy2: "); Serial.print(energy2, 3); Serial.println("kWh");
  Serial.print("Frequency2: "); Serial.print(frequency2, 1); Serial.println("Hz");
  Serial.print("PF2: "); Serial.println(pf2); Serial.println("");
  //phase3
  Serial.print("Voltage3: "); Serial.print(voltage3); Serial.println("V");
  Serial.print("Current3: "); Serial.print(current3); Serial.println("A");
  Serial.print("Power3: "); Serial.print(power3); Serial.println("W");
  Serial.print("Energy3: "); Serial.print(energy3, 3); Serial.println("kWh");
  Serial.print("Frequency3: "); Serial.print(frequency3, 1); Serial.println("Hz");
  Serial.print("PF3: "); Serial.println(pf3); Serial.println("");
}
