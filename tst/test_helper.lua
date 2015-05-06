local os = require("os")
local extract_path = "data"
local repo_name = "test_repo"
local remote_name = "remote"
local submodule_name = "submodule"
local path = extract_path.."/"..repo_name
local remote_path = extract_path.."/"..remote_name
local submodule_path = extract_path.."/"..submodule_name

local signature = {
   name = "A. Tester",
   email = "some@mail.address"
}

local function extract()
   if not io.open( path ) then
      os.execute( "mkdir -p "..extract_path )
      os.execute( "unzip -q repo.zip -d "..extract_path)
   end
end
local function setup()
      os.execute( "rm -rf "..path )
      os.execute( "rm -rf "..remote_path )
      os.execute( "rm -rf "..submodule_path )
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
   remote_path = remote_path,
   submodule_path = submodule_path,
   path = path,
   modification = modification,
   signature = signature,
   --functions
   setup = setup,
   modify = modify,
   extract = extract,
}

