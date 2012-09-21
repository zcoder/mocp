#ifndef CUE_SHEET_FILE_H
#define CUE_SHEET_FILE_H

#include "common.h"
#include "playlist.h"

#define NO_ERROR 0
#define ERROR_PARSE 99990
#define ERROR_NO_STAT 99991
#define ERROR_NOT_SUPPORTED 99992
int load_cue_sheet(struct plist *playlist, char *cwd, const char *file, int *error_flag);
bool is_cue(const struct plist *playlist, int index);
bool is_cue_sheet_file(const char *file);



#endif
