#include <Arduino.h>
#include <Dezibot.h>

#define BAT_CHG_STAT 39
#define BAT_ADC 10
#define BAT_ADC_EN 9
#define VUSB_SENS 38
#define FL_PT_EN 37

// BAT_CHG_STAT einlesen und Ergebnis (High oder Low) ausgeben.
bool getBatChgStat() {
  if (digitalRead(BAT_CHG_STAT) == HIGH) {
    return true;
  } else {
    return false;
  }
}

/*BAT_ADC_EN auf High schalten, 10ms warten und über BAT_ADC Akku-
Spannung einlesen und Ergebnis in Volt ausgeben, danach BAT_ADC_EN wieder
LOW schalten. */
//TODO Überprüfung
float readBatteryVoltage() {
  digitalWrite(BAT_ADC_EN, HIGH);
  delay(10);
  int adcValue = analogRead(BAT_ADC);
  digitalWrite(BAT_ADC_EN, LOW);
  float voltage = (adcValue / 4095.0) * 2 * 3.3;  // Assuming a 12-bit ADC and a voltage divider
  return voltage;
}

Dezibot dezibot = Dezibot();

void init() {
  //a. GPIO16, GPIO17, GPIO18 als Ausgang, Low geschaltet.
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  digitalWrite(16, LOW);
  digitalWrite(17, LOW);
  digitalWrite(18, LOW);
  // b. GPIO21 als Eingang, ohne interne Pull-Up-/Down-Widerstände.
  pinMode(21, INPUT);

  if (getBatChgStat()) {
    Serial.println("High");
  } else {
    Serial.println("Low");
  }
}

uint16_t getAverage(color color) {
  uint32_t sum = 0;
  for (uint i = 0; i < 10; i++) {
    uint16_t scan =
      //Serial.print(scan);
      Serial.print(" ");
    sum = (uint32_t)sum + dezibot.colorDetection.getColorValue(color);
    ;
    Serial.print(sum);
    delay(100);
  }
  Serial.print(color);
  Serial.print(" Sensor 10er average: ");
  return (uint16_t)((sum + 5) / 10);
}

/* RGB-LED  D6  nacheinander  je  1s  rot,  grün,  blau  aufleuchten  lassen  und 
Farbsensor IC5 (VEML6040) auslesen, ob die Farben richtig erkannt wurden.  */
void testRGB() {
  dezibot.multiColorLight.setLed(BOTTOM, WHITE);
  Serial.println("--- WHITE --- ");  // Turn off all LEDs
  delay(1000);
  // Serial.print("Red Sensor 10er average: ");
  Serial.println("RED: ");
  Serial.println(getAverage(VEML_RED));
  Serial.println("GREEN: ");
  Serial.println(getAverage(VEML_GREEN));
  Serial.println("BLUE: ");
  Serial.println(getAverage(VEML_BLUE));
  dezibot.multiColorLight.turnOffLed(BOTTOM);

  dezibot.multiColorLight.setLed(BOTTOM, RED);  // Turn off all LEDs
  Serial.println("--- RED --- ");
  delay(1000);
  // Serial.print("Red Sensor 10er average: ");
  Serial.println("RED: ");
  Serial.println(getAverage(VEML_RED));
  Serial.println("GREEN: ");
  Serial.println(getAverage(VEML_GREEN));
  Serial.println("BLUE: ");
  Serial.println(getAverage(VEML_BLUE));
  dezibot.multiColorLight.turnOffLed(BOTTOM);

  dezibot.multiColorLight.setLed(BOTTOM, GREEN);
  Serial.println("--- GREEN --- ");
  delay(1000);
  // Serial.print("Red Sensor 10er average: ");
  Serial.println("RED: ");
  Serial.println(getAverage(VEML_RED));
  Serial.println("GREEN: ");
  Serial.println(getAverage(VEML_GREEN));
  Serial.println("BLUE: ");
  Serial.println(getAverage(VEML_BLUE));
  dezibot.multiColorLight.turnOffLed(BOTTOM);

  dezibot.multiColorLight.setLed(BOTTOM, BLUE);  // Turn off all LEDs
  Serial.println("--- BLUE --- ");
  delay(1000);
  // Serial.print("Red Sensor 10er average: ");
  Serial.println("RED: ");
  Serial.println(getAverage(VEML_RED));
  Serial.println("GREEN: ");
  Serial.println(getAverage(VEML_GREEN));
  Serial.println("BLUE: ");
  Serial.println(getAverage(VEML_BLUE));
  dezibot.multiColorLight.turnOffLed(BOTTOM);
}

// TODO Set As Vector
void testIMU() {
  Serial.println("--- TEST IMU ---");
  uint32_t sumx = 0;
  uint32_t sumy = 0;
  uint32_t sumz = 0;
  for (uint i = 0; i < 10; i++) {
    IMUResult res = dezibot.motion.detection.getAcceleration();
    //Serial.print(scan);
    sumx = sumx + abs((int32_t)res.x);
    sumy = sumy + abs((int32_t)res.y);
    sumz = sumz + abs((int32_t)res.z);
    delay(100);
  }
  //sumx = ((sumx+5)/10);
  //sumy = ((sumy+5)/10);
  //sumz = ((sumz+5)/10);
  Serial.print("x: ");
  Serial.print((sumx + 5) / 10);
  Serial.print("/");
  Serial.print(((sumx + 5) / 10) / 16);
  Serial.print("g, y: ");
  Serial.print((sumy + 5) / 10);
  Serial.print("/");
  Serial.print(((sumy + 5) / 10) / 16);
  Serial.print("g, z: ");
  Serial.print((sumz + 5) / 10);
  Serial.print("/");
  Serial.print(((sumz + 5) / 10) / 16);
  Serial.println("g");
}

