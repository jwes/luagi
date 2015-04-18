--reflog
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

local reflog_name = "foo"
local commit = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"

describe( "read_reflog #reflog", function()
   local repo = nil
   local err = nil
   local reflog = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      reflog, err = repo:read_reflog( reflog_name )
   end)

   it("should be error free", function()
      assert.is.falsy( err )
   end)

   it("should be empty", function()
      assert.are.equal( 0, reflog:count())
   end)

   describe( "append #reflog", function()
      setup( function()
         reflog:append( commit, test_helper.signature, "bla" )
      end)

      it( "should have one entry", function()
         assert.are.equal( 1, reflog:count() )
      end)

      describe( "entry_by_index #reflog", function()
         it("should be out of bounds", function()
            local entry, err = reflog:entry_by_index( 0 )
            assert.is.not_nil( err )
         end)
         it("should be out of bounds", function()
            local entry, err = reflog:entry_by_index( 2 )
            assert.is.not_nil( err )
         end)
         it("should return an entry", function()
            local entry, err = reflog:entry_by_index( 1 )
            assert.is.falsy( err )
            assert.are.equal( "table", type( entry ))
         end)
      end)

   end)
end)
describe( "rename_reflog #reflog", function() pending("luagi_reflog_rename") end)
describe( "delete_reflog #reflog", function() pending("luagi_reflog_delete") end)
describe( "write #reflog", function() pending("luagi_reflog_write ") end)
describe( "drop #reflog", function() pending("luagi_reflog_drop ") end)
