#define IMU Serial2

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
  Serial.begin(115200);
  Serial2.begin(115200);
  digitalWrite(led, HIGH);

}

void loop() {
  requestData(4, 100);

  if (IMU.available()) {
    serialIN(IMU.read(), 1);
  }
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
        Serial.println("1(0x01) - ");
        Serial.println("Get tared orientation as euler angles");
        IMU.println(":39");
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
      Serial.print("Incoming ");
      Serial.print(currentCSVIndex);
      Serial.print(" = ");
      Serial.println(incomingCSV[currentCSVIndex].toFloat() * 90.0);
    }
    currentCSVIndex++;
    incomingString = "";
  }
  if (incomingByte == '\n') {
    int str_len = incomingString.length() - 1;
    incomingCSV[currentCSVIndex] = incomingString.substring(0, str_len);
    maxCSVIndex = currentCSVIndex;
    if (debugMode == 1) {
      Serial.print("Incoming ");
      Serial.print(currentCSVIndex);
      Serial.print(" = ");
      Serial.println(incomingCSV[currentCSVIndex].toFloat() * 90.0);
      Serial.print("max Index  = ");
      Serial.println(maxCSVIndex);
      Serial.println();
    }
    incomingString = "";
    currentCSVIndex = 0;
  }
}
