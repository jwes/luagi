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
   -- cherrypick
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local commit_id = "55c33519ff7e7ae27372a6f338e635b3c18cca57"
local our_commit_id = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"

describe( "cherry_pick #cherry_pick", function()
   local repo = nil
   local err = nil
   local commit = nil

   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      commit, err = repo:lookup_commit( commit_id )
   end)

   it( "should have a commit prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( commit )
   end)

   it( "should not produe an error", function()
      assert.has_no_error( function() repo:cherrypick( commit ) end)
   end)
end)

describe( "cherry_pick_commit #cherry_pick", function()
   local repo = nil
   local err = nil
   local commit = nil
   local our_commit = nil

   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      commit, err = repo:lookup_commit( commit_id )
      if err then return end
      our_commit, err = repo:lookup_commit( our_commit_id )
   end)

   it( "should have commits prepared", function()
      assert.is.falsy( err )
   end)

   it( "should return an index", function()
      local index, err = repo:cherrypick_commit( commit, our_commit, false, {} )
      assert.is.falsy( err )
      assert.is.not_nil( index )
      assert.are.equal( "userdata", type(index) )
   end)
end)
