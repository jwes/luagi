local head = "117d621c4a920dc1c9d706ee83ffce7d8d982b58"
local lgit = require("lgit")

local signature = {
	name = "firstname lastname",
	email = "firstname.lastname@dn.tld",
	}

print( lgit.version() )

local features = lgit.features()
for k, v in pairs( features ) do
	print( k, v );
end

repo, err = lgit.open(".")
if repo then
	for branch, remote in repo:branches("a") do
		print( branch, remote, branch:name(), branch:is_head() )
	end

	local commit, err = repo:lookup_commit( head );
	if not commit then print(err) return end

	print( "commit", commit:summary(), commit:encoding(), commit:id() )
	print( commit:message() )
	for k, v in pairs( commit:committer() ) do
		print( k, v )
	end

	local tree, err = commit:tree()
	if not tree then print(err) return end
	print( tree, "count", tree:entry_count() )

	local old, err = repo:branch( "old", commit, false,  signature ); 
	if not old then print(err) return end

	local b, err = repo:lookup_branch( "old" )
	if not b then print( err ) return end

	local c = b:move( "new", signature );
	print( c:name() )
	print( "old", old:name())
	c:delete()
	print( "old", old:name())

	local test, err = repo:branch( "test", commit, false,  signature ); 
	if not test then print(err) return end
end

