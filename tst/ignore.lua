local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "add_ignore_rules #ignore", function()
   local repo = nil
   local err = nil

   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      repo:add_ignore_rules( "some" )
   end)

   it( "should have no error", function()
      assert.is.falsy( err )
   end)

   it( "should ignore some", function()
      assert.is.True( repo:is_path_ignored( "some" ))
   end)


end)

describe( "clear_internal_ignore_rules #ignore", function()
   local repo = nil
   local err = nil

   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      repo:add_ignore_rules( "some" )
   end)

   it( "should have no error", function()
      assert.is.falsy( err )
   end)

   it( "should ignore some", function()
      assert.is.True( repo:is_path_ignored( "some" ))
   end)

   it( "should ignore some", function()
      repo:clear_internal_ignore_rules( "some" )
      assert.is.False( repo:is_path_ignored( "some" ))
   end)
end)

describe( "is_path_ignored #ignore", function()
   local repo = nil
   local err = nil

   setup( function()
      test_helper.extract()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "should have no error", function()
      assert.is.falsy( err )
   end)

   it( "should ignore *.ign", function()
      assert.is.True( repo:is_path_ignored( "foo.ign" ) )
   end)
   it( "should ignore *.ign", function()
      assert.is.False( repo:is_path_ignored( "foo" ) )
   end)
   it( "should ignore some", function()
      assert.is.False( repo:is_path_ignored( "some" ))
   end)
end)
   
