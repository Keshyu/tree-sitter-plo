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
    source_file: $ => repeat(choice(
      $.keyword,
      $.name,
      $.string,
      $.call,
      $.punctuation,
    )),
    keyword: $ => choice(
      'is', 'has', 'does', 'must',
      'do',

      'of', 'at', 'as', 'by',
      'from', 'to',
      'in', 'out', 'into', 'outof',
      'upto', 'downto',
    ),
    name: $ => /\w([\w-]*\w)?/,
    string: $ => /"[^\n"]*"/,
    call: $ => seq(
      field('func', $.name),
      token.immediate('(')
    ),
    punctuation: $ => /[\.:,;/()]/,
  }
});
