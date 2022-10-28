#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;
#define ser1 Serial
SoftwareSerial IMU(rxPin, txPin);
int led = 13;

unsigned long previousMillis = 0;

//CSV PARSING
String incomingString;
String incomingCSV[20];
int currentCSVIndex = 0;
int maxCSVIndex;
char incomingByte;
int incomingInt;


void setup() {
  pinMode(led, OUTPUT);
  ser1.begin(115200);
  IMU.begin(115200);
  

}

void loop() {
  requestData(4, 100);
  if (IMU.available()) {
    digitalWrite(led, HIGH);
    serialIN(IMU.read(), 0);
    
    digitalWrite(led, LOW);
  }
}

void printData(){
  ser1.println();
  ser1.print("Roll    = ");
  ser1.println(incomingCSV[2].toFloat() * (180/ PI));
  ser1.print("Pitch   = ");
  ser1.println(incomingCSV[0].toFloat() * (180/ PI));
  ser1.print("Yaw     = ");
  ser1.println(incomingCSV[1].toFloat() * (180/ PI));
  
}
void requestData(int data, int interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    switch (data) {
      case 1:
        Serial.print("37(0x25) - ");
        Serial.println("Get all corrected component sensor data");
        IMU.println(":37");
        break;

      case 2:
        Serial.println("38(0x26) - ");
        Serial.println("Get corrected gyro rate");
        IMU.println(":38");
        break;

      case 3:
        Serial.println("39(0x27) - ");
        Serial.println("Get corrected accelerometer vector");
        IMU.println(":39");
        break;

      case 4:
//        Serial.println("1(0x01) - ");
//        Serial.println("Get tared orientation as euler angles");
        IMU.println(":1");
        break;
    }
  }
}

void serialIN(char incomingSerial, bool debugMode) {
  incomingByte = incomingSerial;
  incomingString += incomingByte;
  if (incomingByte == ',') {
    int str_len = incomingString.length() - 1;
    incomingCSV[currentCSVIndex] = incomingString.substring(0, str_len);
    if (debugMode == 1) {
      ser1.print("Incoming ");
      ser1.print(currentCSVIndex);
      ser1.print(" = ");
      ser1.println(incomingCSV[currentCSVIndex].toFloat() * (180/ PI));
    }
    currentCSVIndex++;
    incomingString = "";
  }
  if (incomingByte == '\n') {
    int str_len = incomingString.length() - 1;
    incomingCSV[currentCSVIndex] = incomingString.substring(0, str_len);
    maxCSVIndex = currentCSVIndex;
    if (debugMode == 1) {
      ser1.print("Incoming ");
      ser1.print(currentCSVIndex);
      ser1.print(" = ");
      ser1.println(incomingCSV[currentCSVIndex].toFloat() * (180/ PI));
      ser1.print("max Index  = ");
      ser1.println(maxCSVIndex);
      ser1.println();
    }
    printData();
    incomingString = "";
    currentCSVIndex = 0;
  }
}
