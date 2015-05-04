local luagi = require("luagi")
local test_helper = require("test_helper")

describe("global config #config", function()

   it( "should not be nil", function()
      local path = luagi:find_global_config()
      assert.is.not_nil( path )
   end)
end)

describe("config parse bool #config", function()

   it( "should be true", function()
      assert.is.True( luagi.parse_bool( "true" ) )
   end)

   it( "should be true", function()
      assert.is_true( luagi.parse_bool( "True" ) )
   end)

   it( "should be true", function()
      assert.is_true( luagi.parse_bool( "trUE" ) )
   end)

   it( "should be true", function()
      assert.is_true( luagi.parse_bool( "TRUE" ) )
   end)

   it( "should be false", function()
      assert.is_false( luagi.parse_bool( "False" ) )
   end)

   it( "should be false", function()
      assert.is_false( luagi.parse_bool( "false" ) )
   end)
   
   it( "should be false", function()
      assert.is_false( luagi.parse_bool( "FALSE" ) )
   end)

   it( "should be false", function()
      assert.is_false( luagi.parse_bool( "falSE" ) )
   end)
end)

describe( "parse_int32 #config", function()
   it( "should be 1", function()
      assert.is_equal( 1, luagi.parse_int32( "1" ))
   end)
   it( "should be 1", function()
      assert.is_equal( 1, luagi.parse_int32( "+1" ))
   end)
   it( "should be 0", function()
      assert.is_equal( 0, luagi.parse_int32( "0" ))
   end)
   it( "should be -1", function()
      assert.is_equal( -1, luagi.parse_int32( "-1" ))
   end)
   it( "should be 2147483647", function()
      assert.is_equal( 2147483647, luagi.parse_int32( "2147483647" ))
   end)
   it( "should be 2147483647", function()
      assert.is_equal( 2147483647, luagi.parse_int32( "+2147483647" ))
   end)
   it( "should be -2147483648", function()
      assert.is_equal( -2147483648, luagi.parse_int32( "-2147483648" ))
   end)
   it( "should have error", function()
      assert.has_error( function() luagi.parse_int32( "2147483648" ) end)
   end)
   it( "should have error", function()
      assert.has_error( function() luagi.parse_int32( "-2147483649") end )
   end)
end)

describe("parse_int64 #config", function()
   it("should be 1", function()
      assert.is_equal(1, luagi.parse_int64( "1" ) )
   end)

   it("should be 1", function()
      assert.is_equal(1, luagi.parse_int64( "+1" ) )
   end)

   it("should be 0", function()
      assert.is_equal(0, luagi.parse_int64( "0" ) )
   end)

   it("should be -1", function()
      assert.is_equal(-1, luagi.parse_int64( "-1" ) )
   end)

   it("should be 9223372036854775807", function()
      assert.is_equal( 9223372036854775807, luagi.parse_int64( "9223372036854775807" ) )
   end)

   it( "should have error", function()
      assert.has_error( function() luagi.parse_int64( "9223372036854775808" ) end )
   end)

   it("should be -9223372036854775807", function()
      assert.is_equal( -9223372036854775807, luagi.parse_int64( "-9223372036854775807" ) )
   end)

   it("should have error", function()
      assert.has_error( function() luagi.parse_int64( "-9223372036854775808" ) end )
   end)
end)

describe( "open_global #config", function()
   local err = nil
   local config = nil
   setup( function()
      test_helper.extract()
      config, err = luagi.open_default_config()
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( config )
      assert.is.not_nil( config.get_bool )
   end)

   it( "should not raise an error", function()
      local c, err = config:open_global()
      assert.is.falsy( err )
      assert.is.not_nil( c )
      assert.is.not_nil( c.get_bool )
   end)
end)

