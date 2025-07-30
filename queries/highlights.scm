(name) @variable
((name) @type
  (#match? @type "^[A-Z]"))
(out_dependency
  "&" @operator)
(implicit_dependency
  "*" @operator)
(keyword) @keyword
(string) @string
(string_block) @string
(interpolation
  "\\(" @punctuation
  ")" @punctuation) @embedded
(call
  func: (name) @function
  "(" @punctuation)
(call_multi
  func: (name) @function
  ":" @punctuation)
(punctuation) @punctuation
(comment) @comment
