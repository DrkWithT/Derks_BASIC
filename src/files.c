#include <stdio.h>
#include "utils/files.h"
#include "utils/safestr.h"

SafeStr read_file_sync(const char* file_path, BlobMode mode) {
    const char* fs_mode = (mode == blob_mode_text) ? "r" : "rb" ;
    FILE* fs = fopen(file_path,  fs_mode);

    SafeStr temp = safe_str_create();

    if (!fs) {
        safe_str_destroy(&temp);
        return temp;
    }

    while (!feof(fs)) {
        const auto ch = fgetc(fs);

        if (ch == EOF) {
            break;
        }

        const auto append_ok = safe_str_append_char(&temp, (char)ch);

        if (!append_ok) {
            safe_str_destroy(&temp);
            fclose(fs);
            return temp;
        }
    }

    fclose(fs);
    return temp;
}
