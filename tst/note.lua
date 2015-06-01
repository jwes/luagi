   --note
local luagi = require("luagi")
local test_helper = require("test_helper")
local ref_name = "refs/heads/master"

describe( "notes #note", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it(" should be opened", function()
      assert.is.falsy( err )
   end)

   it(" should iterate no notes", function()
      local count = 0
      for note, annotated in repo:notes( ref_name ) do
         count = count + 1
      end
      assert.are.equal( 5, count )
   end)

   it( "should have an error", function()
      assert.has_error( function()
         repo:notes()
      end)
   end)
end)

describe( "foreach_note #note", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it(" should be opened", function()
      assert.is.falsy( err )
   end)

   it(" should iterate notes", function()
      local count = 0
      local function f( note, annotated)
         count = count + 1
      end

      repo:foreach_note( f, ref_name )

      assert.are.equal( 5, count )
   end)

   it( "should have an error", function()
      assert.has_error( function()
         repo:foreach_note( function() end )
      end)
   end)
end)

describe( "read_note #note", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
   end)

   it(" should be opened", function()
      assert.is.falsy( err )
   end)

   it(" should read the notes ", function()
      local note = nil
      for note_id, annotated in repo:notes( ref_name ) do
         note, err = repo:read_note( note_id )
         -- not really notes...
         assert.is.not_nil( err )
      end
      assert.is.falsy( note )
   end)

end)

describe( "create_note #note", function()
   local repo = nil
   local err = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
   end)

   pending("create_note" )

end)
describe( "remove_note #note", function() pending("luagi_note_remove") end)
describe( "note_default_ref #note", function() pending("luagi_note_default_ref") end)


describe( "message #note", function() pending("luagi_note_message ") end)
describe( "__tostring #note", function() pending("luagi_note_message ") end)
describe( "id #note", function() pending("luagi_note_id ") end)
