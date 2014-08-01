local function print_flags ( flags )
   if flags 
   then
      for flag, bool in pairs( flags ) do
          if bool then
            print( "\t", flag )
         end
      end
   end
end
local function print_table ( table, prefix ) 
   if table then
      for k, v in pairs( table ) do
         print( prefix, k, v )
         if type(v) == "table" then
            print_table( v, prefix.."\t" )
         end
      end
   end
end

return {
   print_flags=print_flags,
   print_table = print_table
}
