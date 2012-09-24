#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "playlist.h"
#include "cue_sheet_file.h"
#include "log.h"
#include "playlist_file.h"
#include "files.h"

int load_cue_sheet(struct plist *playlist, char *cwd, const char *file, int *error_flag)
{
    logit (">load_cue_sheet::%s", *file ? file : "[[NULL]]");
    
    const char *title;
    const char *file_ape = "/home/zhenka/common/zcoder/mocp/music/69 Eyes - Angels.ape";
    int last_added = -1;
    time_t start_sec;
    time_t end_sec;

    {
        start_sec = 10;
        end_sec = 60;
        title = "(cue) Eyes - Angels - Track1";
        last_added = plist_add (playlist, file_ape);
        plist_set_title_tags (playlist, last_added, title);
        plist_set_cue (playlist, last_added, (time_t) start_sec, (time_t) end_sec, title, file_ape);
    }       
#ifdef bur
    {
        start_sec = 60;
        end_sec = 120;
        title = "(cue) Eyes - Angels - Track2";
        last_added = plist_add (playlist, file_ape);
        plist_set_title_tags (playlist, last_added, title);
        //plist_add_cue(playlist, last_added, title);
        plist_set_cue (playlist, last_added, (time_t) start_sec, (time_t) end_sec, title, file_ape);
    }   
#endif

    *error_flag = 0;
    return 2;
}

bool is_cue(const struct plist *playlist, int index)
{
    assert (playlist != NULL);
    logit (">is_cue::%d", index);

    if (playlist->items[index].type == F_CUE_TRACK)
    {
        return true;
    }
    return false;
}

bool is_cue_sheet_file(const char *file)
{
    logit (">is_cue_sheet_file::%s", file );
    const char *ext = ext_pos( file );
    if (ext && !strcmp(ext, "cue"))
    {
        return true;
    }
    return false;
    
    //TODO check magic
    //TODO check format
}

#if 0
if (file == NULL)
    {
        return false;
    }
    char *ext = strrchr (file, '.');
	char *slash = strrchr (file, '/');

	/* don't treat dot in ./file or /.file as a dot before extension */
	if (ext && (!slash || slash < ext) && ext != file && *(ext-1) != '/')
		ext++;
	else
		ext = NULL;
#endif

