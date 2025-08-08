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

  externals: $ => [
    $.embed_begin,
    $.embed_content,
    $.embed_end,
    $._dummy,
    $._error_sentinel,
  ],

  rules: {
    source_file: $ => seq($._dummy, repeat($._anything)),
    _anything: $ => choice(
      $._experimental,
      $.keyword,
      $.name,
      $.out_dependency,
      $.implicit_dependency,
      $.string,
      $.string_block,
      $.embed,
      $.call,
      $.call_multi,
      $.punctuation,
      $.comment,
    ),
    keyword: $ => choice(
      'is', 'has', 'does', 'must',
      'be', 'do',

      'uses', 'but', 'also', 'and', 'or',

      'of', 'at', 'as', 'by', 'with', 'for',
      'from', 'to', 'upto', 'downto',
      'in', 'out', 'into', 'outof',
      'on', 'off',
    ),
    name: $ => /\w([\w-]*\w)?/,
    out_dependency: $ => seq(
      '&',
      token.immediate(/\w([\w-]*\w)?/),
    ),
    implicit_dependency: $ => seq(
      '*',
      token.immediate(/\w([\w-]*\w)?/),
    ),
    call: $ => seq(
      field('func', $.name),
      token.immediate("("),
    ),
    call_multi: $ => seq(
      field('func', $.name),
      token.immediate(":"),
    ),
    _experimental: $ => choice($.inside_out_call),
    inside_out_call: $ => seq('^-', $.name),
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
    embed: $ => seq(
      $.embed_begin,
      // alias(token.immediate(/\w+/), $.embed_lang),
      // alias(/\w+/, $.embed_lang),
      $.embed_content,
      $.embed_end,
    ),
    interpolation: $ => seq(
      '\\(',
      repeat($._anything),
      ')',
    ),
    punctuation: $ => /[\.:,;/()]/,
    comment: $ => /`.*/,
  }
});
