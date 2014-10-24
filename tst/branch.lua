local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "branch #branch", function() 
   pending("luagi_create_branch") 
end)

describe( "branch lookup", function() 
   local repo = nil
   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
   end)
   describe("lookup_branch #branch", function()
      local branch, err = repo:lookup_branch( "work" )
      local master, err = repo:lookup_branch( "master" )
      it("should exist", function()
         assert.are.equal("userdata", type( branch ) )
         assert.is.falsy( err )
      end)

      it("should be a branch", function()
         
         assert.is.False( branch:is_head() )
         assert.are.equal( "work", branch:name() )
         assert.is.falsy( branch:get_upstream() )
      end)

      it("should be a master", function()
         
         assert.is.True( master:is_head() )
         assert.are.equal( "master", master:name() )
         assert.is.falsy( master:get_upstream() )
      end)

   end)
   
end) 

describe( "delete #branch", function() pending("luagi_delete_branch ") end)
describe( "move #branch", function() pending("luagi_move_branch ") end)
describe( "get_upstream #branch", function() pending("luagi_branch_upstream_get ") end)
describe( "set_upstream #branch", function() pending("luagi_branch_upstream_set ") end)
