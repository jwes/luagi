local wien = require( "wien" );
local repo = wien.open(".");
local function print_flags ( flags )
   if flags 
   then
      for flag, bool in pairs( flags ) do
          if bool then
            print( "\t", flag )
         end
      end
   end
end


for path, flags in repo:status_for_each_ext() do
   -- ignore ignored files --
   if not  flags[ "ignored" ]
   then
      print( path )
      print_flags( flags )
   end
end

print "file status.lua: "
local flags = repo:status_file("status.lua")
print_flags( flags )

print( "should bla.swp be ignored?", repo:status_ignored("bla.swp") )

list, err = repo:status_list() 
if not list 
then
   print( err )
end
print( "status of ", #list, " elements" )

