local os = require("os")
local path = "data/test_repo"

local function setup()
      print( path )

      os.execute( "rm -rf "..path )
      os.execute( "mkdir -p "..path )
      os.execute( "unzip -q repo.zip -d "..path)

   end

return {
   path = path,
   setup = setup
}

