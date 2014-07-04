#pragma once

#define DSGM_MAX_SOUNDS 32
#define DSGM_RESERVED_STREAMS 1

#define DSGM_SOUND_STREAM 0
#define DSGM_SOUND_EFFECT 1

#define DSGM_FORM_SOUND_STREAM(sound)\
{ DSGM_SOUND_STREAM, sound }

#define DSGM_FORM_SOUND_EFFECT(sound)\
{ DSGM_SOUND_EFFECT, sound }

typedef struct {
	u8 type;
	int ID;
	bool loaded;
} DSGM_Sound;

typedef struct {
	DSGM_Sound *sound;
	int effectNumber;
	
	u8 volume;
	u8 panning;
} DSGM_SoundInstance;

extern int DSGM_soundStreamCount;

extern DSGM_SoundInstance DSGM_soundInstances[DSGM_MAX_SOUNDS];
extern int DSGM_soundInstanceCount;

void DSGM_InitSoundFull(int soundStreamCount);
void DSGM_ResetSound(void);
DSGM_SoundInstance *DSGM_AddSoundInstance(DSGM_Sound *sound);

DSGM_SoundInstance *DSGM_PlaySoundFull(DSGM_Sound *sound);
DSGM_SoundInstance *DSGM_PlaySoundAdvancedFull(DSGM_Sound *sound, u8 volume, u8 panning);
void DSGM_SetSoundInstanceVolumeFull(DSGM_SoundInstance *soundInstance, int volume);
void DSGM_SetSoundInstancePanningFull(DSGM_SoundInstance *soundInstance, int panning);
