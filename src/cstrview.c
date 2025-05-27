#include <string.h>
#include "utils/cstrview.h"

CStrView cstr_view_create(const char* src, ptrdiff_t begin, ptrdiff_t length) {
    CStrView self;

    self.ptr = src + begin;
    self.length = length;

    return self;
}

char cstr_view_at(const CStrView* self, ptrdiff_t pos) {
    return self->ptr[pos];
}

bool cstr_view_cmp_other(const CStrView* self, const CStrView* other) {
    if (self->length != other->length) {
        return false;
    }

    return strncmp(self->ptr, other->ptr, self->length) == 0;
}

bool cstr_view_cmp_cstr(const CStrView* self, const char* cstr) {
    if (strlen(cstr) != (size_t)self->length) {
        return false;
    }

    return strncmp(self->ptr, cstr, self->length) == 0;
}
