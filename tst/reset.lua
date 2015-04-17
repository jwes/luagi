local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local parentId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"

describe( "reset #reset", function()
   local repo = nil
   local err = nil
   local object = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      object, err = repo:lookup_object( parentId, "commit" )
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should be unmodified", function()
      local flags = repo:status_file(".gitignore")
      for k, v in pairs( flags ) do
         assert.is.False( v )
      end
   end)

   describe("soft reset one revision", function()
      setup( function()
         repo:reset( object, "soft", test_helper.signature )
      end)
      it("should be not available anymore", function()
         assert.is.falsy( io.open( test_helper.path.."/.gitignore", "r") )
      end)
   end)

end)
describe( "reset_default #reset", function() pending("luagi_reset_default") end)
describe( "reset_index #reset", function() pending("luagi_reset_index") end)
