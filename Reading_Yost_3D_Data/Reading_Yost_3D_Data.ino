#define ser1 Serial
HardwareSerial IMU(PA3, PA2); //RX, TX
HardwareSerial ser3(PB11, PB10); //RX, TX

int led = 13;

unsigned long previousMillis = 0;

//CSV PARSING
String incomingString;
String incomingCSV[20];
int currentCSVIndex = 0;
int maxCSVIndex;
char incomingByte;
int incomingInt;

float roll, pitch, yaw;

void setup() {
  pinMode(led, OUTPUT);
  ser1.begin(115200);
  IMU.begin(115200);
  ser3.begin(115200);
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
        ser1.print("37(0x25) - ");
        ser1.println("Get all corrected component sensor data");
        IMU.println(":37");
        break;

      case 2:
        ser1.println("38(0x26) - ");
        ser1.println("Get corrected gyro rate");
        IMU.println(":38");
        break;

      case 3:
        ser1.println("39(0x27) - ");
        ser1.println("Get corrected accelerometer vector");
        IMU.println(":39");
        break;

      case 4:
        ser1.println("1(0x01) - ");
        ser1.println("Get tared orientation as euler angles");
        IMU.println(":1");
        break;
    }
  }
}

void dataPreparation(){
  roll  = radToDeg(incomingCSV[2].toFloat());
  pitch = radToDeg(incomingCSV[0].toFloat());
  yaw   = radToDeg(incomingCSV[1].toFloat());
  
//  roll  = (incomingCSV[2].toFloat() * (180/ PI));
//  pitch = (incomingCSV[0].toFloat() * (180/ PI));
//  yaw   = (incomingCSV[1].toFloat() * (180/ PI));
}

float radToDeg(float data){
  return data * (180/PI);
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
      ser1.println(incomingCSV[currentCSVIndex].toFloat() * 90.0);
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
      ser1.println(incomingCSV[currentCSVIndex].toFloat() * 90.0);
      ser1.print("max Index  = ");
      ser1.println(maxCSVIndex);
      ser1.println();
    }
    dataPreparation();
    incomingString = "";
    currentCSVIndex = 0;
  }
}
