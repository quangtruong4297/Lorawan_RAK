#include <Arduino.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <ArduinoJson.h>

#define VM_serial Serial1
#define TIME_HANDLE     50
#define MAX_BUFFER_SIZE 50

#define DEBUG 1
//-----------------LORA----------------
#define OTAA_PERIOD   (20000)
#define OTAA_BAND     (RAK_REGION_AS923)
#define OTAA_DEVEUI   {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x88}
#define OTAA_APPEUI   {0x80, 0xe2, 0xac, 0x6d, 0xc5, 0x5a, 0xda, 0xc5}
#define OTAA_APPKEY   {0xd8, 0x73, 0x0d, 0x44, 0x17, 0x3c, 0xac, 0xb1, 0x55, 0x1a, 0x2a, 0x10, 0x3e, 0x4b, 0x39, 0xa8}
uint8_t collected_data[64] = { 0 };
/*************************************
   enum type for LoRa Event
    RAK_LORAMAC_STATUS_OK = 0,
    RAK_LORAMAC_STATUS_ERROR,
    RAK_LORAMAC_STATUS_TX_TIMEOUT,
    RAK_LORAMAC_STATUS_RX1_TIMEOUT,
    RAK_LORAMAC_STATUS_RX2_TIMEOUT,
    RAK_LORAMAC_STATUS_RX1_ERROR,
    RAK_LORAMAC_STATUS_RX2_ERROR,
    RAK_LORAMAC_STATUS_JOIN_FAIL,
    RAK_LORAMAC_STATUS_DOWNLINK_REPEATED,
    RAK_LORAMAC_STATUS_TX_DR_PAYLOAD_SIZE_ERROR,
    RAK_LORAMAC_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,
    RAK_LORAMAC_STATUS_ADDRESS_FAIL,
    RAK_LORAMAC_STATUS_MIC_FAIL,
    RAK_LORAMAC_STATUS_MULTICAST_FAIL,
    RAK_LORAMAC_STATUS_BEACON_LOCKED,
    RAK_LORAMAC_STATUS_BEACON_LOST,
    RAK_LORAMAC_STATUS_BEACON_NOT_FOUND,
 *************************************/
//-------------------------------------
//-----------------time----------------
#define step_50ms             100
#define step_250ms            250
#define step_1s               1
#define step_2s               2
#define step_10s              10
#define step_30s              30
#define step_15s              15
#define step_1m               60
#define step_90s              90
#define step_5m               60*5
#define step_60m              60*60

unsigned long time_50msecond = 0;
unsigned long time_250msecond = 0;
unsigned long time_1second = 0;
unsigned long time_2second = 0;
unsigned long time_10second = 0;
unsigned long time_30second = 0;
unsigned long time_1minute = 0;
unsigned long time_90second = 0;
unsigned long time_5minute = 0;
unsigned long time_60minute = 0;
//----------------end time------------
struct US {
  int freq;
  int temp;
};


US value_US[4];
void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
#if DEBUG
  Serial.begin(9600);
#endif
  lora_setup();
  setup_VM();
  SetNextTimeInterval(time_10second, step_10s * 1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  wait_for_serial_VM_serial();
  if (TimeReached(time_10second)) {
    SetNextTimeInterval(time_10second, step_10s * 1000);
    RTU_Send_Request("modbus", 1, 3, 51, 8, 1);
    uplink_routine();
#if DEBUG
    for (int i = 0; i < 4; i++) {
      Serial.println("value_US[0].freq: " + String(value_US[0].freq));
      Serial.println("value_US[0].temp: " + String(value_US[0].temp));
    }
#endif
  }
//  static uint64_t last = 0;
//  static uint64_t elapsed;
//
//  if ((elapsed = millis() - last) > OTAA_PERIOD) {
//    uplink_routine();
//
//    last = millis();
//  }
  //Serial.printf("Try sleep %ums..", OTAA_PERIOD);
//  api.system.sleep.all(OTAA_PERIOD);
  //Serial.println("Wakeup..");

}
