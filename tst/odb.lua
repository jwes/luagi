local luagi = require("luagi")
local test_helper = require("test_helper")

local object_id = "7a00e2b8fb8f22546224ffd11629dd93a5137d51"
local short_oid = "7a00e2b8fb8f225"

describe( "open_odb", function()
   local odb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      odb, err = luagi.open_odb( test_helper.path.."/.git/objects" )
   end)

   it("should have no errors", function()
      assert.is.falsy( err )
   end)
   it("should return a valid odb", function()
      assert.is.not_nil( odb )
      assert.are.equal( "userdata", type( odb ) )
   end)
   describe( "num_backends #odb", function()
      it("should have two backends", function()
         assert.are.equal( 2, odb:num_backends() )
      end)
   end)
   describe( "foreach #odb", function()
      local count = 0
      local contained = false

      odb:foreach( function( oid )
         count = count + 1
         if oid == object_id then
            contained = true
         end
         return 0
      end)

      it( "should contain the given oid", function()
         assert.is.True( contained )
      end)
      it( "should have many elements", function()
         assert.is.True( 10 < count )
      end)
   end)
   describe( "exists #odb", function()
      it( "should contain the oid", function()
         local res, oid = odb:exists( object_id )
         assert.is.True( res )
         -- complete, 
         assert.are.equal( oid, object_id )
      end)
      it( "should contain the prefixed oid", function()
         local res, oid = odb:exists( short_oid )
         assert.is.True( res ) 
         if res then
            assert.are.equal( object_id, oid )
         else
            assert.is.falsy( oid )
         end
      end)
      it( "should even accept very short variants", function()
         local res, oid = odb:exists( "6" ) 
         assert.is.True( res )
         -- ambiguous, therefore a id can not be produced
         assert.is.falsy( oid )
      end)
   end)
   describe( "read #odb", function()
      local odb_obj = nil
      local err = nil
      setup( function()
        odb_obj, err = odb:read( object_id ) 
     end)
     it( "should not have an error", function()
        assert.is.falsy( err )
     end)

     it( "should be a userdata obj", function()
        assert.is.not_nil( odb_obj )
        assert.are.equal( "userdata", type( odb_obj ) )
     end)

      describe( "id #odb", function()
         it( "should have the right id", function()
            assert.are.equal( object_id, odb_obj:id() )
         end)
      end)

      describe( "type #odb", function()
         it( "should be of type", function()
            assert.are.equal( "tree", odb_obj:type() )
         end)
      end)
      describe( "size #odb", function()
         it( "should have a size", function()
            assert.are.equal( 67, odb_obj:size())
         end)
      end)

      describe( "data #odb", function()
         data = odb_obj:data()
         assert.is.not_nil( data )
         assert.are.equal( 67, #data )
      end)
  end)
end)

describe( "new_odb", function()
   local odb = nil
   local err = nil
   setup( function()
      odb, err = luagi.new_odb()
   end)

   it("should have no errors", function()
      assert.is.falsy( err )
   end)
   it("should return a valid odb", function()
      assert.is.not_nil( odb )
      assert.are.equal( "userdata", type( odb ) )
   end)
   describe( "num_backends #odb", function()
      it("should have no backends", function()
         assert.are.equal( 0, odb:num_backends() )
      end)
   end)
end)

describe( "add_disk_alternate #odb", function() pending("luagi_odb_add_disk_alternate ") end)
describe( "read_header #odb", function() pending("luagi_odb_read_header ") end)
describe( "refresh #odb", function() pending("luagi_odb_refresh ") end)
describe( "write #odb", function() pending("luagi_odb_write ") end)
describe( "open_wstream #odb", function() pending("luagi_odb_open_wstream ") end)
describe( "open_rstream #odb", function() pending("luagi_odb_open_rstream ") end)
describe( "write_pack #odb", function() pending("luagi_odb_write_pack ") end)
describe( "add_backend #odb", function() pending("luagi_odb_add_backend ") end)
describe( "add_alternate #odb", function() pending("luagi_odb_add_alternate ") end)
describe( "get_backend #odb", function() pending("luagi_odb_get_backend ") end)
describe( "indexer #odb", function() pending("luagi_indexer_new ") end)
describe( "read #odb", function() pending("luagi_odb_stream_read ") end)
describe( "write #odb", function() pending("luagi_odb_stream_write ") end)
describe( "finalize_write #odb", function() pending("luagi_odb_stream_finalize_write ") end)
