local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "commit #create #commit", function()
   local repo = nil
   local parentId = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
   local parent = nil
   local signature = { name = "The Test",
                       email = "no@valid.mail.addre.ss" }
   local message = [[Every commit should at least

   have a valid message!]]

   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      parent = repo:lookup_commit( parentId )
   end)
   
   describe("create new", function()
      local tree = parent:tree()
      local parents = {}
      parents[1] = parent
      local ref, err = repo:commit( signature, signature, message, tree, parents )
      it( "should not be nil", function()
         assert.is.not_nil( ref )
      end)
   end)
   

   describe( "amend #commit", function() pending("luagi_commit_amend ") end)
end)

describe( "commit getter", function()
   local repo = nil
   local commitId = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
   local parentId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
   local commit = nil

   local summary = "added gitignore file"
   local message = [[added gitignore file
]]


   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
      commit = repo:lookup_commit( commitId )
   end)


   describe( "id #commit", function()
      it("should be equal", function()
         assert.are.equal( commitId, commit:id() )
      end)
   end)

   describe( "summary #commit", function() 
      it( "should be equal", function()
         assert.are.equal( summary, commit:summary() )
      end)
   end)

   describe( "message #commit", function() 
      it( "should be equal", function()
         assert.are.equal( message, commit:message() )
      end)
   end)
   
   describe( "encoding #commit", function() 
      it( "should have no encoding", function()
         assert.is.falsy( commit:encoding() )
      end)
   end)

   describe( "tree #commit", function() 
      local tree = commit:tree()
      it( "should be a valid tree", function()
         assert.is.not_nil( tree )
      end)
      it( "should have some entries", function()
         assert.are.equal( 3, tree:entry_count() )
      end)
   end)

   describe( "parent_count #commit", function() 
      it( "should be 1", function()
         assert.are.equal( 1,  commit:parent_count() )
      end)
   end)

   describe( "parent #commit", function() 
      it( "should have userdata", function()
         assert.is.not_nil( commit:parent( 1 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:parent( 2 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:parent( 0 ) )
      end)
   end)

   describe( "parent_id #commit", function() 
      it( "should be equal", function()
         assert.are.equal( parentId, commit:parent_id( 1 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:parent_id( 2 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:parent_id( 0 ) )
      end)
   end)

   describe( "ancestor #commit", function() 
      it( "should be equal", function()
         assert.is.not_nil( commit:ancestor( 1 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:ancestor( 3 ) )
      end)
      it( "should not fail", function()
         assert.is.falsy( commit:ancestor( 0 ) )
      end)
   end)
end)

