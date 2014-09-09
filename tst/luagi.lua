--[[ run tests ]]--
package.path = package.path .. "../?.so"
local luagi = require( "luagi" )
print( "running tests", "git version", luagi.version()  )


