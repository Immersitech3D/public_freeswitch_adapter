#include "mod_imm_adapter.h"

/* Prototypes */
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_imm_adapter_shutdown);
SWITCH_MODULE_RUNTIME_FUNCTION(mod_imm_adapter_runtime);
SWITCH_MODULE_LOAD_FUNCTION(mod_imm_adapter_load);

globals_t globals = {0};
static switch_status_t do_config(switch_bool_t reload)
{
	switch_xml_t xml;
	switch_xml_t x_lists;
	switch_xml_t x_list, cfg;
	xml = x_lists = x_list = cfg = NULL;

	if ((xml = switch_xml_open_cfg("imm_adapter.conf", &cfg, NULL))) {
		if ((x_lists = switch_xml_child(cfg, "settings"))) {
			for (x_list = switch_xml_child(x_lists, "param"); x_list; x_list = x_list->next) {
				const char *name = switch_xml_attr(x_list, "name"); // This needs to be const 
				const char *value = switch_xml_attr(x_list, "value");
			
				// Ignore empty/missing attributes
				if (switch_strlen_zero(name)) {
					continue;
				}
				
				if (switch_strlen_zero(value)) {
					continue;
				}
			
				if (!strcmp(name, "license_file_path")) {
      				globals.license_file_path = (char*) value;
				} 
				else if (!strcmp(name, "room_config_file_path")) {
      				globals.room_config_file_path = (char*) value;
				} 
				else if (!strcmp(name, "websocket_config_file_path")) {
					globals.websocket_config_file_path = (char*)value;
				}
				else if (!strcmp(name, "websocket_enabled")) {
					globals.websocket_enabled = atoi(value);
				}
				else if (!strcmp(name, "imm_spatial_output_audio_sample_rate")) {
      				globals.imm_spatial_output_audio_sample_rate = atoi(value);
				} 
				else if (!strcmp(name, "imm_spatial_output_audio_frames_per_buffer")) {
      				globals.imm_spatial_output_audio_frames_per_buffer = atoi(value);
				} 
				else if (!strcmp(name, "imm_spatial_impulse_length")) {
      				globals.imm_spatial_impulse_length = atoi(value);
				} 
				else if (!strcmp(name, "imm_max_string_length")) {
      				globals.imm_max_string_length = atoi(value);
				} 
				else {
					switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Unknown attribute %s\n", name);
				}
			}
		}
	} else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Failed to load Immersitech Adapter module's config!\n");
	}

	return SWITCH_STATUS_SUCCESS;
}

/* SWITCH_MODULE_DEFINITION(name, load, shutdown, runtime)
 * Defines a switch_loadable_module_function_table_t and a static const char[] modname
 */
SWITCH_MODULE_DEFINITION(mod_imm_adapter, mod_imm_adapter_load, mod_imm_adapter_shutdown, NULL);

void show_room_configs(switch_stream_handle_t *stream)
{
	char value[globals.imm_max_string_length];
	memset(value, 0, globals.imm_max_string_length);
    
	imm_core_get_json_all_room_configs(globals.core_handle, value, globals.imm_max_string_length);
	stream->write_function(stream, "Room configurations: %s \n", value);
}

void show_room_by_id(switch_stream_handle_t *stream, char* room_id)
{
	char value[globals.imm_max_string_length];
	memset(value, 0, globals.imm_max_string_length);
    
	imm_core_get_json_room(globals.core_handle, room_id, value, globals.imm_max_string_length);
	stream->write_function(stream, "Room as Json: %s \n", value);
}

void show_rooms(switch_stream_handle_t *stream)
{
	char value[globals.imm_max_string_length];
	memset(value, 0, globals.imm_max_string_length);
    
	imm_core_get_json_all_rooms(globals.core_handle, value, globals.imm_max_string_length);
	stream->write_function(stream, "Rooms as Json: %s \n", value);
}

void move_seat(switch_stream_handle_t *stream, char* room_id, char* participant_id, int seat_index)
{
	imm_core_move_participant_seat(globals.core_handle, room_id, participant_id, seat_index);
	stream->write_function(stream, "Moved participant %s in room %s to seat %i \n", participant_id, room_id, seat_index);
}

