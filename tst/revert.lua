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
local commitId = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
local parentId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"

describe( "revert #revert", function()
   local repo = nil
   local err = nil
   local commit = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      commit, err = repo:lookup_commit( commitId )
   end)

   it("should init without errors", function()
      assert.is.falsy( err )
   end)

   it("revert", function()
      local index, err = repo:index()
      assert.is.falsy( err )
      local count =  #index

      assert.has_no_error( function()
         repo:revert( commit )
      end)

      local index, err = repo:index()
      assert.is.falsy( err )
      assert.are.equal( count -1, #index )
   end)
end)

describe( "revert_commit #revert", function()
   local repo = nil
   local err = nil
   local commit = nil
   local head = nil
   local count = 0
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      commit, err = repo:lookup_commit( commitId )
      if err then return end
      local ref, err = repo:head()
      if err then return end
      head, err = repo:lookup_commit( ref:target() )
      if err then return end

      local index, err = repo:index()
      count = #index
   end)

   it( "should initialize without errors", function()
      assert.is.falsy( err )
   end)

   it( "revert_commit", function()
      local index = nil
      local e = nil
         index, e = repo:revert_commit( commit, head, {} )
      assert.has_no_error( function()
      end)

      assert.is.falsy( e )
      assert.are.equal( count -1, #index )
   end)
end)
