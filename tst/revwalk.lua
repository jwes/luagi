--revwalk
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "revwalk #revwalk", function()
   local repo = nil
   local err = nil
   local revwalk = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      revwalk, err = repo:revwalk()
   end)

   it( "should be prepared", function()
      assert.is.falsy( err )
      assert.are.equal( "userdata", type( revwalk ))
      assert.is.not_nil( revwalk.sorting )
   end)

   describe( "next #revwalk", function()
      it( "should iterate elements", function()
         local count = 0
         while true do
            local rev, err= revwalk:next()
            if not rev then break end
            count = count + 1
          end
          assert.are.equal( 0, count )
       end)
   end)

end)
describe( "reset #revwalk", function() pending("luagi_revwalk_reset ") end)
describe( "push #revwalk", function() pending("luagi_revwalk_push ") end)
describe( "push_glob #revwalk", function() pending("luagi_revwalk_push_glob ") end)
describe( "push_head #revwalk", function() pending("luagi_revwalk_push_head ") end)
describe( "hide #revwalk", function() pending("luagi_revwalk_hide ") end)
describe( "hide_glob #revwalk", function() pending("luagi_revwalk_hide_glob ") end)
describe( "hide_head #revwalk", function() pending("luagi_revwalk_hide_head ") end)
describe( "push_ref #revwalk", function() pending("luagi_revwalk_push_ref ") end)
describe( "hide_ref #revwalk", function() pending("luagi_revwalk_hide_ref ") end)
describe( "sorting #revwalk", function() pending("luagi_revwalk_sorting ") end)
describe( "push_range #revwalk", function() pending("luagi_revwalk_push_range ") end)
describe( "simplify_first_parent #revwalk", function() pending("luagi_revwalk_simplify_first_parent ") end)
describe( "repository #revwalk", function() pending("luagi_revwalk_repository ") end)
describe( "add_hide_callback #revwalk", function() pending(" luagi_revwalk_add_hide_callback ") end)
