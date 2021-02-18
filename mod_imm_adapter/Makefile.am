include $(top_srcdir)/build/modmake.rulesam
MODNAME=mod_imm_adapter

IMMERSITECH_LIBS = /usr/lib/immersitech

mod_LTLIBRARIES = mod_imm_adapter.la
mod_imm_adapter_la_SOURCES  = mod_imm_adapter.cpp mod_imm_event_manager.cpp mod_imm_logger_handler.cpp
mod_imm_adapter_la_CFLAGS   = $(AM_CFLAGS)
mod_imm_adapter_la_LIBADD   = $(switch_builddir)/libfreeswitch.la
mod_imm_adapter_la_LDFLAGS  = -avoid-version -module -no-undefined -shared -L$(IMMERSITECH_LIBS) -limmersitech-engage-core -Xlinker -rpath=$(IMMERSITECH_LIBS)

noinst_LTLIBRARIES = libmodimmadapter.la

libmodimmadapter_la_SOURCES  = $(mod_imm_adapter_la_SOURCES)
libmodimmadapter_la_CFLAGS   = $(AM_CFLAGS) -I.
