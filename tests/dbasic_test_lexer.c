#include <stdio.h>
#include "utils/files.h"
#include "utils/cstrview.h"
#include "frontend/token.h"
#include "frontend/lexer.h"

[[nodiscard]] bool test_lexing(const char* file_path) {
    SafeStr src_str = read_file_sync(file_path, blob_mode_text);

    if (!safe_str_valid(&src_str)) {
        return false;
    }

    LexerState lexer = lexer_create(&src_str);
    Token temp;

    do {
        temp = lexer_next(&lexer);

        if (temp.tag == unknown) {
            fprintf(stderr, "Lexical Error at [%d:%d], unknown token.\n", temp.line, temp.col);
            lexer_destroy(&lexer);
            safe_str_destroy(&src_str);
            return false;
        }
    } while (temp.tag != eof);

    lexer_destroy(&lexer);
    safe_str_destroy(&src_str);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("usage: ./dbasic_test_lexer <file path>\n");
        return 1;
    }

    if (!test_lexing(argv[1])) {
        return 1;
    }
}
