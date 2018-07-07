#include "simplefsk.h"

FSK fsk(7);

String payload_str = "ABC";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  char buf[24] = {0};
  payload_str.toCharArray(buf, 3);
  fsk.set_buffer(buf, 3);
}

void loop() {
fsk.process();
//delayMicroseconds(10);
}
