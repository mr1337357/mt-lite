#include "serial_interface.h"
#include "nvdata.h"
#include "picopb.h"

// include the library
#include <RadioLib.h>

//#define NUGGET_CONNECT

#if defined(WIFI_LORA_32_V3)
#include "boards/heltec_v3.h"
#elif defined(WIFI_LORA_32_V4)
#include "boards/heltec_v4.h"
#elif defined(NUGGET_CONNECT)
#include "boards/nugget_connect.h"
#else
#error unrecognized board
#endif


// flag to indicate that a packet was received
volatile bool receivedFlag = false;
volatile bool transmitFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif

void radioEvent(void)
{
  transmitFlag = true;
}

float freq = 906.875; //frequency
float bw = 250.0; //bandwidth
uint8_t sf = 11; //spread factor
uint8_t cr = 5; //coding rate
uint8_t syncWord=0x2B;
int8_t power=22;
uint16_t pl=16;

void mt_send(uint8_t *buff, int len)
{
  int16_t status = radio.startTransmit(buff,len);
}

void setup() {
  int len;
  Serial.begin(115200);
  delay(1000);
  Serial.printf("System init\n");
  board_init();
  serial_init();
  pinMode(35,OUTPUT);
  nvdata.init();
  len = 4;
  if(nvdata.get("freq",(uint8_t *)&freq,&len)<0)
  {
    nvdata.set("freq",(uint8_t *)&freq,4);
    nvdata.set("bw",(uint8_t *)&bw,4);
    nvdata.set("sf",(uint8_t *)&sf,1);
    nvdata.set("cr",(uint8_t *)&cr,1);
    nvdata.set("syncword",(uint8_t *)&syncWord,1);
    nvdata.set("power",(uint8_t *)&power,1);
    nvdata.set("pl",(uint8_t *)&pl,1);
    nvdata.save();
  }
  else
  {
    len = 4;
    nvdata.get("bw",(uint8_t *)&bw,&len);
    len = 1;
    nvdata.get("sf",(uint8_t *)&sf,&len);
    len = 1;
    nvdata.get("cr",(uint8_t *)&cr,&len);
    len = 1;
    nvdata.get("syncword",(uint8_t *)&syncWord,&len);
    len = 1;
    nvdata.get("power",(uint8_t *)&power,&len);
    len = 1;
    nvdata.get("pl",(uint8_t *)&pl,&len);
  }

  Serial.printf("Radio Init\n");
  int state = radio.begin(freq, bw, sf, cr, syncWord, power, pl, 1.6, false);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  #if defined(WIFI_LORA_32_V4)
  radio.setDio2AsRfSwitch(true);
  #endif

  // set the function that will be called
  // when new packet is received
  radio.setPacketReceivedAction(radioEvent);
  radio.setPacketSentAction(radioEvent);

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
uint64_t led_off_time = 0;

void led_on(int ms)
{
  digitalWrite(35,HIGH);
  led_off_time = millis() + ms;
}

void loop() {
  int i;
  serial_update();
  if(millis() >= led_off_time)
  {
    Serial.printf("tick\n");
    led_off_time += 1000;
    digitalWrite(35,LOW);
  }
  if(transmitFlag)
  {
    int state;
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while (true) { delay(10); }
    }
    transmitFlag = false;
    int len = radio.getPacketLength(true);
    if(len > 0)
    {
      receivedFlag = true;
    }
  }
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
      led_on(200);
      picopb pb(pb_buff,512);
      pb.write_varint(1,1);
      pb.write_string(2,readBuff,len);
      float rssi = radio.getRSSI(true);
      pb.write_i32(3,rssi);
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
