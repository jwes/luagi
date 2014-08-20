local wien = require( "wien" )
local index, err =  wien.create_index()
if not index then print( err ); return end

local caps = index:caps()
for k, v in pairs(caps) do
   print( k, v )
end
print( "index", index:path(), "has", #index, "elements", index:has_conflicts() )
