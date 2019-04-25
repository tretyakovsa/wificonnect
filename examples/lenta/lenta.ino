#include <effects.h>
EFFECTS lenta;
EFFECTS lenta1;
void setup() {
  lenta.init(5, 15);
  lenta1.init(4, 15);
  lenta.setColor(0xFF0000);
  lenta1.setColor(0x0000FF);
  lenta.setBrightness(128);
  lenta1.setBrightness(255);
  lenta.setSpeed(128);
  lenta1.setSpeed(128);
  lenta.setMode(0);
  lenta1.setMode(14);
  lenta.start();
  lenta1.start();
}

void loop() {
  lenta.service();
  lenta1.service();
}
