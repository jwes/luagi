local lgit = require("lgit")
local head = "117d621c4a920dc1c9d706ee83ffce7d8d982b58"
local prev = "c78833afb82aa413f8ad2ad7cb9548af6e74c74c"

local repo, err = lgit.open(".")
if not repo then print( err ) return end
local tree, err = repo:lookup_tree( prev )
if not tree then print( err ) return end
print( tree, tree:entrycount() )
tree.walk()
