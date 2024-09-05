#include <WiFi.h>
#include <MQTT.h>
#include <Ultrasonic.h>

/// วัดระยะ
/*
const int triggerLeft = 0; // ขา Trig ของเซนเซอร์ Ultrasonic ซ้าย
const int echoLeft = 4;    // ขา Echo ของเซนเซอร์ Ultrasonic ซ้าย
const int triggerRight = 14; // ขา Trig ของเซนเซอร์ Ultrasonic ขวา
const int echoRight = 15;   // ขา Echo ของเซนเซอร์ Ultrasonic ขวา
const int triggerMid = 16; // ขา Trig ของเซนเซอร์ Ultrasonic กลาง
const int echoMid = 17;   // ขา Echo ของเซนเซอร์ Ultrasonic กลาง

Ultrasonic ultrasonicLeft(triggerLeft, echoLeft); // สร้างอ็อบเจ็กต์ Ultrasonic สำหรับเซนเซอร์ซ้าย
Ultrasonic ultrasonicRight(triggerRight, echoRight); // สร้างอ็อบเจ็กต์ Ultrasonic สำหรับเซนเซอร์ขวา
Ultrasonic ultrasonicMid(triggerMid, echoMid); // สร้างอ็อบเจ็กต์ Ultrasonic สำหรับเซนเซอร์กลาง
*/
/// ควบคุมมอเตอร์
#define PWM_pin1 13
#define ina_pin1 12
#define inb_pin1 32//27
#define PWM_pin2 19
#define ina_pin2 18
#define inb_pin2 5

const char ssid[] = "Bossai";
const char pass[] = "123456789";
const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="AGV_THE_PEAK";
const char mqtt_client_id[]="clientId-ZJThyVWvu"; // must change this string to a unique value
int MQTT_PORT=1883;
int counter=0;
WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect(mqtt_client_id)) {  
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe(mqtt_topic);
  // client.unsubscribe("/hello");
}

String i ;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  i = payload;
}
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);

  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(PWM_pin1, OUTPUT);
  pinMode(ina_pin1, OUTPUT);
  pinMode(inb_pin1, OUTPUT);

  pinMode(PWM_pin2, OUTPUT);
  pinMode(ina_pin2, OUTPUT);
  pinMode(inb_pin2, OUTPUT);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability
  /*
  float distanceLeft = ultrasonicLeft.read(); // อ่านค่าระยะทางจากเซนเซอร์ Ultrasonic ซ้าย
  float distanceRight = ultrasonicRight.read(); // อ่านค่าระยะทางจากเซนเซอร์ Ultrasonic ขวา
  float distanceMid = ultrasonicMid.read(); // อ่านค่าระยะทางจากเซนเซอร์ Ultrasonic กลาง
*/
  if (!client.connected()) {
    connect();
  }
  if (millis() - lastMillis > 2000) {
    lastMillis = millis();
    client.publish(mqtt_topic, "Counter = " + String(counter++));
      if (i == "for") {
      // Code for moving forward
      Serial.println("for");analogWrite(PWM_pin1, 150);
      Serial.println("for");digitalWrite(ina_pin1, HIGH);
      Serial.println("for");digitalWrite(inb_pin1, LOW);

      Serial.println("for");analogWrite(PWM_pin2, 150);
      Serial.println("for");digitalWrite(ina_pin2, HIGH);
      Serial.println("for");digitalWrite(inb_pin2, LOW);

     }else if (i == "rig") {
      // Code for turning right
      Serial.println("rig");analogWrite(PWM_pin1, 150);
      Serial.println("rig");digitalWrite(ina_pin1, LOW);
      Serial.println("rig");digitalWrite(inb_pin1, HIGH);

      Serial.println("rig");analogWrite(PWM_pin2, 150);
      Serial.println("rig");digitalWrite(ina_pin2, HIGH);
      Serial.println("rig");digitalWrite(inb_pin2, LOW);
     }else if (i == "lef") {
      // Code for turning left
      Serial.println("lef");analogWrite(PWM_pin2, 150);
      Serial.println("lef");digitalWrite(ina_pin2, LOW);
      Serial.println("lef");digitalWrite(inb_pin2, HIGH);

      Serial.println("lef");analogWrite(PWM_pin1, 150);
      Serial.println("lef");digitalWrite(ina_pin1, HIGH);
      Serial.println("lef");digitalWrite(inb_pin1, LOW);
     }else if (i == "bac") {
      // Code for moving backward
      Serial.println("bac");analogWrite(PWM_pin1, 150);
      Serial.println("bac");digitalWrite(ina_pin1, LOW);
      Serial.println("bac");digitalWrite(inb_pin1, HIGH);

      Serial.println("bac");analogWrite(PWM_pin2, 150);
      Serial.println("bac");digitalWrite(ina_pin2, LOW);
      Serial.println("bac");digitalWrite(inb_pin2, HIGH);
     }else if(i == "stop"){
      Serial.println("stop");analogWrite(PWM_pin1, 150);
      Serial.println("stop");digitalWrite(ina_pin1, LOW);
      Serial.println("stop");digitalWrite(inb_pin1, LOW);

      Serial.println("stop");analogWrite(PWM_pin2, 150);
      Serial.println("stop");digitalWrite(ina_pin2, LOW);
      Serial.println("stop");digitalWrite(inb_pin2, LOW);
     }
     }else if(i == "k1"){
      Serial.println("k1");analogWrite(PWM_pin1, 150);
      Serial.println("k1");digitalWrite(ina_pin1, HIGH);
      Serial.println("k1");digitalWrite(inb_pin1, LOW);

      Serial.println("k1");analogWrite(PWM_pin2, 150);
      Serial.println("k1");digitalWrite(ina_pin2, HIGH);
      Serial.println("k1");digitalWrite(inb_pin2, LOW);
      delay(10000);
      Serial.println("k1");analogWrite(PWM_pin2, 150);
      Serial.println("k1");digitalWrite(ina_pin2, LOW);
      Serial.println("k1");digitalWrite(inb_pin2, HIGH);

      Serial.println("k1");analogWrite(PWM_pin1, 150);
      Serial.println("k1");digitalWrite(ina_pin1, HIGH);
      Serial.println("k1");digitalWrite(inb_pin1, LOW);
      delay(1125);
      Serial.println("k1");analogWrite(PWM_pin1, 150);
      Serial.println("k1");digitalWrite(ina_pin1, LOW);
      Serial.println("k1");digitalWrite(inb_pin1, LOW);

      Serial.println("k1");analogWrite(PWM_pin2, 150);
      Serial.println("k1");digitalWrite(ina_pin2, LOW);
      Serial.println("k1");digitalWrite(inb_pin2, LOW);
     }
      else{}
}