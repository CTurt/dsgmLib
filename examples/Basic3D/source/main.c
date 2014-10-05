#include "DSGM.h"

void DSGM_Init(void);

int main(int argc, char **argv) {
	DSGM_Init();
	
	DSGM_LoadRoom(&DSGM_Rooms[DSGM_currentRoom]);
	
	while(1) {
		DSGM_LoopRoom(&DSGM_Rooms[DSGM_currentRoom]);
		
		DSGM_Update();
	}
	
	return 0;
}
