#define AD0 A0  //定义模拟口A5
#define D8 8
#define D9 9

String inputString = "";
bool stringComplete = false;

int intensity = 0;  //光照度数值
int state = 0;
int isAuto = 0;
unsigned long currentMillis;
unsigned long startTime;
int status = 0;

void setup() {
  Serial.begin(115200);
  pinMode(D9, INPUT);
  pinMode(D8, OUTPUT);
}

void loop() {
  currentMillis = millis();


  if (stringComplete) {
    if (inputString == "1") {
      startTime = currentMillis;
      isAuto = 1;
      if (status == 0) {
        digitalWrite(D8, HIGH);
        status = 1;
      }
      log();
    } else if (inputString == "0") {
      startTime = 0;
      status = 0;
      isAuto = 0;
      digitalWrite(D8, LOW);
      log();
    }
    inputString = "";
    stringComplete = false;
  }


  state = digitalRead(D9);
  intensity = analogRead(AD0);

  if (isAuto == 0) {
    if (intensity < 300 && state == 1) {
      startTime = currentMillis;
      if (status == 0) {
        digitalWrite(D8, HIGH);
        status = 1;
        log();
      }
    } else {
      if (status == 1 && (currentMillis - startTime) > 5000) {
        startTime = 0;
        status = 0;
        digitalWrite(D8, LOW);
        log();
      }
    }
  }
}

void log() {

  Serial.print("D:");
  Serial.print(status);
  Serial.print("|I:");
  Serial.print(intensity);
  Serial.print("|S:");
  Serial.print(state);
  Serial.print("|T:");
  Serial.println(currentMillis);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      inputString.replace("\n", "");
      inputString.replace("\r", "");
      inputString.replace(" ", "");
      stringComplete = true;
    }
  }
}
