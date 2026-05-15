const int trigPlastic = 4; // Синий (ID: 1)
const int echoPlastic = 5;

const int trigPaper = 6;   // Желтый (ID: 2)
const int echoPaper = 7;

const int trigOther = 11;  // Серый (ID: 3)
const int echoOther = 12;

const int threshold = 11;       // Дистанция срабатывания (см)
const int quietTime = 30;       // Пауза между опросами датчиков
const long waitTimeout = 15000; // Ожидание 15 секунд
void setup() {
  Serial.begin(9600);
  
  pinMode(trigPlastic, OUTPUT); pinMode(echoPlastic, INPUT);
  pinMode(trigPaper, OUTPUT);   pinMode(echoPaper, INPUT);
  pinMode(trigOther, OUTPUT);   pinMode(echoOther, INPUT);
  
  while(Serial.available() > 0) Serial.read();
  Serial.println("READY"); 
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd.length() > 0) {
       waitForObject();
    }
  }
}

void waitForObject() {
  unsigned long startTime = millis();
  
  while (millis() - startTime < waitTimeout) {
    //Датчик 1: СИНИЙ
    if (checkSensor(trigPlastic, echoPlastic)) {
      Serial.println("ID:1"); // Отправляем только ID
      return;
    }
    delay(quietTime);

    //Датчик 2: ЖЕЛТЫЙ
    if (checkSensor(trigPaper, echoPaper)) {
      Serial.println("ID:2");
      return;
    }
    delay(quietTime);

    //Датчик 3: СЕРЫЙ
    if (checkSensor(trigOther, echoOther)) {
      Serial.println("ID:3");
      return;
    }
    delay(quietTime);
  }

  Serial.println("ID:TIMEOUT");
}

bool checkSensor(int trig, int echo) {
  long distance = getDistance(trig, echo);
  return (distance > 0 && distance <= threshold);
}

long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000); 
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}