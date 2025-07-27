((name) @type
 (#match? @type "^[A-Z]"))
(keyword) @keyword
(string) @string
(call
  func: (name) @function
  "(" @punctuation)
(punctuation) @punctuation
