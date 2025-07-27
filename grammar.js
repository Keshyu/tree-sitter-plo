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
      $.punctuation,
    )),
    keyword: $ => choice(
      'is', 'has', 'does', 'must',
      'do',

      'of', 'from', 'to', 'in', 'outof', 'at',
      'into', 'upto', 'downto',
      'as',
    ),
    name: $ => /\w([\w-]*\w)?/,
    punctuation: $ => /[\.:,;/()]/,
  }
});
