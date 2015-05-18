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
      pending( "should branch return nil?" )
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
describe( "add_submodule_setup #submodule", function() pending("luagi_submodule_add_setup") end)
describe( "add_finalize #submodule", function() pending("luagi_submodule_add_finalize ") end)

describe( "resolve_submodule_url #submodule", function() pending("luagi_submodule_resolve_url") end)
describe( "reload_all_submodules #submodule", function() pending("luagi_submodule_reload_all") end)

describe( "set_url #submodule", function() pending("luagi_submodule_set_url ") end)

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

describe( "update #submodule", function() pending("luagi_submodule_update ") end)
describe( "set_update #submodule", function() pending("luagi_submodule_set_update ") end)
describe( "fetch_recurse #submodule", function() pending("luagi_submodule_fetch_recurse_submodules ") end)
describe( "set_fetch_recurse #submodule", function() pending("luagi_submodule_set_fetch_recurse_submodules ") end)
describe( "init #submodule", function() pending("luagi_submodule_init ") end)
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

describe( "add_to_index #submodule", function() pending("luagi_submodule_add_to_index ") end)
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
