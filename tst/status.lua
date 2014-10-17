local test_helper = require( "test_helper" )

local luagi = require( "luagi" )

--status
describe( "status_for_each #status", function() 
   pending("luagi_status_foreach") 
end)

describe( "status_for_each_ext #status", function() 
   pending("luagi_status_foreach_ext") 
end)

local function assert_status( table, expected )
   for k, v in pairs(table) do
      if k == "workdir_modified" then
         assert.is.True( v )
      else
         assert.is.False( v )      
      end
   end
end

describe( "status_file #status", function() 
   local repo = nil
   setup( function()

      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)
   
   it( "should be false", function()
      local flags = repo:status_file( "testfile" )
      for k, v in pairs(flags) do
         assert.is.False( v )      
      end
   end)

   it( "should fail", function()
      assert.has_error( function() repo:status_file( "invalid/path" ) end)
   end)


   it( "should be workdir_modified", function()
      local path = test_helper.path.."/testfile"
      test_helper.modify( path )

      local flags = repo:status_file( "testfile" )
      assert_status( flags, "workdir_modified" )
   end)
end)

describe( "status_ignored #status", function() 
   pending("luagi_status_should_ignore") 
end)


describe( "status_list #status", function() 
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   describe( "there were no changes", function()
      local list, err = repo:status_list()
      it( "should be 0", function()
         assert.is.equal( 0, #list )
      end)
   end)

   describe( "there was a change", function()
      local path = test_helper.path.."/testfile"
      test_helper.modify( path )

      local list, err = repo:status_list()
      it( "should be 1", function()
         assert.is.equal( 1, #list )
      end)

      it( "has values", function()
         for i =  1, #list do
            local t = list[i]
            assert_status( t.status, "workdir_modified" )

            assert.is.equal( 2, t.index_to_workdir.number_of_files )
            assert.is.equal( "testfile", t.index_to_workdir.new_file.path )
            assert.is.equal( "testfile", t.index_to_workdir.old_file.path )
         end
      end)
   end)
end)
