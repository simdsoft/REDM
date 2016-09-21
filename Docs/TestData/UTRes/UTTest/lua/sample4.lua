for key, value in pairs(haha) do
	print("key:" .. key)
	print("value:",value)
end
print(0 .. 1)
print("haha.value[" .. haha.value .. "]")
print(haha.inside)
print("haha.inside.value[" .. haha.inside.value .. "]")

haha.test = "input from lua"

lua_table = {1, "string", 1.0}

for key, value in pairs(lua_table) do
    print("lua_table[" .. key .. "] = " .. value)
end

function print_table(arg)
    print("arg = ", arg)
    print("arg.name = ", arg.name) 
end

function return_table(arg)
    local ret = {}
    ret.name = arg
    return ret
end