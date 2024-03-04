#include "includes.h"
CANFLY_CONTROLER canfly_ctr;
WiFiUDP Udp;
void setup()
{
  LED_Config();
  Serial.setRxBufferSize(1024);
  Serial.begin(921600);
  WiFi.mode(WIFI_AP);
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.softAP(WIFI_SSID, WIFI_PWD);
  while (WiFi.softAPgetStationNum() == 0)
  {
    LED_Blink();
  }
  struct station_info *info;
  info = wifi_softap_get_station_info();
  canfly_ctr.canfly_ip = info->ip;
  wifi_softap_free_station_info();
  Udp.begin(CanFlyConPort);
}

void loop()
{
  while (WiFi.softAPgetStationNum() == 0){
    LED_Blink();
  }
  if (Serial.available())
  {
    canfly_ctr.rxlen = Serial.read(canfly_ctr.rxtemp, MAX_RX_TEMP);
    if(CanFly_Msg_Valid()){
      Udp.beginPacket(IPAddress(canfly_ctr.canfly_ip), CanFlyPort);
      Udp.write(canfly_ctr.wtxtemp, canfly_ctr.wtxlen);
      Udp.endPacket();
      canfly_ctr.wtxlen=0;
    }

  }
  if (Udp.parsePacket())
  {
    canfly_ctr.wrxlen = Udp.read(canfly_ctr.wrxtemp, MAX_WRX_TEMP);
    Serial.write(canfly_ctr.wrxtemp,canfly_ctr.wrxlen);
  }
  delay(1);
}