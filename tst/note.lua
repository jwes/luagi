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
   local commit = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
   local note = nil
   local noteId = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      note, noteId = repo:create_note( test_helper.signature,
                                    test_helper.signature,
                                    commit,
                                    "this is a note"
                                    )
   end)

   it("should have a valid note", function()
      assert.is.not_nil( noteId )
      assert.are.equal( 40, #noteId )
      assert.is.not_nil( note )
      assert.is.not_nil( note.message )
   end)

   describe( "message #note", function()
      it("should return a message", function()
         assert.are.equal( "this is a note", note:message() )
      end)
   end)

   describe( "__tostring #note", function()
      it("should return a message", function()
         assert.are.equal( "this is a note", tostring( note ) )
      end)
   end)
   describe( "id #note", function()
      it("should return the id", function()
         assert.are.equal( noteId, note:id() )
      end)
   end)
   describe( "note_default_ref #note", function()
      it("should return the default refs/commits", function()
         assert.are.equal("refs/notes/commits", repo:note_default_ref() )
      end)
   end)
end)

describe( "remove_note #note", function()
   local repo = nil
   local err = nil
   local commit = "4aa7714edd19d6c8a0ccfb9a2d8650e69ae2bd09"
   local note = nil
   local noteId = nil
   setup( function()
      test_helper.setup()
      repo, err = luagi.open( test_helper.path )
      if err then return end
      note, noteId = repo:create_note( test_helper.signature,
                                    test_helper.signature,
                                    commit,
                                    "this is a note"
                                    )
   end)

   it("should have a valid note", function()
      assert.is.not_nil( noteId )
      assert.are.equal( 40, #noteId )
      assert.is.not_nil( note )
      assert.is.not_nil( note.message )
   end)

   it( "should remove the note", function()
      note, err = repo:read_note( commit )
      assert.is.falsy( err )
      assert.is.not_nil( note )
      assert.is.not_nil( note.message )

         -- not really notes...
      repo:remove_note( test_helper.signature, test_helper.signature, commit )
      note, err = repo:read_note( commit )
      assert.are.equal( "Note could not be found", err )
   end)
end)
describe( "author #note", function() pending( "luagi_note_author" ) end)
describe( "committer #note", function() pending( "luagi_note_committer" ) end)
