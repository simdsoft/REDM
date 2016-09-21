factory = HgyObjFactory()
obj = factory:CreateObj()

turret = {}
function turret:create()
	local t = {}
	t.hp = 100;
	t.hit = hits:CreateObj()
	print( t, "turret:create " )
	return t
end
function turret:destroy( t )
	print( t, "turret:destroy " )
	hits:DestroyObj( t.hit )
	t = nil;
end