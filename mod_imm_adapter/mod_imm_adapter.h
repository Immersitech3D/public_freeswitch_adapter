#pragma once

#ifndef IMMERSITECH_FREESWITCH_ADAPTER_H_
#define IMMERSITECH_FREESWITCH_ADAPTER_H_

#include <switch.h>
#include "immersitech_core.h"
#include "mod_imm_event_manager.h"
#include "mod_imm_logger_handler.h"

#define IMM_EVENT_MAINT "immersitech::maintenance"
#define IMM_EVENT_CDR "immersitech::cdr"

#ifndef CONF_EVENT_MAINT
#define CONF_EVENT_MAINT "conference::maintenance"
#endif
#ifndef CONF_EVENT_CDR
#define CONF_EVENT_CDR "conference::cdr"
#endif

#ifndef IMM_ADAPTER_VERSION
#define IMM_ADAPTER_VERSION "v0.01.020"
#endif

/**
 * @brief Struct of global properties. 
 * 
 */
typedef struct {
	char* license_file_path;
	char* room_config_file_path;
    char* websocket_config_file_path;
	int websocket_enabled;
	Immersitech_Core core_handle;
	int imm_spatial_output_audio_sample_rate;
    int imm_spatial_output_audio_frames_per_buffer;
    int imm_spatial_impulse_length;
    int imm_max_string_length;
} globals_t;
extern globals_t globals;

/**
 * @brief Event handler to handle events come from mod_conference.
 * 
 * @param event The event from Freeswitch that should be processed
 */
void conference_event_handler(switch_event_t *event);

#endif // IMMERSITECH_FREESWITCH_ADAPTER_H_