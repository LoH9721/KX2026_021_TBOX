/*******************************************************
  * Name    :ModemAudio.h
  * Author  :LoH
  * Function:AC7600-controlled AW87559 receiver audio interface.
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#ifndef __MODEM_AUDIO_H__
#define __MODEM_AUDIO_H__

#include <stdint.h>

/* Reset the private U22 receiver configuration runtime and status. */
void ModemAudio_Init(void);
void ModemAudio_Reset(void);
/* Request one verified 21 dB external-speaker configuration before a voice call. */
uint8_t ModemAudio_RequestSpeakerEnable(void);
/* Request U22 external-speaker shutdown after one terminal voice-call result. */
uint8_t ModemAudio_RequestSpeakerOff(void);
/* Process the private AC7600 I2C AT transaction sequence. */
void ModemAudio_Process(void);
/* Return whether U22 external-speaker mode and 21 dB gain have been verified. */
uint8_t ModemAudio_IsSpeakerReady(void);
/* Return whether U22 external-speaker shutdown completed or was not required. */
uint8_t ModemAudio_IsSpeakerOff(void);
/* Return whether the latest requested U22 configuration failed. */
uint8_t ModemAudio_HasFailure(void);

#endif
