/**
 * @file Plo grammar for tree-sitter
 * @author Keshyu
 * @license GPLv3
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "plo",

  word: $ => $.word,
  externals: $ => [$._indent, $._dedent, $._linebreak, $.error_sentinel],
  supertypes: $ => [$.phrase],

  rules: {
    source_file: $ => optional(seq(
      repeat(seq($.phrase, choice($._linebreak, ';'))),
      seq($.phrase, $._linebreak),
    )),
    phrase: $ => choice(
      $.comma_phrase,
      $.space_phrase,
      $.block_phrase,
      $.dot_phrase,
      $.paren_phrase,
      $.paren,
      $.word,
    ),
    comma_phrase: $ => sep2(',', choice(
      $.space_phrase,
      $.block_phrase,
      $.dot_phrase,
      $.paren_phrase,
      $.paren,
      $.word,
    )),
    space_phrase: $ => seq(
      choice(
        $.block_phrase,
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
      choice(
        $.space_phrase,
        $.block_phrase,
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
    ),
    block_phrase: $ => seq(
      choice(
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
      $._indent,
      $.block,
      $._dedent,
    ),
    block: $ => sep1(choice($._linebreak, ';'), $.phrase),
    dot_phrase: $ => seq(
      choice(
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
      '.',
      choice(
        $.paren_phrase,
        $.paren,
        $.word,
      ),
    ),
    paren_phrase: $ => seq(
      $.word,
      alias($._glued_paren, $.paren),
    ),
    _glued_paren: $ => seq(
      token.immediate('('),
      optional($._paren_content),
      ')',
    ),
    paren: $ => seq('(', optional($._paren_content), ')'),
    _paren_content: $ => choice(
      alias($._inline_comma_phrase, $.comma_phrase),
      alias($._inline_space_phrase, $.space_phrase),
      $.dot_phrase,
      $.paren_phrase,
      $.paren,
      $.word,
    ),
    _inline_comma_phrase: $ => sep2(',', choice(
      alias($._inline_space_phrase, $.space_phrase),
      $.dot_phrase,
      $.paren_phrase,
      $.paren,
      $.word,
    )),
    _inline_space_phrase: $ => seq(
      choice(
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
      choice(
        $.space_phrase,
        $.dot_phrase,
        $.paren_phrase,
        $.paren,
        $.word,
      ),
    ),
    word: $ => /\w+/,
  }
});

function sep2(separator, rule) {
  return seq(rule, repeat1(seq(separator, rule)));
}

function sep1(separator, rule) {
  return seq(rule, repeat(seq(separator, rule)));
}

function sep(separator, rule) {
  return optional(sep1(separator, rule));
}

function repeat2(rule) {
  return seq(rule, repeat1(rule));
}
