local luagi = require("luagi")
local test_helper = require("test_helper")
   --reference
describe( "create_reference #reference", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)
   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it("should create the ref", function()
      local ref, err = repo:create_reference( "refs/tags/the_ref", 
                           "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09",
                           test_helper.signature,
                           "test tag log message" )
                           -- no force flag ) 
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
      local ref, err = repo:create_reference( "refs/tags/the_ref", 
                           "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09",
                           test_helper.signature,
                           "test tag log message" )
                           -- no force flag ) 
      assert.is.not_nil( err ) -- no force should return an error
      assert.is.truthy( err:find( "a reference with that name already exists" ))
      local ref, err = repo:create_reference( "refs/tags/the_ref", 
                           "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09",
                           test_helper.signature,
                           "new test tag log message",
                           true )
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)
end)
describe( "create_reference_matching #reference", function() pending("luagi_reference_create_matching") end)
describe( "create_symbolic_reference #reference", function()
   local repo = nil
   local err = nil
   local symbolic = nil
   local target_ref = "refs/tags/version1"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      symbolic, err = repo:create_symbolic_reference( "refs/tags/symbolic_version1", target_ref,
                                 test_helper.signature,
                                 "symbolic ref" )
   end)
   it("should be prepared", function()
      assert.is.falsy( err )
   end)

   it( "should create a symbolic ref", function()
      assert.is.not_nil( symbolic )
      assert.is.not_nil( symbolic.symbolic_target )
   end)
   describe( "symbolic_target #reference", function()
      it("should return the target on symbolic refs", function()
         local target = symbolic:symbolic_target()
         assert.is.not_nil( target )
         assert.are.equal( target_ref, target )
      end)
      it( "should return nil on normal refs", function()
         local ref, err = repo:lookup_reference( target_ref )
         assert.is.falsy( err )
         local target = ref:symbolic_target()
         assert.is.falsy( target )
      end)
   end)
end)
describe( "create_symbolic_reference_matching #reference", function() pending("luagi_reference_symbolic_create_matching") end)
describe( "lookup_reference #reference", function() pending("luagi_reference_lookup") end)
describe( "reference_name_to_id #reference", function() pending("luagi_reference_name_to_id") end)
describe( "reference_dwim #reference", function() pending("luagi_reference_dwim") end)
describe( "remove_reference #reference", function() pending("luagi_reference_remove") end)
describe( "list_references #reference", function() pending("luagi_reference_list") end)
describe( "foreach_reference #reference", function() pending("luagi_reference_foreach") end)
describe( "foreach_reference_with_glob #reference", function() pending("luagi_reference_foreach_glob") end)
describe( "iterate_references #reference", function() pending("luagi_reference_iterator") end)
describe( "reference_has_log #reference", function() pending("luagi_reference_has_log") end)
describe( "reference_ensure_log #reference", function() pending("luagi_reference_ensure_log") end)


describe( "target #reference", function() pending("luagi_reference_target ") end)
describe( "target_peel #reference", function() pending("luagi_reference_target_peel ") end)
describe( "symbolic_target #reference", function() pending("luagi_reference_symbolic_target ") end)
describe( "type #reference", function() pending("luagi_reference_type ") end)
describe( "name #reference", function() pending("luagi_reference_name ") end)
describe( "resolve #reference", function() pending("luagi_reference_resolve ") end)
describe( "owner #reference", function() pending("luagi_reference_owner ") end)
describe( "set_symbolic_target #reference", function() pending("luagi_reference_symbolic_set_target ") end)
describe( "set_target #reference", function() pending("luagi_reference_set_target ") end)
describe( "rename #reference", function() pending("luagi_reference_rename ") end)
describe( "delete #reference", function() pending("luagi_reference_delete ") end)
describe( "remove #reference", function() pending("luagi_reference_remove ") end)
describe( "list #reference", function() pending("luagi_reference_list ") end)
describe( "foreach #reference", function() pending("luagi_reference_foreach ") end)
describe( "is_branch #reference", function() pending("luagi_reference_is_branch ") end)
describe( "is_remote #reference", function() pending("luagi_reference_is_remote ") end)
describe( "is_tag #reference", function() pending(" luagi_reference_is_tag ") end)
describe( "is_note #reference", function() pending("luagi_reference_is_note ") end)
describe( "peel #reference", function() pending("luagi_reference_peel ") end)
describe( "shorthand #reference", function() pending("luagi_reference_shorthand ") end)

describe( "reference_normalize_name #reference", function()
   it( "should remove the slashes", function()
      local refname = "refs////////something///////bla"
      local expected = "refs/something/bla"
      local res, err = luagi.reference_normalize_name( refname )
      assert.is.equal( expected, res )
   end)
end)

describe( "reference_is_valid_name #reference", function() 
   describe("toplevel", function()
      it("should be true", function()
         assert.is.True( luagi.reference_is_valid_name( "HEAD") )
      end)
      it("should be false", function()
         assert.is.False( luagi.reference_is_valid_name( "something" ))
      end)
      it("should be false", function()
         assert.is.False( luagi.reference_is_valid_name( "^HEADLIKE" ))
      end)
   end)
   describe("refs/", function()
      it("should be true", function()
         assert.is.True( luagi.reference_is_valid_name( "refs/something") )
      end)
      it("should be false", function()
         assert.is.False( luagi.reference_is_valid_name( "refs/@/^18." ))
      end)
   end)
end)
