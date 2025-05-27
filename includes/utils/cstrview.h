#pragma once

#include <stddef.h>

typedef struct cstr_view_t {
    const char* ptr;
    ptrdiff_t length;
} CStrView;

[[nodiscard]] CStrView cstr_view_create(const char* src, ptrdiff_t begin, ptrdiff_t length);

[[nodiscard]] char cstr_view_at(const CStrView* self, ptrdiff_t pos);

[[nodiscard]] bool cstr_view_cmp_other(const CStrView* self, const CStrView* other);

[[nodiscard]] bool cstr_view_cmp_cstr(const CStrView* self, const char* cstr);
