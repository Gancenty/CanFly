#ifndef _CONFIG_H
#define _CONFIG_H
#include "includes.h"

#define CanFly_Controler_Version

#define WIFI_SSID       "CanFly"
#define WIFI_PWD        "20001031"
#define MAX_STA_NUN     5
#define CanFlyPort      88
#define CanFlyConPort   88
#define MAX_RX_TEMP     1024
#define MAX_TX_TEMP     1024
#define MAX_WRX_TEMP     1024
#define MAX_WTX_TEMP     1024

typedef struct CANFLY_CONTROLER
{
    //NET
    float rssi;
    struct ipv4_addr canfly_ip;
    struct ipv4_addr canfly_ctr_ip;
    //USART
    uint8_t rxtemp[MAX_RX_TEMP];
    uint8_t txtemp[MAX_TX_TEMP];
    uint16_t rxlen;
    uint16_t txlen;
    //UDP
    uint8_t wrxtemp[MAX_WRX_TEMP];
    uint8_t wtxtemp[MAX_WTX_TEMP];
    uint16_t wrxlen;
    uint16_t wtxlen;

    float bat_vol;
    float acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z,sta_shock;
    float rol,pit,yaw;

    uint16_t t_rol,t_pit,t_yaw;
    uint16_t c_rol,c_pit,c_yaw,c_thr;
    uint16_t pwm1,pwm2,pwm3,pwm4;
    uint8_t r,g,b,bright;
    uint8_t mode,sflag,cid,cmd0,cmd1;//mode -0姿态自稳 -1自稳和定高 sflag -0锁定 -1解锁

}CANFLY_CONTROLER;
extern CANFLY_CONTROLER canfly_ctr;

#endif
