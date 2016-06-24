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
  --graph
local luagi = require("luagi")
local test_helper = require("test_helper")

local version1 = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
local version2 = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"

describe( "graph_ahead_behind #graph", function()
   local repo = nil
   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
   end)
   it("should be not ahead nor behind", function()
      ahead, behind = repo:graph_ahead_behind( version1, version1)
      assert.are.equal( 0, ahead )
      assert.are.equal( 0, behind )
   end)
   it( "should be ahead by one commit", function()
      ahead, behind = repo:graph_ahead_behind( version1, version2)
      assert.are.equal( 0, ahead )
      assert.are.equal( 1, behind )
   end)
   it( "should be behind by one commit", function()
      ahead, behind = repo:graph_ahead_behind( version2, version1)
      assert.are.equal( 1, ahead )
      assert.are.equal( 0, behind )
   end)
end)

describe( "graph_descendant_of #graph", function()
   local repo = nil
   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
   end)
   it("should not be a descendant", function()
      local d = repo:graph_descendant_of( version1, version1 )
      assert.is.False( d )
   end)
   it("should be a descendant", function()
      local d = repo:graph_descendant_of( version1, version2 )
      assert.is.False( d )
   end)
   it("should be a descendant", function()
      local d = repo:graph_descendant_of( version2, version1 )
      assert.is.True( d )
   end)
end)

