local luagi = require("luagi")
local test_helper = require("test_helper")
local err = nil

describe( "from repo #index", function()
   local index = nil
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      index, err = repo:index()
   end)

   it( "should have result", function()
      assert.is.not_nil( index )
      assert.are.equal( "userdata", type( index ) )
      assert.is.falsy( err )
   end)

   describe( "owner #index", function() 
      it( "should be owned by repo", function()
         assert.are.equal( repo:path(), index:owner():path() )
      end)
   end)
   describe( "caps #index", function() 
      it( "should return a table", function()
         local caps = index:caps()
         assert.is.not_nil( caps )
         for k, v in pairs( caps ) do
            assert.is.False( v )
         end
      end)
   end)
   describe( "__len #index", function() 
      it( "should have 3 elements", function()
         assert.are.equal( 3, #index )
      end)
   end)
   describe( "by_index #index", function()
      it( "should check bounds", function()
         local e, err = index:by_index(0)
         assert.is.falsy( e )
         assert.are.equal( "index out of bounds", err )
         e, err = index:by_index( #index + 1 )
         assert.is.falsy( e )
         assert.are.equal( "index out of bounds", err )
      end)
      it( "should return an entry", function()
         local e, err = index:by_index( #index )
         assert.is.not_nil( e )
         assert.is.falsy( err )
      end)
   end)

   describe( "path #index", function()
      it( "should return a valid path", function()
         local path = index:path()
         assert.is.not_nil( path )
         assert.is.truthy( path:find( test_helper.path.."/.git/index" ) )
      end)
   end)
   describe( "write #index", function()
      it("should not fail", function()
         assert.has_no_error( function() index:write() end)
      end)
   end)
   describe( "has_conflicts #index", function()
      it("should not have conflicts", function()
         assert.is.False( index:has_conflicts() )
      end)
   end)
end)

describe( "clear #index", function()
   local index = nil
   local err = nil
   setup(function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
   end)
   it( "should be setup without problems", function()
      assert.is.falsy( err )
   end)
   it( "should have 3 elements", function()
      assert.are.equal( 3, #index )
   end)

   it( "should have 0 elements after clear", function()
      index:clear()
      assert.are.equal( 0, #index )
   end)

end)

describe( "remove_by_path #index", function()
   local index = nil
   local err = nil
   setup(function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      index, err = repo:index()
   end)
   it( "should be setup without problems", function()
      assert.is.falsy( err )
   end)
   it( "should have 3 elements", function()
      assert.are.equal( 3, #index )
   end)
   it( "should have 2 elements after removal", function()
      assert.has_no_error( function()
         index:remove_by_path( "testfile")
      end)
      assert.are.equal( 2, #index )
      assert.has_no_error( function()
         index:remove_by_path( "some")
      end)
      assert.are.equal( 2, #index )
   end)
end)

describe( "remove #index", function()
   local index = nil
   local err = nil
   setup(function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      index, err = repo:index()
   end)
   it( "should be setup without problems", function()
      assert.is.falsy( err )
   end)
   it( "should have 3 elements", function()
      assert.are.equal( 3, #index )
   end)
   it( "should have 2 elements after removal", function()
      assert.has_no_error( function()
         index:remove( "testfile", "any" )
      end)
      assert.are.equal( 2, #index )
      assert.has_error( function()
         index:remove( "some", "any" )
      end)
      assert.are.equal( 2, #index )
   end)
end)

describe( "remove_directory #index", function()
   local index = nil
   local err = nil
   setup(function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      index, err = repo:index()
   end)
   it( "should be setup without problems", function()
      assert.is.falsy( err )
   end)
   it( "should have 3 elements", function()
      assert.are.equal( 3, #index )
   end)
   pending( "correct testcase" )
end)
describe( "remove_all #index", function()
   local index = nil
   local err = nil
   setup(function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      index, err = repo:index()
   end)
   it( "should be setup without problems", function()
      assert.is.falsy( err )
   end)
   it( "should have 3 elements", function()
      assert.are.equal( 3, #index )
   end)
   it( "should remove all", function()
      local ps = {}
      ps[1] = "*"
      index:remove_all( ps, function( path, matched_pathspec )
         assert.are.equal( ps[1], matched_pathspec )
         if path == ".gitignore" then
            return false
         else
            return 1
         end
      end)
      assert.are.equal( 1, #index )
   end)
end)

describe( "set_caps #index", function()
   local index = nil
   local err = nil
   local caps = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
      if err then return end
      caps = index:caps()
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.is.False( caps.ignore_case )
      assert.is.False( caps.no_filemode )
      assert.is.False( caps.no_symlinks )
      assert.is.False( caps.from_owner )
   end)
   it( "should set the caps", function()
      cap = {
         ignore_case = true,
      }
      assert.is.True( cap.ignore_case )
      assert.has_no_error( function()
         index:set_caps( cap )
      end)
      new_caps = index:caps()
      assert.is.True( new_caps.ignore_case )
      assert.is.False( new_caps.no_filemode )
      assert.is.False( new_caps.no_symlinks )
      assert.is.False( new_caps.from_owner )
   end)
end)

describe( "read_tree #index", function() pending("  luagi_index_read_tree ") end)
describe( "write_tree #index", function() pending("  luagi_index_write_tree ") end)
describe( "by_path #index", function()
   local index = nil
   local entry = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
      if err then return end
      entry, err = index:by_path("testfile", "any")
   end)

   it("should be an entry", function()
      assert.is.falsy( err )
      assert.is.not_nil( entry )
      assert.are.equal( "table", type( entry ) )
      assert.is.not_nil( entry.stage )
   end)

   describe( "entry_stage #index", function()
      it( "should return a stage", function()
         assert.are.equal( "clean", entry.stage )
      end)
   end)
end)

describe( "add #index", function()
   local index = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
      if err then return end
      index:clear()
      file = io.open( test_helper.path.."/testfile" )
      file:write( "foo" )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.are.equal( 0, #index )
   end)

   it( "shouldn't be able to add nil", function()
      local entry = nil
      assert.has_error( function()
         index:add( entry, "any" )
      end)
      assert.are.equal( 0, #index )
   end)
   it( "should be able to add the entry", function()
      local entry = {
         uid = 1000,
         gid = 1000,
         path = "testfile",
         oid = "d95f3ad14dee633a758d2e331151e950dd13e4ed",
         file_size = 8,
         dev = 65033,
         ino = 1704000,
         mode = 33188,
      }
      assert.has_no_error( function()
         index:add( entry, "any" )
      end)
      assert.are.equal( 1, #index )
   end)
   describe( "iterate_conflict #index", function()
      it("should iterate 0 times without error", function()
         local count = 0
         assert.has_no_error( function()
            for ancestor, our, their in index:iterate_conflict() do
               count = count + 1
            end
         end)
         assert.are.equal( 0, count )
      end)
   end)
end)

describe( "add_by_path #index", function()
   local index = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
      if err then return end
      index:clear()
      file = io.open( test_helper.path.."/testfile" )
      file:write( "foo" )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.are.equal( 0, #index )
   end)

   it( "should add the element", function()
      assert.has_no_error( function()
         index:add_by_path( "testfile" )
      end)
      assert.are.equal( 1, #index )
   end)
end)

describe( "add_all #index", function()
   local index = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
      if err then return end
      index:clear()
      file = io.open( test_helper.path.."/testfile" )
      file:write( "foo" )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.are.equal( 0, #index )
   end)

   it( "should add the element", function()
      local pathspec = {}
      pathspec[1] = "*"
      local flags = {
      }
      assert.has_no_error( function()
         index:add_all( pathspec, flags, function( path, matched_spec )
            return path == "testfile"
         end)
      end)
      assert.are.equal( 1, #index )
   end)
end)

describe( "update_all #index", function()
   local index = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo, err = luagi.open( test_helper.path )
      if err then return end
      index, err = repo:index()
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.are.equal( 3, #index )
   end)

   it( "should call all files once", function()
      local pathspec = {}
      pathspec[1] = "*"
      local count = 0
      function all ( path, matched )
         count = count + 1
         assert.are.equal( "*", matched )
      end

      assert.are.equal( 0, count )
      index:update_all( pathspec, all )
      assert.are.equal( #index, count )
   end)
end)

describe( "find #index", function() pending("  luagi_index_find ") end)
describe( "conflict_add #index", function() pending("  luagi_index_conflict_add ") end)
describe( "conflict_get #index", function() pending("  luagi_index_conflict_get ") end)
describe( "conflict_remove #index", function() pending("  luagi_index_conflict_remove ") end)
describe( "conflict_cleanup #index", function() pending("  luagi_index_conflict_cleanup ") end)
