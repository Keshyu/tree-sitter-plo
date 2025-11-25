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
  externals: $ => [$._indent, $._dedent, $.comment, $.error_sentinel],
  extras: $ => [/\s/, $.comment],

  rules: {
    source_file: $ => repeat($._statement),
    _statement: $ => choice(
      $.assignment,
      $.call,
    ),
    assignment: $ => seq($.name, 'is', choice($.name, $.block)),
    call: $ => seq($.name, token.immediate('!')),
    block: $ => seq('do', choice(
      $._statement,
      seq($._indent, repeat1($._statement), $._dedent),
    )),
    name: $ => /[\w\-]+|"([^"]|\\")*"/,
    comment: $ => /`.*/,
  }
});

function sep(rule, separator) {
  return optional(sep1(rule, separator));
}

function sep1(rule, separator) {
  return seq(rule, repeat(seq(separator, rule)));
}
