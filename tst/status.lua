local test_helper = require( "test_helper" )

local luagi = require( "luagi" )

local function assert_status( table, expected )
   for k, v in pairs(table) do
      if k == expected then
         assert.is.True( v )
      else
         assert.is.False( v )      
      end
   end
end

--status
describe( "status_for_each #status", function() 
   local repo = nil
   setup( function()

      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   it( "should only return ignored", function()
      for path, status in repo:status_for_each() do
         local pattern = ".ign"
         local ret = string.match( path, pattern )
         assert.is.True( ret == pattern ) 
      end
   end)

   it( "should return path or ignored", function()
      local path = test_helper.path.."/testfile"
      test_helper.modify( path )
      at_least_one = false
      for rpath, status in repo:status_for_each( ) do
         if rpath == "testfile" then
            assert.is.equal( "testfile", rpath)
            assert_status( status, "workdir_modified" )
            at_least_one = true
         elseif ".ign" == string.match(rpath,".ign") then
            assert_status( status, "ignored" )
         end
      end

      assert.is.True( at_least_one )
   end)
   
end)

describe( "status_for_each_ext #status", function() 
   local repo = nil
   setup( function()

      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   it( "should not return a thing", function()
      not_one = true
      for path, status in repo:status_for_each_ext() do
         not_one = false
      end
      assert.is.True( not_one )
   end)

   it( "should include unmodified", function()
      flags = {
         include_unmodified = true,
      }

      count = 0
      for path, status in repo:status_for_each_ext( flags ) do
         count = count + 1
         assert_status( status, "nothing" )
      end
      assert.equal( 3, count )
   end)

   it( "should return path", function()
      local path = test_helper.path.."/testfile"
      test_helper.modify( path )
      at_least_one = false
      for rpath, status in repo:status_for_each_ext( ) do
         assert.is.equal( "testfile", rpath)
         assert_status( status, "workdir_modified" )
         at_least_one = true
      end

      assert.is.True( at_least_one )
   end)
end)


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
   local repo = nil
   setup( function()

      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)

   it( "should be ignored", function() 
      assert.is.True(repo:status_ignored( "ignored.ign"))
   end)
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
