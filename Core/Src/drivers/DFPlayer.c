/*
 * A library file of functions for controlling the DFPlayer Mini
 * MP3 player module.
 * Uses the HAL STM32 libraries to communicate over UART.
 * 
 * File:   DFPlayer.c
 * Author: tommy
 *
 * Created on 17 June 2019, 8:14 PM
 */


#include "drivers/DFPlayer.h"

/**
 * Send a byte to the UART with flow control.
 */
void DFPlayer_sendByte(unsigned char data) {
    HAL_UART_Transmit(&huart1, &data, 1, 100);
}

/**
 * Receive a byte from the UART buffer
 */
unsigned char DFPlayer_getByte() {
    //Do this in a loop in case the data never arrives
    unsigned int timeout = 500;
    unsigned char ret_byte = 0;
    while(timeout--) {
        if(HAL_UART_Receive(&huart1, &ret_byte, 1, 100) == HAL_OK) {
            return ret_byte;
        }
    }
    
    return 0;
}

/**
 * Checks if there is a byte waiting in the UART receive register.
 * This can be sent from the DFPlayer when something changes, like a
 * song finished playing. 
 * Returns 0x00 if nothing was found. Does not wait for a response.
 */
unsigned char DFPlayer_getWaitingByte() {
	unsigned char ret_byte = 0;
    if(HAL_UART_Receive(&huart1, &ret_byte, 1, 100))
        return ret_byte;
    else
        return 0x00;
}

/**
 * Calculates the checksum value for a normal command.
 * (This might have to change later, I don't know what all of 
 * the commands look like yet)
 * 
 * Checksum = 0 - (0xFF + 0x06 + command + ACK + dataH + dataL) & 0xFFFF
 * 
 */
unsigned int DFPlayer_calculateChecksum(unsigned char command, unsigned char dataH, unsigned char dataL) {
    unsigned int checksum = 0 - (0xFF + 0x06 + command + dataH + dataL);
    checksum = checksum & 0xFFFF;
    
    return checksum;
}

/**
 * Send a command to the DFPlayer mini
 * Sends two data bytes.
 * Overloaded with another function to send only a single byte.
 */
void DFPlayer_sendCommand(unsigned char command, unsigned char dataH, unsigned char dataL) {
    //Get the checksum
    unsigned int checksum = DFPlayer_calculateChecksum(command, dataH, dataL);
    
    //Normal command structure
    DFPlayer_sendByte(0x7E);
    DFPlayer_sendByte(0xFF);
    DFPlayer_sendByte(0x06);
    DFPlayer_sendByte(command);
    DFPlayer_sendByte(0x00); //No ACK needed
    DFPlayer_sendByte(dataH);
    DFPlayer_sendByte(dataL);
    DFPlayer_sendByte(checksum >> 8); //Checksum High byte
    DFPlayer_sendByte(checksum & 0xFF); //Checksum low byte
    DFPlayer_sendByte(0xEF);
}

/**
 * Sends a specified command to the module and then awaits a reply
 * 
 * BYO data buffer array
 */
void DFPlayer_getResponse(unsigned char command, unsigned char dataH, unsigned char dataL, unsigned char buff[]) {
    //Send the request command
    DFPlayer_sendCommand(command, dataH, dataL);
    /*
    for(int num_bytes = 0; num_bytes < 10; num_bytes++) 
        buff[num_bytes] = DFPlayer_getByte();
        */
    HAL_UART_Receive(&huart1, buff, 10, 250);
}

/**
 * Skip to NEXT track
 * @param track_number
 */
void DFPlayer_playNext() {
    DFPlayer_sendCommand(NEXT, 0x00, 0x00);
}

/**
 * Play the PREVIOUS track
 * @param track_number
 */
void DFPlayer_playPrevious() {
    DFPlayer_sendCommand(PREVIOUS, 0x00, 0x00);
}

/**
 * Sets the volume to a value between 0 - 30
 * 
 * @param volume
 */
void DFPlayer_setVolume(int volume) {
    //Check input bounds
    if(volume > 30)
        volume = 30;
    if(volume < 0)
        volume = 0;
    
    //Send the volume command
    DFPlayer_sendCommand(SPECIFY_VOLUME, 0x00, volume);
}

/**
 * Set the playback mode to loop folder.
 */
void DFPlayer_setLoopMode(unsigned char track_number) {
    DFPlayer_sendCommand(PLAYBACK_MODE, 0x00, track_number);
}

/**
 * Just sends the PLAY command. NO track specified.
 */
void DFPlayer_setPlay() {
    DFPlayer_sendCommand(PLAY, 0x00, 0x00);
}

/**
 * Just sends the PAUSE command. No other data.
 */
void DFPlayer_setPause() {
    DFPlayer_sendCommand(PAUSE, 0x00, 0x00);
}

/**
 * Plays a specific track from a specific folder
 */
void DFPlayer_playTrack(unsigned char folder_number, unsigned char track_number) {
    DFPlayer_sendCommand(SPECIFY_FOLDER, folder_number, track_number);
}

/**
 * Plays a whole folder on repeat
 * @return 
 */
void DFPlayer_repeatFolder(unsigned char folder_number) {
    DFPlayer_sendCommand(REPEAT_FOLDER, 0x00, folder_number);
}

/**
 * Get the current status of the device
 */
int DFPlayer_getStatus() {
    unsigned char data[10];
    //Feed the response in to the array
    DFPlayer_getResponse(QUERY_STATUS, 0x00, 0x00, data);
    
    //Return the interesting part of the returned data
    return data[6];
}

/**
 * Queries the status of the current storage devices
 * 
 * NOTE: This command doesn't seem to work
 * @return 
 */
int DFPlayer_getStorageStatus() {
    unsigned char data[10];
    //Feed the response in to the array
    DFPlayer_getResponse(QUERY_STORAGE, 0x00, 0x00, data);
    
    //Return the interesting part of the returned data
    return data[6];
}

/**
 * Queries the number of tracks in a specified folder
 */
int DFPlayer_getTracksInFolder(int folder) {
    unsigned char data[10];
    //Feed the response in to the array
    DFPlayer_getResponse(QUERY_TRACKS_IN_FOLDER, (folder >> 8), (folder & 0xFF), data);
    
    //Return the interesting part of the returned data
    return data[6];
}

/**
 * Sends the soft reset command
 */
void DFPlayer_resetModule() {
    DFPlayer_sendCommand(RESET, 0x00, 0x00);
}
