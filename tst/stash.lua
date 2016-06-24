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
   --stash
local luagi = require( "luagi" )
local test_helper = require( "test_helper" )
local stasher = {
   name = "A. Name",
   email = "has@mail.com"
}

describe( "stash #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher )
   end)

   it( "should not have an error", function()
      assert.is.falsy( err )
   end)

   it( "has an oid", function()
      assert.is.not_nil( oid )
   end)
end)

describe( "stash_for_each #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   local message = "the_message"
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher, message )
   end)

   it( "should have 1 stashed element", function()
      local i = 0
      local lid = nil
      local lmsg = nil
      local loid = nil
      repo:stash_for_each( function( id, msg, stash_oid )
         i = i + 1
         lid = id
         lmsg = msg
         loid = stash_oid
         return 0
      end)
      assert.is.not_nil( lmsg )
      assert.are.equal( oid, loid )
      assert.are.equal( 1, i )
   end)
end)

describe( "stash_drop #stash", function()
   local repo = nil
   local oid = nil
   local err = nil
   local message = "the_message"
   setup( function()
      test_helper.setup()

      --create change
      local file = io.open( test_helper.path.."/testfile", "w" )
      file:write( "more, more, more")
      file:close()

      repo, err = luagi.open( test_helper.path )
      if err then return end

      oid, err = repo:stash( stasher, message )

      repo:stash_drop( 0 )
   end)
   it( "should have no elements", function()
      local i = 0
      repo:stash_for_each( function( id, msg, stash_oid )
         i = i + 1
      end)
      assert.are.equal( 0, i )
   end)
end)
