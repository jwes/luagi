   --refdb
local luagi = require("luagi")
local test_helper = require("test_helper")

describe( "new_refdb #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:new_refdb()
   end)

   it( "should have no error", function()
      assert.is.not_nil( refdb )
      assert.is.falsy( err )
      assert.are.equal( "userdata", type( refdb ) )
   end)
end)

describe( "open_refdb #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:open_refdb()
   end)

   it( "should have no error", function()
      assert.is.not_nil( refdb )
      assert.is.falsy( err )
      assert.are.equal( "userdata", type( refdb ) )
   end)

end)

describe( "compress #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:open_refdb()
   end)

   it( "should compress", function()
      assert.has_no_error( function() refdb:compress() end)
   end)
end)
