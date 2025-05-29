#pragma once

#include "utils/cstrview.h"
#include "utils/safestr.h"

typedef enum token_tag_e {
    unknown,
    spaces,
    comment,
    keyword,
    identifier,
    literal_true,
    literal_false,
    literal_int,
    literal_float,
    literal_string,
    comma,
    lf,
    bracket_left,
    bracket_right,
    paren_left,
    paren_right,
    brace_left,
    brace_right,
    op_access,
    op_call,
    op_plus,
    op_minus,
    op_times,
    op_slash,
    op_is_equal,
    op_not_equal,
    op_greater,
    op_lesser,
    op_assign,
    eof,
} TokenTag;

typedef struct token_t
{
    TokenTag tag;
    int begin;
    int length;
    int line;
    int col;
} Token;

[[nodiscard]] CStrView peek_lexeme(const SafeStr* source, const Token* token);

[[nodiscard]] SafeStr get_lexeme(const SafeStr* source, const Token* token);