describe( "open_level #config", function()
   local err = nil
   local config = nil
   setup( function()
      test_helper.extract()
      config, err = luagi.open_default_config()
   end)

   it("should be prepared", function()
      assert.is.falsy( err )
      assert.is.not_nil( config )
      assert.is.not_nil( config.get_bool )
   end)

   it( "should not raise an error", function()
      local c, err = config:open_level( "highest" )
      assert.is.falsy( err )
      assert.is.not_nil( c.get_bool )
   end)
   it( "should not raise an error", function()
      local c, err = config:open_level( "system" )
      if err then
         assert.is.truthy( err:find( "No config file" ) )
      else
         assert.is.not_nil( c.get_bool )
      end
   end)
   it( "should not raise an error", function()
      local c, err = config:open_level( "xdg" )
      if err then
         assert.is.truthy( err:find( "No config file" ) )
      else
         assert.is.not_nil( c.get_bool )
      end
   end)
   it( "should not raise an error", function()
      local c, err = config:open_level( "global" )
      assert.is.falsy( err )
      assert.is.not_nil( c.get_bool )
   end)
   it( "should not raise an error", function()
      local c, err = config:open_level( "local" )
      if err then
         assert.is.truthy( err:find( "No config file" ) )
      else
         assert.is.not_nil( c.get_bool )
      end
   end)
   it( "should not raise an error", function()
      local c, err = config:open_level( "app" )
      if err then
         assert.is.truthy( err:find( "No config file" ) )
      else
         assert.is.not_nil( c.get_bool )
      end
   end)
end)

describe( "new #config", function()
   local config = nil
   local err = nil
   setup( function()
      test_helper.setup()
      config, err = luagi.new_config()
      -- needs file added
   end)
   
   it("should have created a config", function()
      assert.is.falsy( err )
      assert.is.not_nil( config.get_bool )
   end)
   describe( "foreach #config", function()
      it( "should have no elements", function()
         local count = 0
         assert.has_no_error( function()
            config:foreach( function()
               count = count + 1
            end)
         end)
         assert.are.equal( 0, count )
      end)
   end)

   describe( "has error, no file added", function()
      assert.has_error( function()
         config:set_string( "foo", "bar" )
      end)
   end)

   describe( "add_file_ondisk #config", function()
      setup(function()
         config:add_file_ondisk( test_helper.path.."/my_config", "app" )
      end)
      describe( "set int64 , get int64", function()
         it(" should return the right value", function()
            local key = "app.int64"
            local value = 192381023009009
            assert.has_no_error( function()
               config:set_int64( key, value )
            end)
            assert.are.equal( value, config:get_int64( key ) )
         end)
      end)

      describe( "set int32 , get int32", function()
         it(" should return the right value", function()
            local key = "app.int32"
            local value = 1923810009
            assert.has_no_error( function()
               config:set_int32( key, value )
            end)
            assert.are.equal( value, config:get_int32( key ) )
            assert.are.equal( value, config:get_int64( key ) )
         end)
      end)
      describe( "get bool, set bool #config", function()
         it(" should return the right value", function()
            local key = "app.bool"
            local value = true
            assert.has_no_error( function()
               config:set_bool( key, value )
            end)
            assert.are.equal( value, config:get_bool( key ) )
            assert.is.falsy( config:get_int32( key ) )
            assert.is.falsy( config:get_int64( key ) )
         end)
      end)

      describe( "set string, get string", function()
         it(" should throw no error", function()
            local key = "app.string"
            local value = "some string"
            assert.has_no_error( function()
               config:set_string( key, value )
            end)
            assert.are.equal( value, config:get_string( key ) )
         end)
      end)

      describe( "get_entry #config", function()
         it(" should throw no error", function()
            local key = "app.entry"
            local value = "a somewhat longer string"
            config:set_string( key, value )
            local v, k, l = config:get_entry( key )
            assert.is.not_nil( k )
            assert.are.equal( value, v )
            assert.are.equal( key, k )
            assert.are.equal( "highest", l )
         end)
         
      end)
   end)
end)

describe( "snapshot #config", function() pending("luagi_config_snapshot ") end)
describe( "refresh #config", function() pending("luagi_config_refresh ") end)
describe( "get_multivar #config", function() pending("luagi_config_get_mulitvar_foreach ") end)
describe( "iterator_new #config", function() pending("luagi_config_iterator_new ") end)
describe( "iterator_glob_new #config", function() pending("luagi_config_iterator_glob_new ") end)
describe( "multivar_iterator #config", function() pending("luagi_config_multivar_iterator ") end)
describe( "set_bool #config", function() pending("luagi_config_set_bool ") end)
describe( "set_multivar #config", function() pending("luagi_config_set_multivar ") end)
describe( "delete_entry #config", function() pending("luagi_config_delete_entry ") end)
describe( "delete_multivar #config", function() pending("luagi_config_delete_mulitvar ") end)
describe( "foreach_match #config", function() pending("luagi_config_foreach_match ") end)
describe( "get_mapped #config", function() pending("luagi_config_get_mapped ") end)
describe( "backend_foreach_match #config", function() pending("luagi_config_backend_foreach_match ") end)