void testMotorR() {
  Serial.println("--- TEST MOTOR RIGHT ---");
  dezibot.motion.rotateAntiClockwise();
  testIMU();
  dezibot.motion.stop();
}

void testMotorL() {
  Serial.println("--- TEST MOTOR LEFT ---");
  dezibot.motion.rotateClockwise();
  testIMU();
  dezibot.motion.stop();
}

// Test 7 - 9
void testIRTransitor() {
  Serial.println("--- TEST IR ---");
  uint32_t frontIR = dezibot.lightDetection.getValue(IR_FRONT);
  uint32_t leftIR = dezibot.lightDetection.getValue(IR_LEFT);
  uint32_t backIR = dezibot.lightDetection.getValue(IR_BACK);
  uint32_t rightIR = dezibot.lightDetection.getValue(IR_RIGHT);

  Serial.print("front: ");
  Serial.print(frontIR);
  Serial.print(", ");
  Serial.print("left: ");
  Serial.print(leftIR);
  Serial.print(", ");
  Serial.print("back: ");
  Serial.print(backIR);
  Serial.print(", ");
  Serial.print("right: ");
  Serial.println(rightIR);

  digitalWrite(16, HIGH);
  delay(100);
  frontIR = dezibot.lightDetection.getValue(IR_FRONT);
  leftIR = dezibot.lightDetection.getValue(IR_LEFT);
  backIR = dezibot.lightDetection.getValue(IR_BACK);
  rightIR = dezibot.lightDetection.getValue(IR_RIGHT);
  Serial.print("front: ");
  Serial.print(frontIR);
  Serial.print(", ");
  Serial.print("left: ");
  Serial.print(leftIR);
  Serial.print(", ");
  Serial.print("back: ");
  Serial.print(backIR);
  Serial.print(", ");
  Serial.print("right: ");
  Serial.println(rightIR);
  digitalWrite(16, LOW);
  delay(100);

  digitalWrite(17, HIGH);

  frontIR = dezibot.lightDetection.getValue(IR_FRONT);
  leftIR = dezibot.lightDetection.getValue(IR_LEFT);
  backIR = dezibot.lightDetection.getValue(IR_BACK);
  rightIR = dezibot.lightDetection.getValue(IR_RIGHT);
  Serial.print("front: ");
  Serial.print(frontIR);
  Serial.print(", ");
  Serial.print("left: ");
  Serial.print(leftIR);
  Serial.print(", ");
  Serial.print("back: ");
  Serial.print(backIR);
  Serial.print(", ");
  Serial.print("right: ");
  Serial.println(rightIR);
  digitalWrite(17, LOW);
  delay(100);

  digitalWrite(18, HIGH);
  delay(100);
  frontIR = dezibot.lightDetection.getValue(IR_FRONT);
  leftIR = dezibot.lightDetection.getValue(IR_LEFT);
  backIR = dezibot.lightDetection.getValue(IR_BACK);
  rightIR = dezibot.lightDetection.getValue(IR_RIGHT);
  Serial.print("front: ");
  Serial.print(frontIR);
  Serial.print(", ");
  Serial.print("left: ");
  Serial.print(leftIR);
  Serial.print(", ");
  Serial.print("back: ");
  Serial.print(backIR);
  Serial.print(", ");
  Serial.print("right: ");
  Serial.println(rightIR);
  digitalWrite(18, LOW);
  delay(100);
}

/*Phototransistor für 850nm auf der Platine mit Pull-Down-Widerstand über
GPIO21 einlesen (Soll Low). Dann IR-LED D7 über IR_LED0 einschalten und
GPIO21 wieder einlesen (Soll High). IR_LED0 wieder auf Low ziehen und
Ergebnisse ausgeben.*/
void testIRLed() {
  Serial.println("--- Test IR LED ---");
  Serial.print("off: ");
  Serial.println(digitalRead(21));
  
  delay(100);
  Serial.print("front: ");
  dezibot.infraredLight.front.setState(true);
  delay(10);
  Serial.println(digitalRead(21));
  delay(10);
  dezibot.infraredLight.front.setState(false);
  delay(100);
  Serial.print("bottom: ");
  dezibot.infraredLight.bottom.setState(true);
  delay(10);
  Serial.println(digitalRead(21));
  delay(10);
  dezibot.infraredLight.bottom.setState(false);
}

void setup() {
  dezibot.begin();
  Serial.begin(115200);
  init();

  Serial.println("Dezibot-Test initialized.");

  Serial.println(digitalRead(BAT_CHG_STAT));

  float batteryVoltage = readBatteryVoltage();
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  testRGB();
  delay(1000);
  testIMU();
  delay(1000);
  testMotorL();
  delay(1000);
  testMotorR();
  delay(1000);
  testIRTransitor();
  delay(1000);
  testIRLed();
  delay(1000);
}

void loop() {
  testRGB();
  delay(1000);
  testIMU();
  delay(1000);
  testMotorL();
  delay(1000);
  testMotorR();
  delay(1000);
  testIRTransitor();
  delay(1000);
  testIRLed();
  delay(1000);
}
