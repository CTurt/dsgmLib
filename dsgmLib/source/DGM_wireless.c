#include "DSGM.h"

char DSGM_buffer[4096];
size_t DSGM_wirelessDataLength = 0;
bool DSGM_newWirelessData = false;

void DSGM_WirelessHandler(int packetID, int readlength) {
	Wifi_RxRawReadPacket(packetID, readlength, (unsigned short *)DSGM_buffer);
	
	DSGM_wirelessDataLength = readlength - 32;
	DSGM_newWirelessData = true;
}

void DSGM_InitNiFi(void) {
	DSGM_Debug("NiFi initiating\n");
	
	setWirelessMode(WIRELESS_MODE_NIFI);
	
	Wifi_InitDefault(false);
	
	Wifi_SetPromiscuousMode(1);
	
	Wifi_EnableWifi();
	
	Wifi_RawSetPacketHandler(DSGM_WirelessHandler);
	
	Wifi_SetChannel(10);
	
	DSGM_Debug("NiFi initiated\n");
}

void DSGM_SendWirelessData(unsigned short *buffer, int length) {
	if(Wifi_RawTxFrame(length, 0x0014, buffer) != 0) DSGM_Debug("Error calling RawTxFrame\n");
}
