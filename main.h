#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdarg.h>

#define CONFIG_DIR      ".moc"

/* Exit status on fatal exit. */
#define EXIT_FATAL	2

/* Maximal string length sent/received. */
#define MAX_SEND_STRING	4096

/* Maximum path length, we don't consider exceptions like mounted NFS */
#ifndef PATH_MAX
# if defined(_POSIX_PATH_MAX)
#  define PATH_MAX	_POSIX_PATH_MAX /* Posix */
# elif defined(MAXPATHLEN)
#  define PATH_MAX	MAXPATHLEN      /* Solaris? Also linux...*/
# else
#  define PATH_MAX	4096             /* Suppose, we have 4096 */
# endif
#endif

#define LOCK(mutex)	pthread_mutex_lock (&mutex)
#define UNLOCK(mutex)	pthread_mutex_unlock (&mutex)

void *xmalloc (const size_t size);
void *xrealloc (void *ptr, const size_t size);
char *xstrdup (const char *s);
void fatal (const char *format, ...);
char *create_file_name (const char *file);
int proper_sound_driver (const char *driver);
int isdir (const char *file);

#endif
