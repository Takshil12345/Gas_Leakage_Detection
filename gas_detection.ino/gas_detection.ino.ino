#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
#include <SoftwareSerial.h>
// #include <Serial.h>
SoftwareSerial Serial1(2, 3);
#include <Servo.h>
Servo servo;

#define DT A0
#define SCK A1
#define sw 4
#define servoPin A2
int sensorPin = A5;
int read_value;
int set = 150;
const int servotime1 = 5;
const int servotime2 = 6;
const int servotime3 = A3;



const int RELAY2 = A4;
int a = 0;
int b = 0;
long sample = 0;
float val = 0;
long count = 0;

unsigned long readCount(void) {
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT, HIGH);
  digitalWrite(SCK, LOW);
  Count = 0;
  pinMode(DT, INPUT);
  while (digitalRead(DT))
    ;
  for (i = 0; i < 24; i++) {
    digitalWrite(SCK, HIGH);
    Count = Count << 1;
    digitalWrite(SCK, LOW);
    if (digitalRead(DT))
      Count++;
  }
  digitalWrite(SCK, HIGH);
  Count = Count ^ 0x800000;
  digitalWrite(SCK, LOW);
  return (Count);
}

void setup() {
  servo.attach(servoPin);
  servo.write(180);
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(servotime1, INPUT_PULLUP);
  pinMode(servotime2, INPUT_PULLUP);
  pinMode(servotime3, INPUT_PULLUP);

  pinMode(RELAY2, OUTPUT);
  lcd.begin(16, 2);
  lcd.print(" Weight ");
  lcd.setCursor(0, 1);
  lcd.print(" Measurement ");
  delay(1000);
  lcd.clear();
  calibrate();
}

void loop() {
  read_value = (analogRead(sensorPin));
  read_value = read_value - 50;
  if (read_value < 0) {
    read_value = 0;
  }
  count = readCount();
  int w = (((count - sample) / val) - 2 * ((count - sample) / val));

  lcd.setCursor(0, 0);
  lcd.print("M.Weight=");
  lcd.setCursor(10, 0);
  lcd.print(w);
  lcd.print("g ");
  lcd.setCursor(0, 1);
  lcd.print("Gas=");
  lcd.print(read_value);
  lcd.print("  ");

  delay(200);
  if (digitalRead(sw) == 0) {
    val = 0;
    sample = 0;
    w = 0;
    count = 0;
    calibrate();
  }

  if (read_value > set) {

    digitalWrite(RELAY2, HIGH);
    servo.write(0);
    delay(1000);

    pawan();
  }

  if (read_value < set) {
    digitalWrite(RELAY2, LOW);

    delay(500);
  }
  if (digitalRead(servotime1) == LOW) {
    servo.write(0);
    delay(10000);
    servo.write(180);
    delay(1000);
  }
  if (digitalRead(servotime2) == LOW) {
    servo.write(0);
    delay(20000);
    servo.write(180);
    delay(1000);
  }
  if (digitalRead(servotime3) == LOW) {
    // servo.write(0);
    //  delay(30000);
    servo.write(180);
    delay(1000);
  }

  if ((w >= 50 && w <= 100)) {

    digitalWrite(RELAY2, HIGH);
    pawan1();
  }
}


void calibrate() {
  lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0, 1);
  lcd.print("Please Wait...");
  for (int i = 0; i < 100; i++) {
    count = readCount();
    sample += count;
  }
  sample /= 100;
  lcd.clear();
  lcd.print("Put 100g & wait");
  count = 0;
  while (count < 1000) {
    count = readCount();
    count = sample - count;
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(1000);
  for (int i = 0; i < 100; i++) {
    count = readCount();
    val += sample - count;
  }
  val = val / 100.0;
  val = val / 100.0;  // put here your calibrating weight
  lcd.clear();
}

void pawan() {
  lcd.clear();
  lcd.print("SMS SEND TO.....");
  lcd.setCursor(0, 1);
  lcd.print("GAS LEAKAGE......");
  delay(2000);
  Serial1.println("AT");
  delay(500);
  Serial.print("");
  Serial1.println("AT+CMGF=1");
  delay(500);
  Serial.print("");
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("+919136066077");
  Serial1.println('"');
  delay(500);
  Serial1.print("Emergency!!Gas Leakage ");
  delay(500);

  Serial.print("");
  Serial1.write(26);
  delay(1000);
  Serial.print("");

  lcd.clear();
  //loop();
}

void pawan1() {
  lcd.clear();
  lcd.print("SMS SEND TO.......");
  lcd.setCursor(0, 1);
  lcd.print("LOAD CELL..........");
  delay(2000);
  Serial1.println("AT");
  delay(500);
  Serial.print("");
  Serial1.println("AT+CMGF=1");
  delay(500);
  Serial.print("");
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("+919136066077");
  Serial1.println('"');
  delay(500);
  Serial1.print("UNDER 100G WEIGHT ");
  delay(500);

  Serial.print("");
  Serial1.write(26);
  delay(1000);
  Serial.print("");
  delay(1000);
  digitalWrite(RELAY2, LOW);
  lcd.clear();
  //loop();
}
