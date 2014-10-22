local test_helper = require( "test_helper" )

local luagi = require( "luagi" )
describe("version #base", function()
   describe( "current #base", function()
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

describe("init #base", function()
   describe("positive #base", function()
      local repopath = "data/testrepo.git"
      local repo, err = luagi.init( repopath, true )
      it( "should be a repo", function()
         assert.is.not_nil( repo )
         assert.are.equal( "userdata", type( repo ) )
      end)
      it( "should be null", function()
         assert.is.falsy( err )
      end)
      
      it( "should be bare", function()
         assert.is_true( repo:is_bare() )
      end)
      it( "should be empty", function()
         assert.is_true( repo:is_empty() )
      end)
   end)

   describe("invalid path #base", function()
      local repopath = "data/path/that/does/not/exist"
      local opts = {}
      local repo, err = luagi.init_ext( repopath, opts )
      it( "should be nil", function()
         assert.is.falsy( repo )
      end)
      it( "err is filled", function()
         assert.is.not_nil( err )
         assert.are.equal( "string", type( err ) )
      end)
   end)
end)

describe( "open #base", function()
   setup(function()
      test_helper.extract()
   end)

   it("should not be null", function()
      local repo, err = luagi.open(test_helper.path)
      assert.is.not_nil( repo )
      assert.are.equal( "userdata", type( repo ) )

      assert.is.falsy( err )
   end)

   it("should be null", function()
      local repo, err = luagi.open("data/no/valid/path")
      assert.is.falsy( repo )

      assert.is.not_nil( err )
      assert.are.equal( "string", type( err ) )
   end)
end)

describe( "create_index", function() 

   it("should not be nil", function()
      local index, err = luagi.create_index() 
      assert.is.not_nil( index )
      assert.are.equal( "userdata", type( index ) )

      assert.is.falsy( err )
   end)
end)


describe( "merge_files", function() pending("pending") end)
describe( "discover", function() pending("pending") end)
describe( "open_ext", function() pending("pending") end)
describe( "open_bare", function() pending("pending") end)
describe( "init_ext", function() pending("pending") end)
describe( "patch_buffers", function() pending("pending") end)
 
