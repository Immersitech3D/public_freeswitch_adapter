#include "mod_imm_adapter.h"
#include "mod_imm_event_manager.h"

mod_imm_event_manager::mod_imm_event_manager(Immersitech_Core core) {
	_core = core;
}

mod_imm_event_manager::~mod_imm_event_manager() {
}

void mod_imm_event_manager::create_room_event(std::string room_id) {
	switch_event_t *event;
    switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	event->event_user_data = (void*) _core;
	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Send imm core address %p to room with id:%s.\n", event->event_user_data, room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-create-room");
	switch_event_fire(&event);
}

void mod_imm_event_manager::destroy_room_event(std::string room_id) {
    switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-destroy-room");
	switch_event_fire(&event);
}

void mod_imm_event_manager::add_participant_event(std::string room_id, std::string participant_id, std::string participant_name) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", participant_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Name", participant_name.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-create-participant");
	switch_event_fire(&event);
}

void mod_imm_event_manager::remove_participant_event(std::string room_id, std::string participant_id) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", participant_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-destroy-participant");
	switch_event_fire(&event);
}

void mod_imm_event_manager::move_participant_event(std::string room_id, std::string participant_id, immersitech_seat seat) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", participant_id.c_str());
	
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Id", std::to_string(seat.id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-X", std::to_string(seat.position.x).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-Y", std::to_string(seat.position.y).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Seat-Position-Z", std::to_string(seat.position.z).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-move-participant");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_participant_state_event(std::string room_id, std::string participant_id, std::string control_to_edit, std::string value) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State-Id", control_to_edit.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State", value.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Participant-Id", participant_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-participant-state");
	switch_event_fire(&event);
}

void mod_imm_event_manager::set_all_participants_state_event(std::string room_id, std::string control_to_edit, std::string value) {
    switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State-Id", control_to_edit.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "State", value.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-set-all-participants-state");
	switch_event_fire(&event);
}

void mod_imm_event_manager::change_room_config_event(std::string room_id, int room_config_id) {
	switch_event_t *event;
	switch_event_create_subclass(&event, SWITCH_EVENT_CUSTOM, IMM_EVENT_MAINT);
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Id", room_id.c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Room-Config-Id", std::to_string(room_config_id).c_str());
	switch_event_add_header_string(event, SWITCH_STACK_BOTTOM, "Action", "immersitech-change-room-config");
	switch_event_fire(&event);
}
