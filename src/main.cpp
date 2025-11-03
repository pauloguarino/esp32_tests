#include <Arduino.h>

#define ESP32_LED 2
#define YELLOW_LED 23
#define BUTTON 36
#define MS 1000
#define ESP32_LED_T 2*MS
#define YELLOW_LED_T 500*MS
#define DEBOUNCE_TIME 250
#define PRESCALER_1MHZ 80
#define SERIAL_MONITOR_BAUDRATE 115200

// put function declarations here:
void toggle_led_isr();
void timer_isr();

volatile bool esp32_led_state = false;
volatile unsigned long button_press_time = 0;
volatile unsigned long last_button_press_time = 0;
hw_timer_t *yellow_led_timer = NULL;

void setup() {
  // put your setup code here, to run once:
  pinMode(ESP32_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(SERIAL_MONITOR_BAUDRATE);
  attachInterrupt(BUTTON, toggle_led_isr, FALLING);
  yellow_led_timer = timerBegin(0, PRESCALER_1MHZ, true); // número de timer, prescaler (80 MHz), ascendente o descendente
  timerAttachInterrupt(yellow_led_timer, timer_isr, true);
  timerAlarmWrite(yellow_led_timer, 500*MS, true); // timer, final del contador, reload
  timerAlarmEnable(yellow_led_timer);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(esp32_led_state){
    esp32_led_state = false;
    digitalWrite(ESP32_LED, LOW);
    Serial.println("Low");
  }else{
    esp32_led_state = true;
    digitalWrite(ESP32_LED, HIGH);
    Serial.println("High");
  }
  delay(ESP32_LED_T);
}

void IRAM_ATTR toggle_led_isr(){
  button_press_time = millis();
  if(button_press_time - last_button_press_time > DEBOUNCE_TIME){
    if(timerAlarmEnabled(yellow_led_timer)){
      timerAlarmDisable(yellow_led_timer);
    }else{
      timerAlarmEnable(yellow_led_timer);
    }
    if(esp32_led_state){
      esp32_led_state = false;
      digitalWrite(ESP32_LED, LOW);
    }else{
      esp32_led_state = true;
      digitalWrite(ESP32_LED, HIGH);
    }
    last_button_press_time = button_press_time;
  }
}

void IRAM_ATTR timer_isr(){
  digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
}