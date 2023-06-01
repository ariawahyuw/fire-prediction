#include <low_power.h>
#include <STM32LowPower.h>
#include <SPI.h>
#include <LoRa.h>
#include <DHTStable.h>
#include <INA226.h>


#define LED_BUILTIN PC13
#define LORA_ON PB15
#define outputA PA0
#define outputB PB2
#define windSpeed PA3
#define soilPin PA2
#define windDir PB0
#define E PA13
#define s2 PB5
#define s1 PB4
#define s0 PB3

INA226 INA(0x40);                                                                         
DHTStable DHT;

//Variable & constant definitions
int period = 2000;
unsigned int counter = 0; // B/W counter for sensor
float speedwind = 0; // Wind speed (m/s)
int counterLevel = 0; //dimulai nol
volatile int aState;  //deklarasi
int aLastState;  //deklarasi
int Tinggi = 0;
float V = 0;
float I = 0;
int soilHumid = 0;
int windDirection = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LORA_ON, OUTPUT);
  pinMode(windDir, INPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(PA12, OUTPUT);
  LowPower.begin();
  //LowPower.attachInterruptWakeup(PA12, changeStateWhenInterrupt, CHANGE, DEEP_SLEEP_MODE);
  //LowPower.attachInterruptWakeup(outputA, changeStateWhenInterrupt, CHANGE, DEEP_SLEEP_MODE);
  INA.begin();
  INA.setMaxCurrentShunt(0.2, 0.1, false); //max amp and Rshunt
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LORA_ON, LOW);
  //Serial.print(F("[SX1276] Initializing ... "));
  LoRa.setPins(PA4, PB14, PB1);
  if (!LoRa.begin(915E6)){
    while (true);
  }
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(7);
  LoRa.setPreambleLength(8);
  LoRa.setTxPower(6);
  LoRa.setSyncWord(0x12);
  LoRa.setGain(0);
  aLastState = digitalRead(outputA);
}

void loop() {
  digitalWrite(PA12, LOW);
  digitalWrite(E, LOW);
  V = INA.getBusVoltage();
  Serial.println(V, 3);
  I = INA.getCurrent_mA();
  Serial.println(I, 3);
  aState = digitalRead(outputA);
  windvelocity();
  windspeed();
  soilHumid = analogRead(soilPin);
  soilHumid = map(soilHumid, 1024, 0, 0, 100);
  int chk = DHT.read11(PA8);
  float h = DHT.getHumidity();
  float t = DHT.getTemperature();
  if (chk != 0) {
    h = 0;
    t = 0;
  }
  if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counterLevel++;
     } else {
       counterLevel--;
     }
     Tinggi=counterLevel;
  }
  for (uint8_t i = 0; i < 8; i++){
    setMultiplexer(i);
    int windVal = digitalRead(windDir);
    delay(10);
    if (windVal == LOW){
      windDirection = i + 1;
    }
  }                
  Serial.print(F("[SX1276] Transmitting packet ... "));
  LoRa.beginPacket();
  LoRa.print(t,1);
  LoRa.print(",");
  LoRa.print(h,1);
  LoRa.print(",");
  LoRa.print(Tinggi);
  LoRa.print(",");
  LoRa.print(speedwind);
  LoRa.print(",");
  LoRa.print(soilHumid);
  LoRa.print(",");
  LoRa.print(windDirection);
  LoRa.print(",");
  LoRa.print(V, 3);
  LoRa.print(",");
  LoRa.print(I, 3);
  LoRa.endPacket();
  LoRa.end();
  Serial.println("Done!");
  blink(2);
  aLastState = aState;
  digitalWrite(LORA_ON, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(PA12, HIGH);
  LowPower.shutdown(300000);
}

void addcount(){  
  counter++;
}

void windvelocity(){
  speedwind = 0;
  counter = 0;
  attachInterrupt(windSpeed, addcount, CHANGE);
  unsigned long millis();
  long startTime = millis();
  while(millis() < startTime + period) {}
  detachInterrupt(windSpeed);
}

void setMultiplexer( int i ){  
  int s0Value = i & 0x01;      // get value of first bit
  int s1Value = (i>>1) & 0x01; // get value of second bit
  int s2Value = (i>>2) & 0x01; // get value of third bit
  digitalWrite( s0, s0Value ); // turn first pin on or off
  digitalWrite( s1, s1Value ); // turn second pin on or off
  digitalWrite( s2, s2Value ); // turn third pin on or off
}
 
void windspeed(){
  // Constants definitions
  //radioMill = 90;  Distance from center windmill to outer cup (mm)
  //jml_celah = 18; jumlah celah sensor
  speedwind = ((2 * 3.14159265 * 90 * ((counter/18)*60)/(period/1000))/60) / 1000; // Calculate wind speed on m/s
}

void changeStateWhenInterrupt(){
 delay(1000);
}
 
void blink(int n) {
  for(int i=0;i<n;i++){
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
      delay(200);                       // wait for a second
      digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
      delay(200);                       // wait for a second
  }
}
