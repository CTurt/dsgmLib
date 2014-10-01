#pragma once

#ifndef WIRELESS_DEFINES
enum WIRELESS_MODE {
  WIRELESS_MODE_WIFI,
  WIRELESS_MODE_NIFI,
};
extern enum WIRELESS_MODE wirelessMode;
inline void setWirelessMode(enum WIRELESS_MODE mode);
#define WIRELESS_DEFINES
#endif

#define DSGM_wirelessData (DSGM_buffer + 32)

extern char DSGM_buffer[4096];
extern size_t DSGM_wirelessDataLength;
extern bool DSGM_newWirelessData;

void DSGM_WirelessHandler(int packetID, int readlength);
void DSGM_InitNiFi(void);
#define DSGM_DisableWireless() Wifi_DisableWifi()

void DSGM_SendWirelessData(unsigned short *buffer, int length);
