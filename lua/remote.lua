local luagi = require( "luagi" )

local repo, err = luagi.open( "." )
if not repo then print( err ) exit(1) end

for i, r in ipairs( repo:remotes() ) do
   print( i, r )
   local remote, err = repo:load_remote( r )
   if not remote then print( err ) exit(1) end

   print( remote:name() )
   print( remote:url() )
   print( remote:pushurl() )

   local heads, err = remote:ls()
   print( err )
   -- unconneced ls is not allowed --
   if heads then exit(1) end 


end


