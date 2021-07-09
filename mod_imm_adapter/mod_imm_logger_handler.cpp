#include "mod_imm_logger_handler.h"

mod_imm_logger_handler::mod_imm_logger_handler() {
}

mod_imm_logger_handler::~mod_imm_logger_handler() {
}

void mod_imm_logger_handler::handle(imm_log_level level, const char* str) {
    switch_log_level_t freeswitch_level = get_current_imm_log_level(level);
    switch_log_printf(SWITCH_CHANNEL_LOG, freeswitch_level, "%s\n", str);
}
switch_log_level_t mod_imm_logger_handler::get_current_imm_log_level(imm_log_level level) {
    switch (level)
    {        
        case imm_log_level::IMM_LOG_DEBUG:      return SWITCH_LOG_DEBUG;
        case imm_log_level::IMM_LOG_WARNING:    return SWITCH_LOG_WARNING;
        case imm_log_level::IMM_LOG_INFO:       return SWITCH_LOG_INFO;
        case imm_log_level::IMM_LOG_ERROR:      return SWITCH_LOG_ERROR;
    }
    return SWITCH_LOG_DEBUG;
}