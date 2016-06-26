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
   --pack
local luagi = require("luagi")
local test_helper = require("test_helper")
describe( "new_packbuilder #pack", function()
   local repo = nil
   local err = nil
   local pack = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      pack, err = repo:new_packbuilder()
   end)
   it("should not have an error", function()
      assert.is.falsy( err )
      assert.is.not_nil( pack )
      assert.is.not_nil( pack.insert_tree )
   end)
   describe( "set_threads #pack", function()
      it("should set the right amount", function()
         assert.are.equal( 1, pack:set_threads( 1 ) )
         assert.are.equal( 1100, pack:set_threads( 1100 ) )
         assert.are.equal( 0, pack:set_threads( 0 ) )
      end)
   end)

   describe( "count #pack", function()
      it("should be zero", function()
         assert.are.equal( 0, pack:count() )
      end)
   end)
   describe( "write #pack", function()
      it("should be zero", function()
         local pstr = pack:write()
         local s, e = pstr:find( "PAC" )
         assert.are.equal( 1, s )
         assert.are.equal( 3, e )
         assert.are.equal( 32, #pstr )
      end)
   end)
   describe( "hash #pack", function()
      it("should be zero", function()
         assert.are.equal( "0000000000000000000000000000000000000000", pack:hash() )
      end)
   end)
   describe( "written #pack", function()
      it("should be zero", function()
         assert.are.equal( 0, pack:written() )
      end)
   end)
end)
describe( "insert #pack", function()
   local repo = nil
   local err = nil
   local pack = nil
   local id = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09" 
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      pack, err = repo:new_packbuilder()
      if err then return end
      pack:insert( id, "commit" )
   end)
   it("should not have an error", function()
      assert.is.falsy( err )
      assert.is.not_nil( pack )
      assert.is.not_nil( pack.insert_tree )
   end)

   describe( "count #pack", function()
      it("should be zero", function()
         assert.are.equal( 1, pack:count() )
      end)
   end)
   describe( "write #pack", function()
      it("should be zero", function()
         local pstr = pack:write()
         local s, e = pstr:find( "PAC" )
         assert.are.equal( 1, s )
         assert.are.equal( 3, e )
         assert.are.equal( 199, #pstr )
      end)
   end)
   describe( "hash #pack", function()
      it("should be zero", function()
         assert.are.equal( "0000000000000000000000000000000000000000", pack:hash() )
      end)
   end)
   describe( "written #pack", function()
      it("should be zero", function()
         assert.are.equal( 1, pack:written() )
      end)
   end)
end)

describe( "insert_tree #pack", function()
   local id = "064ee3e735ab74ab468be05c34e7f7fda011cb3b"
   local repo = nil
   local err = nil
   local pack = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      pack, err = repo:new_packbuilder()
      if err then return end
      pack:insert_tree( id )
   end)
   it("should not have an error", function()
      assert.is.falsy( err )
      assert.is.not_nil( pack )
      assert.is.not_nil( pack.insert_tree )
   end)

   describe( "count #pack", function()
      it("should be zero", function()
         assert.are.equal( 3, pack:count() )
      end)
   end)
   describe( "write #pack", function()
      it("should be zero", function()
         local pstr = pack:write()
         local s, e = pstr:find( "PAC" )
         assert.are.equal( 1, s )
         assert.are.equal( 3, e )
         assert.are.equal( 145, #pstr )
      end)
   end)
   describe( "hash #pack", function()
      it("should be zero", function()
         assert.are.equal( "0000000000000000000000000000000000000000", pack:hash() )
      end)
   end)
   describe( "written #pack", function()
      it("should be zero", function()
         assert.are.equal( 3, pack:written() )
      end)
   end)
end)

describe( "insert_commit #pack", function()
   local repo = nil
   local err = nil
   local pack = nil
   local id = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09" 
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      pack, err = repo:new_packbuilder()
      if err then return end
      pack:insert_commit( id )
   end)
   it("should not have an error", function()
      assert.is.falsy( err )
      assert.is.not_nil( pack )
      assert.is.not_nil( pack.insert_tree )
   end)

   describe( "count #pack", function()
      it("should be zero", function()
         assert.are.equal( 7, pack:count() )
      end)
   end)
   describe( "write #pack", function()
      it("should be zero", function()
         local pstr = pack:write()
         local s, e = pstr:find( "PAC" )
         assert.are.equal( 1, s )
         assert.are.equal( 3, e )
         assert.are.equal( 457, #pstr )
      end)
   end)
   describe( "hash #pack", function()
      it("should be zero", function()
         assert.are.equal( "0000000000000000000000000000000000000000", pack:hash() )
      end)
   end)
   describe( "written #pack", function()
      it("should be zero", function()
         assert.are.equal( 7, pack:written() )
      end)
   end)
end)

describe( "write_file #pack", function()
   local repo = nil
   local err = nil
   local pack = nil
   local id = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09" 
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      pack, err = repo:new_packbuilder()
      if err then return end
      pack:insert_commit( id )
      pack:write_file( test_helper.extract_path )
   end)
   it("should not have an error", function()
      assert.is.falsy( err )
      assert.is.not_nil( pack )
      assert.is.not_nil( pack.insert_tree )
   end)

   describe( "count #pack", function()
      it("should be zero", function()
         assert.are.equal( 7, pack:count() )
      end)
   end)
   describe( "write #pack", function()
      it("should be zero", function()
         local pstr = pack:write()
         local s, e = pstr:find( "PAC" )
         assert.are.equal( 1, s )
         assert.are.equal( 3, e )
         assert.are.equal( 32, #pstr )
      end)
   end)
   describe( "hash #pack", function()
      it("should be zero", function()
         assert.are.equal( "1811f0739e5ef2777866e93a6f75bd33442613ea", pack:hash() )
      end)
   end)
   describe( "written #pack", function()
      it("should be zero", function()
         assert.are.equal( 0, pack:written() )
      end)
   end)
   describe( "foreach #pack", function()
      local count = 0
      local sum = 0
      local function f( msg )
         sum = sum + #msg 
         count = count + 1
      end
      pack:foreach( f )
      assert.are.equal( 2, count )
      assert.are.equal( 32, sum )
   end)
end)
describe( "set_callbacks #pack", function() pending("luagi_packbuilder_set_callbacks ") end)
