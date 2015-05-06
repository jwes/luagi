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
   local symbolic_name = "refs/tags/symbolic_version1"  
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      symbolic, err = repo:create_symbolic_reference( symbolic_name, target_ref,
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
   describe( "target #reference", function()
      it("should return nil on symbolic refs", function()
         local target = symbolic:target()
         assert.is.falsy( target )
      end)
      it( "should return the oid on normal refs", function()
         local ref, err = repo:lookup_reference( target_ref )
         assert.is.falsy( err )
         local target = ref:target()
         assert.is.not_nil( target )
         assert.are.equal( "3a3e73745d1a2ba679362d51e0a090a3ee03aad6", target )
      end)
   end)
   describe( "is_branch #reference", function()
      it( "should be false", function()
         assert.is.False( symbolic:is_branch() )
      end)
   end)
   describe( "is_remote #reference", function()
      it( "should be false", function()
         assert.is.False( symbolic:is_remote() )
      end)
   end)
   describe( "is_tag #reference", function()
      it( "should be true", function()
         assert.is.True( symbolic:is_tag() )
      end)
   end)
   describe( "is_note #reference", function()
      it( "should be false", function()
         assert.is.False( symbolic:is_note() )
      end)
   end)
   describe( "type #reference", function()
      it("should be of type symbolic", function()
         assert.are.equal( "symbolic", symbolic:type() )
      end)
   end)
   describe( "name #reference", function()
      it("should return the name", function()
         assert.are.equal( symbolic_name, symbolic:name() )
      end)
   end)
   describe( "resolve #reference", function()
      it("should return the ref, we're pointing to", function()
         local copy, err = symbolic:resolve()
         assert.is.falsy( err )
         assert.is.not_nil( copy.symbolic_target )
         assert.are.equal( target_ref, copy:name() )
      end)
   end)
   describe( "peel #reference", function()
      it("should return a commit", function()
         local commit, err = symbolic:peel( "commit" )
         assert.is.falsy( err )
         assert.is.not_nil( commit )
         assert.is.not_nil( commit.amend )

         assert.are.equal( "some content", commit:summary())
      end)

      it("should not return a tag", function()
         local tag, err = symbolic:peel( "tag" )
         assert.is.not_nil( err )
         assert.is.falsy( tag )
      end)
   end)
end)
describe( "create_symbolic_reference_matching #reference", function() pending("luagi_reference_symbolic_create_matching") end)
describe( "lookup_reference #reference", function()
   local repo = nil
   local err = nil
   local symbolic = nil
   local target_ref = "refs/tags/version1"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "should return a reference", function()
      local ref, err = repo:lookup_reference( target_ref )
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)
   it( "should not return a reference", function()
      local ref, err = repo:lookup_reference( "not_a_valid_refname" )
      assert.is.falsy( ref )
      assert.is.not_nil( err )
      assert.is.truthy( err:find("is not valid" ))
   end)
   it( "should not return a reference", function()
      local ref, err = repo:lookup_reference( "refs/tags/not_existing" )
      assert.is.falsy( ref )
      assert.is.not_nil( err )
      assert.is.truthy( err:find("not found" ))
   end)
   describe( "reference_name_to_id #reference", function()
      local id = repo:reference_name_to_id( target_ref ) 
      assert.are.equal( "3a3e73745d1a2ba679362d51e0a090a3ee03aad6", id )
   end)
end)
describe( "reference_dwim #reference", function()
   local repo = nil
   local err = nil
   local symbolic = nil
   local target_ref = "refs/tags/version1"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "should return a reference", function()
      local ref, err = repo:reference_dwim( "version1" )
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)
end)

