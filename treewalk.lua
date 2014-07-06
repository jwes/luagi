local lgit = require("lgit")
local test_commit = "faa4d576cea4a7cab230cc5fde274becd41bac6d"

repo, err = lgit.open(".")
if not repo then print( err ) return end

local commit, err = repo:lookup_commit( test_commit );
if not commit then print( err ) return end

local tree, err = commit:tree()
if not tree then print( err ) return end

print( "starting walk" )
for root, entry in tree:walk("post") do
   print(entry:id(), root, entry:name(), entry:type(), entry:filemode() )
end
print( "ending walk" )
