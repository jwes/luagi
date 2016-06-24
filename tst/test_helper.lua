--[=[
  MIT License
  
  Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]=]--
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
   extract_path = extract_path,
   path = path,
   modification = modification,
   signature = signature,
   --functions
   setup = setup,
   modify = modify,
   extract = extract,
}

