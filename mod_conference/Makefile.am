include $(top_srcdir)/build/modmake.rulesam
MODNAME=mod_conference

IMMERSITECH_LIBS = /usr/local/lib/immersitech

mod_LTLIBRARIES = mod_conference.la
mod_conference_la_SOURCES  = mod_conference.c conference_api.c conference_loop.c conference_al.c conference_cdr.c conference_video.c
mod_conference_la_SOURCES += conference_event.c conference_member.c conference_utils.c conference_file.c conference_record.c
mod_conference_la_CFLAGS   = $(AM_CFLAGS) -I.
mod_conference_la_LIBADD   = $(switch_builddir)/libfreeswitch.la
mod_conference_la_LDFLAGS  = -avoid-version -module -no-undefined -shared -L$(IMMERSITECH_LIBS) -limmersitech -Xlinker -rpath=$(IMMERSITECH_LIBS)

$(am_mod_conference_la_OBJECTS):  mod_conference.h

if HAVE_OPENAL
mod_conference_la_LDFLAGS += -lopenal -lm
mod_conference_la_CFLAGS += -DOPENAL_POSITIONING
endif


noinst_LTLIBRARIES = libmodconference.la

libmodconference_la_SOURCES  = $(mod_conference_la_SOURCES)
libmodconference_la_CFLAGS   = $(AM_CFLAGS) -I.

noinst_PROGRAMS = test/test_image test/test_member

test_test_image_SOURCES = test/test_image.c
test_test_image_CFLAGS = $(AM_CFLAGS) -I. -DSWITCH_TEST_BASE_DIR_FOR_CONF=\"${abs_builddir}/test\" -DSWITCH_TEST_BASE_DIR_OVERRIDE=\"${abs_builddir}/test\"
test_test_image_LDFLAGS = $(AM_LDFLAGS) -avoid-version -no-undefined $(freeswitch_LDFLAGS) $(switch_builddir)/libfreeswitch.la $(CORE_LIBS) $(APR_LIBS) -L$(IMMERSITECH_LIBS) -limmersitech-engage-core -Xlinker -rpath=$(IMMERSITECH_LIBS)
test_test_image_LDADD = libmodconference.la

test_test_member_SOURCES = test/test_member.c
test_test_member_CFLAGS = $(AM_CFLAGS) -I. -DSWITCH_TEST_BASE_DIR_FOR_CONF=\"${abs_builddir}/test\" -DSWITCH_TEST_BASE_DIR_OVERRIDE=\"${abs_builddir}/test\"
test_test_member_LDFLAGS = $(AM_LDFLAGS) -avoid-version -no-undefined $(freeswitch_LDFLAGS) $(switch_builddir)/libfreeswitch.la $(CORE_LIBS) $(APR_LIBS) -L$(IMMERSITECH_LIBS) -limmersitech-engage-core -Xlinker -rpath=$(IMMERSITECH_LIBS)
test_test_member_LDADD = libmodconference.la

TESTS = $(noinst_PROGRAMS)
