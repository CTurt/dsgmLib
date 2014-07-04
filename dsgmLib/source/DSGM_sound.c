#include "DSGM.h"

int DSGM_soundStreamCount = 0;

DSGM_SoundInstance DSGM_soundInstances[DSGM_MAX_SOUNDS];
int DSGM_soundInstanceCount = 0;

void DSGM_InitSoundFull(int soundStreamCount) {
	mmInitDefault("nitro:/soundbank.bin");
	DSGM_soundStreamCount = soundStreamCount;
}

void DSGM_ResetSound(void) {
	/*mmStop();
	mmEffectCancelAll();
	
	int i;
	for(i = 0; i < DSGM_soundInstanceCount; i++) {
		if(DSGM_soundInstances[i].sound->type == DSGM_SOUND_STREAM) {
			// MaxMod is broken :(
			//mmUnload(DSGM_soundInstances[i].sound->ID);
			//DSGM_soundInstances[i].sound->loaded = false;
		}
		else {
			//mmUnloadEffect(DSGM_soundInstances[i].sound->ID - DSGM_soundStreamCount);
			//DSGM_soundInstances[i].sound->loaded = false;
		}
	}
	
	DSGM_soundInstanceCount = 0;*/
}

DSGM_SoundInstance *DSGM_AddSoundInstance(DSGM_Sound *sound) {
	// still playing?
	if(DSGM_soundInstanceCount == DSGM_MAX_SOUNDS) DSGM_soundInstanceCount = sound->type == DSGM_SOUND_STREAM ? 0 : DSGM_RESERVED_STREAMS;
	return &DSGM_soundInstances[DSGM_soundInstanceCount++];
}

DSGM_SoundInstance *DSGM_PlaySoundFull(DSGM_Sound *sound) {
	DSGM_SoundInstance *soundInstance = DSGM_AddSoundInstance(sound);
	
	if(sound->type == DSGM_SOUND_STREAM) {
		if(!sound->loaded) {
			mmLoad(sound->ID);
			sound->loaded = true;
		}
		mmStart(sound->ID, MM_PLAY_LOOP);
	}
	else {
		if(!sound->loaded) {
			mmLoadEffect(sound->ID - DSGM_soundStreamCount);
			sound->loaded = true;
		}
		soundInstance->effectNumber = mmEffect(sound->ID - DSGM_soundStreamCount);
		mmEffectRelease(soundInstance->effectNumber);
	}
	
	soundInstance->volume = 255;
	soundInstance->panning = 128;
	
	return soundInstance;
}

DSGM_SoundInstance *DSGM_PlaySoundAdvancedFull(DSGM_Sound *sound, u8 volume, u8 panning) {
	DSGM_SoundInstance *soundInstance = DSGM_AddSoundInstance(sound);
	
	if(sound->type == DSGM_SOUND_STREAM) {
		if(!sound->loaded) {
			mmLoad(sound->ID);
			sound->loaded = true;
		}
		mmStart(sound->ID, MM_PLAY_LOOP);
	}
	else {
		if(!sound->loaded) {
			mmLoadEffect(sound->ID - DSGM_soundStreamCount);
			sound->loaded = true;
		}
		mm_sound_effect effect = {
			{ sound->ID - DSGM_soundStreamCount } ,
			(int)(1.0f * (1<<10)),					// rate
			0,										// handle
			soundInstance->volume = volume,			// volume (255 = max)
			soundInstance->panning = panning,		// panning (128 = center)
		};
		soundInstance->effectNumber = mmEffectEx(&effect);
		mmEffectRelease(soundInstance->effectNumber);
	}
	
	return soundInstance;
}

void DSGM_SetSoundInstanceVolumeFull(DSGM_SoundInstance *soundInstance, int volume) {
	if(soundInstance->sound->type == DSGM_SOUND_STREAM) mmSetModuleVolume(volume);
	if(soundInstance->sound->type == DSGM_SOUND_EFFECT) mmEffectVolume(soundInstance->effectNumber, volume);
}

void DSGM_SetSoundInstancePanningFull(DSGM_SoundInstance *soundInstance, int panning) {
	if(soundInstance->sound->type == DSGM_SOUND_EFFECT) mmEffectPanning(soundInstance->effectNumber, panning);
}