void change_room_config(switch_stream_handle_t *stream, char* room_id, int room_config_id) {
	imm_core_change_room_config(globals.core_handle, room_id, room_config_id);
	stream->write_function(stream, "Changed room %s config to %i \n", room_id, room_config_id);
}

void set_participant_state(switch_stream_handle_t *stream, char* room_id, char* participant_id, char* state, char* value) {
	imm_core_set_participant_state(globals.core_handle, room_id, participant_id, state, value);
	stream->write_function(stream, "Set participant %s in room %s %s to %s \n", participant_id, room_id, state, value);
}

void get_participant_state(switch_stream_handle_t *stream, char* room_id, char* participant_id, char* state) {
	int result = imm_core_get_participant_state(globals.core_handle, room_id, participant_id, state);
	stream->write_function(stream, "Get participant %s state %s: %s \n", participant_id, state, result);
}

void set_all_participants_state(switch_stream_handle_t *stream, char* room_id, char* state, char* value) {
	imm_core_set_all_participants_state(globals.core_handle, room_id, state, value);
	stream->write_function(stream, "Set all participants in room %s %s to %s \n", room_id, state, value);
}

void get_all_participants_state(switch_stream_handle_t *stream, char* room_id, char* state) {
	char value[globals.imm_max_string_length];
	memset(value, 0, globals.imm_max_string_length);
	imm_core_get_all_participants_state(globals.core_handle, room_id, state, value, globals.imm_max_string_length);
	stream->write_function(stream, "Get all participants state: %s \n", value);
}

#define IMM_ADAPTER_USAGE "\nget_version \nshow_room_configs  \nshow_rooms  \nshow_room_by_id <room_id>  \nmove_seat <room_id> <participant_id> <seat_index>  \nchange_room_config <room_id> <room_config_id>  \nset_participant_state <room_id> <participant_id> <control_to_edit> <value>  \nget_participant_state <room_id> <participant_id> <control_to_edit>  \nset_all_participants_state <room_id> <control_to_edit> <value>  \nget_all_participants_state <room_id> <control_to_edit> \nget_defined_states"
#define IMM_ADAPTER_PARTICIPANT_STATES "\nIMM_CONTROL_STEREO_BYPASS_ENABLE | IMM_CONTROL_MUTE_ENABLE | IMM_CONTROL_ANC_ENABLE | IMM_CONTROL_AGC_ENABLE | IMM_CONTROL_MIXING_3D_ENABLE | IMM_CONTROL_DEVICE | IMM_CONTROL_HALF_SPAN_ANGLE | IMM_CONTROL_MASTER_GAIN"


