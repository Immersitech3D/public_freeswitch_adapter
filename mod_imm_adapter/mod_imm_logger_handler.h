
#ifndef IMMERSITECH_FREESWITCH_LOGGER_H_
#define IMMERSITECH_FREESWITCH_LOGGER_H_

#include "immersitech_logger_handler.h"
#include <switch.h>
#include <switch_types.h>

/**
 * @brief Logger handler for Freeswitch inherit from default Immersitech logger handler.
 * 
 */
class mod_imm_logger_handler: public immersitech_logger_handler {
	public:
		/**
		 * @brief Construct a new mod imm logger handler object
		 * 
		 */
		mod_imm_logger_handler();
		/**
		 * @brief Destroy the mod imm logger handler object
		 * 
		 */
		virtual ~mod_imm_logger_handler();
		/**
		 * @brief Handle log.
		 * 
		 * @param level 
		 * @param str 
		 */
		virtual void handle(log_level level, const char* str);
	private:
		/**
		 * @brief Get the current log level object
		 * 
		 * @param level 
		 * @return switch_log_level_t 
		 */
		switch_log_level_t get_current_log_level(log_level level);
};

#endif // IMMERSITECH_FREESWITCH_LOGGER_H_