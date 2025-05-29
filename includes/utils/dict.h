#pragma once

#include <stddef.h>
#include <stdlib.h>
#include "utils/cstrview.h"

#define DECL_DICT_TRIPLE(item_t)\
typedef struct dict_triple_##item_t {\
    const char* key;\
    ptrdiff_t key_length;\
    item_t value;\
} DictTriple_##item_t;\

#define DECL_DICT_OF(item_t)\
typedef struct dict_node_##item_t DictNode_##item_t;\
\
typedef struct dict_node_##item_t {\
    size_t hash;\
    item_t data;\
    DictNode_##item_t* left;\
    DictNode_##item_t* right;\
} DictNode_##item_t;\
\
typedef struct dict_##item_t {\
    DictNode_##item_t* root;\
} Dict_##item_t;\
\
[[maybe_unused]] bool dict_##item_t##_load(Dict_##item_t* self, const DictTriple_##item_t* entry);\
\
[[nodiscard]] Dict_##item_t dict_##item_t##_create(const DictTriple_##item_t* entries, ptrdiff_t n);\
\
void impl_dict_node_##item_t##_destroy(DictNode_##item_t* node);\
\
void dict_##item_t##_destroy(Dict_##item_t* self);\
\
[[nodiscard]] item_t dict_##item_t##_get(const Dict_##item_t* self, const CStrView* key, item_t dud);\

#define IMPL_DICT_OF(item_t)\
[[maybe_unused]] bool dict_##item_t##_load(Dict_##item_t* self, const DictTriple_##item_t* entry) {\
    CStrView key_sv = cstr_view_create(entry->key, 0, entry->key_length);\
    const size_t key_hash = cstr_view_hash(&key_sv);\
    DictNode_##item_t* temp = malloc(sizeof(DictNode_##item_t));\
    if (!temp) {\
        return false;\
    }\
    temp->hash = key_hash;\
    temp->data = entry->value;\
    temp->left = nullptr;\
    temp->right = nullptr;\
    if (!self->root) {\
        self->root = temp;\
        return true;\
    }\
    DictNode_##item_t* prev = nullptr;\
    DictNode_##item_t* cursor = self->root;\
    while (cursor) {\
        if (cursor->hash == temp->hash) {\
            free(temp);\
            temp = nullptr;\
            return false;\
        }\
        prev = cursor;\
        cursor = (key_hash < cursor->hash) ? cursor->left : cursor->right ;\
    }\
    if (key_hash < prev->hash) {\
        prev->left = temp;\
    } else {\
        prev->right = temp;\
    }\
    return true;\
}\
\
[[nodiscard]] Dict_##item_t dict_##item_t##_create(const DictTriple_##item_t* entries, ptrdiff_t n) {\
    Dict_##item_t temp;\
    temp.root = nullptr;\
    for (ptrdiff_t entry_idx = 0; entry_idx < n; ++entry_idx) {\
        dict_##item_t##_load(&temp, entries + entry_idx);\
    }\
    return temp;\
}\
\
void impl_dict_node_##item_t##_destroy(DictNode_##item_t* node) {\
    if (!node) return;\
    impl_dict_node_##item_t##_destroy(node->left);\
    impl_dict_node_##item_t##_destroy(node->right);\
    free(node);\
}\
\
void dict_##item_t##_destroy(Dict_##item_t* self) {\
    impl_dict_node_##item_t##_destroy(self->root);\
}\
\
[[nodiscard]] item_t dict_##item_t##_get(const Dict_##item_t* self, const CStrView* key, item_t dud) {\
    DictNode_##item_t* cursor = self->root;\
    const size_t key_hash = cstr_view_hash(key);\
    if (!cursor) {\
        return dud;\
    }\
    if (cursor->hash == key_hash) {\
        return cursor->data;\
    }\
    DictNode_##item_t* prev = nullptr;\
    while (cursor) {\
        if (cursor->hash == key_hash) {\
            prev = cursor;\
            break;\
        }\
        prev = cursor;\
        cursor = (key_hash < cursor->hash) ? cursor->left : cursor->right ;\
    }\
    return prev->data;\
}\

#define DECL_DICT_VAR(item_t, name)Dict_##item_t name;\

#define CALL_DICT_METHOD(item_t, method_name, ...) dict_##item_t##_##method_name(__VA_ARGS__);\
