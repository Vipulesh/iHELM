#include <SoftwareSerial.h>
#include <Wire.h>  // Wire library - used for I2C communication
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>
#include <ADXL345.h>
#define RX 2
#define TX 3
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
String AP = "realme narzo 30";       // AP NAME
String PASS = "VipuL23#T"; // AP PASSWORD
String API = "2DRE0AY1MVYF01XI";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1="field1";
String field3 = "field3";
String field2 = "field2";
String field4="field4";
String field5="field5";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor1 = 1; int valSensor2 = 1; int valSensor3 = 1; int valSensor4 = 1; int valSensor5 = 1;
SoftwareSerial esp8266(RX,TX); 
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
float roll,pitch;
double ax,ay,az;
void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  if(!accel.begin())
   {
      Serial.println("No valid sensor found");
      while(1);
   }
  accel.setRange(ADXL345_RANGE_16_G);
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
}

void loop() {
 valSensor1 = getSensorData1();
 valSensor2 = getSensorData2();
 valSensor3 = getSensorData3();
 valSensor4 = getSensorData4();
 valSensor5 = getSensorData5();
String getData = "GET /update?api_key="+ API+"&"+ field1 +"="+String(valSensor1) +"&"+ field2 +"="+String(valSensor2) +"&"+ field3 +"="+String(valSensor3)+"&"+ field4 +"="+String(valSensor4)+"&"+ field5 +"="+String(valSensor5);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(150);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}
void calculation(){
  double xyz[3];
  adxl.getAcceleration(xyz);
  ax = xyz[0];
  ay = xyz[1];
  az = xyz[2];
  Serial.print("Acc_X : ");
  Serial.print(ax*9.8);
  Serial.print("  Acc_Y : ");
  Serial.print(ay*9.8);
  Serial.print("  Acc_Z : ");
  Serial.print(az*9.8);
  Serial.print(" m/s^2 ");
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
  X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
  Z_out = Z_out/256;
  float r1=Y_out/Z_out;
  roll=atan(r1)*180/PI;
  float p1=-X_out/sqrt(Y_out*Y_out+Z_out*Z_out);
    pitch=atan(p1)*180/PI;
    Serial.println();
  Serial.println(roll-87.82);
  Serial.println(pitch+3.93);
  Serial.println();
}
float getSensorData1(){
  calculation();
  return (pitch-87.82); // Replace with your own sensor code
}
float getSensorData2(){
  calculation();
  return (roll+3.93); // Replace with your own sensor code
}
float getSensorData3(){
  calculation();
  return az*9.8; // Replace with your own sensor code
}
float getSensorData4(){
  calculation();
  return ay*9.8; // Replace with your own sensor code
}
float getSensorData5(){
  calculation();
  return ax*9.8; // Replace with your own sensor code
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
