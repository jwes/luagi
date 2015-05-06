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
end)

describe( "diff_tree_to_workdir_with_index #diff", function() pending("luagi_diff_tree_to_workdir_with_index") end)
describe( "diff_commit_as_email #diff", function() pending("luagi_diff_commit_as_email") end)

describe( "print #diff", function() pending("print") end)

describe( "merge #diff", function() pending("luagi_diff_merge ") end)
describe( "find_similar #diff", function() pending("luagi_diff_find_similar ") end)
describe( "format_email #diff", function() pending("luagi_diff_format_email ") end)

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
