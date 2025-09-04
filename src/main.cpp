#include <Arduino.h>

#define LED 2
#define T 500
// put function declarations here:
int myFunction(int, int);

float i = 0;

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  i += 0.1;
  digitalWrite(LED, HIGH);
  Serial.println("High");
  delay(T);
  digitalWrite(LED, LOW);
  Serial.println("Low");
  delay(T);
  Serial.print(">sin:");
  Serial.println(sin(i));
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}