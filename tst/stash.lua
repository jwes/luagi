   --stash
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local stasher = {
   name = "A. Name",
   email = "has@mail.com"
}

describe( "stash #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher )
   end)

   it( "should not have an error", function()
      assert.is.falsy( err )
   end)

   it( "has an oid", function()
      assert.is.not_nil( oid )
   end)
end)

describe( "stash_for_each #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   local message = "the_message"
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher, message )
   end)

   it( "should have 1 stashed element", function()
      local i = 0
      local lid = nil
      local lmsg = nil
      local loid = nil
      repo:stash_for_each( function( id, msg, stash_oid )
         i = i + 1
         lid = id
         lmsg = msg
         loid = stash_oid
         return 0
      end)
      assert.is.not_nil( lmsg )
      assert.are.equal( oid, loid )
      assert.are.equal( 1, i )
   end)
end)

describe( "stash_drop #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   local message = "the_message"
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher, message )

      repo:stash_drop( 0 )
   end)
   it( "should have no elements", function()
      local i = 0
      repo:stash_for_each( function( id, msg, stash_oid )
         i = i + 1
      end)
      assert.are.equal( 0, i )
   end)
end)
