local wien = require( "wien" );
local repo = wien.open(".");

for path, flags in repo:status_for_each() do
   -- ignore ignored files --
   if not  flags[ "ignored" ]
   then
      print( path )
      for flag, bool in pairs( flags ) do
         if bool then
            print( "\t", flag )
         end
      end
   end
end

