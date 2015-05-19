local luagi = require( "luagi" )
local test_helper = require( "test_helper" )

describe( "lookup_submodule #submodule", function()
   local repo = nil
   local err = nil
   local sub = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it( "should return an error", function()
      local err = nil
      local mysub, err = repo:lookup_submodule( "unused_name" )
      assert.is.truthy( err:find( "No submodule named") )
      assert.is.falsy( mysub )
   end)

   it("should find the submodule", function()
      assert.is.falsy( err )
      assert.is.not_nil( sub )
      assert.is.not_nil( sub.add_finalize )
   end)
   describe( "name #submodule", function()
      it("should have the name submodule", function()
         assert.are.equal( "submodule", sub:name() )
      end)
   end)
   describe( "path #submodule", function()
      it("should have the path submodule", function()
         assert.are.equal( "submodule", sub:path() )
      end)
   end)
   describe( "url #submodule", function()
      it("should have the right url", function()
         assert.is.truthy( sub:url():find( "submodule" ))
      end)
   end)
   describe( "branch #submodule", function()
      it("should have the right branch", function()
         assert.are.equal( nil, sub:branch() )
      end)
   end)
   describe( "head_id #submodule", function()
      it( "should return a valid id", function()
         assert.are.equal( "304e9d28f104aba1384f135da0a6fdd126e73c6f", sub:head_id() )
      end)
   end)
   describe( "wd_id #submodule", function()
      it( "should return a valid id", function()
         assert.are.equal( "304e9d28f104aba1384f135da0a6fdd126e73c6f", sub:wd_id() )
      end)
   end)
   describe( "index_id #submodule", function()
      it( "should return a valid id", function()
         assert.are.equal( "304e9d28f104aba1384f135da0a6fdd126e73c6f", sub:index_id() )
      end)
   end)
   describe( "status #submodule", function()
      it( "should return the status", function()
         local stats = sub:status()
         assert.is.True( stats.in_workdir )
         assert.is.True( stats.in_head )
         assert.is.True( stats.in_index )
         for k, v in pairs( stats.workdir ) do
            assert.is.False( v )
         end
         for k, v in pairs( stats.index ) do
            assert.is.False( v )
         end
      end)
   end)
   describe( "location #submodule", function()
      it( "should return the status", function()
         local stats = sub:location()
         assert.is.True( stats.in_workdir )
         assert.is.True( stats.in_head )
         assert.is.True( stats.in_index )
         for k, v in pairs( stats.workdir ) do
            assert.is.False( v )
         end
         for k, v in pairs( stats.index ) do
            assert.is.False( v )
         end
      end)
   end)
   describe( "__tostring #submodule", function()
      it("should return the name", function()
         assert.are.same( sub:name(), tostring( sub ))
      end)
   end)
end)

describe( "foreach_submodule #submodule", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it("should have one element", function()
      local count = 0
      local s = nil
      local n = nil
      function f( sub, name )
         s = sub
         n = name
         count = count + 1
         return 0
      end

      repo:foreach_submodule( f )

      assert.are.equal( 1, count )
      assert.are.equal( "submodule", n )
      assert.is.not_nil( s )
      assert.is.not_nil( s.add_finalize )

   end)
end)

describe( "add_submodule #submodule", function()
   local repo = nil
   local subrepo = nil
   local sub = nil
   local err = nil
   local treeid = "064ee3e735ab74ab468be05c34e7f7fda011cb3b"
   setup( function()
      test_helper.setup()
      local flags = {
         bare = true,
         mkdir = true,
         mkpath = true,
      }
      repo, err = luagi.open( test_helper.path )
      if err then return end
      subrepo, err = luagi.init_ext( test_helper.extract_path.."/subtwo.git", flags )
   end)

   it( "should have created a bare repo", function()
      assert.is.falsy( err )
      assert.is.True( subrepo:is_bare() )
   end)

   describe( "adding a new submodule", function()
      local path = "subtwo"
      setup( function()
         sub, err = repo:add_submodule_setup( "../subtwo.git", path )
         if err then return end
      end)

      it( "should have no error", function()
         assert.is.falsy( err )
      end)

      it("should throw an error", function()
         assert.has_error( function()
            -- no head
            sub:add_finalize()
         end)
      end)

      it("should have two submodules", function()
         local count = 0
         function f( sub, name )
            count = count + 1
            return 0
         end
         repo:foreach_submodule( f )
         assert.are.equal( 2, count )
      end)
      describe( "reload_all_submodules #submodule", function() 
         it("should not have any error", function()
            repo:reload_all_submodules() 
         end)
      end)
   end)
end)

