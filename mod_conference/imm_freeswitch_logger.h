#ifndef IMMERSITECH_FREESWITCH_LOGGER_H_
#define IMMERSITECH_FREESWITCH_LOGGER_H_

#include "immersitech_logger.h"
#include <switch.h>
#include <switch_types.h>

/**
 * @brief Logger handler for Freeswitch inherit from default Immersitech logger handler.
 * 
 */
class imm_freeswitch_logger: public imm_logger_handler {
	public:
		/**
		 * @brief Construct a new freeswitch logger handler object
		 * 
		 */
		imm_freeswitch_logger();
		/**
		 * @brief Destroy the freeswitch logger handler object
		 * 
		 */
		virtual ~imm_freeswitch_logger();
		/**
		 * @brief Handle log.
		 * 
		 * @param level 
		 * @param str 
		 */
		virtual void handle(imm_log_level level, const char* str);
	private:
		/**
		 * @brief Get the current log level object
		 * 
		 * @param level 
		 * @return SWITCH_LOG_level_t 
		 */
		switch_log_level_t get_current_imm_log_level(imm_log_level level);
};

#endif // IMMERSITECH_FREESWITCH_LOGGER_H_