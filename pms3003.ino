/*
 This example demonstrate how to read pm2.5 value on PMS 3003 air condition sensor

 PMS 3003 pin map is as follow:
    PIN1  :VCC, connect to 5V
    PIN2  :GND
    PIN3  :SET, 0:Standby mode, 1:operating mode
    PIN4  :RXD :Serial RX
    PIN5  :TXD :Serial TX
    PIN6  :RESET
    PIN7  :NC
    PIN8  :NC

 In this example, we only use Serial to get PM 2.5 value.

 The circuit:
 * RX is digital pin 6 (connect to TX of PMS 3003)
 * TX is digital pin 7 (connect to RX of PMS 3003)
 * SET is digital pin 5 (connect to SET of PMS 3003)

 */

#include <SoftwareSerial.h>

#define PMS_PIN_SET D5
#define PMS_PIN_RX D7
#define PMS_PIN_TX D6
#define PMS_BAUDRATE 9600

SoftwareSerial pms(PMS_PIN_RX, PMS_PIN_TX); // Initialize serial communiation with PMS 3003

#define pmsDataLen 22
uint8_t buf[pmsDataLen];
int idx = 0;
int pm10 = 0;
int pm25 = 0;
int pm100 = 0;

void setup() {
  Serial.begin(115200);
  pms.begin(PMS_BAUDRATE); // PMS 3003 UART has baud rate 9600
}

void loop() { // run over and over
  uint8_t c = 0;
  idx = 0;
  memset(buf, 0, pmsDataLen);

  while (true) {
    while (c != 0x42) {
      while (!pms.available());
      c = pms.read();
    }
    while (!pms.available());
    c = pms.read();
    if (c == 0x4d) {
      // now we got a correct header)
      buf[idx++] = 0x42;
      buf[idx++] = 0x4d;
      break;
    }
  }

  while (idx != pmsDataLen) {
    while(!pms.available());
    buf[idx++] = pms.read();
  }

  pm10 = ( buf[10] << 8 ) | buf[11];
  pm25 = ( buf[12] << 8 ) | buf[13];
  pm100 = ( buf[14] << 8 ) | buf[15];

  Serial.print("pm2.5: ");
  Serial.print(pm25);
  Serial.print(" ug/m3   ");
  Serial.print("pm10: ");
  Serial.print(pm10);
  Serial.print(" ug/m3   ");
  Serial.print("pm100: ");
  Serial.print(pm100);
  Serial.println(" ug/m3");

}
