//int MotorPin2 = 2;
//int MotorPin3 = 3;
//int MotorPin4 = 4;
//int MotorPin5 = 5;
#define PWM_pin1 13
#define ina_pin1 12
#define inb_pin1 32

#define PWM_pin2 19
#define ina_pin2 18
#define inb_pin2 5

void setup() {
  //pinMode(MotorPin2, OUTPUT);
  //pinMode(MotorPin3, OUTPUT);
  //pinMode(MotorPin4, OUTPUT);
  //pinMode(MotorPin5, OUTPUT);
  pinMode(PWM_pin1,OUTPUT);
  pinMode(ina_pin1,OUTPUT);
  pinMode(inb_pin1,OUTPUT);
  pinMode(PWM_pin2,OUTPUT);
  pinMode(ina_pin2,OUTPUT);
  pinMode(inb_pin2,OUTPUT);

  //pinMode(leftSensorPin, INPUT);
  //pinMode(rightSensorPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  //int leftSensorValue = analogRead(leftSensorPin);  // ใช้ analogRead() เพื่ออ่านค่าต่อเนื่อง
  //int rightSensorValue = analogRead(rightSensorPin);  // ใช้ analogRead() เพื่ออ่านค่าต่อเนื่อง

  //int threshold = 500; // ค่าตรวจจับสีขาว/ดำ
/*
  if (leftSensorValue < threshold && rightSensorValue < threshold) {
    digitalWrite(MotorPin2, LOW);
    digitalWrite(MotorPin3, HIGH);
    digitalWrite(MotorPin4, LOW);
    digitalWrite(MotorPin5, HIGH);
    Serial.println("Move Forward");
  } else if (leftSensorValue >= threshold && rightSensorValue < threshold) {
    digitalWrite(MotorPin2, HIGH);
    digitalWrite(MotorPin3, LOW);
    digitalWrite(MotorPin4, LOW);
    digitalWrite(MotorPin5, HIGH);
    Serial.println("Turn Right");
  } else if (leftSensorValue < threshold && rightSensorValue >= threshold) {
    digitalWrite(MotorPin2, LOW);
    digitalWrite(MotorPin3, HIGH);
    digitalWrite(MotorPin4, HIGH);
    digitalWrite(MotorPin5, LOW);
    Serial.println("Turn Left");
  } else {
    digitalWrite(MotorPin2, LOW);
    digitalWrite(MotorPin3, LOW);
    digitalWrite(MotorPin4, LOW);
    digitalWrite(MotorPin5, LOW);
    Serial.println("Stop");
  }
*/
  //forward 
  analogWrite(PWM_pin1,255);
  digitalWrite(ina_pin1,HIGH);
  digitalWrite(inb_pin1,LOW);
  analogWrite(PWM_pin2,255);
  digitalWrite(ina_pin2,HIGH);
  digitalWrite(inb_pin2,LOW);
  delay(2000);
  //right 
  analogWrite(PWM_pin1,127);
  digitalWrite(ina_pin1,LOW);
  digitalWrite(inb_pin1,HIGH);
  analogWrite(PWM_pin2,255);
  digitalWrite(ina_pin2,HIGH);
  digitalWrite(inb_pin2,LOW);
  delay(2000);
  //Left
  analogWrite(PWM_pin2,127);
  digitalWrite(ina_pin2,LOW);
  digitalWrite(inb_pin2,HIGH);
  analogWrite(PWM_pin1,255);
  digitalWrite(ina_pin1,HIGH);
  digitalWrite(inb_pin1,LOW);
  delay(2000);
  //back
  analogWrite(PWM_pin1,255);
  digitalWrite(ina_pin1,LOW);
  digitalWrite(inb_pin1,HIGH);
  analogWrite(PWM_pin2,255);
  digitalWrite(ina_pin2,LOW);
  digitalWrite(inb_pin2,HIGH);
  delay(2000);

}
