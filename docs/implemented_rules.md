# Implemented Rules

The core engine implements all standard rules of Xiangi except for the following:

- Currently, there is no mechanism for declaring a draw.
- Repeat move patterns with a period longer than 4 configurations are not prohibited.
  - ABABAB, ABCABCABC, and ABCDABCDABCD are prohibited
  - Patterns of the form ABCD[X]ABCD[X]ABCD[X] are not prohibited