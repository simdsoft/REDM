print(g_test.m_test)
print(g_test:is_test())
print(g_test:ret_int())
print(g_test:ret_mul(4))

temp = test(4)
print(temp.m_test)
c = temp:create_a()
a = g_test:get()
temp:set(a)
print(temp.m_test)
print(temp:is_base())
print(temp:is_test())

function objinfo(obj)
	local meta = getmetatable(obj)
	if meta ~= nil then
		metainfo(meta)
	else
		print("no object infomation !!")
	end
end

function metainfo(meta)

	if meta ~= nil then
		local name = meta["__name"]
		if name ~= nil then
			metainfo(meta["__parent"])
			print("<"..name..">")
			for key,value in pairs(meta) do
				--[print("key:",key)]
				--[print("value:",value)]
				if not string.find(key, "__..") then
					if type(value) == "function" then
						print("\t[f]"..name..":"..key.."()")
					elseif type(value) == "userdata" then
						print("\t[v]"..name..":"..key)
					end
				end
			end
		end
	end
end
print("g_test-> ", g_test)
print(objinfo(g_test))