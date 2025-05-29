#include "frontend/lexer.h"
#include "frontend/token.h"
#include "utils/cstrview.h"
#include "utils/dict.h"
#include <stddef.h>

static constexpr DictTriple_TokenTag lang_words[] = {
    (DictTriple_TokenTag) {
        .key = "DEFUN",
        .key_length = 5,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "WITH",
        .key_length = 4,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "DO",
        .key_length = 2,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "END",
        .key_length = 3,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "LET",
        .key_length = 3,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "CONST",
        .key_length = 5,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "IF",
        .key_length = 2,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = "RETURN",
        .key_length = 6,
        .value = keyword
    },
    (DictTriple_TokenTag) {
        .key = ":",
        .key_length = 1,
        .value = op_access
    },
    (DictTriple_TokenTag) {
        .key = "CALL",
        .key_length = 4,
        .value = op_call
    },
    (DictTriple_TokenTag) {
        .key = "+",
        .key_length = 1,
        .value = op_plus
    },
    (DictTriple_TokenTag) {
        .key = "-",
        .key_length = 1,
        .value = op_minus
    },
    (DictTriple_TokenTag) {
        .key = "*",
        .key_length = 1,
        .value = op_times
    },
    (DictTriple_TokenTag) {
        .key = "/",
        .key_length = 1,
        .value = op_slash
    },
    (DictTriple_TokenTag) {
        .key = "==",
        .key_length = 2,
        .value = op_is_equal
    },
    (DictTriple_TokenTag) {
        .key = "!=",
        .key_length = 2,
        .value = op_not_equal
    },
    (DictTriple_TokenTag) {
        .key = ">",
        .key_length = 1,
        .value = op_greater
    },
    (DictTriple_TokenTag) {
        .key = "<",
        .key_length = 1,
        .value = op_lesser
    },
    (DictTriple_TokenTag) {
        .key = "=",
        .key_length = 1,
        .value = op_slash
    }
};


LexerState lexer_create(const SafeStr* source) {
    LexerState temp;

    temp.specials = CALL_DICT_METHOD(TokenTag, create, lang_words, 9);
    temp.view = cstr_view_create(source->ptr, 0, source->length);
    temp.pos = 0;
    temp.end = (int)source->length;
    temp.line = 1;
    temp.col = 1;

    return temp;
}

void lexer_destroy(LexerState* self) {
    Dict_TokenTag* self_specials_ref = &self->specials;
    CALL_DICT_METHOD(TokenTag, destroy, self_specials_ref);
}

bool lexer_at_end(const LexerState* self) {
    return self->pos >= self->end;
}

Token lexer_lex_single(LexerState* self, TokenTag tag) {
    const auto start = self->pos;

    ++self->pos;

    return (Token) {
        .begin = start,
        .length = 1,
        .line = self->line,
        .col = self->col,
        .tag = tag,
    };
}

void lexer_update_file_pos(LexerState* self, char c) {
    if (c == '\n') {
        ++self->line;
        self->col = 1;
    } else {
        ++self->col;
    }
}

Token lexer_lex_spaces(LexerState* self) {
    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

        if (!match_spaces(symbol)) {
            break;
        }

        lexer_update_file_pos(self, symbol);
        ++self->pos;
        ++count;
    }

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = spaces
    };
}

Token lexer_lex_comment(LexerState* self) {
    lexer_update_file_pos(self, cstr_view_at(&self->view, (ptrdiff_t)self->pos));
    ++self->pos; // skip 1st '#'

    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

        if (symbol == '#') {
            // skip closing '#' of comment to prevent infinite lexing loop
            ++self->pos;
            break;
        }

        lexer_update_file_pos(self, symbol);
        ++self->pos;
        ++count;
    }

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = comment
    };
}

Token lexer_lex_word(LexerState* self) {
    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

        if (!match_alpha(symbol)) {
            break;
        }

        lexer_update_file_pos(self, symbol);
        ++self->pos;
        ++count;
    }

    const Dict_TokenTag* self_specials_ref = &self->specials;
    const TokenTag temp_tag = CALL_DICT_METHOD(TokenTag, get, self_specials_ref, &self->view, identifier);

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = temp_tag
    };
}

Token lexer_lex_numeric(LexerState* self) {
    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;
    auto dots = 0;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

        if (!match_numeric(symbol)) {
            break;
        }

        if (symbol == '.') {
            ++dots;
        }

        lexer_update_file_pos(self, symbol);
        ++self->pos;
        ++count;
    }

    TokenTag temp = unknown;

    if (dots == 0) {
        temp = literal_int;
    } else if (dots == 1) {
        temp = literal_float;
    }

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = temp
    };
}

Token lexer_lex_string(LexerState* self) {
    lexer_update_file_pos(self, cstr_view_at(&self->view, (ptrdiff_t)self->pos));
    ++self->pos; // skip 1st quote to avoid lexer infinite loop

    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;
    bool closed = false;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);
        lexer_update_file_pos(self, symbol);

        if (symbol == '\"') {
            ++self->pos; // see previous comment for reason
            closed = true;
            break;
        }

        ++self->pos;
        ++count;
    }

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = (closed) ? literal_string : unknown
    };
}

Token lexer_lex_operator(LexerState* self) {
    const auto start = self->pos;
    auto count = 0;
    const auto located_line = self->line;
    const auto located_col = self->col;

    while (!lexer_at_end(self)) {
        const auto symbol = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

        if (!match_op_symbol(symbol)) {
            break;
        }

        lexer_update_file_pos(self, symbol);
        ++self->pos;
        ++count;
    }

    const Dict_TokenTag* self_specials_ref = &self->specials;
    const TokenTag temp_tag = CALL_DICT_METHOD(TokenTag, get, self_specials_ref, &self->view, unknown);

    return (Token) {
        .begin = start,
        .length = count,
        .line = located_line,
        .col = located_col,
        .tag = temp_tag
    };
}

Token lexer_next(LexerState* self) {
    if (lexer_at_end(self)) {
        return (Token) {
            .begin = self->pos,
            .length = 1,
            .line = self->line,
            .col = self->col,
            .tag = eof
        };
    }

    const char peeked = cstr_view_at(&self->view, (ptrdiff_t)self->pos);

    switch (peeked) {
    case '#':
        return lexer_lex_comment(self);
    case '\n':
        return lexer_lex_single(self, lf);
    case ',':
        return lexer_lex_single(self, comma);
    case '[':
        return lexer_lex_single(self, bracket_left);
    case ']':
        return lexer_lex_single(self, bracket_right);
    case '(':
        return lexer_lex_single(self, paren_left);
    case ')':
        return lexer_lex_single(self, paren_right);
    case '{':
        return lexer_lex_single(self, brace_left);
    case '}':
        return lexer_lex_single(self, brace_right);
    default:
        break;
    }

    if (match_spaces(peeked)) {
        return lexer_lex_spaces(self);
    } else if (match_numeric(peeked)) {
        return lexer_lex_numeric(self);
    } else if (match_op_symbol(peeked)) {
        return lexer_lex_operator(self);
    } else if (match_alpha(peeked)) {
        return lexer_lex_word(self);
    }

    return lexer_lex_single(self, unknown);
}