describe( "resolve_submodule_url #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should return a relative path", function()
      assert.are.equal( sub:url() , repo:resolve_submodule_url( sub:url() ) )
   end)

end)

describe( "set_url #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should set an url", function()
      local url = "ssh://url.to.server/with/git.git"
      sub:set_url( url )
      assert.are.equal( url, sub:url() )
   end)
end)

describe( "ignore #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should have a submodule", function()
      assert.is.falsy( err )
      assert.is.not_nil( sub )
      assert.is.not_nil( sub.add_finalize )
   end)

   it("should show and set ignores", function()
      local default = "none"
      assert.are.equal( default, sub:ignore() )
      sub:set_ignore( "dirty" );
      assert.are.equal( "dirty", sub:ignore() )
      sub:set_ignore( "all" );
      assert.are.equal( "all", sub:ignore() )
      sub:set_ignore( "untracked" );
      assert.are.equal( "untracked", sub:ignore() )
      sub:set_ignore( "none" );
      assert.are.equal( "none", sub:ignore() )
      sub:set_ignore( "reset" );
      assert.are.equal( default, sub:ignore() )
      sub:set_ignore( "default" );
      assert.are.equal( "dirty", sub:ignore() )
   end)
end)

describe( "update #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should have a submodule", function()
      assert.is.falsy( err )
      assert.is.not_nil( sub )
      assert.is.not_nil( sub.add_finalize )
   end)

   it("should show and set ignores", function()
      local default = "checkout"
      assert.are.equal( default, sub:update() )
      assert.are.equal( default, sub:set_update( "rebase" ))
      assert.are.equal( "rebase", sub:update() )
      assert.are.equal( "rebase", sub:set_update( "merge" ))
      assert.are.equal( "merge", sub:update() )
      assert.are.equal( "merge", sub:set_update( "none" ))
      assert.are.equal( "none", sub:update() )
      assert.are.equal( "none", sub:set_update( "default" ))
      assert.are.equal( default, sub:update() )
      assert.are.equal( "default", sub:set_update( "checkout" ))
      assert.are.equal( default, sub:update() )
      assert.are.equal( default, sub:set_update( "reset" ))
      assert.are.equal( default, sub:update() )
   end)
end)

describe( "fetch_recurse #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should set fetch a submodule", function()
      assert.are.equal( "no", sub:fetch_recurse() )
      assert.are.equal( "no", sub:set_fetch_recurse("no") )
      assert.are.equal( "no", sub:fetch_recurse() )
      assert.are.equal( "no", sub:set_fetch_recurse("on demand") )
      assert.are.equal( "on_demand", sub:fetch_recurse() )
      assert.are.equal( "on_demand", sub:set_fetch_recurse("reset") )
      assert.are.equal( "no", sub:fetch_recurse() )
   end)
end)

describe( "init #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should not have an error", function()
      assert.has_no_error( function()
         sub:init()
      end)
   end)

end)
describe( "sync #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should run without an error", function()
      assert.has_no_error( function()
         sub:sync()
      end)
   end)
end)
describe( "reload #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should run without an error", function()
      assert.has_no_error( function()
         sub:reload()
      end)
   end)
end)

describe( "add_to_index #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should not have any errors",function()
      assert.has_no_error( function()
         sub:add_to_index()
      end)
   end)

   it("should not have any errors",function()
      assert.has_no_error( function()
         sub:add_to_index(true)
      end)
   end)

end)
describe( "open_repository #submodule", function()
   local repo = nil
   local sub = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      sub, err = repo:lookup_submodule( "submodule" )
   end)

   it("should return a repository",function()
      assert.is.falsy( err )
      r, err = sub:open_repository()
      assert.is.falsy( err )
      assert.are.equal( repo.path, r.path )
      --collect garbage to check if free on r removes repo
      r = nil
      collectgarbage("collect")      
      assert.has_no_error(function()
         repo:path()
      end)
   end)
end)
