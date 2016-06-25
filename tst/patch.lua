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
local luagi = require("luagi")
local test_helper = require("test_helper")

local treeid = "064ee3e735ab74ab468be05c34e7f7fda011cb3b"
local tree2 = "2add3e2b70b08e7fa390dc24ba70920b78ed1076"

describe( "patch #diff", function()
   local diff = nil
   local err = nil
   local patch = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local t1, err = repo:lookup_tree( treeid )
      if err then return end
      local t2, err = repo:lookup_tree( tree2 )
      if err then return end
      diff, err = repo:diff_tree_to_tree( t1, t2 )
      if err then return end
      patch, err = diff:patch(1)
   end)
   it("should be out of bounds", function()
      assert.is.falsy( err )
      local p, err = diff:patch(0)
      assert.is.falsy( p )
      assert.is.truthy( err:find("out of bounds") )
      err = nil
      local p, err = diff:patch(1000)
      assert.is.falsy( p )
      assert.is.truthy( err:find("out of bounds") )
      err = nil
   end)

   it("should have no error", function()
      assert.is.falsy( err )
      assert.is.not_nil( patch.get_delta )
   end)

   describe( "get_delta #patch", function()
      it("should return a valid delta", function()
         local delta = patch:get_delta()
         assert.is.True( delta.not_binary )
         assert.are.equal( 0, delta.similarity )
         assert.are.equal( ".gitignore", delta.new_file.path )
         assert.are.equal( ".gitignore", delta.old_file.path )
      end)
   end)
   describe( "num_hunks #patch", function()
      it("should have a hunk", function()
         assert.are.equal( 1, patch:num_hunks() )
      end)
   end)
   describe( "line_stats #patch", function()
      it("should return valid line stats", function()
         local context, additions, deletions = patch:line_stats()
         assert.are.equal( 0, context )
         assert.are.equal( 1, additions )
         assert.are.equal( 0, deletions )
      end)
   end)
   describe( "get_hunk #patch", function()
      it( "should check bounds", function()
         local hunk, err = patch:get_hunk(0)
         assert.is.falsy( hunk )
         assert.is.truthy( err:find( "out of bounds" ) )
         err = nil
         local hunk, err = patch:get_hunk(1000)
         assert.is.falsy( hunk )
         assert.is.truthy( err:find( "out of bounds" ) )
         err = nil
      end)
      it( "should return a hunk", function()
         local hunk, num_lines = patch:get_hunk(1)
         assert.are.equal( "@@ -0,0 +1 @@\n", hunk.header )
         assert.are.equal( 1 , num_lines )
      end)
   end)
   describe( "num_lines_in_hunk #patch", function()
      it("should return a line", function()
         assert.are.equal( 1, patch:num_lines_in_hunk( 1 ) )
      end)
   end)
   describe( "get_line_in_hunk #patch", function()
      it("should return a line", function()
         assert.are.equal( "*.ign\n", patch:get_line_in_hunk( 1, 1 ).content )
      end)
   end)
   describe( "size #patch", function()
      it("should calculate the size", function()
         assert.are.equal( 7, patch:size())
      end)
   end)
   describe( "to_buf #patch", function()
      it("should return a buffer", function()
         assert.are.truthy( patch:to_buf():find( "+++ b/.gitignore\n"))
      end)
   end)
end)

describe( "print #patch", function() pending("luagi_patch_print ") end)
describe( "patch_buffers #patch #base", function() pending("pending") end)
