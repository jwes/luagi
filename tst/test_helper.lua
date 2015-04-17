local os = require("os")
local path = "data/test_repo"

local signature = {
   name = "A. Tester",
   email = "some@mail.address"
}

local function extract()
   if not io.open( path ) then
      os.execute( "mkdir -p "..path )
      os.execute( "unzip -q repo.zip -d "..path)
   end
end
local function setup()
      os.execute( "rm -rf "..path )
      extract()
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
   signature = signature,
   --functions
   setup = setup,
   modify = modify,
   extract = extract,
}

