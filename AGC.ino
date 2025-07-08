#include "Setup.h"
#include "CAutoPot.h"

ChipSelectPins CS;
ProbePointPins PP;
ButtonPins     BUT;
LedPins        LED;
Timer          timer;

Hardware HW(&CS, &PP, &BUT, &LED, &timer);

// Parameter order; ChipSelect,ProbePoint,Window,LowThresh,HighThresh
COffsetPot offsetPot1(CS.offset1, PP.primaryOffset,  50, 324, 700);
COffsetPot offsetPot2(CS.offset2, PP.preGain      , 100, 324, 700);
CGainPot   gainPot   (CS.gain,    PP.preGain      ,  10          );

void setup() {
  HW.init();

  offsetPot1.begin(127);
  offsetPot2.begin(127);
  gainPot.begin(127);
  timer.restart();
}

void loop() {
  const float uS_delay = 10000;

  while (timer.uS() < uS_delay);
 
//  float uS0=0, uS1=0, uS2=0, uS3=0;
  timer.restart();
  LED.activity.toggle();

//  uS0 = timer.uS();

  if (BUT.halt.read() != 0) {
    offsetPot1.update();
//  uS1 = timer.uS();
    offsetPot2.update();
//  uS2 = timer.uS();

    if (offsetPot1.inZone) {
      gainPot.update();
    }
  }
//  uS3 = timer.uS();
  
  
//  Serial.printf("microS: %8.2f %8.2f %8.2f %8.2f\n", uS0, uS1, uS2, uS3);

  LED.all.deactivate();
  digitalWrite(33, LOW); // Example: Turn on a specific LED

  // --- Serial Debugging Output ---
  Serial.print("Sensor1:");       Serial.print(offsetPot1.getSensorValue());
  Serial.print("\t offset1:");    Serial.print(offsetPot1.getLevel());
  Serial.print("\t Sensor2:");    Serial.print(offsetPot2.getSensorValue());
  Serial.print("\t offset2:");    Serial.print(offsetPot2.getLevel());
  Serial.print("\t GainSensor:"); Serial.print(gainPot.getSensorValue());
  Serial.print("\t GainLevel:");  Serial.println(gainPot.getLevel());

  delay(1);
}