SWITCH_STANDARD_API(imm_adapter_api_function)
{
	int ok = 0;
	char *mycmd = NULL;
	if (cmd) {
		char *argv[10] = { 0 };
		int argc = 0;
		mycmd = strdup(cmd);
		argc = switch_split(mycmd, ' ', argv);
		if (argc == 0) {
			goto end;
		}
		if (!strcmp(argv[0], "show_room_configs")) {
			show_room_configs(stream);
			ok = 1;
			goto end;
		} else if (!strcmp(argv[0], "show_rooms")) {
			show_rooms(stream);
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "show_room_by_id")) {
			printf("Show room id: %s. \n", argv[1]);
			show_room_by_id(stream, argv[1]);
			ok = 1;
			goto end;
		} else if (!strcmp(argv[0], "move_seat")){
			if (argc == 4) {
				printf("Move participant id %s in room: %s at seat index %s. \n", argv[2], argv[1], argv[3]);
				move_seat(stream, argv[1], argv[2], atoi(argv[3]));
				ok = 1;
			}
			goto end;
		} else if (!strcmp(argv[0], "change_room_config")){
			if (argc == 3) {
				printf("Change room config to config_id %s in room: %s. \n", argv[2], argv[1]);
				change_room_config(stream, argv[1], atoi(argv[2]));
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_all_participants_state")){
			if (argc == 4) {
				printf("Set all participants state:%s to %s in room: %s. \n", argv[2], argv[3], argv[1]);
				set_all_participants_state(stream, argv[1], argv[2], argv[3]);
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_all_participants_state")){
			if (argc == 3) {
				get_all_participants_state(stream, argv[1], argv[2]);
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_participant_state")){
			if (argc == 5) {
				printf("Set participant %s state:%s to %s in room: %s. \n", argv[2], argv[3], argv[4], argv[1]);
				set_participant_state(stream, argv[1], argv[2], argv[3], argv[4]);
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_state")){
			if (argc == 4) {
				get_participant_state(stream, argv[1], argv[2], argv[3]);
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_defined_states")){
			
			stream->write_function(stream, "Defined participant states: %s \n", IMM_ADAPTER_PARTICIPANT_STATES);
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_version")){

			// Write the Engage core Versions
			char value[IMM_ENGAGE_CORE_VERSION_MAX_LENGTH];
			memset(value, 0, IMM_ENGAGE_CORE_VERSION_MAX_LENGTH);
			imm_core_get_version(globals.core_handle, value);
			stream->write_function(stream, value);

			// Write the Adapter Version
			memset(value, 0, IMM_ENGAGE_CORE_VERSION_MAX_LENGTH);
			strcat(value, "Engage Adapter Version: ");
			strcat(value, IMM_ADAPTER_VERSION);
			strcat(value, "\n");
			stream->write_function(stream, value);

			ok = 1;
			goto end;
		}
	}

 end:

	if (!ok) {
		stream->write_function(stream, "Usage: %s\n", IMM_ADAPTER_USAGE);
	}

	switch_safe_free(mycmd);

	return SWITCH_STATUS_SUCCESS;
}

/* Macro expands to: switch_status_t mod_imm_adapter_load(switch_loadable_module_interface_t **module_interface, switch_memory_pool_t *pool) */
SWITCH_MODULE_LOAD_FUNCTION(mod_imm_adapter_load)
{
	switch_api_interface_t *api_interface;
	/* connect my internal structure to the blank pointer passed to me */
	*module_interface = switch_loadable_module_create_module_interface(pool, modname);

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "Hello Immersitech Freeswitch Adapter!\n");

	memset(&globals, 0, sizeof(globals_t));
	do_config(SWITCH_FALSE);

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "license file path: %s, room config file path:%s.\n", globals.license_file_path, globals.room_config_file_path);
	globals.core_handle = create_immersitech_core(
		globals.license_file_path, 
		globals.room_config_file_path, 
		globals.imm_spatial_output_audio_sample_rate, 
		globals.imm_spatial_output_audio_frames_per_buffer,
		immersitech_output_format::IMM_OUTPUT_FORMAT_STEREO_INTERLEAVED, // Freeswitch only uses interleaved audio
		globals.imm_spatial_impulse_length);

	// Create a freeswitch event manager and add it to the core
	mod_imm_event_manager* event_manager = new mod_imm_event_manager(globals.core_handle);
	imm_core_add_event_manager(globals.core_handle, event_manager);

	// Create a freeswitch logger and add it to the core
	mod_imm_logger_handler* logger_handler = new mod_imm_logger_handler();
	logger::get_logger()->initialize(logger_handler);

	// Create a websocket event manager and add it to the core (if specified to enable)
	if (globals.websocket_enabled) {
		imm_core_enable_websockets(globals.core_handle, globals.websocket_config_file_path);
	}
	
	/* create/register custom event message type */
	if (switch_event_reserve_subclass(IMM_EVENT_MAINT) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't register subclass %s!\n", IMM_EVENT_MAINT);
		return SWITCH_STATUS_TERM;
	}

	/* Subscribe to presence request events */
	if (switch_event_bind(modname, SWITCH_EVENT_CUSTOM, CONF_EVENT_MAINT, conference_event_handler, &globals) != SWITCH_STATUS_SUCCESS) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Couldn't subscribe to conference custom events!\n");
	}
	else {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Subscribed to conference custom events!\n");
	}

	SWITCH_ADD_API(api_interface, "imm_adapter", "Immersitech general commands", imm_adapter_api_function, IMM_ADAPTER_USAGE);
	
	/* indicate that the module should continue to be loaded */
	return SWITCH_STATUS_SUCCESS;
}

/*
  Called when the system shuts down
  Macro expands to: switch_status_t mod_imm_adapter_shutdown() */
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_imm_adapter_shutdown)
{
	switch_event_unbind_callback(conference_event_handler);
	switch_event_free_subclass(IMM_EVENT_MAINT);
	destroy_immersitech_core(globals.core_handle);
	return SWITCH_STATUS_SUCCESS;
}

