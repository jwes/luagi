local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local blobid = "69f48e529e7c65b4df2bdf7fd774ed18d934d5d1"

   -- blob
describe( "lookup_blob #blob", function ()
   test_helper.extract()
   local repo = luagi.open( test_helper.path )
   blob, err  = repo:lookup_blob( blobid )
   it( "should have a result", function()
      assert.is.falsy( err )
      assert.is.not_nil( blob )
   end)
end)


describe( "lookup_blob_prefix #blob", function() pending("luagi_blob_lookup_prefix") end)

describe( "create_blob_from_workdir #blob", function()
   local repo = nil
   setup( function ()   
      test_helper.extract()
      repo = luagi.open( test_helper.path )
   end)

   describe( "create_blob_from_disk #blob", function() pending("luagi_blob_create_fromdisk") end)
   describe( "create_blob_from_chunks #blob", function() pending("luagi_blob_create_fromchunks") end)
   describe( "create_blob_from_buffer #blob", function() pending("luagi_blob_create_frombuffer") end)

end)

describe( "with blob", function()
   local blob = nil
   local repo = nil
   setup( function ()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
      blob = repo:lookup_blob( blobid )
   end)
   
   describe( "id #blob", function()
      it( "should have equal ids", function()
         assert.are.equal( blobid, blob:id() )
      end)
   end)

   describe( "owner #blob", function()
      local owner, err = blob:owner()
      local ownerhead = owner:head()
      local repohead = repo:head()

      it( "should have an owner", function()
         assert.is.falsy( err )
         assert.is.not_nil( owner )
         assert.are.equal( repohead:name(), ownerhead:name() )
      end)
   end)

   describe( "filtered_content #blob", function() pending("luagi_blob_filtered_content ") end)
   describe( "is_binary #blob", function()
      it( "should be false", function()
         assert.is.False( blob:is_binary() )
      end)
   end)

   describe( "raw_size #blob", function()
      it( "has a defined size", function()
         assert.are.equal( 6, blob:raw_size() )
      end)
   end)

   describe( "raw_content #blob", function()
      it( "has a content", function()
         assert.are.equal( [[*.ign
]], blob:raw_content() )
      end)
   end)

   describe( "patch #blob", function() pending("luagi_patch_from_blobs ") end)
   describe( "patch_with_buffer #blob", function()
      buffer = [[NO MORE DATA]]
      patch = blob:patch_with_buffer(nil, buffer )
      total, add, del = patch:line_stats()

      it( "has differences", function()
         assert.are.equal( 1, add )
         assert.are.equal( 1, del )
      end)
   end)
end)
