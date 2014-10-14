local luagi = require( "luagi" )
describe("version", function()
   describe( "current", function()
      mayor, minor, patch = luagi.version()

      it( "mayor", function()
         assert.are.equal( 0, mayor )
      end)
      it("minor", function()
         assert.are.equal( 21, minor )
      end)
      it("patch", function()
         assert.are.equal( 0, patch )
      end)
   end)
end)

describe("init", function()
   describe("positive", function()
      local repopath = "data/testrepo.git"
      local repo, err = luagi.init( repopath, true )
      it( "repo not nil", function()
         assert.is.not_nil( repo )
         assert.are.equal( "userdata", type( repo ) )
      end)
      it( "err should be null", function()
         assert.is.falsy( err )
      end)
      
      it( "bare test", function()
         assert.is_true( repo:is_bare() )
      end)
      it( "empty test", function()
         assert.is_true( repo:is_empty() )
      end)
   end)

   describe("invalid path", function()
      local repopath = "data/path/that/does/not/exist"
      local repo, err = luagi.init( repopath, true )
      it( "repo should be null", function()
         assert.is.falsy( repo )
      end)
      it( "err is filled", function()
         assert.is.not_nil( err )
         assert.are.equal( "string", type( err ) )
      end)
   end)
end)
