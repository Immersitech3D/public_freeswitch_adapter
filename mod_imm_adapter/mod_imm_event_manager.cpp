#include "mod_imm_adapter.h"
#include "mod_imm_event_manager.h"

mod_imm_event_manager::mod_imm_event_manager(imm_handle core) {
	_core = core;
}

mod_imm_event_manager::~mod_imm_event_manager() {
}

void mod_imm_event_manager::create_room_event(int room_id) {
	switch_event_t *event;
    switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	event->event_user_data = (void*) _core;
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Send imm core address %p to room with id:%s.\n", event->event_user_data, std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-create-room");
	switch_event_fire(&event);
}

void mod_imm_event_manager::destroy_room_event(int room_id) {
    switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-destroy-room");
	switch_event_fire(&event);
}

void mod_imm_event_manager::add_participant_event(int room_id, int participant_id, const char* participant_name, imm_participant_configuration config) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", std::to_string(participant_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Name", participant_name);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Num-Channels", std::to_string(config.input_number_channels).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Sampling-Rate", std::to_string(config.input_sampling_rate).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Type", std::to_string(config.type).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-add-participant");
	switch_event_fire(&event);
}

void mod_imm_event_manager::remove_participant_event(int room_id, int participant_id) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", std::to_string(participant_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-remove-participant");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_participant_seat_event(int room_id, int participant_id, imm_seat seat) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", std::to_string(participant_id).c_str());
	
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Id", std::to_string(seat.id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-X", std::to_string(seat.position.x).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-Y", std::to_string(seat.position.y).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-Z", std::to_string(seat.position.z).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Heading-Azimuth", std::to_string(seat.heading.azimuth_heading).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Heading-Elevation", std::to_string(seat.heading.elevation_heading).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-participant-seat");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_participant_position_event(int room_id, int participant_id, imm_position position, imm_heading heading) {
	switch_event_t* event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", std::to_string(participant_id).c_str());

	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Position-X", std::to_string(position.x).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Position-Y", std::to_string(position.y).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Position-Z", std::to_string(position.z).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Heading-Azimuth", std::to_string(heading.azimuth_heading).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Heading-Elevation", std::to_string(heading.elevation_heading).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-participant-position");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_participant_state_event(int room_id, int participant_id, imm_audio_control control_to_edit, int value) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State-Id", std::to_string(control_to_edit).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State", std::to_string(value).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", std::to_string(participant_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-participant-state");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_all_participants_state_event(int room_id, imm_audio_control control_to_edit, int value) {
    switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State-Id", std::to_string(control_to_edit).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State", std::to_string(value).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-all-participants-state");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_room_layout_event(int room_id, int room_layout_id) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", std::to_string(room_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Config-Id", std::to_string(room_layout_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-change-room-config");
	switch_event_fire(&event);
}
