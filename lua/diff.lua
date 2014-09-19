#!/usr/bin/env lua
local luagi = require( "luagi" );
local wh = require("luagi_helper" );
local repo = luagi.open(".");
local rev = "c78833afb82aa413f8ad2ad7cb9548af6e74c74c"
local rev2 = "117d621c4a920dc1c9d706ee83ffce7d8d982b58"

local commit, err = repo:lookup_commit( rev );
if not commit then print( err ) return end

local commit2, err = repo:lookup_commit( rev2 );
if not commit2 then print( err ) return end

local tree, err = commit:tree()
if not tree then print( err ) return end

local tree2, err = commit2:tree()
if not tree2 then print( err ) return end

local diff, err = repo:diff_tree_to_tree( tree, tree2 )
if not diff then print( err ) return end

--print( diff:is_sorted() )--
print( diff:num_deltas() )
print( diff:num_deltas( "added") )

--[[
function files ( delta, progress )
   --wh.print_table( delta, "\t" )--
   print ("p:", progress ) 
   return 0
end

function hunks ( delta, old_start, old_lines, new_start, new_lines, header )
   print( old_start, old_lines, new_start, new_lines, header )
   return 0
end

function lines ( delta, header, origin, old_linenr, new_linenr, num_lines, content )
   print( header, origin, old_linenr, new_linenr, num_lines )
   print( content )
   return 0
end

diff:foreach(files, hunks, lines )
print( " == print test == " )
diff:print( "patch", lines )
diff:find_similar()
diff:print( "patch", lines )
]]--
