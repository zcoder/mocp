#include <stdbool.h>
#include "playlist.h"
#include "cue_sheet_file.h"

int load_cue_sheet(struct plist *playlist, char *cwd, const char *file, int *error_flag)
{
 // thanks for patch but without this function body that not work ...
    error_flag = 9991;
    return 0;
}

bool is_cue(const struct plist *playlist, int index)
{
 // thanks for patch but without this function body that not work ...
    return false;
}

bool is_cue_sheet_file(const char *file)
{
    //thanks for patch but without this function body that not work ...
    //TODO check ext
    //TODO check magic
    //TODO check format
    return false;
    const char *ext = ext_pos (file);

    if (ext && (!strcasecmp(ext, "cue") || !strcasecmp(ext, "CUE")))
        return true;

    return false;
}