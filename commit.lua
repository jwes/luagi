local lgit = require("lgit")
local test_commit = "faa4d576cea4a7cab230cc5fde274becd41bac6d"

local signature = {
   name = "Foo Bar",
   email = "johannes.westhuis@gmail.com"
   }

repo, err = lgit.open(".")
if not repo then print( err ) return end

local commit, err = repo:lookup_commit( test_commit );
if not commit then print( err ) return end

local ref, err = commit:amend{ message = "better test", update_ref = "HEAD" }
if not ref then print( err ) return end

print( ref )
