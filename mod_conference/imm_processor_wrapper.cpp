#include "imm_processor_wrapper.h"

#include <stdlib.h>
#include <string.h>
#include "imm_freeswitch_logger.h"

#define IMM_PROCESSOR_ROOM_ID 0
#define IMM_PROCESSOR_INPUT_ID 0
#define IMM_PROCESSOR_OUTPUT_ID 1

static char *imm_license_path = NULL;
static char *imm_room_layout_path = NULL;
static char *imm_websocket_path = NULL;

void configure_immersitech_library(const char *_imm_license_path, const char *_imm_room_layout_path, const char *_imm_websocket_path) {
	if (imm_license_path) {
		free(imm_license_path);
		imm_license_path = NULL;
	}
	if(_imm_license_path) {
		size_t len = strlen(_imm_license_path);
		imm_license_path = (char *)malloc(len + 1); // NULL isn't counted by strlen
		strncpy(imm_license_path, _imm_license_path, len);
		imm_license_path[len] = '\0';
	}

	if (imm_room_layout_path) {
		free(imm_room_layout_path);
		imm_room_layout_path = NULL;
	}
	if(_imm_room_layout_path) {
		size_t len = strlen(_imm_room_layout_path);
		imm_room_layout_path = (char *)malloc(len + 1); // NULL isn't counted by strlen
		strncpy(imm_room_layout_path, _imm_room_layout_path, len);
		imm_room_layout_path[len] = '\0';
	}

	if (imm_websocket_path) {
		free(imm_websocket_path);
		imm_websocket_path = NULL;
	}
	if(_imm_websocket_path) {
		size_t len = strlen(_imm_websocket_path);
		imm_websocket_path = (char *)malloc(len + 1); // NULL isn't counted by strlen
		strncpy(imm_websocket_path, _imm_websocket_path, len);
		imm_websocket_path[len] = '\0';
	}
	
	// Upon loading mod_conference module, we set up Immersitech logging one time
	imm_freeswitch_logger* logger_handler = new imm_freeswitch_logger();
	IMM_LOGGER->initialize(logger_handler);
	imm_enable_logging(true);
	imm_set_log_level( 0 ); // set log level to least restrictive level and allow freeswitch to control level
}

imm_handle create_immersitech_processor(int sampling_rate, int number_of_channels, int interval) {
	imm_error_code error_code;
	imm_library_configuration config;
	imm_participant_configuration participant_config;
	imm_handle handle;

	// Create the library, a room, and our input and output participants
	config.interleaved = true; // Audio in freeswitch is always interleaved
	config.output_number_channels = number_of_channels;
	config.output_number_frames = interval * 0.001 * sampling_rate;
	config.output_sampling_rate = sampling_rate;
	config.spatial_quality = IMM_SPATIAL_QUALITY;
	handle = imm_initialize_library(imm_license_path ? imm_license_path : IMM_LICENSE_PATH_DEFAULT,
		imm_room_layout_path, imm_websocket_path, config, &error_code);
	if (error_code != IMM_ERROR_NONE) {
		imm_destroy_library(handle);
		return NULL;
	}

	error_code = imm_create_room(handle, IMM_PROCESSOR_ROOM_ID);
	if (error_code != IMM_ERROR_NONE) {
		imm_destroy_library(handle);
		return NULL;
	}

	// Currently assume all input audio is resampled and same number of channels as output
	participant_config.input_number_channels = number_of_channels;
	participant_config.input_sampling_rate = sampling_rate;

	participant_config.type = IMM_PARTICIPANT_SOURCE_ONLY;
	error_code = imm_add_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, "input", participant_config);
	if (error_code != IMM_ERROR_NONE) {
		imm_destroy_room(handle, IMM_PROCESSOR_ROOM_ID);
		imm_destroy_library(handle);
		return NULL;
	}

	participant_config.type = IMM_PARTICIPANT_LISTENER_ONLY;
	error_code = imm_add_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID, "output", participant_config);
	if (error_code != IMM_ERROR_NONE) {
		imm_destroy_room(handle, IMM_PROCESSOR_ROOM_ID);
		imm_destroy_library(handle);
		return NULL;
	}
	
	// Print information about this participant and the library
	print_library_info(handle);
	
	return handle;
}

void destroy_immersitech_processor(imm_handle handle) {
	imm_remove_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID);
	imm_remove_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID);
	imm_destroy_room(handle, IMM_PROCESSOR_ROOM_ID);
	imm_destroy_library(handle);
}

imm_error_code immersitech_process(imm_handle handle, short* input, int num_in_frames, short* output) {
	imm_error_code error_code;
	error_code = imm_input_audio_short(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, input, num_in_frames);
	if (error_code != IMM_ERROR_NONE) {
		return error_code;
	}
	error_code = imm_output_audio_short(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID, output);
	return error_code;
}

imm_error_code immersitech_set_state(imm_handle handle, imm_audio_control control, int value) {
	return imm_set_all_participants_state(handle, IMM_PROCESSOR_ROOM_ID, control, value);
}

imm_error_code immersitech_get_state(imm_handle handle, imm_audio_control control, int* value) {
	return imm_get_participant_state(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, control, value);
}

imm_error_code immersitech_set_position(imm_handle handle, imm_position position) {
	imm_heading heading = {0,0};
	return imm_set_participant_position(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, position, heading);
}

imm_error_code immersitech_get_position(imm_handle handle, imm_position* position) {
	imm_heading heading;
	return imm_get_participant_position(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, position, &heading);
}

void print_library_info(imm_handle handle) {
	imm_library_configuration config;
	printf("Immersitech Library Information:\n");
	printf("Version: %s\n", imm_get_version());
	printf("License: %s\n", imm_get_license_info(handle));
	config = imm_get_library_configuration(handle);
	printf("Interleaved: %d\n", config.interleaved);
	printf("Number Channels: %i\n", config.output_number_channels);
	printf("Number Frames: %i\n", config.output_number_frames);
	printf("Sampling Rate: %i\n", config.output_sampling_rate);
	printf("Spatial Quality: %i\n", config.spatial_quality);
}