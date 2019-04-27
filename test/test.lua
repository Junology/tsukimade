function makevec(x, y)
   local vec = {}
   vec["x"] = x
   vec["y"] = y
   return vec
end

-- Initialize test result
test = true

v = makevec(3,4)

-- Test norm
test = test and (veclib.norm(v) - 5.0 < 0.0001)

-- Test gradient descent
function f(v)
   return math.sin(v["x"])*math.sin(v["y"])
end

u = v

for i=1,1000 do
   u = veclib.add_vecs(u, veclib.gradient(0.1, f, u))
end

test = test and (f(u) > f(v))

-- Write test result
if test
then
   veclib.write_result(1)
else
   veclib.write_result(0)
end
