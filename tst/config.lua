local luagi = require("luagi")

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


