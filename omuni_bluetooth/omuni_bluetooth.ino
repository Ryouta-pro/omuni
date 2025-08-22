#include <PS4Controller.h>
#include <cstdint>

void setup() {
  Serial.begin(115200);
  PS4.begin();
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    if (PS4.LStickX()) {
        int8_t num1 = PS4.LStickX();
        String str1 = String(static_cast<int>(num1));
        Serial.print(str1+"\n");
    }
    if (PS4.LStickY()) {
        int8_t num2 = PS4.LStickY();
        String str2 = String(static_cast<int>(num2));
        Serial.print(str2+"\n");
    }
    if (PS4.RStickX()) {
        int8_t num3 = PS4.LStickY();
        String str3 = String(static_cast<int>(num3));
        Serial.print(str3+"\n");
    }
    delay(1000);
  }
}
