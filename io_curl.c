/*
 * MOC - music on console
 * Copyright (C) 2005 Damian Pietras <daper@daper.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#define DEBUG

#include "main.h"
#include "log.h"
#include "io.h"
#include "io_curl.h"

/* TODO:
 *   - proxy support
 */

void io_curl_init ()
{
	curl_global_init (CURL_GLOBAL_NOTHING);
}

void io_curl_cleanup ()
{
	curl_global_cleanup ();
}

static size_t write_callback (void *data, size_t size, size_t nmemb,
		void *stream)
{
	struct io_stream *s = (struct io_stream *)stream;


	s->curl_buf_fill = size * nmemb;
	debug ("Got %ld bytes", s->curl_buf_fill);
	s->curl_buf = (char *)xmalloc (s->curl_buf_fill);
	memcpy (s->curl_buf, data, s->curl_buf_fill);

	return s->curl_buf_fill;
}

static int debug_callback (CURL *curl ATTR_UNUSED, curl_infotype i, char *msg,
		size_t size, void *d ATTR_UNUSED)
{
	if (i == CURLINFO_TEXT || i == CURLINFO_HEADER_IN
			|| i == CURLINFO_HEADER_OUT) {
		char *type;
		char *log = (char *)xmalloc (size + 1);

		switch (i) {
			case CURLINFO_TEXT:
				type = "INFO";
				break;
			case CURLINFO_HEADER_IN:
				type = "RECV HEADER";
				break;
			case CURLINFO_HEADER_OUT:
				type = "SEND HEADER";
				break;
			default:
				type = "";
		}
		
		strncpy (log, msg, size);
		if (size > 0 && log[size-1] == '\n')
			log[size-1] = 0;
		else
			log[size] = 0;
		debug ("CURL: [%s] %s", type, log);
		free (log);
	}
	
	return 0;
}

