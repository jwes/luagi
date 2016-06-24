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
local refspecs = {}
   --remotes
describe( "is_valid_remote_name #remote", function()
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_name( "foo" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_name( "origin" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_name( "master" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_name( "something/slashy" ) )
   end)
   it("should be invalid", function()
      assert.is.False( luagi.is_valid_remote_name( "*special*!char\\s'" ) )
   end)
end)

describe( "remotes #remote", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it("should return a table with one element", function()
      remotes, err = repo:remotes()
      assert.is.falsy( err )
      assert.are.equal( "table", type( remotes ) )
      assert.are.equal( "origin", remotes[1] )
   end)
end)

describe( "lookup_remote #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should return a loaded remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote.pushurl )
   end)

   describe( "url #remote", function() 
      it("should return the right url", function()
         assert.are.equal("../remote/", remote:url() )
      end)
   end)
   describe( "pushurl #remote", function() 
      it("should not return a url( url is used to push )", function()
         assert.is.falsy(remote:pushurl() )
      end)
   end)
   describe( "name #remote", function()
      it("should return origin", function()
         assert.are.equal("origin", remote:name() )
      end)
   end)
end)

describe( "create_anon_remote #remote", function()
   pending(" check why this produces an error" )
--[[   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:create_anon_remote( test_helper.remote_path,  ) 
   end)

   it("should return a valid remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote )
      assert.is.not_nil( remote.pushurl )
      assert.are.equal( "", remote:name() )
   end)
end)
describe( "create_remote_with_fetch #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:create_remote( { name= "origin", url = test_helper.remote_path, fetch = "+refs/heads/*" })
   end)

   it("should return a valid remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote )
      assert.is.not_nil( remote.pushurl )
      assert.are.equal( "origin", remote:name() )
   end)
end)
describe( "create_remote #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:create_remote({name= "new", url = test_helper.remote_path  })
   end)

   it("should return a valid remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote )
      assert.is.not_nil( remote.pushurl )
      assert.are.equal( "new", remote:name() )
   end)
]]
end)

describe( "set_url #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should return a loaded remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote.pushurl )
   end)

   it( "should save the url", function()
      local url = "http://some.testurl/git.git" 
      remote:set_url( url )
      assert.are.equal( url, remote:url() )
   end)
   it( "should save the url", function()
      local url = "ssh://some.testurl/git.git" 
      remote:set_pushurl( url )
      assert.are.equal( url, remote:pushurl() )
   end)
   describe( "refspec_count #remote", function()
      it( "should have one refspec", function()
         assert.are.equal( 1, remote:refspec_count() )
      end)
   end)
   describe( "get_refspec #remote", function()
      it( "should check bounds", function()
         local spec, err = remote:get_refspec( 0 )
         assert.is.truthy( err:find( "bounds" ))
         spec, err = remote:get_refspec( 1000 )
         assert.is.truthy( err:find( "bounds" ))
      end)
      it( "should return the refspec", function()
         spec = {
            dest = "refs/remotes/origin/*",
            force = true,
            src = "refs/heads/*",
            direction = "fetch",
            representation = "+refs/heads/*:refs/remotes/origin/*"
         }
         assert.are.same( spec, remote:get_refspec(1) )
      end)
   end)
   describe( "get_fetch_refspecs #remote", function()
      it("should return a list of fetch refspecs", function()
         local list, err = remote:get_fetch_refspecs()
         assert.are.equal( 1, #list )
         assert.are.equal( "+refs/heads/*:refs/remotes/origin/*", list[1] )
      end)
   end)
   describe( "stats #remote", function()
      it("should return a table filled with zeros", function()
         for k, v in pairs( remote:stats() ) do
            assert.are.equal( 0, v )
         end
      end)
   end)
   describe( "autotag #remote", function()
      it("should return an autotag string", function()
         assert.are.equal( "auto", remote:autotag() )
      end)
   end)
   describe( "save #remote", function()
      it("should not have an error", function()
         assert.has_no_error( function()
            remote:save()
         end)
      end)
   end)
end)

describe( "refspecs #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
      remote:clear_refspecs()
   end)

   it("should be prepared", function()
      list = remote:get_fetch_refspecs()
      assert.are.equal( 0, #list )
      list = remote:get_push_refspecs()
      assert.are.equal( 0, #list )
   end)

   describe( "add_fetch #remote", function()
      it("should contain the new refspec", function()
         remote:clear_refspecs()
         local spec = "my_own_refspec"
         remote:add_fetch(spec)
         list = remote:get_fetch_refspecs()
         assert.are.equal( 1, #list )
         assert.are.equal( spec, list[1] )
         remote:add_fetch(spec)
         list = remote:get_fetch_refspecs()
         assert.are.equal( 2, #list )
         assert.are.equal( spec, list[1] )
         assert.are.equal( spec, list[2] )
         local push = remote:get_push_refspecs()
         assert.are.equal( 0, #push )
      end)
   end)
   describe( "set_fetch_refspecs #remote", function()
      it("should contain only the new refspec", function()
         remote:clear_refspecs()
         local list = {}
         list[1] = "my_own_refspec"
         list[2] = "some_other_refspec"
         remote:set_fetch_refspecs( list )
         local result = remote:get_fetch_refspecs()
         assert.are.same( list, result )
         local push = remote:get_push_refspecs()
         assert.are.equal( 0, #push )
      end)
   end)

   describe( "add_push #remote", function()
      it( "should add new push refspecs", function()
         remote:clear_refspecs()
         local spec = "my_own_refspec"
         remote:add_push(spec)
         local list = remote:get_push_refspecs()
         assert.are.equal( 1, #list )
         assert.are.equal( spec, list[1] )
         remote:add_push(spec)
         list = remote:get_push_refspecs()
         assert.are.equal( 2, #list )
         assert.are.equal( spec, list[1] )
         assert.are.equal( spec, list[2] )
         local fetch = remote:get_fetch_refspecs()
         assert.are.equal( 0, #fetch )
      end)
   end)

   describe( "set_push_refspecs #remote", function()
      it("should contain only the new refspec", function()
         remote:clear_refspecs()
         local list = {}
         list[1] = "my_own_refspec"
         list[2] = "some_other_refspec"
         remote:set_push_refspecs( list )
         local result = remote:get_push_refspecs()
         assert.are.same( list, result )
         local fetch = remote:get_fetch_refspecs()
         assert.are.equal( 0, #fetch )
      end)
   end)
end)

describe( "ls #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should return a loaded remote", function()
      assert.is.falsy( err )
      assert.is.not_nil( remote.pushurl )
   end)

   it("should return list of heads", function()
      assert.is.False( remote:is_connected())
      local heads, err = remote:ls()
      assert.is.truthy( err:find( "connected" ))
      remote:set_url( test_helper.remote_path )
      remote:connect()
      assert.is.True( remote:is_connected())

      local heads, err = remote:ls()
      local master = "refs/heads/master"
      local oid = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
      assert.is.falsy( err )
      for i, head in ipairs( heads ) do
         if( head.name == "HEAD" ) then
            assert.are.equal( master, head.target )
         else
            assert.are.equal( master, head.name )
         end
         assert.are.equal( oid, head.oid )
      end
   end)
end)

describe( "fetch and download #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should load without an error", function()

      remote:set_url( test_helper.remote_path )
      assert.has_error( function()
         remote:download( refspecs )
      end)
      assert.has_no_error( function()
         remote:fetch( refspecs, test_helper.signature )
      end)
      assert.has_no_error( function()
         remote:download( refspecs )
      end)
   end)
end)

describe( "stop #remote", function() pending("              luagi_remote_stop                    ") end)
describe( "set_callbacks #remote", function()
   pending( "callbacks only really used on smart transports" )
   --[[
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)
   it("should load without an error", function()

      function f( a, b, c, d, e, f, g )
         print( a,b,c,d,e,f,g )
      end
      remote:set_smart_transport()
      remote:set_url( test_helper.remote_path )
      remote:set_callbacks({
         sideband_progress = f,
         transfer_progress = f,
         completion = f,
         update_tips = f,
      })
         remote:fetch( test_helper.signature )
         remote:download()
   end)
   ]]
end)

describe( "set_autotag #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)
   it("should accept the autotag string all", function()
      local autotag = "all"
      remote:set_autotag( autotag )
      assert.are.equal( autotag, remote:autotag( ) )
   end)
   it("should react releaxed and take auto", function()
      remote:set_autotag( "foo" )
      assert.are.equal( "auto", remote:autotag( ) )
   end)

   it("should accept the autotag string none", function()
      local autotag = "none"
      remote:set_autotag( autotag )
      assert.are.equal( autotag, remote:autotag( ) )
   end)
   it("should accept the autotag string auto", function()
      local autotag = "auto"
      remote:set_autotag( autotag )
      assert.are.equal( autotag, remote:autotag( ) )
   end)
end)

describe( "rename #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should have a new name", function()
      local new_name = "foo"
      local name = "origin"
      remote, err = repo:lookup_remote( name )
      assert.is.not_nil( remote )
      repo:rename_remote( name, new_name )
      remote, err = repo:lookup_remote( name )
      assert.is.falsy( remote )
   end)

end)

describe( "fetch_head #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should apply the set boolean values", function()
      assert.is.True( remote:fetch_head() )
      remote:set_fetch_head( false )
      assert.is.False( remote:fetch_head() )
   end)
end)
describe( "update_tips #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)
   it("should get some update tip callback", function()
      remote:set_url( test_helper.remote_path )
      remote:fetch( refspecs,  test_helper.signature )

      assert.has_no_error( function()
         remote:update_tips( test_helper.signature )
      end)
   end)
end)

describe( " prune_refs #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:lookup_remote( "origin" )
   end)

   it("should return zero", function()
      assert.are.equal( 0, remote:prune_refs() )
   end)
   it("should have no error", function()
      assert.has_no_error( function()
         remote:set_url( test_helper.remote_path )
         remote:connect()
         remote:prune()
      end)
   end)
end)

describe( " upload #remote", function() pending( "luagi_remote_upload" ) end)
describe( " push #remote", function() pending( "luagi_remote_push" ) end)

describe( " delete #remote", function()
   local repo = nil
   local err = nil
   local name = "origin"
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it("hould delete the remote", function()
      assert.is.falsy( err )
      remote, err = repo:lookup_remote( name )
      assert.is.falsy( err )
      assert.is.not_nil( remote )
      assert.is.not_nil( remote.prune_refs )
      repo:delete_remote( name )
      remote, err = repo:lookup_remote( name )
      assert.is.truthy( err:find("does not exist" ))
      assert.is.falsy( remote )
   end)
end)
