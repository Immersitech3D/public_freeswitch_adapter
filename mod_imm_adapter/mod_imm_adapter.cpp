#include "mod_imm_adapter.h"
#include <sstream>

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
				else if (!strcmp(name, "room_layout_file_path")) {
      				globals.room_layout_file_path = (char*) value;
				} 
				else if (!strcmp(name, "websocket_config_file_path")) {
					globals.websocket_config_file_path = (char*)value;
				}
				else if (!strcmp(name, "websocket_enabled")) {
					globals.websocket_enabled = atoi(value);
				}
				else if (!strcmp(name, "imm_output_audio_sample_rate")) {
      				globals.imm_output_audio_sample_rate = atoi(value);
				} 
				else if (!strcmp(name, "imm_output_audio_frames_per_buffer")) {
      				globals.imm_output_audio_frames_per_buffer = atoi(value);
				} 
				else if (!strcmp(name, "imm_spatial_quality")) {
      				globals.imm_spatial_quality = atoi(value);
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

#define IMM_ADAPTER_USAGE "\nget_version \nget_license_info \nget_library_configuration \nget_json_all_room_layouts \nget_room_count \nget_room_layout <room_id>\nget_participant_count <room_id> \nget_participant_position <room_id> <participant_id> \nget_participant_seat <room_id> <participant_id> \nget_participant_state <room_id> <participant_id> <control> \nget_participant_configuration <room_id> <participant_id> \nget_participant_name <room_id> <participant_id> \nget_participant_spherical <room_id> <listener_id> <source_id> \nset_room_layout <room_id> <layout_id> \nset_participant_name <room_id> <participant_id> <name> \nset_all_participants_state <room_id> <control> <value> \nset_participant_state <room_id> <participant_id> <control> <value> \nset_participant_position <room_id> <participant_id> <x> <y> <z> <azimuth_heading> <elevation_heading> \nset_participant_seat <room_id> <participant_id> <seat_id>"

SWITCH_STANDARD_API(imm_adapter_api_function)
{
	int ok = 0;
	char *mycmd = NULL;
	std::stringstream ss;
	if (cmd) {
		char *argv[10] = { 0 };
		int argc = 0;
		mycmd = strdup(cmd);
		argc = switch_split(mycmd, ' ', argv);
		if (argc == 0) {
			goto end;
		}
		if (!strcmp(argv[0], "get_version")) {

			// Write the Adapter Version
			ss << "Immersitech Library Version: " << imm_get_version() << std::endl;
			ss << "Engage Freeswitch Adapter Version: " << IMM_ADAPTER_VERSION << std::endl;
			stream->write_function(stream, ss.str().c_str());

			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_license_info")) {
			stream->write_function(stream, imm_get_license_info(globals.core_handle));
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_library_configuration")) {
			imm_library_configuration lib_config = imm_get_library_configuration(globals.core_handle);
			ss << "output_sampling_rate: " << lib_config.output_sampling_rate << std::endl;
			ss << "output_number_channels: " << lib_config.output_number_channels << std::endl;
			ss << "output_number_frames: " << lib_config.output_number_frames << std::endl;
			ss << "spatial_quality: " << lib_config.spatial_quality << std::endl;
			ss << "interleaved: " << lib_config.interleaved << std::endl;
			stream->write_function(stream, ss.str().c_str());
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_json_all_room_layouts")) {
			stream->write_function(stream, imm_get_json_all_room_layouts(globals.core_handle));
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_room_count")) {
			int room_count;
			imm_error_code error_code = imm_get_room_count(globals.core_handle, &room_count);
			if (error_code == IMM_ERROR_NONE) {
				ss << room_count;
				stream->write_function(stream, ss.str().c_str());
			}
			ok = 1;
			goto end;
		}
		else if (!strcmp(argv[0], "get_room_layout")) {
			if (argc == 2) {
				int layout_id;
				int room_id = std::stoi(argv[1]);
				imm_error_code error_code = imm_get_room_layout(globals.core_handle, room_id, &layout_id);
				if (error_code == IMM_ERROR_NONE) {
					ss << layout_id;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_count")) {
			if (argc == 2) {
				int participant_count;
				int room_id = std::stoi(argv[1]);
				imm_error_code error_code = imm_get_participant_count(globals.core_handle, room_id, &participant_count);
				if (error_code == IMM_ERROR_NONE) {
					ss << participant_count;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_position")) {
			if (argc == 3) {
				imm_position position;
				imm_heading heading;
				int room_id = std::stoi(argv[1]);
				int participant_id = std::stoi(argv[2]);
				imm_error_code error_code = imm_get_participant_position(globals.core_handle, room_id, participant_id, &position, &heading);
				if (error_code == IMM_ERROR_NONE) {
					ss << "Participant " << participant_id << " in Room " << room_id << " position:" << std::endl;
					ss << "X: " << position.x << std::endl;
					ss << "Y: " << position.y << std::endl;
					ss << "Z: " << position.z << std::endl;
					ss << "Azimuth Heading: " << heading.azimuth_heading << std::endl;
					ss << "Elevation Heading: " << heading.elevation_heading << std::endl;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_seat")) {
			if (argc == 3) {
				imm_seat seat;
				int room_id = std::stoi(argv[1]);
				int participant_id = std::stoi(argv[2]);
				imm_error_code error_code = imm_get_participant_seat(globals.core_handle, room_id, participant_id, &seat);
				if (error_code == IMM_ERROR_NONE) {
					ss << "Participant " << participant_id << " in Room " << room_id << " position:" << std::endl;
					ss << "ID: " << seat.id << std::endl;
					ss << "X: " << seat.position.x << std::endl;
					ss << "Y: " << seat.position.y << std::endl;
					ss << "Z: " << seat.position.z << std::endl;
					ss << "Azimuth Heading: " << seat.heading.azimuth_heading << std::endl;
					ss << "Elevation Heading: " << seat.heading.elevation_heading << std::endl;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_state")) {
			if (argc == 4) {
				int room_id = std::stoi(argv[1]);
				int participant_id = std::stoi(argv[2]);
				imm_audio_control control = imm_string_to_audio_control(argv[3]);
				int value;
				imm_error_code error_code = imm_get_participant_state(globals.core_handle, room_id, participant_id, control, &value);
				if (error_code == IMM_ERROR_NONE) {
					stream->write_function(stream, std::to_string(value).c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_configuration")) {
			if (argc == 3) {
				int room_id = std::stoi(argv[1]);
				int participant_id = std::stoi(argv[2]);
				imm_participant_configuration part_config;
				imm_error_code error_code = imm_get_participant_configuration(globals.core_handle, room_id, participant_id, &part_config);
				if (error_code == IMM_ERROR_NONE) {
					ss << "Participant " << participant_id << std::endl;
					ss << "input_sampling_rate " << part_config.input_sampling_rate << std::endl;
					ss << "input_number_channels " << part_config.input_number_channels << std::endl;
					ss << "type " << part_config.type << std::endl;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_name")) {
			if (argc == 3) {
				int room_id = std::stoi(argv[1]);
				int participant_id = std::stoi(argv[2]);
				char name[2000];
				imm_error_code error_code = imm_get_participant_name(globals.core_handle, room_id, participant_id, name, 2000);
				if (error_code == IMM_ERROR_NONE) {
					stream->write_function(stream, name);
				}
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "get_participant_spherical")) {
			if (argc == 4) {
				int room_id = std::stoi(argv[1]);
				int listener_id = std::stoi(argv[2]);
				int source_id = std::stoi(argv[3]);
				int az, el, dt;
				imm_error_code error_code = imm_get_participant_spherical(globals.core_handle, room_id, listener_id, source_id, &az, &el, &dt);
				if (error_code == IMM_ERROR_NONE) {
					ss << "azimuth: " << az << std::endl;
					ss << "elevation: " << el << std::endl;
					ss << "distance: " << dt << std::endl;
					stream->write_function(stream, ss.str().c_str());
				}
				ok = 1;
			}
			goto end;
		} else if (!strcmp(argv[0], "set_log_level")) {
			if (argc == 2) {
				imm_set_log_level(atoi(argv[1]));
				ok = 1;
			}
			goto end;
		} else if (!strcmp(argv[0], "set_participant_seat")){
			if (argc == 4) {
				imm_set_participant_seat(globals.core_handle, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
				ok = 1;
			}
			goto end;
		} else if (!strcmp(argv[0], "set_room_layout")){
			if (argc == 3) {
				imm_set_room_layout(globals.core_handle, atoi(argv[1]), atoi(argv[2]));
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_all_participants_state")){
			if (argc == 4) {
				imm_set_all_participants_state(globals.core_handle, atoi(argv[1]), imm_string_to_audio_control(argv[2]), atoi(argv[3]));
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_participant_state")){
			if (argc == 5) {
				imm_set_participant_state(globals.core_handle, atoi(argv[1]), atoi(argv[2]), imm_string_to_audio_control(argv[3]), atoi(argv[4]));
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_participant_position")) {
			if (argc == 8) {
				imm_set_participant_position(globals.core_handle, atoi(argv[1]), atoi(argv[2]), { atoi(argv[3]), atoi(argv[4]), atoi(argv[5]) }, { atoi(argv[6]), atoi(argv[7])});
				ok = 1;
			}
			goto end;
		}
		else if (!strcmp(argv[0], "set_participant_name")) {
			if (argc == 4) {
				imm_set_participant_name(globals.core_handle, atoi(argv[1]), atoi(argv[2]), argv[3]);
				ok = 1;
			}
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

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "license file path: %s\n", globals.license_file_path);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "room config file path:%s.\n", globals.room_layout_file_path);
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_NOTICE, "websocket config file path:%s.\n", globals.websocket_config_file_path);
	imm_error_code error_code;
	imm_library_configuration library_config;
	library_config.output_number_channels = 2; // Need 2 channel output for spatial audio to work
	library_config.output_sampling_rate = globals.imm_output_audio_sample_rate;
	library_config.output_number_frames = globals.imm_output_audio_frames_per_buffer;
	library_config.interleaved = true; // Freeswitch only uses interleaved audio
	library_config.spatial_quality = globals.imm_spatial_quality;
	imm_enable_logging(true); // Turn on logging by default to make debugging easier
	globals.core_handle = imm_initialize_library(globals.license_file_path, globals.room_layout_file_path, globals.websocket_config_file_path, library_config, &error_code);

	// Create a freeswitch event manager and add it to the core
	mod_imm_event_manager* event_manager = new mod_imm_event_manager(globals.core_handle);
	imm_add_event_manager(globals.core_handle, event_manager);

	// Create a freeswitch logger and add it to the core
	mod_imm_logger_handler* logger_handler = new mod_imm_logger_handler();
	IMM_LOGGER->initialize(logger_handler);

	// Start the websocket server if the config file says it is enabled
	if (globals.websocket_enabled) {
		imm_enable_websockets(globals.core_handle, true);
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
	imm_destroy_library(globals.core_handle);
	return SWITCH_STATUS_SUCCESS;
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

	int room_id = atoi(conference_name);
	int member_id = atoi(participant_id);

	if(action != NULL) {
		if(strcmp(action, "conference-create") == 0) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Create conferernce with id: %s.\n", conference_name);
			imm_create_room(globals.core_handle, room_id);
		}
		else if (strcmp(action, "conference-destroy") == 0) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Destroy conference with id: %s.\n", conference_name);
			imm_destroy_room(globals.core_handle, room_id);
		}
		else if (strcmp(action, "add-member") == 0) {
			imm_participant_configuration participant_config;
			participant_config.input_number_channels = atoi(conference_channels);
			participant_config.input_sampling_rate = atoi(conference_rate);
			participant_config.type = IMM_PARTICIPANT_LISTENER_ONLY;
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Create member with conference id: %s, member id: %s, conference sample rate: %s, channels: %s\n", conference_name, participant_id, conference_rate, conference_channels);
			imm_add_participant(globals.core_handle, room_id, member_id, participant_name, participant_config);
		}
		else if ( (strcmp(action, "del-member") == 0) || (strcmp(action, "stop-recording") == 0)) {
			switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Member id: %s left conference id: %s.\n", participant_id, conference_name);
			imm_remove_participant(globals.core_handle, room_id, member_id);
		}
		else if (strcmp(action, "start-talking") == 0) {
			std::string message;
			message.append("{\"participant_id\": \"");
			message.append(participant_id);
			message.append("\", \"action\": \"start-talking\"}");
			imm_send_custom_websocket_event(globals.core_handle, room_id, message.c_str());
		}
		else if (strcmp(action, "stop-talking") == 0) {
			std::string message;
			message.append("{\"participant_id\": \"");
			message.append(participant_id);
			message.append("\", \"action\": \"stop-talking\"}");
			imm_send_custom_websocket_event(globals.core_handle, room_id, message.c_str());
		}
		else if (strcmp(action, "start-recording") == 0) { // Add a listener only participant for recording
			imm_participant_configuration participant_config;
			participant_config.input_number_channels = atoi(conference_channels);
			participant_config.input_sampling_rate = atoi(conference_rate);
			participant_config.type = IMM_PARTICIPANT_LISTENER_ONLY;
			imm_add_participant(globals.core_handle, room_id, member_id, "recorder", participant_config );
		}
	}
}
