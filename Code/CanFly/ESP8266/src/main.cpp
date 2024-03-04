#include "includes.h"
CANFLY canfly;
WiFiUDP Udp;
void setup() {
  LED_Config();
  Serial.begin(921600);
  WiFi.mode(WIFI_STA);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status()!=WL_CONNECTED)
  {
    LED_Blink();
  }
  Udp.begin(CanFlyPort);
}

void loop() {
  while (WiFi.status() != WL_CONNECTED){
    LED_Blink();
  }
  if (Serial.available())
  {
    canfly.rxlen = Serial.read(canfly.rxtemp, MAX_RX_TEMP);
    if(CanFly_Msg_Valid()){
      Udp.beginPacket(WiFi.dnsIP(), CanFlyPort);
      Udp.write(canfly.wtxtemp, canfly.wtxlen);
      Udp.endPacket();
      canfly.wtxlen=0;
    }

  }
  if (Udp.parsePacket())
  {
    canfly.wrxlen = Udp.read(canfly.wrxtemp, MAX_WRX_TEMP);
    Serial.write(canfly.wrxtemp,canfly.wrxlen);
  }
  delay(1);
}