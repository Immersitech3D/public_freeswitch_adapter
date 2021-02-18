
#ifndef IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_
#define IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_

#include "immersitech_event_manager.h"

/**
 * @brief Event manager for Freeswitch inherit from default immsersitech_event_manager.
 * 
 */
class mod_imm_event_manager : public immersitech_event_manager {
	public:
	    mod_imm_event_manager(Immersitech_Core core);
		virtual ~mod_imm_event_manager();
		virtual void create_room_event(std::string room_id);
		virtual void destroy_room_event(std::string room_id);
		virtual void add_participant_event(std::string room_id, std::string participant_id, std::string participant_name);
		virtual void remove_participant_event(std::string room_id, std::string participant_id);
		virtual void move_participant_event(std::string room_id, std::string participant_id, immersitech_seat seat);
		virtual void set_participant_state_event(std::string room_id, std::string participant_id, std::string control_to_edit, std::string value);
		virtual void set_all_participants_state_event(std::string room_id, std::string control_to_edit, std::string value);
		virtual void change_room_config_event(std::string room_id, int room_config_id);
	private:
		Immersitech_Core _core;
};

#endif // IMMERSITECH_FREESWITCH_EVENT_MANAGER_H_