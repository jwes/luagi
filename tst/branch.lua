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

describe( "branch #branch", function() 
   local repo = nil
   setup( function()
      test_helper.extract()
      repo = luagi.open( test_helper.path )
   end)
   describe("create branch", function()
      local name = "new_branch"
      local commitId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
      local commit = repo:lookup_commit( commitId )
      local branch = repo:branch( name, commit, { name = "tester", email = "mctest@test.tt"} )

      it("should have the name", function()
         assert.are.equal( name, branch:name() )
      end)

      it( "should habe the same commitId", function()
         assert.are.equal( commitId, branch:target() )
      end)
   end)
end)

describe( "branch lookup", function() 
   local repo = nil
   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
   end)
   describe("lookup_branch #branch", function()
      local branch, err = repo:lookup_branch( "work" )
      local master, err = repo:lookup_branch( "master" )
      it("should exist", function()
         assert.are.equal("userdata", type( branch ) )
         assert.is.falsy( err )
      end)

      it("should be a branch", function()
         
         assert.is.False( branch:is_head() )
         assert.are.equal( "work", branch:name() )
         assert.is.falsy( branch:get_upstream() )
      end)

      it("should be a master", function()
         
         assert.is.True( master:is_head() )
         assert.are.equal( "master", master:name() )
         assert.are.not_nil( master:get_upstream() )
      end)

   end)
   
end) 

describe( "delete #branch", function() 
   local repo = nil
   local to_delete = nil
   local name = "new_branch"
   local commitId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
   local signature = { name = "tester", email = "mctest@test.tt"}

   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local commit = repo:lookup_commit( commitId )
      to_delete = repo:branch( name, commit, signature ) 
   end)

   it( "a branch lookup should fail after deletion", function()
      to_delete:delete();

      local branch, err = repo:lookup_branch( name )

      assert.is.falsy( branch )
      assert.are.equal( "string", type( err ) )
   end)
end)

describe( "move #branch", function() 
   local repo = nil
   local to_move = nil
   local name = "move_branch"
   local new_name = "moved_branch"
   local commitId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
   local signature = { name = "tester", email = "mctest@test.tt"}

   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local commit = repo:lookup_commit( commitId )
      to_move = repo:branch( name, commit, signature ) 
   end)

   it( "should be found under the new name", function()
      local branch, err = to_move:move( new_name, signature )
      assert.are.equal( "userdata", type( branch ) )
      assert.are.equal( new_name, branch:name() )
      assert.is.falsy( err )

      local old_branch, err = repo:lookup_branch( name )
      assert.is.falsy( old_branch )
      assert.are.equal( "string", type( err ) )

      local new_branch, err = repo:lookup_branch( new_name )
      assert.are.equal( "userdata", type( new_branch ) )
      assert.are.equal( commitId, new_branch:target() )
   end)
end)

describe( "upstream #branch", function() 
   local repo = nil
   local name = "upstream"
   local commitId = "3a3e73745d1a2ba679362d51e0a090a3ee03aad6"
   local signature = { name = "tester", email = "mctest@test.tt"}
   local branch = nil

   setup( function()
      test_helper.setup()
      repo = luagi.open( test_helper.path )
      local commit = repo:lookup_commit( commitId )
      branch = repo:branch( name, commit, signature )
   end)

   describe( "no upstream", function()
      up = branch:get_upstream()
      it( "should have no upstream", function()
         assert.is.falsy( up )
      end)
   end)

   describe( "new upstream", function()
      branch:set_upstream( "master" )
      it( "should return userdata", function()
         assert.are.equal("userdata", type( branch:get_upstream() ))
      end)
   end)

   describe( "new upstream", function()
      it( "should have error", function()
         assert.has_error( function()
            branch:set_upstream( "not-existant" )
         end)
      end)
   end)
  
end)
