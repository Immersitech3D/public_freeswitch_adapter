#pragma once

/* Immersitech Processor
The immersitech SDK is an audio mixing engine, meaning multiple inputs and outputs.
If however you'd like a more traditional audio effect library with a single input and output,
this Immersitech Processor wrapper is here to help.
The Immersitech Processor provides an ultra simple interface to process your audio with 
Immersitech effects with a single function call. With this wrapper there is no need to track
participant IDs, room IDs, or an other extra features from the full featured library.
*/

#ifndef IMMERSITECH_PROCESSOR_WRAPPER_H_
#define IMMERSITECH_PROCESSOR_WRAPPER_H_

#include "immersitech.h"
#include <stdio.h>

// Set these parameters to match your conference settings
#define IMM_SPATIAL_QUALITY 1

// Set these paths to match your local file system and file names
// You can enter a room layout and websocket path if later you decide to use them
#define IMM_LICENSE_PATH "/usr/local/lib/immersitech/my_license.dat"
#define IMM_ROOM_LAYOUT_PATH NULL
#define IMM_WEBSOCKET_PATH NULL

// Create / Destroy your immersitech processor
// Note that interval is the conference internal in milliseconds, such as 20 milliseconds.
imm_handle create_immersitech_processor(int sampling_rate, int number_of_channels, int interval);
void destroy_immersitech_processor(imm_handle handle);

// Process your audio directly through the processor
void immersitech_process(imm_handle handle, short* input, int num_in_frames, short* output);

// Change the audio effects states at any time
imm_error_code immersitech_set_state(imm_handle handle, imm_audio_control control, int value);
imm_error_code immersitech_get_state(imm_handle handle, imm_audio_control control, int* value);

// Print information about the library
void print_library_info(imm_handle handle);

#endif // IMMERSITECH_PROCESSOR_WRAPPER_H_