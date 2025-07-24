#include "serial_interface.h"
#include "nvdata.h"
#include "picopb.h"

// include the library
#include <RadioLib.h>

//#define NUGGET_CONNECT

#if defined(WIFI_LORA_32_V3)
#include "boards/heltec_v3.h"
#elif defined(NUGGET_CONNECT)
#include "boards/nugget_connect.h"
#else
#error unrecognized board
#endif


// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we got a packet, set the flag
  receivedFlag = true;
}

float freq = 906.875;

void setup() {
  int len;
  Serial.begin(115200);
  delay(1000);
  Serial.printf("System init\n");
  board_init();
  serial_init();

  nvdata.init();
  len = 4;
  if(nvdata.get("freq",(uint8_t *)&freq,&len)<0)
  {
    nvdata.set("freq",(uint8_t *)&freq,4);
    nvdata.save();
  }

  Serial.printf("Radio Init\n");
  int state = radio.begin(freq, 250.0, 11, 5, 0x2B, 22, 16, 1.6, false);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // set the function that will be called
  // when new packet is received
  radio.setPacketReceivedAction(setFlag);

  // start listening for LoRa packets
  Serial.print(F("[SX1262] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }
}

void loop() {
  int i;
  serial_update();
  // check if the flag is set
  if(receivedFlag) {
    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    uint8_t readBuff[256];
    uint8_t pb_buff[512];
    uint8_t len = radio.getPacketLength(true);
    for(i=0;i<256;i++)
    {
      readBuff[i] = 0;
    }
    int state = radio.readData(readBuff,len);

    if (state == RADIOLIB_ERR_NONE) {
      picopb pb(pb_buff,512);
      pb.write_varint(1,1);
      pb.write_string(2,readBuff,len);
      serial_send(pb_buff,pb.get_length());

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);

    }
  }
}