void io_curl_open (struct io_stream *s, const char *url)
{
	int running;
	
	s->source = IO_SOURCE_CURL;
	s->url = NULL;
	s->http_headers = NULL;
	s->curl_buf = NULL;
	s->curl_buf_fill = 0;

	if (!(s->curl_multi_handle = curl_multi_init())) {
		logit ("curl_multi_init() returned NULL");
		return;
	}
	
	if (!(s->curl_handle = curl_easy_init())) {
		logit ("curl_easy_init() returned NULL");
		return;
	}

	s->curl_multi_status = CURLM_OK;
	s->curl_status = CURLE_OK;

	s->url = xstrdup (url);

	curl_easy_setopt (s->curl_handle, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt (s->curl_handle, CURLOPT_WRITEFUNCTION,
			write_callback);
	curl_easy_setopt (s->curl_handle, CURLOPT_WRITEDATA, s);
	curl_easy_setopt (s->curl_handle, CURLOPT_USERAGENT,
			PACKAGE_NAME"/"PACKAGE_VERSION);
	curl_easy_setopt (s->curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt (s->curl_handle, CURLOPT_URL, s->url);
	curl_easy_setopt (s->curl_handle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt (s->curl_handle, CURLOPT_MAXREDIRS, 15);

#ifdef DEBUG
	curl_easy_setopt (s->curl_handle, CURLOPT_VERBOSE, 1);
	curl_easy_setopt (s->curl_handle, CURLOPT_DEBUGFUNCTION,
			debug_callback);
#endif
	
	/* TODO: 
	curl_easy_setopt (s->curl_handle, CURLOPT_HTTPHEADER, s->http_headers);
	*/

	if ((s->curl_multi_status = curl_multi_add_handle(s->curl_multi_handle,
					s->curl_handle)) != CURLM_OK)
		return;

	debug ("Starting curl...");
	do {
		s->curl_multi_status = curl_multi_perform (s->curl_multi_handle,
			&running);
	} while (s->curl_multi_status == CURLM_CALL_MULTI_PERFORM);

	s->opened = 1;
}

void io_curl_close (struct io_stream *s)
{
	assert (s != NULL);
	assert (s->source == IO_SOURCE_CURL);

	if (s->url)
		free (s->url);
	if (s->http_headers)
		curl_slist_free_all (s->http_headers);
	if (s->curl_buf)
		free (s->curl_buf);

	if (s->curl_multi_handle && s->curl_handle)
		curl_multi_remove_handle (s->curl_multi_handle, s->curl_handle);
	if (s->curl_handle)
		curl_easy_cleanup (s->curl_handle);
	if (s->curl_multi_handle)
		curl_multi_cleanup (s->curl_multi_handle);
}

ssize_t io_curl_read (struct io_stream *s, char *buf, size_t count)
{
	int running = 1;
	CURLMsg *msg;
	int msg_queue_num;
	
	assert (s != NULL);
	assert (s->source == IO_SOURCE_CURL);
	assert (s->curl_multi_handle != NULL);
	assert (s->curl_handle != NULL);

	while (!s->curl_buf_fill && running && s->curl_handle
			&& (s->curl_multi_status == CURLM_CALL_MULTI_PERFORM
				|| s->curl_multi_status == CURLM_OK)) {
		if (s->curl_multi_status != CURLM_CALL_MULTI_PERFORM) {
			fd_set read_fds, write_fds, exc_fds;
			int max_fd;
			int ret;

			logit ("Doing select()...");

			FD_ZERO (&read_fds);
			FD_ZERO (&write_fds);
			FD_ZERO (&exc_fds);

			s->curl_multi_status == curl_multi_fdset (
					s->curl_multi_handle,
					&read_fds, &write_fds, &exc_fds,
					&max_fd);
			if (s->curl_multi_status != CURLM_OK) {
				logit ("curl_multi_fdset() failed");
				return -1;
			}

			ret = select (max_fd + 1, &read_fds, &write_fds,
					&exc_fds, NULL);

			if (ret < 0 && errno == EINTR) {
				logit ("Interrupted");
				return 0;
			}
			if (ret < 0) {
				s->errno_val == errno;
				logit ("select() failed");
				return -1;
			}
		}

		s->curl_multi_status = curl_multi_perform (s->curl_multi_handle,
			&running);
	}

	debug ("running: %d", running);

	while ((msg = curl_multi_info_read(s->curl_multi_handle,
					&msg_queue_num)))
		if (msg->msg == CURLMSG_DONE) {
			s->curl_status = msg->data.result;
			curl_easy_cleanup (s->curl_handle);
			s->curl_handle = NULL;
			break;
		}

	if ((s->curl_multi_status == CURLM_OK || s->curl_multi_status
				== CURLM_CALL_MULTI_PERFORM)
			&& s->curl_status == CURLE_OK) {
		long to_copy = MIN ((long)count, s->curl_buf_fill);

		debug ("Copying %ld bytes", to_copy);

		memcpy (buf, s->curl_buf, to_copy);
		s->curl_buf_fill -= to_copy;
		memmove (s->curl_buf, s->curl_buf + to_copy,
				s->curl_buf_fill);
		
		return to_copy;
	}

	return -1;
}

/* Set the error string for the stream. */
void io_curl_strerror (struct io_stream *s)
{
	const char *err = "OK";
	
	assert (s != NULL);
	assert (s->source == IO_SOURCE_CURL);

	if (!s->curl_multi_handle || !s->curl_handle) {
		err = "Could not create a curl handle";
	}
	else if (s->curl_multi_status != CURLM_OK)
		err = curl_multi_strerror(s->curl_multi_status);
	else if (s->curl_status != CURLE_OK)
		err = curl_easy_strerror(s->curl_status);

	s->strerror = xstrdup (err);
}

int io_curl_ok (const struct io_stream *s)
{
	assert (s != NULL);
	assert (s->source == IO_SOURCE_CURL);

	return s->curl_multi_handle
		&& s->curl_handle
		&& (s->curl_multi_status == CURLM_OK
				|| s->curl_multi_status
				== CURLM_CALL_MULTI_PERFORM)
		&& s->curl_status == CURLE_OK
		&& !s->errno_val;
}