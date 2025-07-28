((name) @type
 (#match? @type "^[A-Z]"))
(keyword) @keyword
(string) @string
(interpolation
  "\\(" @punctuation
  ")" @punctuation) @embedded
(call
  func: (name) @function
  "(" @punctuation)
(punctuation) @punctuation
