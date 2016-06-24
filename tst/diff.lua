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
   --diff
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local treeid = "064ee3e735ab74ab468be05c34e7f7fda011cb3b"
local tree2 = "2add3e2b70b08e7fa390dc24ba70920b78ed1076"

describe( "diff_tree_to_index #diff", function()
   local repo = nil
   local err = nil
   local diff = nil

   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      local index, err = repo:index()
      if err then return end
      local tree, err = repo:lookup_tree( treeid )
      if err then return end
      diff, err = repo:diff_tree_to_index(tree, index)
      if err then return end
   end)
   it("should have no error", function()
      assert.is.falsy( err )
   end)

   describe( "num_deltas #diff", function()
      it("should be four", function()
         assert.are.equal( 6, diff:num_deltas() )
      end)
   end)
   describe( "foreach #diff", function()
      it("should have 4 lines in 4 hunks", function()
         local files = 0
         local hunks = 0
         local lines = 0
         local function file()
            files = files + 1
            return 0
         end
         local function hunk()
            hunks = hunks + 1
            return 0
         end
         local function line( d, h, l)
            lines = lines + 1
            return 0
         end
         diff:foreach( file, hunk, line )
         assert.are.equal( 6, files )
         assert.are.equal( 6, hunks )
         assert.are.equal( 8, lines )
      end)
   end)

   describe( "get_delta #diff", function()
      it("should check bounds", function()
         local delta, err = diff:get_delta(0)
         assert.is.falsy( delta )
         assert.is.not_nil( err )
         local delta, err = diff:get_delta(1000)
         assert.is.falsy( delta )
         assert.is.not_nil( err )
      end)
      it("should return a valid delta", function()
         local delta, err = diff:get_delta(1)
         assert.is.falsy( err )
         assert.are.equal( ".gitignore", delta.old_file.path )
         assert.are.equal( 0, delta.old_file.size )
         assert.are.equal( ".gitignore", delta.new_file.path )
         assert.are.equal( 6, delta.new_file.size )
      end)
   end)
   describe( "is_sorted #diff", function()
      it("should be sorted", function()
         assert.is.False( diff:is_sorted() )
      end)
   end)
end)

describe( "diff_tree_to_tree #diff", function()
   local diff = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local t1, err = repo:lookup_tree( treeid )
      if err then return end
      local t2, err = repo:lookup_tree( tree2 )
      if err then return end
      diff, err = repo:diff_tree_to_tree( t1, t2 )
   end)

   it("should have no error", function()
      assert.is.falsy( err )
   end)

   it("should be four", function()
      assert.are.equal( 4, diff:num_deltas() )
   end)
end)

describe( "diff_index_to_workdir #diff", function()
   local diff = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local index, err = repo:index()
      if err then return end
      diff, err = repo:diff_index_to_workdir( index )
   end)

   it("should have no error", function()
      assert.is.falsy( err )
   end)

   it("should be zero", function()
      assert.are.equal( 0, diff:num_deltas() )
   end)
end)

local commit_id = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"

describe( "diff_tree_to_workdir #diff", function()
   local diff = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local tree, err = repo:lookup_tree( treeid )
      if err then return end
      diff, err = repo:diff_tree_to_workdir( tree )
   end)

   it("should have no error", function()
      assert.is.falsy( err )
   end)

   it("should be one", function()
      assert.are.equal( 1, diff:num_deltas() )
   end)
   describe( "print #diff", function()
      it("should call the callback line by line", function()
         local delta = nil
         local hunks = nil
         local lines = nil
         local function f( a, b, c )
            delta = a
            hunks = b
            lines = c
            return 0
         end
         diff:print( "name_status", f )
         assert.are.equal( 1, delta.number_of_files )
         assert.are.equal( 1, hunks.old_lines )
         assert.are.equal( 0, hunks.new_lines )
         assert.are.equal( "mode content\n", lines.content )
      end)
   end)
end)

describe( "diff_commit_as_email #diff", function()
   local repo = nil 
   local err = nil
   local commit = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      commit, err = repo:lookup_commit( commit_id )
   end)

   it( "should be initialized", function()
      assert.is.falsy( err )
      assert.is.not_nil( commit )
      assert.is.not_nil( commit.ancestor )
   end)

   it( "should return a email string", function()

      local result = repo:diff_commit_as_email( commit, 1, 1, {} )
      assert.is.truthy( result:find("Subject:" ))
      assert.is.truthy( result:find("From:" ))
      assert.is.truthy( result:find(".gitignore" ))
   end)
end)

describe( "diff_tree_to_workdir_with_index #diff", function()
   local diff = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local tree, err = repo:lookup_tree( treeid )
      if err then return end
      diff, err = repo:diff_tree_to_workdir_with_index( tree )
   end)

   it("should have no error", function()
      assert.is.falsy( err )
   end)

   it("should have six deltas", function()
      assert.are.equal( 6, diff:num_deltas() )
   end)

end)

describe( "merge #diff", function()
   local diff = nil
   local onto = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local tree, err = repo:lookup_tree( treeid )
      onto, err = repo:diff_tree_to_workdir( tree )
      if err then return end
      diff, err = repo:diff_tree_to_workdir_with_index( tree )
   end)

   it("should perform a merge", function()
      assert.are.equal( 1, onto:num_deltas() )
      onto:merge( diff )
      assert.are.equal( 6, onto:num_deltas() )
   end)
end)

describe( "format_email", function() pending("luagi_diff_format_email ") end)
describe( "find_similar #diff", function() pending("luagi_diff_find_similar ") end)

describe( "get_stats", function()
   local stats = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      local tree, err = repo:lookup_tree( treeid )
      if err then return end
      local diff, err = repo:diff_tree_to_workdir( tree )
      if err then return end
      stats, err = diff:get_stats()
   end)

   it( "should have no error", function()
      assert.is.falsy( err )
   end)

   describe( "files_changed #diff", function()
      it( "should return zero", function()
         assert.are.equal( 1, stats:files_changed() )
      end)
   end)
   describe( "insertions #diff", function()
      it( "should return zero", function()
         assert.are.equal( 0, stats:insertions() )
      end)
   end)
   describe( "deletions #diff", function()
      it( "should return zero", function()
         assert.are.equal( 1, stats:deletions() )
      end)
   end)
   describe( "__tostring #diff", function()
      it( "should return zero", function()
         assert.are.equal( "", tostring( stats ) )
      end)
   end)

end)
