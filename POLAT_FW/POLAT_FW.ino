#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define SENSOR_SIZE 1

Adafruit_MPR121 cap = Adafruit_MPR121();

long long now = -1;
long long previous = -1;
long long reset_timer = -1;
int diff = 0;

void setup() {
  Serial.begin(115200);

  while (!Serial) {  // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  do {
    Serial.println("Initializing P.O.L.A.T.");
    if (!cap.begin(0x5A, &Wire, 20, 6)) {
      Serial.println("Problem connecting to MPR121!");
      delay(100);
      while (1)
        ;
    }

    cap.writeRegister(MPR121_ECR, 0x88);        // Initial baseline 5-bit of reading, ELE0-ELE7 Enabled

    //cap.writeRegister(MPR121_CONFIG1, 0xD0);

    /* Enable Autoconfig */
    cap.writeRegister(MPR121_AUTOCONFIG0, 0x0B); // 0xCB

    // correct values for Vdd = 3.3V
    cap.writeRegister(MPR121_UPLIMIT, 200);      // ((Vdd - 0.7)/Vdd) * 256
    cap.writeRegister(MPR121_TARGETLIMIT, 180);  // UPLIMIT * 0.9
    cap.writeRegister(MPR121_LOWLIMIT, 130);     // UPLIMIT * 0.65

    cap.writeRegister(MPR121_NHDR, 0x01);
    cap.writeRegister(MPR121_NHDF, 0x01);
    cap.writeRegister(MPR121_MHDR, 0x01);
    cap.writeRegister(MPR121_MHDF, 0x01);
    cap.writeRegister(MPR121_NCLT, 0x06);
    cap.writeRegister(MPR121_FDLR, 0x04);
  } while (cap.touched());

  previous = millis();
}

void loop() {
  now = millis();
  diff = now - previous;

  if (diff < 20) {
    return;
  }
  previous = now;

#ifdef PRINT_RAW_DATA
  Serial.print(cap.filteredData(0));
  Serial.print(",");
  Serial.print(cap.baselineData(0));
  Serial.print(",");
  Serial.print(cap.filteredData(1));
  Serial.print(",");
  Serial.print(cap.baselineData(1));
  Serial.print(",");
  Serial.print(cap.filteredData(2));
  Serial.print(",");
  Serial.print(cap.baselineData(2));
  Serial.print(",");
  Serial.print(cap.filteredData(3));
  Serial.print(",");
  Serial.print(cap.baselineData(3));
  Serial.print(",");
  Serial.print(cap.filteredData(4));
  Serial.print(",");
  Serial.print(cap.baselineData(4));
  Serial.print(",");
  Serial.print(cap.filteredData(5));
  Serial.print(",");
  Serial.print(cap.baselineData(5));
  Serial.print(",");
  Serial.print(cap.filteredData(6));
  Serial.print(",");
  Serial.print(cap.baselineData(6));
  Serial.print(",");
  Serial.print(cap.filteredData(7));
  Serial.print(",");
  Serial.print(cap.baselineData(7));
  Serial.print(":");
  Serial.print(cap.touched());
  Serial.print("\n");
#endif
  uint16_t touched = cap.touched();
  uint8_t region_count = 0;
  int32_t x = 0;
  int32_t y = 0;

  if (touched & 0b10000000)  // Region 0
  {
    region_count++;
    x += -32768;
    y += 32768;
  }
  if (touched & 0b00000001)  // Region 1
  {
    region_count++;
    x += 32768;
    y += 32768;
  }
  if (touched & 0b00000100)  // Region 2
  {
    region_count++;
    x += 32768;
    y += -32768;
  }
  if (touched & 0b00100000)  // Region 3
  {
    region_count++;
    x += -32768;
    y += -32768;
  }
  if (touched & 0b01000000)  // Region 4
  {
    region_count++;
    x += -19660;
    y += 19660;
  }
  if (touched & 0b00000010)  // Region 5
  {
    region_count++;
    x += 19660;
    y += 19660;
  }
  if (touched & 0b00001000)  // Region 6
  {
    region_count++;
    x += 19660;
    y += -19660;
  }
  if (touched & 0b00010000)  // Region 7
  {
    region_count++;
    x += -19660;
    y += -19660;
  }

  if (region_count > 0) {
    x /= region_count;
    y /= region_count;
  }

  Serial.print("(");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(")");

  previous = now;
}
