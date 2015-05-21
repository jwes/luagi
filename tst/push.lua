   --push
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "push #push", function()
   local repo = nil
   local remote = nil
   local err = nil
   local push = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:load_remote( "origin" )
      if err then return end
      push, err = remote:new_push( )
   end)

   it("should have no errors", function()
      assert.is.falsy( err )
      assert.is.not_nil( push )
      assert.is.not_nil( push.foreach_status )
   end)
   describe( "set_options #push", function()
      it("should not throw an error", function()
         assert.has_no_error( function()
            local opts = {
               threads = 4,
            }
            push:set_options(opts)
         end)
      end)
   end)
   describe( "finish #push", function()
      it("should not throw an error", function()
         assert.has_no_error( function()
            remote:set_local_transport()
            remote:set_url( test_helper.remote_path )
            push:finish()
         end)
         assert.is.True( push:unpack_ok() )
      end)
   end)

end)
describe( "set_callbacks #push", function() pending("luagi_push_set_callbacks ") end)
describe( "add_refspec #push", function() pending("luagi_push_add_refspec ") end)
describe( "update_tips #push", function() pending("luagi_push_update_tips ") end)
describe( "foreach_status #push", function() pending("luagi_push_status_foreach ") end)
