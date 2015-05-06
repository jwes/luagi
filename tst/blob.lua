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


describe( "lookup_blob_prefix #blob", function()
   test_helper.extract()
   local repo = luagi.open( test_helper.path )
   blob, err = repo:lookup_blob( "69f48e5" )
   it( "should have a result", function()
      assert.is.falsy( err )
      assert.is.not_nil( blob )
   end)
end)


describe( "create_blob_from_workdir #blob", function()
   local repo = nil
   local relative = "fwkdir"
   local path = test_helper.path.."/"..relative
   local content = "some 'random' data"  

   setup( function ()   
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      
      local file = io.open( path, "w" )
      file:write( content ) 
      file:close()
   end)

   describe( "with opened repo", function()
      local oid, err = repo:create_blob_from_workdir( relative )
   
      it( "is valid", function()
         assert.is.falsy( err )
         assert.is.not_nil( oid )
      end)

      describe( "is a blob object", function()
         local blob, err = repo:lookup_blob( oid )
         it( "is valid", function()
            assert.is.falsy( err )
            assert.is.not_nil( blob )
         end)

         it( "can be read", function()
            assert.are.equal(content, blob:raw_content() )
         end)
      end)
   end)
end)

describe( "create_blob_from_disk #blob", function()
   local repo = nil
   local relative = "fwkdir"
   local path = test_helper.path.."/"..relative
   local content = "some 'random' data"  

   setup( function ()   
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      
      local file = io.open( path, "w" )
      file:write( content ) 
      file:close()
   end)

   describe( "with opened repo", function()
      local oid, err = repo:create_blob_from_disk( path )
   
      it( "is valid", function()
         assert.is.falsy( err )
         assert.is.not_nil( oid )
      end)

      describe( "is a blob object", function()
         local blob, err = repo:lookup_blob( oid )
         it( "is valid", function()
            assert.is.falsy( err )
            assert.is.not_nil( blob )
         end)

         it( "can be read", function()
            assert.are.equal(content, blob:raw_content() )
         end)
      end)
   end)
end)
describe( "create_blob_from_buffer #blob", function()
   local repo = nil
   local content = "some 'random' data"  

   setup( function ()   
      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   describe( "with opened repo", function()
      local oid, err = repo:create_blob_from_buffer( content )
   
      it( "is valid", function()
         assert.is.falsy( err )
         assert.is.not_nil( oid )
      end)

      describe( "is a blob object", function()
         local blob, err = repo:lookup_blob( oid )
         it( "is valid", function()
            assert.is.falsy( err )
            assert.is.not_nil( blob )
         end)

         it( "can be read", function()
            assert.are.equal(content, blob:raw_content() )
         end)
      end)
   end)
end)

describe( "create_blob_from_chunks #blob", function()
   local repo = nil
   local content = "some 'random' data"  
   local has_content = false
   function content_provider()
      if not has_content then
         has_content = true
         return content
      else
         return nil
      end
   end
   setup( function ()   
      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   describe( "with opened repo", function()
      local oid, err = repo:create_blob_from_chunks( content_provider )
   
      it( "is valid", function()
         assert.is.falsy( err )
         assert.is.not_nil( oid )
      end)

      describe( "is a blob object", function()
         local blob, err = repo:lookup_blob( oid )
         it( "is valid", function()
            assert.is.falsy( err )
            assert.is.not_nil( blob )
         end)

         it( "can be read", function()
            assert.are.equal(content, blob:raw_content() )
         end)
      end)
   end)
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

   describe( "filtered_content #blob", function()
      it( "has a content", function()
         assert.are.equal( [[*.ign
]], blob:filtered_content( "ignored.ign" ) )
      end)
   end)

   describe( "patch #blob", function()
      local master = nil
      local work = nil
      setup( function()
         master, err = repo:lookup_blob( "6d0839289fe61890ebfd5d0c57a5c7ccb691570d" )
         work, errw = repo:lookup_blob( "1178a1e6d258c5b90e861fc119f957fdddefbe47" )

         it( "should have both", function()
            assert.is.falsy( err )
            assert.is.falsy( errw )
            assert.is.not_nil( master )
            assert.is.not_nil( work )
         end)
      end)

      describe( "patch", function() 
         patch = master:patch( nil, work )
         ctx, add, del = patch:line_stats()
         it( "should have a line more", function()
            assert.are.equal( 1, add )
            assert.are.equal( 1, del )
            assert.are.equal( 0, ctx )
         end)
      end)
   end)

   describe( "patch_with_buffer #blob", function()
      buffer = [[NO MORE DATA]]
      patch = blob:patch_with_buffer(nil, buffer )
      ctx, add, del = patch:line_stats()

      it( "has differences", function()
         assert.are.equal( 1, add )
         assert.are.equal( 1, del )
      end)
   end)
end)
