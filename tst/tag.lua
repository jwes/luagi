   --tag
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local commit = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
local version_str = "version1"
local signature = {
   name = "T. Test",
   email = "some@mail.net"
}

describe( "lookup_tag #tag", function()
   local tag = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      tag, err = repo:lookup_tag( commit )
   end )

   it( "should be of type userdata", function()
      assert.is.falsy( err )
      assert.is.not_nil( tag )
      assert.are.equal( "userdata", type(tag) )
   end)
end)

describe( "create_tag #tag", function()
   local tag = nil
   local err = nil
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local c, err = repo:lookup_object( commit, "commit" )
      tag, err = repo:create_tag( "new_tag", c, signature, "a test tag" )
   end )

   it( "should be of type string", function()
      assert.is.falsy( err )
      assert.is.not_nil( tag )
      assert.are.equal( "string", type(tag) )
   end)

   it( "should have 2 element", function()
      local list = repo:list_tags()
      assert.are.equal( 2, #list )
   end)
end)

describe( "create_tag_annotation #tag", function()
   local repo = nil
   local id = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local c, err = repo:lookup_object( commit, "commit" )
      if not err then
         id, err = repo:create_tag_annotation( "tag", c, signature, "message" )
      end 
   end)

   it( "should not have returned an error", function()
      assert.is.falsy( err )
   end)
   it( "should return a valid id", function()
      assert.is.not_nil( id )
      assert.are.equal( "string", type( id ))
      assert.are.equal( 40, #id )
   end)
   it( "should have 1 element", function()
      local list = repo:list_tags()
      assert.are.equal( 1, #list )
   end)
end)

describe( "create_tag_from_buffer #tag", function() 
   local tag = nil
   local invalid = nil
   local invalid_err = nil
   local err = nil
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      invalid, invalid_err = repo:create_tag_from_buffer( "simply a buffer without format")
      tag, err = repo:create_tag_from_buffer( "simply a buffer without format")
   end )

   it( "should have returned an error", function()
      assert.is.not_nil( invalid_err )
   end)
   it( "should not have returned an id", function()
      assert.is.falsy( invalid )
   end)

   it( "should not have returned an error", function()
      assert.is.falsy( err )
   end)
   it( "should return a valid id", function()
      assert.is.not_nil( tag )
      assert.are.equal( "string", type( tag ))
      assert.are.equal( 40, #tag )
   end)
   it( "should have 1 element", function()
      local list = repo:list_tags()
      assert.are.equal( 1, #list )
   end)

end)

describe( "create_tag_lightweight #tag", function()
   local tag = nil
   local err = nil
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local c, err = repo:lookup_object( commit, "commit" )
      tag, err = repo:create_tag_lightweight( "new_tag", c )
   end )

   it( "should not have returned an error", function()
      assert.is.falsy( err )
   end)
   it( "should return a valid id", function()
      assert.is.not_nil( tag )
      assert.are.equal( "string", type( tag ))
      assert.are.equal( 40, #tag )
   end)
   it( "should have 2 element", function()
      local list = repo:list_tags()
      assert.are.equal( 2, #list )
   end)
end)

describe( "delete_tag #tag", function()
   local list = nil
   local before = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      before = repo:list_tags()
      repo:delete_tag( version_str )

      list = repo:list_tags()
   end)

   it( "should be one", function()
      assert.are.equal( 1, #before )
   end)

   it( "should be empty", function()
      assert.are.equal( 0, #list )
   end)
end)

describe( "list_tags #tag", function()
   local list = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      list = repo:list_tags()
   end)

   it( "should have 1 element", function()
      assert.are.equal( 1, #list )
   end)
   it( "should be 'version1'", function()
      for i = 1, #list do
         assert.are.equal(version_str, list[i])
      end
   end)
end)

describe( "list_tags_matching #tag", function()
   local list = nil
   local version = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      list = repo:list_tags_matching("foo")
      version = repo:list_tags_matching(version_str)
   end)

   it( "should be empty", function()
      assert.are.equal( 0, #list )
   end)

   it( "should be empty", function()
      assert.are.equal( 1, #version )
      assert.are.equal(version_str, version[1] )
   end)
end)

describe( "foreach_tag #tag", function()
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)
   it( "should return one element with name 'version1'", function()
      local i = 0
      repo:foreach_tag( function(name, oid)
         i = i + 1
         assert.are.equal("refs/tags/version1", name)
         assert.are.equal( commit, oid)
         return 0
      end )
      assert.are.equal( 1, i )
   end)
end)

describe( "tag object methods", function()
   local repo = nil
   local tag = nil
   local id = nil
   local tag_name = "new_tag"
   local target = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      target, err = repo:lookup_object( commit, "commit" )
      if err then return end

      id, err = repo:create_tag( tag_name, target, signature, "test" )
      if err then return end

      tag, err = repo:lookup_tag( id )
   end )

   it( "should not have an error", function()
      assert.is.falsy( err )
   end)

   it( "should have an tag", function()
      assert.is.not_nil( tag )
   end)

   describe( "id #tag", function()
      it("should have the same id", function()
         assert.are.equal( id, tag:id() )
      end)
   end)

   describe( "owner #tag", function()
      it("should belong to the correct repo", function()
         assert.are.equal( repo:path(), tag:owner():path())
      end)
   end)

   describe( "target #tag", function()
      it("should have the correct target", function()
         assert.are.equal( target:id(), tag:target():id() )
      end)
   end)

   describe( "target_id #tag", function()
      it("should have the correct target id", function()
         assert.are.equal( target:id(), tag:target_id() )
      end)
   end)

   describe( "name #tag", function()
      it("should return the correct name", function()
         assert.are.equal( tag_name, tag:name() )
      end)
   end)

   describe( "__tostring #tag", function()
      it("should return the correct name", function()
         assert.are.equal( tag_name, tostring(tag ))
      end)
   end)

   describe( "tagger #tag", function() 
      it( "should return the correct tagger", function()
         sig = tag:tagger()
         assert.are.equal( signature.name, sig.name )
         assert.are.equal( signature.email, sig.email )
      end)
   end)

   describe( "message #tag", function() 
      it( "should return the message", function()
         assert.are.equal( "test", tag:message() )
      end)
   end)

   describe( "peel #tag", function()
      it( "should return an object", function()
         assert.is.not_nil( tag:peel() ) 
      end)
   end)
end)
