   --merge
local luagi = require("luagi")
local test_helper = require("test_helper")

local head_master = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
local head_work = "55c33519ff7e7ae27372a6f338e635b3c18cca57"
local base_ref = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"

describe( "merge_base #merge", function()
   local repo = nil
   local err = nil
   local base = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      assert.has_no_error( function()
         base, err =  repo:merge_base( head_work, head_master )
      end)
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should have base ref as base", function()
      assert.are.equal( base_ref, base )
   end)

end)

describe( "merge_base_many #merge", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should have base ref as base", function()
      local input = {}
      input[1] = head_master
      input[2] = head_work
      local base = nil
      assert.has_no_error( function()
         base, err =  repo:merge_base_many( input )
      end)
      assert.is.falsy( err )
      assert.are.equal( base_ref, base )
   end)

   it( "should need two oids", function()
      local input = {}
      input[1] = head_master
      local base = nil
      assert.has_no_error( function()
         base, err =  repo:merge_base_many( input )
      end)
      assert.is.falsy( base )
      assert.is.truthy( err:find("least two commits") )
   end)

   it( "should not be too picky", function()
      local input = {}
      input[1] = head_master
      input[2] = head_master
      input[3] = head_master
      input[4] = head_master
      input[5] = head_master
      local base = nil
      assert.has_no_error( function()
         base, err =  repo:merge_base_many( input )
      end)
      assert.is.falsy( err )
      assert.are.equal( head_master, base )
   end)

end)

describe( "merge_base_octopus #merge", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "should find base_ref", function()
      local input = {}
      input[1] = head_master
      input[2] = head_work
      input[3] = head_master
      input[4] = head_work
      input[5] = base_ref
      local base = nil
      assert.has_no_error( function()
         base, err =  repo:merge_base_octopus( input )
      end)
      assert.is.falsy( err )
      assert.are.equal( base_ref, base )
   end)
end)

describe( "merge_head_from_ref #merge", function()
   local repo = nil
   local err = nil
   local merge_head = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      local ref,  err = repo:lookup_reference( "refs/heads/work" )
      if err then return end
      merge_head, err = repo:merge_head_from_ref( ref )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( merge_head )
      assert.are.equal( "userdata", type( merge_head ) )
   end)
end)

describe( "merge_head_from_fetchhead #merge", function()
   pending( "set up test remote")
end)

describe( "merge_head_from_id #merge", function()
   local repo = nil
   local err = nil
   local merge_head = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      merge_head, err = repo:merge_head_from_id( head_work )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( merge_head )
      assert.are.equal( "userdata", type( merge_head ) )
      assert.is.not_nil( merge_head.__tostring )
   end)
end)

describe( "merge_trees #merge", function() pending("luagi_merge_trees") end)
describe( "merge_commits #merge", function() pending("luagi_merge_commits") end)
describe( "merge #merge", function() pending("luagi_merge") end)
describe( "merge_file_from_index #merge", function() pending("luagi_merge_file_from_index") end)
describe( "merge_analysis #merge", function() pending("luagi_merge_analysis") end)

describe( "merge_files #merge #base", function()
   local repo = nil
   local err = nil
   local result = nil
   local path = test_helper.path.."/testfile"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      local file = io.open( path , "r" )
      local content = file:read("*all")

      local ancestor = {}
      ancestor[ "path" ] = path
      ancestor[ "content" ] = content
      local ours  = {}
      ours[ "path" ] = path
      ours[ "content" ] = content.."ours"
      local theirs  = {}
      theirs[ "path" ] = path
      theirs[ "content" ] = content.."theirs"
      result, err = luagi.merge_files( ancestor, ours, theirs )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( result )
      assert.are.equal( "userdata", type( result ) )
      assert.is.not_nil( result.automergable )
   end)

   describe( "automergable #merge", function()
      it("should produce conflicts", function()
         assert.is.False( result:automergable() )
      end)
   end)

   describe( "path #merge", function()
      it( "should return path", function()
         assert.are.equal( path, result:path() )
      end)
   end)

   describe( "mode #merge", function()
      it( "should return the filemode", function()
         assert.are.equal( 33188, result:mode() )
      end)
   end)
   describe( "content #merge", function()
      it( "should return a mergable content", function()
         assert.is.truthy( result:content():find( "<<<<<" ) )
      end)
   end)
end)
