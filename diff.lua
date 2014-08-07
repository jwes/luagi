#!/usr/bin/env lua
local wien = require( "wien" );
local wh = require("wien_helper" );
local repo = wien.open(".");
local rev = "c78833afb82aa413f8ad2ad7cb9548af6e74c74c"

local commit, err = repo:lookup_commit( rev );
if not commit then print( err ) return end

local tree, err = commit:tree()
if not tree then print( err ) return end

local diff, err = repo:diff_tree_to_workdir( tree )
if not diff then print( err ) return end

print( diff:is_sorted() )
print( diff:num_deltas() )

function files ( delta, progress )
   --wh.print_table( delta, "\t" )--
   print ("p:", progress ) 
   return 0
end

function hunks ( delta, old_start, old_lines, new_start, new_lines, header )
   print( old_start, old_lines, new_start, new_lines, header )
   return 0
end

function lines ( delta, header, origin, old_linenr, new_linenr, num_lines )
   print( header, origin, old_linenr, new_linenr, num_lines )
   return 0
end

diff:foreach(files, hunks )
--diff:foreach(files,  hunks, lines)
