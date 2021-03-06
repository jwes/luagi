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
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local parentId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
local function gen_setup()
   test_helper.setup()
   local repo, err = luagi.open( test_helper.path )
   if err then return repo, err  end
   local object, err = repo:lookup_object( parentId, "commit" )
   return repo, err, object
end 

describe( "reset #reset", function()
   local repo = nil
   local err = nil
   local object = nil
   setup( function()
      repo, err, object = gen_setup()
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should be unmodified", function()
      local flags = repo:status_file(".gitignore")
      for k, v in pairs( flags ) do
         assert.is.False( v )
      end
   end)

   describe("soft reset one revision", function()
      setup( function()
         repo:reset( object, "soft", {}, test_helper.signature )
      end)
      it("should not be available anymore", function()
         assert.is.falsy( io.open( test_helper.path.."/.gitignore", "r") )
      end)
   end)
end)

local function prepare_index(path)
   local repo, err, object = gen_setup()
   if err then return repo, err, object end
   local index, err = repo:index()
   if err then return repo, err, object end
   file, err = io.open(test_helper.path.."/"..path, "w")
   if err then return repo, err, object end
   file:write( "foo bar" )
   index:add_by_path( path )
   index:write()
   return repo, err, object
end

describe( "reset_default #reset", function()
   local repo = nil
   local err = nil
   local object = nil
   local path = "testfile"
   setup( function()
      repo, err, object = prepare_index(path) 
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should be unmodified", function()
      local flags = repo:status_file(path)
      assert.is.True( flags.index_modified )
   end)

   describe( "reset_default #reset", function()
      setup( function()
         path_spec = {}
         path_spec[1] = path
         repo:reset_default( object, path_spec )
      end)

      it("should not be available anymore", function()
         local flags = repo:status_file(path)
         assert.is.False( flags.index_modified )
         assert.is.True( flags.workdir_modified )
      end)
   end)
end)

describe( "reset_index #reset", function()
   local repo = nil
   local err = nil
   local path = "testfile"
   setup( function()
      repo, err = prepare_index(path) 
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should be unmodified", function()
      local flags = repo:status_file(path)
      assert.is.True( flags.index_modified )
   end)

   describe( "reset_index #reset", function()
      setup( function()
         path_spec = {}
         path_spec[1] = path
         repo:reset_index( path_spec )
      end)

      it("should not be available anymore", function()
         local flags = repo:status_file(path)
         assert.is.True( flags.index_deleted )
         assert.is.True( flags.workdir_new )
         assert.is.False( flags.workdir_modified )
      end)
   end)
end)
