/**
 * @file Plo grammar for tree-sitter
 * @author Keshyu
 * @license GPLv3
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "plo",

  word: $ => $.name,

  rules: {
    source_file: $ => repeat($._anything),
    _anything: $ => choice(
      $.keyword,
      $.name,
      $.string,
      $.string_block,
      $.call,
      $.call_multi,
      $.punctuation,
      $.comment,
    ),
    keyword: $ => choice(
      'is', 'has', 'does', 'must',
      'be', 'do',

      'for',

      'of', 'at', 'as', 'by', 'with',
      'on', 'off',
      'from', 'to', 'upto', 'downto',
      'in', 'out', 'into', 'outof',
    ),
    name: $ => /\w([\w-]*\w)?/,
    string: $ => seq(
      '"',
      repeat(choice(
        /[^\n"\\]+/,
        /\\[^(]/,
        $.interpolation,
      )),
      '"',
    ),
    string_block: $ => seq(repeat1(/-`.*/), '--'),
    interpolation: $ => seq(
      '\\(',
      repeat($._anything),
      ')',
    ),
    call: $ => seq(
      field('func', $.name),
      token.immediate("("),
    ),
    call_multi: $ => seq(
      field('func', $.name),
      token.immediate(":"),
    ),
    punctuation: $ => /[\.:,;/()]/,
    comment: $ => /`.*/,
  }
});
