
#ifndef IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_
#define IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_

#include "immersitech_event_manager.h"

/// @brief Event manager for Freeswitch that inherits from default imm_event_manager.
///
/// Will pass a pointer to the imm_handle during the create a room event
class mod_imm_event_manager : public imm_event_manager {
	public:
	    mod_imm_event_manager(imm_handle core);
		virtual ~mod_imm_event_manager();
		virtual void create_room_event(int room_id);
		virtual void destroy_room_event(int room_id);
		virtual void add_participant_event(int room_id, int participant_id, const char* participant_name, imm_participant_configuration config);
		virtual void remove_participant_event(int room_id, int participant_id);
		virtual void set_participant_seat_event(int room_id, int participant_id, imm_seat seat);
		virtual void set_participant_position_event(int room_id, int participant_id, imm_position position, imm_heading heading);
		virtual void set_participant_state_event(int room_id, int participant_id, imm_audio_control control_to_edit, int value);
		virtual void set_all_participants_state_event(int room_id, imm_audio_control control_to_edit, int value);
		virtual void set_room_layout_event(int room_id, int room_layout_id);
	private:
		imm_handle _core;
};

#endif // IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_