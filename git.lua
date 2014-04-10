
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

	local b, err = repo:branch_lookup( "old" )
	if not b then
		print( err )
	else 
		local c = b:move( "new", signature );
		print ( c:name() )
		c:delete()
	end
end

