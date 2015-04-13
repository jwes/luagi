   --tag
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local commit = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
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
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      local c, err = repo:lookup_object( commit, "commit" )
      tag, err = repo:create_tag( "new_tag", c, signature, "a test tag" )
   end )

   it( "should be of type string", function()
      assert.is.falsy( err )
      assert.is.not_nil( tag )
      assert.are.equal( "string", type(tag) )
   end)
end)

describe( "create_tag_annotation #tag", function() pending("luagi_tag_annotation_create") end)
describe( "create_tag_from_buffer #tag", function() pending("luagi_tag_create_frombuffer") end)
describe( "create_tag_lightweight #tag", function() pending("luagi_tag_create_lightweight") end)
describe( "delete_tag #tag", function() pending("luagi_tag_delete") end)

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
         assert.are.equal("version1",list[i])
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
      version = repo:list_tags_matching("version1")
   end)

   it( "should be empty", function()
      assert.are.equal( 0, #list )
   end)

   it( "should be empty", function()
      assert.are.equal( 1, #version )
      assert.are.equal("version1", version[1] )
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


describe( "id #tag", function() pending("luagi_tag_id ") end)
describe( "owner #tag", function() pending("luagi_tag_owner ") end)
describe( "target #tag", function() pending("luagi_tag_target ") end)
describe( "targer_id #tag", function() pending("luagi_tag_target_id ") end)
describe( "name #tag", function() pending("luagi_tag_name ") end)
describe( "__tostring #tag", function() pending("luagi_tag_name ") end)
describe( "tagger #tag", function() pending("luagi_tag_tagger ") end)
describe( "message #tag", function() pending("luagi_tag_message ") end)
describe( "peel #tag", function() pending("luagi_tag_peel ") end)
