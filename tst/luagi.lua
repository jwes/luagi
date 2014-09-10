--[[ run tests ]]--
package.path = package.path .. "../?.so"
local luagi = require( "luagi" )
print( "running tests", "git version", luagi.version()  )

local repopath = "/tmp/testrepo.git"
local repo, err = luagi.init( repopath, true )

if not repo then print(err); os.exit(1) end

if not repo:is_bare() then print "should be bare"; os.exit(1) end
if not repo:is_empty() then print "should be empty"; os.exit(1) end

