local os = require("os")
local path = "data/test_repo"

local function setup()
      os.execute( "rm -rf "..path )
      os.execute( "mkdir -p "..path )
      os.execute( "unzip -q repo.zip -d "..path)
   end

local modification = ".........\n" 

local function modify( path )
   local file = io.open( path, "a" )
   file:write( modification ) 
   file:close()
end

return {
   --parameters
   path = path,
   modification = modification,
   --functions
   setup = setup,
   modify = modify,
}

