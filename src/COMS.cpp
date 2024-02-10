#include "variables.h"
#include <WebSerial.h>
#include <HardwareSerial.h>


//HardwareSerial Serial2(2); // Use hardware serial 2 for the second connection




// void logData() {
//   yield();
//   WebSerial.print(gyroXdata);
//   WebSerial.print(",");
//   WebSerial.print(gyroYdata);
//   WebSerial.print(",");
//   WebSerial.print(gyroZdata);
//   WebSerial.print(",");
//   WebSerial.print(accXdata);
//   WebSerial.print(",");
//   WebSerial.print(accYdata);
//   WebSerial.print(",");
//   WebSerial.println(accZdata);
//   yield();
// }

void logData() {
  yield();
  Serial2.print(gyroXdata);
  Serial2.print(",");
  Serial2.print(gyroYdata);
  Serial2.print(",");
  Serial2.print(gyroZdata);
  Serial2.print(",");
  Serial2.print(accXdata);
  Serial2.print(",");
  Serial2.print(accYdata);
  Serial2.print(",");
  Serial2.println(accZdata);
  yield();
}



void sendStatus() {
    yield();
    WebSerial.print(" s");
    WebSerial.println(standing);
    WebSerial.print(" p");
    WebSerial.println(power);
    WebSerial.print(" a");
    WebSerial.println(varAng);
    yield();

    WebSerial.print(String(millis() - time0)+'\n');
}


