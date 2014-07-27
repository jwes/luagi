local wien = require( "wien" );
local repo = wien.open(".");

for path, flags in repo:status_for_each() do
   print( path, flags )
end

