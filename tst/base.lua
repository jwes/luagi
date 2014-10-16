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
      test_helper.setup()
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

describe( "create_index", function() pending("pending") end)
describe( "reference_normalize_name", function() pending("pending") end)
describe( "reference_is_valid_name", function() pending("pending") end)
describe( "merge_files", function() pending("pending") end)
describe( "wrap_odb", function() pending("pending") end)
describe( "discover", function() pending("pending") end)
describe( "open_ext", function() pending("pending") end)
describe( "open_bare", function() pending("pending") end)
describe( "init", function() pending("pending") end)
describe( "init_ext", function() pending("pending") end)
describe( "new_odb", function() pending("pending") end)
describe( "open_odb", function() pending("pending") end)
describe( "hash", function() pending("pending") end)
describe( "hashfile", function() pending("pending") end)
describe( "find_global_config", function() pending("pending") end)
describe( "find_xdg_config", function() pending("pending") end)
describe( "find_system_config", function() pending("pending") end)
describe( "open_default_config", function() pending("pending") end)
describe( "new_config", function() pending("pending") end)
describe( "open_ondisk_config", function() pending("pending") end)
describe( "parse_bool", function() pending("pending") end)
describe( "parse_int32", function() pending("pending") end)
describe( "parse_int64", function() pending("pending") end)
describe( "patch_buffers", function() pending("pending") end)
 
