local bitmap = require "lbitmap"

local map = bitmap.new(100)
print(map)
print(#map)
for i = 1, 100 do
    map[i] = true
end
print(map[10])
--bitmap.size(stdin)
