#include <stdbool.h>
#include "playlist.h"
#include "cue_sheet_file.h"
#include "log.h"
#include "playlist_file.h"

int load_cue_sheet(struct plist *playlist, char *cwd, const char *file, int *error_flag)
{
 // thanks for patch but without this function body that not work ...
    logit (">load_cue_sheet::%s", *file ? file : "[[NULL]]");
    //plist_load(playlist, cwd, file, 0);
    int item = plist_add(playlist, "/home/zhenka/common/zcoder/mocp/music/69 Eyes - Angels.ape" );
    plist_set_title_tags (playlist, item, "Eyes - Angels");
    //plist_add_cue(playlist, item, "Eyes - Angels" );
    //plist_set_cue(playlist, item, 50, 90, "Eyes - Angels", "/home/zhenka/common/zcoder/mocp/music/69 Eyes - Angels.ape");
    error_flag = 9990;
    return 1;
}

bool is_cue(const struct plist *playlist, int index)
{
 // thanks for patch but without this function body that not work ...
    logit (">is_cue::%d", index);
    return false;
}

bool is_cue_sheet_file(const char *file)
{
    //thanks for patch but without this function body that not work ...
    //TODO check ext
    //TODO check magic
    //TODO check format
    logit (">is_cue_sheet_file::%s", *file ? file : "[[NULL]]");
    return true;
    /*const char *ext = ext_pos (file);

    if (ext && (!strcasecmp(ext, "cue") || !strcasecmp(ext, "CUE")))
        return true;

    return false;*/
}