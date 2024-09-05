#include <WiFi.h>
#include <MQTT.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder1; // ตัวแปรสำหรับ Encoder แรก
ESP32Encoder encoder2; // ตัวแปรสำหรับ Encoder ที่สอง

#define PWM_pin1 13
#define ina_pin1 12
#define inb_pin1 23//27
#define PWM_pin2 19
#define ina_pin2 18
#define inb_pin2 5

const char ssid[] = "Bossai";
const char pass[] = "123456789";
const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_topic[] = "AGV_THE_PEAK";
const char mqtt_client_id[] = "clientId-ZJThyVWvuG"; // must change this string to a unique value
int MQTT_PORT = 1883;
int counter = 0;
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
}

String i;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  i = payload;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);

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

void controlSpeed() {
  int target_count = 1500;

  if (encoder1_reached_target && encoder2_reached_target) {
    analogWrite(PWM_pin1, 255); // Maximum speed for motor 1
    analogWrite(PWM_pin2, 255); // Maximum speed for motor 2
    
    // Reset counters and status flags
    encoder1.clearCount();
    encoder2.clearCount();
    encoder1_reached_target = false;
    encoder2_reached_target = false;
  } else if (encoder1.getCount() >= target_count && encoder2.getCount() < target_count) {
    analogWrite(PWM_pin1, 100); // Reduce speed for motor 1
    analogWrite(PWM_pin2, 255); // Maximum speed for motor 2
  } else if (encoder1.getCount() < target_count && encoder2.getCount() >= target_count) {
    analogWrite(PWM_pin1, 255); // Maximum speed for motor 1
    analogWrite(PWM_pin2, 100); // Reduce speed for motor 2
  } else {
    analogWrite(PWM_pin1, 150); // Default speed for motor 1
    analogWrite(PWM_pin2, 150); // Default speed for motor 2
  }
}

unsigned long start_time = 0;
void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 2000) {
    lastMillis = millis();
    client.publish(mqtt_topic, "Counter = " + String(counter++));
    
    // Control AGV movement based on MQTT message
    if (i == "for") {
      // Code for moving forward
      digitalWrite(ina_pin1, HIGH);
      digitalWrite(inb_pin1, LOW);
      digitalWrite(ina_pin2, HIGH);
      digitalWrite(inb_pin2, LOW);
      start_time = millis(); // เริ่มต้นเวลาการเคลื่อนที่ "for"
    } else if (i == "rig") {
      // Code for turning right
      digitalWrite(ina_pin1, LOW);
      digitalWrite(inb_pin1, HIGH);
      digitalWrite(ina_pin2, HIGH);
      digitalWrite(inb_pin2, LOW);
      // Don't use encoder counts, so reset them
      encoder1.clearCount();
      encoder2.clearCount();
      start_time = millis(); // เริ่มต้นเวลาการเลี้ยวขวา
    } else if (i == "lef") {
      // Code for turning left
      digitalWrite(ina_pin1, HIGH);
      digitalWrite(inb_pin1, LOW);
      digitalWrite(ina_pin2, LOW);
      digitalWrite(inb_pin2, HIGH);
      encoder1.clearCount();
      encoder2.clearCount();
    } else if (i == "bac") {
      // Code for moving backward
      digitalWrite(ina_pin1, LOW);
      digitalWrite(inb_pin1, HIGH);
      digitalWrite(ina_pin2, LOW);
      digitalWrite(inb_pin2, HIGH);
    } else if (i == "stop") {
      // Code for stopping
      digitalWrite(ina_pin1, LOW);
      digitalWrite(inb_pin1, LOW);
      digitalWrite(ina_pin2, LOW);
      digitalWrite(inb_pin2, LOW);
    } else if (i == "k1") {
    unsigned long current_time = millis();
      if (current_time - start_time < 5000) {
        // Move forward for 5 seconds
        digitalWrite(ina_pin1, HIGH);
        digitalWrite(inb_pin1, LOW);
        digitalWrite(ina_pin2, HIGH);
        digitalWrite(inb_pin2, LOW);
        controlSpeed(); // เรียกใช้ controlSpeed() เพื่อตั้งค่าความเร็วของมอเตอร์
      } else if (current_time - start_time < 10000) {
        // Turn right for 5 seconds
        digitalWrite(ina_pin1, LOW);
        digitalWrite(inb_pin1, HIGH);
        digitalWrite(ina_pin2, HIGH);
        digitalWrite(inb_pin2, LOW);
      } else {
        // Stop after 10 seconds
        digitalWrite(ina_pin1, LOW);
        digitalWrite(inb_pin1, LOW);
        digitalWrite(ina_pin2, LOW);
        digitalWrite(inb_pin2, LOW);
      } 
    }else {
      // Default: stop
      digitalWrite(ina_pin1, LOW);
      digitalWrite(inb_pin1, LOW);
      digitalWrite(ina_pin2, LOW);
      digitalWrite(inb_pin2, LOW);
    }

    controlSpeed();
  }
}
