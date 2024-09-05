#include <ESP32Encoder.h>

ESP32Encoder encoder1; // ตัวแปรสำหรับ Encoder แรก
ESP32Encoder encoder2; // ตัวแปรสำหรับ Encoder ที่สอง

void setup() {
  Serial.begin(115200);

  // กำหนดการใช้งาน pull-up resistors
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  // Attach Encoder 1 ที่ขา 27 และ 26
  encoder1.attachHalfQuad(27, 26);
  // Attach Encoder 2 ที่ขา 35 และ 34
  encoder2.attachHalfQuad(35, 34);

  // เริ่มต้นค่าตั้งต้นของ Encoder 1
  encoder1.clearCount();
  // เริ่มต้นค่าตั้งต้นของ Encoder 2
  encoder2.clearCount();

  Serial.println("Encoder Start = " + String((int32_t)encoder1.getCount()) + " " + String((int32_t)encoder2.getCount()));
}

void loop() {
  // อ่านค่าจาก Encoder ทั้งสอง
  int32_t count1 = encoder1.getCount();
  int32_t count2 = encoder2.getCount();

  // แสดงผลลัพธ์
  Serial.print("Encoder counts: ");
  Serial.print(count1);
  Serial.print(" ");
  Serial.println(count2);

  delay(100);
}
