#pragma once

#include <stddef.h>
#include "utils/cstrview.h"

typedef struct safe_str_t {
    char* ptr;
    ptrdiff_t length;
    ptrdiff_t capacity;
} SafeStr;

[[nodiscard]] SafeStr safe_str_create();

[[nodiscard]] SafeStr safe_str_create_from(const char* cstr);

void safe_str_destroy(SafeStr* self);

[[nodiscard]] bool safe_str_grow(SafeStr* self);

[[nodiscard]] bool safe_str_valid(const SafeStr* self);

[[nodiscard]] char safe_str_at(const SafeStr* self, ptrdiff_t pos);

[[maybe_unused]] bool safe_str_append_char(SafeStr* self, char c);

[[maybe_unused]] bool safe_str_append_view(SafeStr* self, const CStrView* view);

[[nodiscard]] bool safe_str_cmp_other(const SafeStr* self, const SafeStr* other);

[[nodiscard]] bool safe_str_cmp_view(const SafeStr* self, const CStrView* view);

[[nodiscard]] CStrView safe_str_to_view(const SafeStr* self, ptrdiff_t begin, ptrdiff_t length);

[[nodiscard]] SafeStr safe_str_substr(const SafeStr* self, ptrdiff_t begin, ptrdiff_t length);
