ACLOCAL_AMFLAGS = -I m4
SUBDIRS = themes decoder_plugins
AM_CPPFLAGS = -DSYSTEM_THEMES_DIR=\"$(pkgdatadir)/themes\" \
	      -DPLUGIN_DIR=\"$(plugindir)/$(DECODER_PLUGIN_DIR)\"

bin_PROGRAMS = mocp
mocp_SOURCES = log.cpp \
	       log.h \
	       protocol.h \
	       protocol.cpp \
	       server.cpp \
	       server.h \
	       main.cpp \
	       common.cpp \
	       common.h \
	       playlist.cpp \
	       playlist.h \
	       fifo_buf.cpp \
	       fifo_buf.h \
	       out_buf.cpp \
	       out_buf.h \
	       audio.cpp \
	       audio.h \
	       decoder.cpp \
	       decoder.h \
	       interface.cpp \
	       interface.h \
	       interface_elements.cpp \
	       interface_elements.h \
	       menu.cpp \
	       menu.h \
	       files.cpp \
	       files.h \
	       options.cpp \
	       options.h \
	       player.cpp \
	       player.h \
	       cue_sheet_file.cpp \
	       cue_sheet_file.h \
	       playlist_file.cpp \
	       playlist_file.h \
	       themes.cpp \
	       themes.h \
	       keys.cpp \
	       keys.h \
	       io.cpp \
	       io.h \
	       compat.cpp \
	       compat.h \
	       audio_conversion.cpp \
	       audio_conversion.h \
	       rbtree.cpp \
	       rbtree.h \
	       tags_cache.cpp \
	       tags_cache.h \
	       utf8.cpp \
	       utf8.h \
	       rcc.cpp \
	       rcc.h \
	       audio_helper.h \
	       audio_helper.cpp \
	       softmixer.cpp \
	       softmixer.h \
	       lyrics.h \
	       lyrics.cpp \
	       lists.h \
	       lists.cpp \
	       equalizer.h \
	       equalizer.cpp
EXTRA_mocp_SOURCES = gnugetopt.h \
		     getopt.cpp \
		     getopt1.cpp \
		     md5.cpp \
		     md5.h \
		     null_out.cpp \
		     null_out.h \
		     sndio_out.cpp \
		     sndio_out.h \
		     oss.cpp \
		     oss.h \
		     alsa.cpp \
		     alsa.h \
		     io_curl.cpp \
		     io_curl.h \
		     jack.cpp \
		     jack.h
man_MANS = mocp.1
mocp_LDADD = @EXTRA_OBJS@ -lltdl
mocp_DEPENDENCIES = @EXTRA_OBJS@
mocp_LDFLAGS = @EXTRA_LIBS@ $(RCC_LIBS) -export-dynamic
EXTRA_DIST = README_equalizer mocp.1 THANKS keymap.example Doxyfile \
	         doxy_pages/decoder_api.doxy doxy_pages/main_page.doxy \
	         doxy_pages/sound_output_driver_api.doxy
EXTRA_DIST += @EXTRA_DISTS@
EXTRA_DIST += tools/README tools/md5check.sh tools/maketests.sh
noinst_DATA = tools/README
noinst_SCRIPTS = tools/md5check.sh tools/maketests.sh

doc_DATA = config.example THANKS README README_equalizer keymap.example
