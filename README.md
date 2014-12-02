luacmark
==========

A simple lua binding to
[CommonMark](https://github.com/jgm/CommonMark), a C
library that can parse
[CommonMark](http://commonmark.org/).

Usage
-----

    local cmark = require("cmark")
    local markdown_string = [[
    This is an
    example [CommonMark](http://commonmark.org/)
    paragraph.
    ]]
    local html_string = cmark.markdown(markdown_string)
