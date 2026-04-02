#include "tree_sitter/array.h"
#include "tree_sitter/parser.h"

enum TokenType {
    INDENT,
    DEDENT,
    LINEBREAK,
    ERROR_SENTINEL,
};

typedef struct {
    uint16_t indent;
} Scanner;

bool tree_sitter_plo_external_scanner_scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    if (valid_symbols[ERROR_SENTINEL]) {
        return false;
    }

    Scanner *scanner = (Scanner *)payload;
    bool found_end_of_line = false;
    uint16_t curr_indent = 0;

    lexer->mark_end(lexer);

    for (;;) {
        if (lexer->lookahead == '\n') {
            found_end_of_line = true;
            curr_indent = 0;
            lexer->advance(lexer, true);
        } else if (lexer->lookahead == ' ') {
            curr_indent++;
            lexer->advance(lexer, true);
        } else if (lexer->lookahead == '\t') {
            curr_indent += 8;
            lexer->advance(lexer, true);
        } else if (lexer->lookahead == '\r' || lexer->lookahead == '\f') {
            lexer->advance(lexer, true);
        } else if (lexer->eof(lexer)) {
            found_end_of_line = true;
            curr_indent = 0;
            break;
        } else {
            break;
        }
    }

    if (found_end_of_line) {
        if (valid_symbols[INDENT] && curr_indent > scanner->indent) {
            scanner->indent = curr_indent;
            lexer->result_symbol = INDENT;
            return true;
        }
        if (valid_symbols[DEDENT] && curr_indent < scanner->indent) {
            scanner->indent = curr_indent;
            lexer->result_symbol = DEDENT;
            return true;
        }
        if (valid_symbols[LINEBREAK]) {
            lexer->result_symbol = LINEBREAK;
            return true;
        }
    }

    return false;
}

unsigned tree_sitter_plo_external_scanner_serialize(
    void *payload,
    char *buffer
) {
    Scanner *scanner = (Scanner *)payload;
    size_t size = 0;
    buffer[size++] = (char)(scanner->indent & 0xFF);
    buffer[size++] = (char)((scanner->indent >> 8) & 0xFF);
    return size;
}

void tree_sitter_plo_external_scanner_deserialize(
    void *payload,
    const char *buffer,
    unsigned length
) {
    Scanner *scanner = (Scanner *)payload;
    if (length > 0) {
        size_t size = 0;
        scanner->indent = (unsigned char)buffer[size++];
        scanner->indent |= (unsigned char)buffer[size++] << 8;
    }
}

void *tree_sitter_plo_external_scanner_create() {
    Scanner *scanner = calloc(1, sizeof(Scanner));
    scanner->indent = 0;
    return scanner;
}

void tree_sitter_plo_external_scanner_destroy(void *payload) {
    Scanner *scanner = (Scanner *)payload;
    free(scanner);
}
