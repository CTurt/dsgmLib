#pragma once

#define DSGM_wirelessData (DSGM_buffer + 32)

extern char DSGM_buffer[4096];
extern size_t DSGM_wirelessDataLength;
extern bool DSGM_newWirelessData;

void DSGM_WirelessHandler(int packetID, int readlength);
void DSGM_InitNiFi(void);
#define DSGM_DisableWireless() Wifi_DisableWifi()

void DSGM_SendWirelessData(unsigned short *buffer, int length);
