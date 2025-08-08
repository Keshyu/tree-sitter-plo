#include "tree_sitter/alloc.h"
#include "tree_sitter/array.h"
#include "tree_sitter/parser.h"

#include <assert.h>
#include <stdint.h>


typedef enum {
    EMBED_BEGIN,
    EMBED_CONTENT,
    EMBED_END,
    DUMMY,
    ERROR_SENTINEL,
} TokenType;

typedef struct {
    uint8_t embed_delim_len;
} Scanner;


bool tree_sitter_plo_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
    Scanner *scanner = (Scanner *)payload;
    if (valid_symbols[EMBED_BEGIN] && lexer->lookahead == '-') {
        // printf("1: %d\n", lexer->get_column(lexer));
        lexer->mark_end(lexer);
        scanner->embed_delim_len = 0;
        lexer->advance(lexer, false);
        scanner->embed_delim_len++;
        for (uint32_t i = 0; i < 2; i++) {
            if (lexer->lookahead == '-') {
                lexer->advance(lexer, false);
                scanner->embed_delim_len++;
            } else {
                return false;
            }
        }
        // printf("2: %d\n", lexer->get_column(lexer));
        while (lexer->lookahead == '-') {
            lexer->advance(lexer, false);
            scanner->embed_delim_len++;
        }
        lexer->mark_end(lexer);
        lexer->result_symbol = EMBED_BEGIN;
        return true;
    }
    else if (valid_symbols[EMBED_CONTENT]) {
        while (true) {
            while (lexer->lookahead != '-') {
                if (lexer->eof(lexer)) {
                    lexer->result_symbol = EMBED_CONTENT;
                    return true;
                }
                lexer->advance(lexer, false);
            }
            lexer->mark_end(lexer);

            uint8_t embed_delim_len = 0;
            lexer->advance(lexer, false);
            embed_delim_len++;
            while (lexer->lookahead == '-') {
                lexer->advance(lexer, false);
                embed_delim_len++;
                if (embed_delim_len == scanner->embed_delim_len) {
                    lexer->result_symbol = EMBED_CONTENT;
                    return true;
                }
            }
        }
    }
    else if (valid_symbols[EMBED_END] && lexer->lookahead == '-') {
        lexer->mark_end(lexer);
        uint8_t embed_delim_len = 0;
        lexer->advance(lexer, false);
        embed_delim_len++;
        for (uint32_t i = 0; i < 2; i++) {
            if (lexer->lookahead == '-') {
                lexer->advance(lexer, false);
                embed_delim_len++;
            } else {
                return false;
            }
        }
        while (lexer->lookahead == '-') {
            lexer->advance(lexer, false);
            embed_delim_len++;
        }
        if (embed_delim_len == scanner->embed_delim_len) {
            lexer->mark_end(lexer);
            lexer->result_symbol = EMBED_END;
            return true;
        } else {
            return false;
        }
    }
    else if (valid_symbols[ERROR_SENTINEL]) {
        return false;
    }
    else if (valid_symbols[DUMMY]) {
                    assert(false);
        lexer->result_symbol = DUMMY;
        return true;
    }
    else {
        return false;
    }
}

void *tree_sitter_plo_external_scanner_create() {
    return ts_malloc(sizeof(Scanner));
}

void tree_sitter_plo_external_scanner_destroy(void *payload) {
    ts_free((Scanner *)payload);
}

unsigned tree_sitter_plo_external_scanner_serialize(void *payload, char *buffer) {
    Scanner *scanner = (Scanner *)payload;
    unsigned size = 0;
    // buffer[size++] = (char)(scanner->embed_delim_len & 0xFF);
    // buffer[size++] = (char)((scanner->embed_delim_len >> 8) & 0xFF);
    buffer[size++] = (char)scanner->embed_delim_len;
    return size;
}

void tree_sitter_plo_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
    if (length == 0) return;

    Scanner *scanner = (Scanner *)payload;
    unsigned size = 0;
    // scanner->embed_delim_len = (uint8_t)buffer[size++]
    //                            | ((uint8_t)buffer[size++] << 8);
    scanner->embed_delim_len = (uint8_t)buffer[size++];
    assert(size == length);
}
