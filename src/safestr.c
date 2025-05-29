#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "utils/safestr.h"
#include "utils/cstrview.h"

static constexpr ptrdiff_t safe_str_dud_capacity = 0;
static constexpr ptrdiff_t safe_str_default_capacity = 16;

SafeStr safe_str_create() {
    SafeStr self;

    self.ptr = calloc((size_t)safe_str_default_capacity, sizeof(char));

    if (self.ptr != nullptr) {
        self.capacity = safe_str_default_capacity;
        self.length = safe_str_dud_capacity;
    } else {
        self.capacity = safe_str_dud_capacity;
        self.length = safe_str_dud_capacity;
    }

    return self;
}

SafeStr safe_str_create_from(const char* cstr) {
    SafeStr self;
    const auto cstr_count = strlen(cstr);
    char* data = calloc(cstr_count, sizeof(char));

    if (data != nullptr) {
        self.ptr = data;
        self.capacity = cstr_count;
    } else {
        self.ptr = nullptr;
        self.capacity = 0L;
    }

    self.length = 0L;

    return self;
}

void safe_str_destroy(SafeStr* self) {
    if (safe_str_valid(self)) {
        free(self->ptr);
        self->ptr = nullptr;
        self->capacity = 0;
        self->length = 0;
    }
}

bool safe_str_grow(SafeStr* self) {
    if (!safe_str_valid(self)) {
        return false;
    }

    auto old_capacity = self->capacity;
    auto new_capacity = old_capacity * 2;

    char* new_buffer = calloc(new_capacity, sizeof(char));

    if (!new_buffer) {
        return false;
    }

    memcpy(new_buffer, self->ptr, (size_t)old_capacity * sizeof(char));
    new_buffer[old_capacity] = '\0';

    free(self->ptr);
    self->ptr = new_buffer;
    self->capacity = new_capacity;

    return true;
}

bool safe_str_valid(const SafeStr* self) {
    return self->ptr != nullptr && self->capacity > safe_str_dud_capacity;
}

char safe_str_at(const SafeStr* self, ptrdiff_t pos) {
    if (!safe_str_valid(self) || pos < safe_str_dud_capacity || pos >= self->length) {
        return '\0';
    }

    return self->ptr[pos];
}

bool safe_str_append_char(SafeStr* self, char c) {
    if (!safe_str_valid(self)) {
        return false;
    }

    if (self->length >= self->capacity) {
        if (!safe_str_grow(self)) {
            return false;
        }
    }

    self->ptr[self->length] = c;
    self->ptr[self->length + 1] = '\0';
    ++self->length;

    return true;
}

bool safe_str_append_view(SafeStr* self, const CStrView* view) {
    const char* view_it = view->ptr;
    const char* view_end = view_it + view->length;

    for (; view_it != view_end; ++view_it) {
        if (!safe_str_append_char(self, *view_it)) {
            return false;
        }
    }

    return true;
}

bool safe_str_cmp_other(const SafeStr* self, const SafeStr* other) {
    if (!safe_str_valid(self) || !safe_str_valid(other)) {
        return false;
    }

    if (self->length != other->length) {
        return false;
    }

    if (self->ptr != nullptr && other->ptr != nullptr) {
        return strncmp(self->ptr, other->ptr, (size_t)self->length) == 0;
    }

    return false;
}

bool safe_str_cmp_view(const SafeStr* self, const CStrView* view) {
    if (!safe_str_valid(self) || !cstr_view_valid(view)) {
        return false;
    }

    if (self->length != view->length) {
        return false;
    }

    return strncmp(self->ptr, view->ptr, (size_t)self->length) == 0;
}

CStrView safe_str_to_view(const SafeStr* self, ptrdiff_t begin, ptrdiff_t length) {
    if (!safe_str_valid(self)) {
        return (CStrView) {
            .ptr = nullptr,
            .length = 0
        };
    }

    return (CStrView) {
        .ptr = self->ptr + begin,
        .length = length
    };
}

SafeStr safe_str_substr(const SafeStr* self, ptrdiff_t begin, ptrdiff_t length) {
    if (!safe_str_valid(self)) {
        return (SafeStr) {
            .ptr = nullptr,
            .capacity = safe_str_dud_capacity,
            .length = safe_str_dud_capacity
        };
    }
    
    SafeStr sub = safe_str_create();
    const char* copy_it = self->ptr + begin;
    const char* copy_end = copy_it + length;

    for (; copy_it != copy_end; ++copy_it) {
        if (!safe_str_append_char(&sub, *copy_it)) {
            safe_str_destroy(&sub);
            break;
        }
    }

    return sub;
}
