#include <stdio.h>
#include "frontend/token.h"
#include "frontend/lexer.h"
#include "utils/safestr.h"

static const char* fake_code = "# hello world #\n"
"DEFUN main WITH args DO\n"
"    PRINT \"Hello World!\"\n"
"END\n";

[[nodiscard]] bool test_lexing(const char* source) {
    SafeStr source_str = safe_str_create_from(source);

    if (!safe_str_valid(&source_str)) {
        perror("test_lexing: failed to allocate safe string!\n");
        safe_str_destroy(&source_str);
        return false;
    }

    LexerState lexer = lexer_create(&source_str);
    Token temp;

    do {
        temp = lexer_next(&lexer);

        if (temp.tag == unknown) {
            fprintf(stderr, "Lexical error at source [%d:%d], unknown token!\n", temp.line, temp.col);
            return false;
        }
    } while (temp.tag != eof);

    lexer_destroy(&lexer);
    safe_str_destroy(&source_str);
    return true;
}

int main() {
    if (!test_lexing(fake_code)) {
        return 1;
    }
}
