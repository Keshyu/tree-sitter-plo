(keyword) @keyword
(name) @variable
((name) @type
  (#match? @type "^[A-Z]"))
(out_dependency
  "&" @operator)
(implicit_dependency
  "*" @operator)
(call
  func: (name) @function
  "(" @punctuation)
(call_multi
  func: (name) @function
  ":" @punctuation)
(inside_out_call
  "^-" @keyword
  (name) @function)
(string) @string
(string_block) @string
(embed
  (embed_begin) @string
  (embed_lang) @injection.language
  (embed_content) @injection.content
  (embed_end) @string)
(interpolation
  "\\(" @punctuation
  ")" @punctuation) @embedded
(punctuation) @punctuation
(comment) @comment
