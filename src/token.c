#include "frontend/token.h"
#include "utils/safestr.h"

CStrView peek_lexeme(const SafeStr* source, const Token* token) {
    if (!safe_str_valid(source)) {
        return (CStrView) {
            .ptr = nullptr,
            .length = 0
        };
    }

    return (CStrView) {
        .ptr = source->ptr + (ptrdiff_t)token->begin,
        .length = (ptrdiff_t)token->length
    };
}

SafeStr get_lexeme(const SafeStr* source, const Token *token) {
    return safe_str_substr(source, (ptrdiff_t)token->begin, (ptrdiff_t)token->length);
}
