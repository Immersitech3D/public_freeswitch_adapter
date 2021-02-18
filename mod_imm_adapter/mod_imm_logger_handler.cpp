#include "mod_imm_logger_handler.h"
mod_imm_logger_handler::mod_imm_logger_handler() {
}

mod_imm_logger_handler::~mod_imm_logger_handler() {
}

void mod_imm_logger_handler::handle(log_level level, const char* str) {
    switch_log_level_t freeswitch_level = get_current_log_level(level);
    switch_log_printf(SWITCH_CHANNEL_LOG, freeswitch_level, "%s\n", str);
}
switch_log_level_t mod_imm_logger_handler::get_current_log_level(log_level level) {
    switch (level)
    {        
        case log_level::LOG_DEBUG:      return SWITCH_LOG_DEBUG;
        case log_level::LOG_WARNING:    return SWITCH_LOG_WARNING;
        case log_level::LOG_INFO:       return SWITCH_LOG_INFO;
        case log_level::LOG_ERROR:      return SWITCH_LOG_ERROR;
    }
    return SWITCH_LOG_DEBUG;
}