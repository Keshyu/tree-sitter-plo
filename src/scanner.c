#include "tree_sitter/array.h"
#include "tree_sitter/parser.h"


enum TokenType {
    INDENT,
    DEDENT,
    COMMENT,
    ERROR_SENTINEL
};

typedef struct {
    uint16_t indent_length;
} Scanner;

static inline void advance(TSLexer *lexer) { lexer->advance(lexer, false); }

static inline void skip(TSLexer *lexer) { lexer->advance(lexer, true); }

bool tree_sitter_plo_external_scanner_scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    Scanner *scanner = (Scanner *)payload;

    if (valid_symbols[ERROR_SENTINEL]) {
        return false;
    }

    bool found_end_of_line = false;
    uint16_t curr_indent_length = 0;

    for (;;) {
        if (lexer->lookahead == '\n') {
            found_end_of_line = true;
            curr_indent_length = 0;
            skip(lexer);
        } else if (lexer->lookahead == ' ') {
            curr_indent_length++;
            skip(lexer);
        } else if (lexer->lookahead == '\r' || lexer->lookahead == '\f') {
            curr_indent_length = 0;
            skip(lexer);
        } else {
            break;
        }
    }

    if (found_end_of_line) {
        if (valid_symbols[INDENT] && curr_indent_length > scanner->indent_length) {
            scanner->indent_length = curr_indent_length;
            lexer->result_symbol = INDENT;
            return true;
        }
        if (valid_symbols[DEDENT] && curr_indent_length < scanner->indent_length) {
            scanner->indent_length = curr_indent_length;
            lexer->result_symbol = DEDENT;
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
    buffer[size++] = (char)(scanner->indent_length & 0xFF);
    buffer[size++] = (char)((scanner->indent_length >> 8) & 0xFF);
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
        scanner->indent_length = (unsigned char)buffer[size++];
        scanner->indent_length |= (unsigned char)buffer[size++] << 8;
    }
}

void *tree_sitter_plo_external_scanner_create() {
    Scanner *scanner = calloc(1, sizeof(Scanner));
    scanner->indent_length = 0;
    return scanner;
}

void tree_sitter_plo_external_scanner_destroy(void *payload) {
    Scanner *scanner = (Scanner *)payload;
    free(scanner);
}
