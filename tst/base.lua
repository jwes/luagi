local luagi = require( "luagi" )

describe("luagi base", function()
   it("repo base", function()
      local repopath = "/tmp/testrepo.git"
      local repo, err = luagi.init( repopath, true )
      assert.is.not_nil( repo )
      
      it( "bare test", function()
         assert.is_true( repo:is_bare() )
      end)
      it( "empty test", function()
         assert.is_true( repo:is_empty() )
      end)
   end)
end)