describe( "remove_reference, list_references #reference", function()
   local repo = nil
   local err = nil
   local symbolic_name = "refs/tags/symbolic_version1"
   local direct_name = "refs/tags/another_ref"
   local target_ref = "refs/tags/version1"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      local ref, err = repo:create_symbolic_reference( symbolic_name, target_ref,
                                 test_helper.signature,
                                 "symbolic ref" )
      if err then return end
      ref, err = repo:create_reference( direct_name, "3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                                 test_helper.signature,
                                 "another ref" )
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      local refs, err = repo:list_references()
      assert.is.falsy( err )
      assert.are.equal( "table", type( refs ))
      assert.are.equal( 6, #refs )
   end)

   it( "should remove a normal ref", function()
      local num = #repo:list_references()
      repo:remove_reference( direct_name )
      assert.are.equal( num -1, #repo:list_references() )
   end)
   it( "should remove a symbolic ref", function()
      local num = #repo:list_references()
      repo:remove_reference( symbolic_name )
      assert.are.equal( num -1, #repo:list_references() )
   end)
end)
describe( "foreach_reference #reference", function()
   local repo = nil
   local err = nil
   local count = 0
   local is_string = true
   local function f( oid )
      is_string = is_string and type( oid ) == "string"
      count = count + 1
      return 0
   end
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it("should iterate all references", function()
      count = 0
      is_string = true

      repo:foreach_reference( f )
      assert.are.equal( 4, count )
      assert.is.False( is_string )
   end)
   it("should iterate all references", function()
      count = 0
      is_string = true

      repo:foreach_reference( f, true )
      assert.are.equal( 4, count )
      assert.is.True( is_string )
   end)
end)
describe( "foreach_reference_with_glob #reference", function()
   local repo = nil
   local err = nil
   local count = 0
   local is_string = true
   local function f( oid )
      is_string = is_string and type( oid ) == "string"
      count = count + 1
      return 0
   end
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it("should iterate all tags", function()
      count = 0
      is_string = true

      repo:foreach_reference_with_glob("refs/tags/*", f )
      assert.are.equal( 1, count )
      assert.is.True( is_string )
   end)
   it("should iterate all matching references", function()
      count = 0
      is_string = true

      repo:foreach_reference_with_glob("*w*", f )
      assert.are.equal( 1, count )
      assert.is.True( is_string )
   end)
end)

describe( "reference_has_log #reference", function()
   local repo = nil
   local err = nil
   local symbolic_name = "refs/tags/symbolic_version1"
   local direct_name = "refs/tags/another_ref"
   local target_ref = "refs/tags/version1"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      local ref, err = repo:create_symbolic_reference( symbolic_name, target_ref,
                                 test_helper.signature,
                                 "symbolic ref" )
      if err then return end
      ref, err = repo:create_reference( direct_name, "3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                                 test_helper.signature,
                                 "direct ref" )
   end)

   it( "should have a log", function()
      assert.is.True( repo:reference_has_log("refs/heads/master" ))
   end)
   it( "should have a log", function()
      assert.is.True( repo:reference_has_log("refs/heads/work" ))
   end)
   it( "should't have a log", function()
      assert.is.False( repo:reference_has_log( target_ref ))
   end)
   it( "should't have a log", function()
      assert.is.False( repo:reference_has_log( symbolic_name ))
   end)
   it( "should't have a log", function()
      assert.is.False( repo:reference_has_log( direct_name ))
   end)

end)

describe( "lookup_reference", function()
   local repo = nil
   local err = nil
   local target_ref = "refs/tags/version1"
   local ref = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      ref, err = repo:lookup_reference( target_ref )
   end)
   
   describe( "is_branch #reference", function()
      it( "should be false", function()
         assert.is.False( ref:is_branch() )
      end)
   end)
   describe( "is_remote #reference", function()
      it( "should be false", function()
         assert.is.False( ref:is_remote() )
      end)
   end)
   describe( "is_tag #reference", function()
      it( "should be true", function()
         assert.is.True( ref:is_tag() )
      end)
   end)
   describe( "is_note #reference", function()
      it( "should be false", function()
         assert.is.False( ref:is_note() )
      end)
   end)
   describe( "type #reference", function()
      it("should be of type oid", function()
         assert.are.equal( "oid", ref:type() )
      end)
   end)
   describe( "name #reference", function()
      it("should return its name", function()
         assert.are.equal( target_ref, ref:name() )
      end)
   end)
   describe( "symbolic_target #reference", function()
      it("should return nil", function()
         assert.is.falsy( ref:symbolic_target() )
      end)
   end)
   describe( "target_peel #reference", function()
      it("should return nil", function()
         assert.is.falsy( ref:target_peel() )
      end)
   end)
   describe( "resolve #reference", function()
      it("should return a copy of ref", function()
         local copy, err = ref:resolve()
         assert.is.falsy( err )
         assert.are.not_same( copy, ref )
         assert.is.not_nil( copy.symbolic_target )
         assert.are.equal( ref:name(), copy:name() )
      end)
   end)
   describe( "shorthand #reference", function()
      it("should return the short version", function()
         assert.are.equal( "version1", ref:shorthand())
      end)
   end)
   describe( "peel #reference", function()
      it("should return a commit", function()
         local commit, err = ref:peel( "commit" )
         assert.is.falsy( err )
         assert.is.not_nil( commit )
         assert.is.not_nil( commit.amend )

         assert.are.equal( "some content", commit:summary())
      end)

      it("should not return a tag", function()
         local tag, err = ref:peel( "tag" )
         assert.is.not_nil( err )
         assert.is.falsy( tag )
      end)
   end)
end)

describe( "set_symbolic_target #reference", function()
   local repo = nil
   local err = nil
   local direct_name = "refs/my/reference"
   local work_name = "refs/heads/work"
   local master_name = "refs/heads/master"
   local ref = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      ref, err = repo:create_symbolic_reference( direct_name, work_name,
                                 test_helper.signature,
                                 "another ref" )
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)

   it( "should set the target", function()
      local new_ref, err = ref:set_symbolic_target( master_name, test_helper.signature, "set the target" )
      assert.is.falsy( err )
      assert.are.equal( master_name, new_ref:symbolic_target() )
   end)
end)

describe( "set_target #reference", function()
   local repo = nil
   local err = nil
   local direct_name = "refs/tags/another_ref"
   local ref = nil
   local new_target = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      ref, err = repo:create_reference( direct_name, "3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                                 test_helper.signature,
                                 "another ref" )
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)

   it( "should set the target", function()
      local new_ref, err = ref:set_target( new_target, test_helper.signature, "set the target" )
      assert.is.falsy( err )
      assert.are.equal( new_target, new_ref:target() )
   end)
end)
describe( "rename #reference", function()
   local repo = nil
   local err = nil
   local direct_name = "refs/tags/another_ref"
   local ref = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      ref, err = repo:create_reference( direct_name, "3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                                 test_helper.signature,
                                 "another ref" )
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)
   it( "should rename itself", function()
      local name = "refs/tags/something_different"
      ref:rename( name , test_helper.signature, "rename it!") 
      -- seems to be cached or not updated
      assert.are.equal( direct_name, ref:name())

      local renamed, err = repo:lookup_reference( name )
      assert.is.falsy( err )
      assert.is.not_nil( renamed )
      assert.is.not_nil( renamed.symbolic_target )
   end)
end)
describe( "delete #reference", function()
   local repo = nil
   local err = nil
   local direct_name = "refs/tags/another_ref"
   local ref = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      ref, err = repo:create_reference( direct_name, "3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                                 test_helper.signature,
                                 "another ref" )
      assert.are.equal( 5, #repo:list_references())
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( ref )
      assert.is.not_nil( ref.symbolic_target )
   end)
   it("should delete itself", function()
      ref:delete()
      assert.are.equal( 4, #repo:list_references())
   end)
end)

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
