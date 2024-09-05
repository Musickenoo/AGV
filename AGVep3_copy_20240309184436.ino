#include <WiFi.h>
#include <MQTT.h>
#include <Ultrasonic.h>
#include <ESP32Encoder.h>

/// ควบคุมมอเตอร์
#define PWM_pin1 13
#define ina_pin1 12
#define inb_pin1 32//27
#define PWM_pin2 19
#define ina_pin2 18
#define inb_pin2 5

ESP32Encoder encoder1; // ตัวแปรสำหรับ Encoder แรก
ESP32Encoder encoder2; // ตัวแปรสำหรับ Encoder ที่สอง

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
bool encoder1_reached_target = false;
bool encoder2_reached_target = false;

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

  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  encoder1.attachHalfQuad(27, 26); // Attach Encoder 1 ที่ขา 27 และ 26
  encoder2.attachHalfQuad(14, 25); // Attach Encoder 2 ที่ขา 14 และ 25

  encoder1.clearCount(); // เริ่มต้นค่าตั้งต้นของ Encoder 1
  encoder2.clearCount(); // เริ่มต้นค่าตั้งต้นของ Encoder 2
  
  connect();
}

int target_count = 1500;

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
      Serial.println("for");digitalWrite(ina_pin1, HIGH);
      Serial.println("for");digitalWrite(inb_pin1, LOW);

      Serial.println("for");digitalWrite(ina_pin2, HIGH);
      Serial.println("for");digitalWrite(inb_pin2, LOW);
      if (encoder1.getCount() >= target_count && encoder1.getCount() >= target_count) {
        Serial.println("for");analogWrite(PWM_pin1, 150);
        Serial.println("for");analogWrite(PWM_pin2, 150);

        // Reset counters and status flags
        encoder1.clearCount();
        encoder2.clearCount();

      } else if (encoder1.getCount() >= target_count && encoder2.getCount() < target_count) {
         Serial.println("for");analogWrite(PWM_pin1, 100); // Reduce speed for motor 1
         Serial.println("for");analogWrite(PWM_pin2, 150); // Maximum speed for motor 2
      } else if (encoder1.getCount() < target_count && encoder2.getCount() >= target_count) {
         Serial.println("for");analogWrite(PWM_pin1, 150); // Maximum speed for motor 1
         Serial.println("for");analogWrite(PWM_pin2, 100); // Reduce speed for motor 2
      } else{
        Serial.println("for");analogWrite(PWM_pin1, 150);
        Serial.println("for");analogWrite(PWM_pin2, 150);
      }

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
      Serial.println("bac");digitalWrite(ina_pin1, LOW);
      Serial.println("bac");digitalWrite(inb_pin1, HIGH);

      Serial.println("bac");digitalWrite(ina_pin2, LOW);
      Serial.println("bac");digitalWrite(inb_pin2, HIGH);
      if (encoder1.getCount() >= target_count && encoder1.getCount() >= target_count) {
        Serial.println("bac");analogWrite(PWM_pin1, 150);
        Serial.println("bac");analogWrite(PWM_pin2, 150);

        // Reset counters and status flags
        encoder1.clearCount();
        encoder2.clearCount();

      } else if (encoder1.getCount() >= target_count && encoder2.getCount() < target_count) {
        Serial.println("bac");analogWrite(PWM_pin1, 100); // Reduce speed for motor 1
        Serial.println("bac");analogWrite(PWM_pin2, 150); // Maximum speed for motor 2
      } else if (encoder1.getCount() < target_count && encoder2.getCount() >= target_count) {
        Serial.println("bac");analogWrite(PWM_pin1, 150); // Maximum speed for motor 1
        Serial.println("bac");analogWrite(PWM_pin2, 100); // Reduce speed for motor 2
      } else{
        Serial.println("bac");analogWrite(PWM_pin1, 150);
        Serial.println("bac");analogWrite(PWM_pin2, 150);
      }
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
      if (encoder1.getCount() >= target_count && encoder1.getCount() >= target_count) {
        Serial.println("k1");analogWrite(PWM_pin1, 150);
        Serial.println("k1");analogWrite(PWM_pin2, 150);

        // Reset counters and status flags
        encoder1.clearCount();
        encoder2.clearCount();

      } else if (encoder1.getCount() >= target_count && encoder2.getCount() < target_count) {
         Serial.println("k1");analogWrite(PWM_pin1, 100); // Reduce speed for motor 1
         Serial.println("k1");analogWrite(PWM_pin2, 150); // Maximum speed for motor 2
      } else if (encoder1.getCount() < target_count && encoder2.getCount() >= target_count) {
         Serial.println("k1");analogWrite(PWM_pin1, 150); // Maximum speed for motor 1
         Serial.println("k1");analogWrite(PWM_pin2, 100); // Reduce speed for motor 2
      } else{
        Serial.println("k1");analogWrite(PWM_pin1, 150);
        Serial.println("k1");analogWrite(PWM_pin2, 150);
      }
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