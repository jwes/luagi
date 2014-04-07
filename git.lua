
local lgit = require("lgit")

print( lgit.version() )

local features = lgit.features()
for k, v in pairs( features ) do
	print( k, v );
end

