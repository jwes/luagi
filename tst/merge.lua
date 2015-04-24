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

describe( "merge_base_octopus #merge", function() pending("luagi_merge_base_octopus") end)

describe( "merge_head_from_ref #merge", function() pending("luagi_merge_head_from_ref") end)
describe( "merge_head_from_fetchhead #merge", function() pending("luagi_merge_head_from_fetchhead") end)
describe( "merge_head_from_id #merge", function() pending("luagi_merge_head_from_id") end)
describe( "merge_trees #merge", function() pending("luagi_merge_trees") end)
describe( "merge_commits #merge", function() pending("luagi_merge_commits") end)
describe( "merge #merge", function() pending("luagi_merge") end)
describe( "merge_file_from_index #merge", function() pending("luagi_merge_file_from_index") end)
describe( "merge_analysis #merge", function() pending("luagi_merge_analysis") end)

describe( "__tostring #merge", function() pending("luagi_merge_head_id ") end)
describe( "automergable #merge", function() pending("luagi_mergeresult_automergable ") end)
describe( "path #merge", function() pending("luagi_mergeresult_path ") end)
describe( "mode #merge", function() pending("luagi_mergeresult_mode ") end)
describe( "content #merge", function() pending("luagi_mergeresult_content ") end)

describe( "merge_files #merge #base", function() pending("pending") end)
