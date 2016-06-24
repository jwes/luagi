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
   --refdb
local luagi = require("luagi")
local test_helper = require("test_helper")

describe( "new_refdb #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:new_refdb()
   end)

   it( "should have no error", function()
      assert.is.not_nil( refdb )
      assert.is.falsy( err )
      assert.are.equal( "userdata", type( refdb ) )
   end)
end)

describe( "open_refdb #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:open_refdb()
   end)

   it( "should have no error", function()
      assert.is.not_nil( refdb )
      assert.is.falsy( err )
      assert.are.equal( "userdata", type( refdb ) )
   end)

end)

describe( "compress #refdb", function()
   local refdb = nil
   local err = nil
   setup( function()
      test_helper.setup()
      local repo = luagi.open( test_helper.path )
      refdb, err = repo:open_refdb()
   end)

   it( "should compress", function()
      assert.has_no_error( function() refdb:compress() end)
   end)
end)
