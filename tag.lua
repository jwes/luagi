local luagi = require("luagi")

local repo, err = luagi.open(".")
if not repo then print( err ) return end

function print_tag( name, oid )
   print( name, oid )
end

repo:foreach_tag( print_tag )

