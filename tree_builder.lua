local wien = require("wien")

repo, err = wien.open(".")
if not repo then print( err ) return end

local builder, err = wien.tree_builder()
if not builder then print( err ) return end
function filter ( entry )
	print( entry:name() )
	return false; 
end

print( builder:entry_count() )


