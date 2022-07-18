/* 
 * File:   DFPlayer.h
 * Author: tommy
 *
 * Created on 17 June 2019, 8:15 PM
 */

#ifndef DFPLAYER_H
#define	DFPLAYER_H

#include "stm32l4xx_hal.h"

//The UART port
extern UART_HandleTypeDef huart1;

//Some command definitions
#define SPECIFY_TRACK               0x03
#define SPECIFY_VOLUME              0x06
#define PLAYBACK_MODE               0x08
#define PLAYBACK_MODE_REPEAT        0
#define PLAYBACK_MODE_FOLDER_REPEAT 1
#define PLAYBACK_MODE_SINGLE_REPEAT 2
#define PLAYBACK_MODE_RANDOM        3
#define PLAY                        0x0D
#define RESUME_PLAY                 0x03
#define PAUSE                       0x0E
#define NEXT                        0x01
#define PREVIOUS                    0x02
#define SPECIFY_FOLDER              0x0F
#define QUERY_STATUS                0x42
#define QUERY_TRACKS_IN_FOLDER      0x4E
#define QUERY_STORAGE               0x3F
#define RESET                       0x0C
#define MP3_FOLDER                  0x12
#define REPEAT_FOLDER               0x17

void DFPlayer_sendByte(unsigned char data);
unsigned char DFPlayer_getByte();
unsigned char DFPlayer_getWaitingByte();
unsigned int DFPlayer_calculateChecksum(unsigned char command, unsigned char dataH, unsigned char dataL);
void DFPlayer_sendCommand(unsigned char command, unsigned char dataH, unsigned char dataL);
void DFPlayer_getResponse(unsigned char command, unsigned char dataH, unsigned char dataL, unsigned char buff[]);
void DFPlayer_playNext();
void DFPlayer_playPrevious();
void DFPlayer_setVolume(int volume);
void DFPlayer_setLoopMode(unsigned char track_num);
void DFPlayer_setPlay();
void DFPlayer_setPause();
void DFPlayer_playTrack(unsigned char folder_num, unsigned char track_num);
void DFPlayer_repeatFolder(unsigned char folder_number);
int DFPlayer_getStatus();
int DFPlayer_getStorageStatus();
int DFPlayer_getTracksInFolder(int folder);
void DFPlayer_resetModule();


#endif	/* DFPLAYER_H */