void create_imm_room(char* room_id, char* name, char* description) {		
	imm_core_create_room(globals.core_handle, room_id, name, description);
}

void destroy_imm_room(char* room_id) {
	imm_core_destroy_room(globals.core_handle, room_id);
}

void create_imm_participant(char* room_id, char* participant_id, char* participant_name, int input_sample_rate, int num_channels) {
	// Currently hard coded to 1 input channel and regular participants only
	imm_core_add_participant(globals.core_handle, room_id, participant_id, participant_name, input_sample_rate, num_channels, 1);
}

void remove_imm_participant(char* room_id, char* participant_id) {
	imm_core_remove_participant(globals.core_handle, room_id, participant_id);
}

void imm_participant_stop_talking_event(char* room_id, char* participant_id) {
	std::string message;
	message.append("{\"participant_id\": \"");
	message.append(participant_id);
	message.append("\", \"action\": \"stop-talking\"}");
	imm_core_send_custom_websocket_event(globals.core_handle, room_id, message.c_str());
}

void imm_participant_start_talking_event(char* room_id, char* participant_id) {
	std::string message;
	message.append("{\"participant_id\": \"");
	message.append(participant_id);
	message.append("\", \"action\": \"start-talking\"}");
	imm_core_send_custom_websocket_event(globals.core_handle, room_id, message.c_str());
}

void conference_event_handler(switch_event_t *event) {
	char* action = switch_event_get_header(event, "Action");
	char* conference_name = switch_event_get_header(event, "Conference-Name");
	char* participant_id = switch_event_get_header(event, "Member-ID");
	char* participant_name = switch_event_get_header(event, "Member-Name");
	char* conference_rate = switch_event_get_header(event, "Conference-Rate");
	char* conference_channels = switch_event_get_header(event, "Conference-Channels");

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Action name: %s\n", action);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Conference-Name: %s\n", conference_name);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Member-ID: %s\n", participant_id);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Member-Name: %s\n", participant_name);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Conference-Rate: %s\n", conference_rate);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Conference-Channels: %s\n", conference_channels);

	if(action != NULL) {
		if(strcmp(action, "conference-create") == 0) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Create conferernce with id: %s.\n", conference_name);
			create_imm_room(conference_name, "freeswitch conference",  "");
		}
		else if (strcmp(action, "conference-destroy") == 0) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Destroy conference with id: %s.\n", conference_name);
			destroy_imm_room(conference_name);
		}
		else if (strcmp(action, "add-member") == 0) {
			int sample_rate = atoi(conference_rate);
			int num_channels = atoi(conference_channels);
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Create member with conference id: %s, member id: %s, conference sample rate: %d, channels: %d\n", conference_name, participant_id, sample_rate, num_channels);
			create_imm_participant(conference_name, participant_id, participant_name, sample_rate, num_channels);
		}
		else if ( (strcmp(action, "del-member") == 0) || (strcmp(action, "stop-recording") == 0)) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Member id: %s left conference id: %s.\n", participant_id, conference_name);
			remove_imm_participant(conference_name, participant_id);
		}
		else if (strcmp(action, "start-talking") == 0) {
			imm_participant_start_talking_event(conference_name, participant_id);
		}
		else if (strcmp(action, "stop-talking") == 0) {
			imm_participant_stop_talking_event(conference_name, participant_id);
		}
		else if (strcmp(action, "start-recording") == 0) { // Add a listener only participant for recording
			int sample_rate = atoi(conference_rate);
			int num_channels = atoi(conference_channels);
			imm_core_add_participant(globals.core_handle, conference_name, participant_id, "recorder", sample_rate, num_channels, 3);
		}
	}
}
