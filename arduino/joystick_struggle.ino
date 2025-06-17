#include <Adafruit_CircuitPlayground.h>
#include <math.h>

// Define joystick pins
#define VRX_PIN A1
#define VRY_PIN A2
#define SW_PIN A3

void setup() {
  CircuitPlayground.begin();
}

double realMagnitude(float angle, float radius) {
  
}

void loop() {
  CircuitPlayground.clearPixels();
  // Read joystick values
  int x = analogRead(VRX_PIN) - 510;
  int y = -analogRead(VRY_PIN) + 510;
  int sw = analogRead(SW_PIN);
  float angle;
  if (atan2(y, x) < 0) {
    angle = M_PI + (M_PI + atan2(y, x));
  } else {
    angle = atan2(y, x);
  }
  int led = 9 - round(angle/(2 * M_PI) * 10 - 1);
  float magnitude = sqrt(pow(x, 2) + pow(y, 2)) / 510;


  // Print joystick values to serial monitor
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(", SW: ");
  Serial.print(sw);
  Serial.print(", angle: ");
  Serial.print(angle);
  Serial.print(" magnitude: ");
  Serial.print(magnitude);
  Serial.print(" led: ");
  Serial.println(led);

  // Control LEDs based on joystick position
  
  CircuitPlayground.setPixelColor(led, 0, round(255*magnitude), 0);
  delay(100);

}