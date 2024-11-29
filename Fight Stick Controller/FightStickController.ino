#include <XInput.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, 1, NEO_GRB + NEO_KHZ800);
#define KeyCount  16
#define Brightness 128

int lastButtonState[KeyCount];
int buttonMapMain[KeyCount] = {A5, A4, A3,  A1, 13, 5, 9, 6, 10, 8, A0,                              12, 4, 3, 2, A2}; //30
int buttonMap[KeyCount] = { DPAD_LEFT , DPAD_DOWN , DPAD_RIGHT , BUTTON_X, BUTTON_Y,  BUTTON_RB, BUTTON_LB, TRIGGER_LEFT, TRIGGER_RIGHT, BUTTON_B, BUTTON_A, BUTTON_BACK, BUTTON_START, BUTTON_L3, BUTTON_R3, DPAD_UP};

void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.
  for (int index = 0; index < KeyCount; index++)
  {
    pinMode(buttonMapMain[index], INPUT_PULLUP);
  }
  pinMode(0, OUTPUT);
  XInput.begin();
}

void loop() {

  bool changed = false;
  // Read pin values
  for (int index = 0; index < KeyCount; index++)
  {
    int currentButtonState = !digitalRead(buttonMapMain[index]);
    if (lastButtonState[index] != currentButtonState) {
      if (index > 2 && index != 15) {
        XInput.setButton(buttonMap[index], currentButtonState);
        lastButtonState[index] = currentButtonState;
      } else {
        changed = true;
        lastButtonState[index] = currentButtonState;
      }
    }
  }
  if (changed) {
    XInput.setDpad(lastButtonState[15], lastButtonState[1], lastButtonState[0], lastButtonState[2]);
  }
  int lightStatus = LOW;
  for (int index = 0; index < KeyCount; index++)
  {
    if (lastButtonState[index] == HIGH) {
      lightStatus = HIGH;
    }

    if (index < 11 || index == 15) {
      int lightID = (index != 15 ? index : 11);
      //      int brightness = ;
      pixels.setPixelColor(2 * lightID, pixels.Color((lastButtonState[index] == HIGH ? random(25) : 0), (lastButtonState[index] == HIGH ? random(25) : 0), (lastButtonState[index] == HIGH ? random(25) : 0)));
      pixels.setPixelColor(2 * lightID + 1, pixels.Color((lastButtonState[index] == HIGH ? random(25) : 0), (lastButtonState[index] == HIGH ? random(25) : 0), (lastButtonState[index] == HIGH ? random(25) : 0)));
      pixels.show();
    }
  }
  digitalWrite(0, lightStatus);
  delay(50);
}
