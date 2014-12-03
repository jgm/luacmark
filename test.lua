#!/usr/bin/env lua

local cmark = require("cmark")

local expect = [[
<h1>A First Level Header</h1>
<h2>A Second Level Header</h2>
<p>Now is the time for all good men to come to
the aid of their country. This is just a
regular paragraph.</p>
<p>The quick brown fox jumped over the lazy
dog's back.</p>
<h3>Header 3</h3>
<blockquote>
<p>This is a blockquote.</p>
<p>This is the second paragraph in the blockquote<br />
with a br tag.</p>
<h2>This is an H2 in a blockquote</h2>
</blockquote>
]]

local result = cmark.markdown([[
A First Level Header
====================

A Second Level Header
---------------------

Now is the time for all good men to come to
the aid of their country. This is just a
regular paragraph.

The quick brown fox jumped over the lazy
dog's back.

### Header 3

> This is a blockquote.
>
> This is the second paragraph in the blockquote  
> with a br tag.
>
> ## This is an H2 in a blockquote
]])

if expect == result then
  print("Pass!")
else
  print("Fail!")
end

n = Node.new()
print(tostring(n))

