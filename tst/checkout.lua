   --checkout
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local file = "testfile"

describe( "checkout_head #checkout", function()
   local repo = nil

   setup(function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      local file = io.open( test_helper.path.."/"..file, "w" )
      file:write( "foo, bar, baz" )
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( repo )

      local flags = repo:status_file( file )
      assert.is.True( flags.workdir_modified )
   end)

   describe( "checkout should revert the changes", function()
      setup(function()
         assert.has_no_error( function() repo:checkout_head({}) end) -- no options == dry-run

         local opts = {
            flags = {
               force = true,
            },
         }
         repo:checkout_head(opts) -- no options == dry-run
      end)

      it( "should be unmodified", function()
         local flags = repo:status_file( file )
         assert.is.False( flags.workdir_modified )
      end)
   end)
end)

describe( "checkout_index #checkout", function() pending("luagi_checkout_index") end)
describe( "checkout_tree #checkout", function() pending("luagi_checkout_tree") end)


