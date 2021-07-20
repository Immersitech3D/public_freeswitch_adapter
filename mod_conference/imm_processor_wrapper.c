#include "imm_processor_wrapper.h"

#define IMM_PROCESSOR_ROOM_ID 0
#define IMM_PROCESSOR_INPUT_ID 0
#define IMM_PROCESSOR_OUTPUT_ID 1

imm_handle create_immersitech_processor() {
	imm_error_code error_code;
	imm_library_configuration config;
	imm_participant_configuration participant_config;
	imm_handle handle;
	
	// Let us log to freeswitch as it is easier to debug
	imm_enable_logging(true);

	// Create the library, a room, and our input and output participants
	config.interleaved = true; // Audio in freeswitch is always interleaved
	config.output_number_channels = IMM_NUMBER_CHANNELS;
	config.output_number_frames = IMM_NUMBER_FRAMES;
	config.output_sampling_rate = IMM_SAMPLING_RATE;
	config.spatial_quality = IMM_SPATIAL_QUALITY;
	handle = imm_initialize_library(IMM_LICENSE_PATH, IMM_ROOM_LAYOUT_PATH, IMM_WEBSOCKET_PATH, config, &error_code);
	imm_create_room(handle, IMM_PROCESSOR_ROOM_ID);

	// Currently assume all input audio is resampled and same number of channels as output
	participant_config.input_number_channels = IMM_NUMBER_CHANNELS;
	participant_config.input_sampling_rate = IMM_SAMPLING_RATE;
	participant_config.type = IMM_PARTICIPANT_SOURCE_ONLY;
	imm_add_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, "input", participant_config);
	participant_config.type = IMM_PARTICIPANT_LISTENER_ONLY;
	imm_add_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID, "output", participant_config);
	return handle;
}

void destroy_immersitech_processor(imm_handle handle) {
	imm_remove_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID);
	imm_remove_participant(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID);
	imm_destroy_room(handle, IMM_PROCESSOR_ROOM_ID);
	imm_destroy_library(handle);
}

void immersitech_process(imm_handle handle, short* input, int num_in_frames, short* output) {
	imm_input_audio_short(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, input, num_in_frames);
	imm_output_audio_short(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_OUTPUT_ID, output);
}

imm_error_code immersitech_set_state(imm_handle handle, imm_audio_control control, int value) {
	return imm_set_all_participants_state(handle, IMM_PROCESSOR_ROOM_ID, control, value);
}

imm_error_code immersitech_get_state(imm_handle handle, imm_audio_control control, int* value) {
	return imm_get_participant_state(handle, IMM_PROCESSOR_ROOM_ID, IMM_PROCESSOR_INPUT_ID, control, value);
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