#pragma once

#include <stddef.h>
#include "frontend/token.h"
#include "utils/dict.h"

/* symbol matching functions */

[[nodiscard]] inline bool match_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

[[nodiscard]] inline bool match_numeric(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

[[nodiscard]] inline bool match_spaces(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}

[[nodiscard]] inline bool match_op_symbol(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}


/* lexer utility types */

DECL_DICT_TRIPLE(TokenTag)

DECL_DICT_OF(TokenTag)


/* lexer decl */

typedef struct lexer_state_t {
    DECL_DICT_VAR(TokenTag, specials)
    CStrView view;
    int pos;
    int end;
    int line;
    int col;
} LexerState;

[[nodiscard]] LexerState lexer_create(const SafeStr* source);

void lexer_destroy(LexerState* self);

void lexer_update_file_pos(LexerState* self, char c);

[[nodiscard]] bool lexer_at_end(const LexerState* self);

[[nodiscard]] Token lexer_lex_single(LexerState* self, TokenTag tag);

[[nodiscard]] Token lexer_lex_spaces(LexerState* self);

[[nodiscard]] Token lexer_lex_comment(LexerState* self);

[[nodiscard]] Token lexer_lex_word(LexerState* self);

[[nodiscard]] Token lexer_lex_numeric(LexerState* self);

[[nodiscard]] Token lexer_lex_string(LexerState* self);

[[nodiscard]] Token lexer_lex_operator(LexerState* self);

[[nodiscard]] Token lexer_next(LexerState* self);
