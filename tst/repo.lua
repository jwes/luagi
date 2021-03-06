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
describe( "with test_repo", function()
   local repo = nil
   local err = nil
   setup(function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)
   it( "should open the repo", function()
      assert.is.falsy( err )
      assert.is.not_nil( repo )
   end)

   describe( "index #repo", function()
      local index = nil
      setup( function()
          index, err = repo:index()
      end)
      it( "should open an index", function()
         assert.is.falsy( err )
         assert.is.not_nil( index )
         assert.are.equal( "userdata", type( index ) )
      end)
   end)
   describe( "head #repo", function()
      local head = nil
      setup( function()
         head, err = repo:head()
      end)
      it( "should return the head", function()
         assert.is.falsy( err )
         assert.is.not_nil( head )
         assert.are.equal( "userdata", type( head ) )
         assert.has_no_error( function() head:name() end)
      end)
   end)
   describe( "is_head_detached #repo", function()
      it("should return false", function()
         assert.is.False( repo:is_head_detached() )
      end)
   end)
   describe( "is_head_unborn #repo", function()
      it("should return false", function()
         assert.is.False( repo:is_head_unborn() )
      end)
   end)
   describe( "is_empty #repo", function()
      it("should return false", function()
         assert.is.False( repo:is_empty() )
      end)
   end)
   describe( "is_bare #repo", function()
      it("should return false", function()
         assert.is.False( repo:is_bare() )
      end)
   end)
   describe( "is_shallow #repo", function()
      it("should return false", function()
         assert.is.False( repo:is_shallow() )
      end)
   end)
   describe( "path #repo", function()
      it("should have the testhelper path", function()
         assert.is.not_nil( repo:path():find( test_helper.path ))
      end)
   end)
   describe( "workdir #repo", function()
      it("should have the testhelper path", function()
         assert.is.not_nil( repo:path():find( test_helper.path ))
      end)
   end)
   describe( "config #repo", function()
      local config = nil
      setup(function()
         config, err = repo:config()
      end)

      it( "should return a config", function()
         assert.is.falsy( err )
         assert.is.not_nil( config )
         assert.are.equal( "userdata", type( config ) )
      end)
   end)
   describe( "odb #repo", function()
      local odb = nil
      setup(function()
         odb, err = repo:odb()
      end)

      it( "should return an odb", function()
         assert.is.falsy( err )
         assert.is.not_nil( odb )
         assert.are.equal( "userdata", type( odb ) )
         assert.has_no_error( function() odb:refresh() end)
      end)
    end)
   describe( "refdb #repo", function()
      local refdb = nil
      setup(function()
         refdb, err = repo:refdb()
      end)

      it( "should return a refdb", function()
         assert.is.falsy( err )
         assert.is.not_nil( refdb )
         assert.are.equal( "userdata", type( refdb ) )
         assert.is.not_nil( refdb.compress )
      end)
   end)
   describe( "get_namespache #repo", function()
      it("should return a namespace", function()
         assert.is.falsy( repo:get_namespace() )
         assert.has_no_error(function() repo:set_namespace( "test" ) end)
         assert.are.equal( "test", repo:get_namespace() )
         assert.has_no_error(function() repo:set_namespace( nil ) end)
         assert.is.falsy( repo:get_namespace() )
      end)
   end)
   describe( "state #repo", function()
      it("should return none", function()
         local state = repo:state()
         assert.are.equal( "string", type( state ) )
         assert.are.equal( "none", state )
      end)
   end)
   describe( "config_snapshot #repo", function()
      local config = nil
      setup( function()
         config, err = repo:config_snapshot()
      end)

      it( "should return a config", function()
         assert.is.falsy( err )
         assert.is.not_nil( config )
         assert.is.not_nil( config.get_mapped )
      end)
   end)
   describe( "message #repo", function()
      it( "should not return a string, message only contains a value after a faild merge/cherry-pick/etc", function()
         local message, err = repo:message()
         assert.is.not_nil( err )
         assert.is.falsy( message )
      end)
   end)
   describe( "remove_message #repo", function()
      it( "should return an error, no message available", function()
         assert.has_error( function() repo:remove_message() end)
      end)
   end)
   describe( "cleanup_state #repo", function()
      it( "should set state to none" , function()
         repo:cleanup_state()
         assert.are.equal("none", repo:state())
      end)
   end)
   describe( "hashfile #repo", function()
      it( "should return a hash of filename", function()
         local hash, err = repo:hashfile( "testfile", "commit" )
         assert.is.falsy( err )
         assert.is.truthy( hash:find( "4e82707c3e93" ))
      end)
   end)
end)

describe( "set_workdir #repo", function()
   local repo =  nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "should not accept a dir", function()
      assert.has_no_error( function() repo:set_workdir( "/tmp" ) end)
      assert.is.truthy( repo:workdir():find( "tmp" ))
   end)
end)

describe( "foreach_mergehead #repo", function()
   local repo =  nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "has no error", function()
      assert.is.falsy( err )
   end)

   it( "has no mergehead", function()
      assert.has_error( function() repo:foreach_mergehead() end)
   end)
end)

describe( "foreach_fetchhead #repo", function()
   local repo =  nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "has no error", function()
      assert.is.falsy( err )
   end)

   it( "has no fetchhead", function()
     err = repo:foreach_fetchhead(function(num, ref_name, remote_url, oid, is_merge) return 0 end)
     assert.is.falsy( err )
   end)
end)

describe( "set_head #repo", function()
   local repo =  nil
   local err = nil
   local id = "55c33519ff7e7ae27372a6f338e635b3c18cca57"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "has no error", function()
      assert.is.falsy( err )
      assert.is.not_nil( repo )
   end)

   it( "set_head", function()
      assert.has_no_error( function()
         repo:set_head( "refs/heads/work", test_helper.signature, "switch" )
      end)
      local ref, err = repo:head()
      assert.is.falsy( err )
      assert.are.equal( id, ref:target() )
   end)
end)

describe( "set_head_detached #repo", function()
   local repo =  nil
   local err = nil
   local id = "55c33519ff7e7ae27372a6f338e635b3c18cca57"
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "has no error", function()
      assert.is.falsy( err )
      assert.is.not_nil( repo )
   end)

   it( "set_head_detached", function()
      assert.has_no_error( function()
         repo:set_head_detached( id, test_helper.signature, "switch" )
      end)
      local ref, err = repo:head()
      assert.is.falsy( err )
      assert.are.equal( id, ref:target() )

      assert.is.True( repo:is_head_detached() )
   end)
end)

describe( "detach_head #repo", function()
   local repo =  nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it( "has no error", function()
      assert.is.falsy( err )
      assert.is.not_nil( repo )
   end)

   it( "detach head", function()
      assert.is.False( repo:is_head_detached() )
      assert.has_no_error( function()
         repo:detach_head( test_helper.signature, "some msg" )
      end)
      assert.is.True( repo:is_head_detached() )
   end)
end)
