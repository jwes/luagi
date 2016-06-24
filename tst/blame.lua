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
local luagi = require("luagi")
local test_helper = require("test_helper")

describe( "blame_file #blame", function() 
   local blame = nil
   local repo = nil
   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
      blame, err = repo:blame_file( "some/folder/testfile2" )
   end)

   it( "should have result", function()
      assert.is.not_nil( blame )
      assert.are.equal("userdata", type( blame ))
      assert.is.falsy( err )
   end)
  
   it( "should have n hunks", function()
      assert.is.equal( 1, blame:count() )
   end)

   describe( "hunks by index", function()
      local hunk, err = blame:byindex( 1 )
      it( "should be a valid object", function()
         assert.is.not_nil( hunk )
         assert.are.equal("table", type( hunk ))
         assert.is.falsy( err )
      end)

      it( "should have the right content", function()
         assert.is.equal("3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                         hunk.final.oid )
      end)

      it( "should return error", function()
         local h, err = blame:byindex( 0 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)
      it( "should return error", function()
         local h, err = blame:byindex( -10 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)
      it( "should return error", function()
         local h, err = blame:byindex( 2 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)
      it( "should return error", function()
         local h, err = blame:byindex( 10 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)

   end)

   describe( "hunks by line #blame", function()
      local hunk, err = blame:byline( 1 )
      it( "should be a valid object", function()
         assert.is.not_nil( hunk )
         assert.are.equal("table", type( hunk ))
         assert.is.falsy( err )
      end)

      it( "should have the right content", function()
         assert.is.equal("3a3e73745d1a2ba679362d51e0a090a3ee03aad6",
                         hunk.final.oid )
      end)

      it( "should return error", function()
         local h, err = blame:byline( 0 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)
      it( "should return error", function()
         local h, err = blame:byline( -10 )
         assert.is.falsy(h)
         assert.are.equal("string", type(err) )
      end)

   end)

   describe( "buffer #blame", function() 
      io.input( test_helper.path.."/some/folder/testfile2" )
      content = io.read("*a") 
      content = content.."some more content"
      local bufblame, err = blame:buffer( content )
      it( "should have a result", function()
         assert.is.equal("userdata", type( bufblame ) )
         assert.is.falsy( err )
      end)
      it( "should have 2 hunks", function()
         assert.is.equal( 2, bufblame:count() )
      end)

      it( "should have no name", function()
         local hunk, err = bufblame:byindex( 2 )
         assert.is.falsy( hunk.final.signature.name )
      end)
   end)
   
end)
   
