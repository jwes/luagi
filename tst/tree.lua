local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local treeid = "064ee3e735ab74ab468be05c34e7f7fda011cb3b"
local entryid = "d1bddcecedb5ccd77aab1ee491563e4152bba1a9"

describe( "tree", function()
   local tree = nil

   setup( function()
      test_helper.extract()
      local repo = luagi.open( test_helper.path )
      tree = repo:lookup_tree( treeid )
   end)

   it( "should not be nil", function() 
      assert.is.not_nil( tree )
   end)

   describe( "entry_count #tree", function() 
      it( "should have a entry", function()
         assert.are.equal( 1, tree:entry_count() )
      end)
   end)

   describe( "id #tree", function()
      it( "should return the same id as used", function()
         assert.are.equal( treeid, tree:id() )
      end)
   end)
   describe( "entry_by* #tree", function()
      local entry_id = tree:entry_byid( entryid )
      local entry_name = tree:entry_byname( "folder" )
      local entry_index = tree:entry_byindex( 1 )
      local entry_path = tree:entry_bypath( "folder" ) 
      it( "should return an entry", function()
         assert.is.not_nil( entry_id )
         assert.are.equal( "userdata", type( entry_id ) )
      end)
      it( "should return an entry", function()
         assert.is.not_nil( entry_name )
         assert.are.equal( "userdata", type( entry_name ) )
      end)
      it( "should return an entry", function()
         assert.is.not_nil( entry_index )
         assert.are.equal( "userdata", type( entry_index ) )
      end)
      it( "should return an entry", function()
         assert.is.not_nil( entry_path )
         assert.are.equal( "userdata", type( entry_path ) )
      end)
      it( "should be the same", function()
         assert.are.equal( 0, entry_id:compare( entry_name ))
         assert.are.equal( 0, entry_id:compare( entry_index ) )
         assert.are.equal( 0, entry_id:compare( entry_path ) )
      end)
   end)

   describe( "walk #tree", function()
      count = 0
      for entry in tree:walk() do
         count = count + 1

      end

      it("should be 1", function()
         assert.are.equal( 1, count )
      end)
   end)

end) 

describe( "tree entry", function()
   local entry = nil
   setup( function()
      test_helper.extract()
      local repo = luagi.open( test_helper.path )
      local tree = repo:lookup_tree( treeid )
      entry = tree:entry_byindex( 1 )
   end)

   it( "should not be nil", function()
      assert.is.not_nil( entry )
   end)

   describe( "name #tree", function() 
     it( "should have the same name", function()
        assert.are.equal( "folder", entry:name() )
     end)
   end)

   describe( "filemode #tree", function() 
      it( "should have the correct filemode", function()
         assert.are.equal( "40000", entry:filemode() )
      end)
   end)
   describe( "id #tree", function()
      it( "should have a none nil id", function()
         assert.is.not_nil( entry:id() )
         assert.are.equal( entryid, entry:id() )
      end)
   end)
   describe( "type #tree", function() 
      it( "should have the tree type", function()
         assert.are.equal( "tree", entry:type() )
      end)
    end)
end)

describe( "tree_builder #tree", function()
   local builder = nil
   setup( function()
      test_helper.extract()
      local repo = luagi.open( test_helper.path )
      local tree = repo:lookup_tree( treeid )
      builder = tree:create_builder()
   end)
   
   it( "should not be nil", function()
      assert.is.not_nil( builder )
   end)

   describe( "clear #tree", function() pending("luagi_tree_builder_clear ") end)
   describe( "get #tree", function() pending("luagi_tree_builder_get ") end)
   describe( "insert #tree", function() pending("luagi_tree_builder_insert ") end)
   describe( "remove #tree", function() pending("luagi_tree_builder_remove ") end)
   describe( "write #tree", function() pending("luagi_tree_builder_write ") end)
   describe( "filter #tree", function() pending("luagi_tree_builder_filter ") end)
end)
