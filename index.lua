local wien = require( "wien" )
local index, err =  wien.create_index()
if not index then print( err ); return end

function index_info( index )
   local caps = index:caps()
   for k, v in pairs(caps) do
      print( k, v )
   end
   print( "index", index:path(), "has", #index, "elements", index:has_conflicts() )
end

index_info( index )

local repo, err = wien.open(".")
if not repo then print( err ); return end

local rind, err = repo:index()
if not rind then print( err ); return end

index_info( rind )
