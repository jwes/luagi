local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

local commit = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
local t = "commit"

describe( "object", function()
   local repo = nil
   local obj = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      obj, err = repo:lookup_object( commit, t )
   end)

   it( "should have no error", function()
      assert.is.falsy( err )
   end)

   describe( "id #object", function()
      it ("should have the right id", function()
         assert.are.equal( commit, obj:id() )
      end)
   end)

   describe( "type #object", function()
      it( "should be a commit", function()
         assert.are.equal( t, obj:type() )
      end)
   end)
end)
