local luagi = require("luagi")
local test_helper = require("test_helper")
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

describe( "is_valid_remote_url #remote", function()
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_url( test_helper.remote_path ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_url( "git://at.somserver.org" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_url( "ssh://at.somserver.org" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_url( "http://at.somserver.org/some/path/somewhere.git" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_valid_remote_url( "https://at.somserver.org/repo" ) )
   end)

   it("should be invalid, local names are checked", function()
      assert.is.False( luagi.is_valid_remote_url( "../not/existing" ))
   end)
   it("should be invalid, local names are checked", function()
      assert.is.False( luagi.is_valid_remote_url( "/not/existing" ))
   end)
end)


describe( "is_supported_remote_url #remote", function()
   it("should be valid", function()
      assert.is.True( luagi.is_supported_remote_url( test_helper.remote_path ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_supported_remote_url( "git://at.somserver.org" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_supported_remote_url( "ssh://at.somserver.org" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_supported_remote_url( "http://at.somserver.org/some/path/somewhere.git" ) )
   end)
   it("should be valid", function()
      assert.is.True( luagi.is_supported_remote_url( "https://at.somserver.org/repo" ) )
   end)
   it("should be invalid, local names are checked", function()
      assert.is.False( luagi.is_supported_remote_url( "../not/existing" ))
   end)
   it("should be invalid, local names are checked", function()
      assert.is.False( luagi.is_supported_remote_url( "/not/existing" ))
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

describe( "load_remote #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:load_remote( "origin" )
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

describe( "create_anon_remote #remote", function() pending("luagi_remote_create_anonymous") end)
describe( "create_remote_with_fetch #remote", function() pending("luagi_remote_create_with_fetchspec") end)
describe( "create_remote #remote", function() pending("luagi_remote_create") end)

describe( "set_url #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:load_remote( "origin" )
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
end)

describe( "save #remote", function() pending("              luagi_remote_save                    ") end)
describe( "add_fetch #remote", function() pending("         luagi_remote_add_fetch               ") end)
describe( "set_fetch_refspecs #remote", function() pending("luagi_remote_set_fetch_refspecs      ") end)
describe( "add_push #remote", function() pending("          luagi_remote_add_push                ") end)
describe( "get_push_refspecs #remote", function() pending(" luagi_remote_get_push_refspecs       ") end)
describe( "set_push_refspecs #remote", function() pending(" luagi_remote_set_push_refspecs       ") end)
describe( "clear_refspecs #remote", function() pending("    luagi_remote_clear_refspecs          ") end)
describe( "ls #remote", function()
   local repo = nil
   local err = nil
   local remote = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      remote, err = repo:load_remote( "origin" )
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
      remote:set_local_transport()
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

describe( "download #remote", function() pending("          luagi_remote_download                ") end)
describe( "fetch #remote", function() pending("             luagi_remote_fetch                   ") end)
describe( "stop #remote", function() pending("              luagi_remote_stop                    ") end)
describe( "check_cert #remote", function() pending("        luagi_remote_check_cert              ") end)
describe( "set_transport #remote", function() pending("     luagi_remote_set_transport           ") end)
describe( "set_callbacks #remote", function() pending("     luagi_remote_set_callbacks           ") end)
describe( "stats #remote", function() pending("             luagi_remote_stats                   ") end)
describe( "autotag #remote", function() pending("           luagi_remote_autotag                 ") end)
describe( "set_autotag #remote", function() pending("       luagi_remote_set_autotag             ") end)
describe( "rename #remote", function() pending("            luagi_remote_rename                  ") end)
describe( "fetch_head #remote", function() pending("        luagi_remote_update_fetch_head       ") end)
describe( "set_fetch_head #remote", function() pending("    luagi_remote_set_update_fetch_head   ") end)
describe( "update_tips #remote", function() pending("       luagi_remote_update_tips             ") end)
