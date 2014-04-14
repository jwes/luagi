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

	local commit, err = repo:commit_lookup( head );
	if not commit then print(err) return end

	print( "commit", commit )

	local old, err = repo:create_branch( "old", commit, false,  signature ); 
	if not old then print(err) return end

	local b, err = repo:branch_lookup( "old" )
	if not b then print( err ) return end

	local c = b:move( "new", signature );
	print( c:name() )
	print( "old", old:name())
	c:delete()
	print( "old", old:name())

	local test, err = repo:create_branch( "test", commit, false,  signature ); 
	if not test then print(err) return end
end

