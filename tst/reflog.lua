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
--reflog
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

local reflog_name = "foo"
local commit = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"

describe( "read_reflog #reflog", function()
   local repo = nil
   local err = nil
   local reflog = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      reflog, err = repo:read_reflog( reflog_name )
   end)

   it("should be error free", function()
      assert.is.falsy( err )
   end)

   it("should be empty", function()
      assert.are.equal( 0, reflog:count())
   end)

   describe( "append #reflog", function()
      setup( function()
         reflog:append( commit, test_helper.signature, "bla" )
      end)

      it( "should have one entry", function()
         assert.are.equal( 1, reflog:count() )
      end)

      describe( "entry_by_index #reflog", function()
         it("should be out of bounds", function()
            local entry, err = reflog:entry_by_index( 0 )
            assert.is.falsy( entry )
            assert.is.truthy( err:find("out of bounds") )
         end)
         it("should be out of bounds", function()
            local entry, err = reflog:entry_by_index( 2 )
            assert.is.falsy( entry )
            assert.is.truthy( err:find("out of bounds") )
         end)
         it("should return an entry", function()
            local entry, err = reflog:entry_by_index( 1 )
            assert.is.falsy( err )
            assert.are.equal( "table", type( entry ))
         end)
      end)
      describe( "write #reflog", function()
         it("should have no error", function()
            assert.has_no_error( function() reflog:write() end)
         end)
      end)
   end)
end)
describe( "delete_reflog #reflog", function()
   setup( function()
      test_helper.setup()
   end)
   it( "should have an error", function()
      assert.has_error( function() reflog:delete_reflog("test") end)
   end)
end)
describe( "rename_reflog #reflog", function()
   setup( function()
      test_helper.setup()
   end)
   it( "should have an error", function()
      assert.has_error( function() reflog:rename_reflog("test", "test2") end)
   end)
end)
describe( "read_reflog #reflog", function()
   local repo = nil
   local err = nil
   local reflog = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end

      reflog, err = repo:read_reflog( reflog_name )
   end)

   it("should be error free", function()
      assert.is.falsy( err )
   end)

   it("should be empty", function()
      assert.are.equal( 0, reflog:count())
   end)

   describe( "drop #reflog", function()
      it("should be empty", function()
         reflog:append( commit, test_helper.signature, "bar" )
         reflog:drop(1)
         assert.are.equal( 0, reflog:count())
      end)
      it("should check bounds", function()
         reflog:append( commit, test_helper.signature, "baz" )
         assert.has_error( function() reflog:drop(0) end )
      end)
      it("should check bounds", function()
         reflog:append( commit, test_helper.signature, "foo" )
         assert.has_error( function() reflog:drop(7) end )
      end)
   end)
end)

