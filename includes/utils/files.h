#pragma once

#include "utils/safestr.h"

typedef enum blob_mode_e : unsigned char {
    blob_mode_binary,
    blob_mode_text
} BlobMode;

[[nodiscard]] SafeStr read_file_sync(const char* file_path, BlobMode mode);
