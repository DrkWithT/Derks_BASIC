#include <stddef.h>
#include <string.h>
#include "utils/cstrview.h"

static constexpr ptrdiff_t cstr_view_dud_n = 0;
static constexpr size_t cstr_view_hash_base = 5;

CStrView cstr_view_create(const char* src, ptrdiff_t begin, ptrdiff_t length) {
    CStrView self;

    self.ptr = src + begin;
    self.length = length;

    return self;
}

bool cstr_view_valid(const CStrView* self) {
    return self->ptr != nullptr;
}

char cstr_view_at(const CStrView* self, ptrdiff_t pos) {
    if (!cstr_view_valid(self) || pos < cstr_view_dud_n || pos > self->length) {
        return '\0';
    }

    return self->ptr[pos];
}

bool cstr_view_cmp_other(const CStrView* self, const CStrView* other) {
    if (!cstr_view_valid(self) || !cstr_view_valid(other)) {
        return false;
    }

    if (self->length != other->length) {
        return false;
    }

    return strncmp(self->ptr, other->ptr, self->length) == 0;
}

bool cstr_view_cmp_cstr(const CStrView* self, const char* cstr) {
    if (!cstr_view_valid(self) || !cstr) {
        return false;
    }

    if (strlen(cstr) != (size_t)self->length) {
        return false;
    }

    return strncmp(self->ptr, cstr, self->length) == 0;
}

size_t cstr_view_hash(const CStrView* self) {
    size_t place_value = 1;
    size_t result = 0;

    for (ptrdiff_t view_offset = 0; view_offset < self->length; ++view_offset, place_value *= cstr_view_hash_base) {
        result += place_value * (unsigned char)self->ptr[view_offset];
    }

    return result;
}
