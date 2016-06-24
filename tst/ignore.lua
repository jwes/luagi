--[=[
  MIT License
  
  Copyright (c) 2016 Johannes Westhuis, Alexander Schulz
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]=]--
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
   
