result = true

xs = list.create_empty_list()

-- push test
xs = list.push_back(xs, 1)
xs = list.push_back(xs, 2)
xs = list.push_back(xs, 3)
xs = list.push_front(xs, 4)
xs = list.push_front(xs, 5)

result = result and list.get_length(xs) == 5
result = result and list.get_value(xs, 0) == 5
result = result and list.get_value(xs, 1) == 4
result = result and list.get_value(xs, 2) == 1
result = result and list.get_value(xs, 3) == 2
result = result and list.get_value(xs, 4) == 3

-- pop test
xs = list.push_front(xs, 9)
xs = list.pop_front(xs)
xs = list.pop_back(xs)

result = result and list.get_length(xs) == 4
result = result and list.get_value(xs, 0) == 5
result = result and list.get_value(xs, 1) == 4
result = result and list.get_value(xs, 2) == 1
result = result and list.get_value(xs, 3) == 2

-- filter test
xs = list.push_front(xs, 3)
xs = list.push_front(xs, 2)
xs = list.push_back(xs, 1)

xs = list.filter(
   xs,
   function(x)
	  if math.fmod(x,2) == 0
	  then
		 return 1
	  else
		 return 0
	  end
   end
)

result = result and list.get_length(xs) == 3
result = result and list.get_value(xs, 0) == 2
result = result and list.get_value(xs, 1) == 4
result = result and list.get_value(xs, 2) == 2

-- ordering
result = result and type(Ordering.LT) == "userdata"
result = result and type(Ordering.GT) == "userdata"
result = result and type(Ordering.EQ) == "userdata"

function adicnorm(p, m)
   if m == 0
   then
	  return 0
   else
	  return (m%p) + (1/p)*adicnorm(p, math.floor(m/p) )
   end
end

function adic3order(m, n)
   if adicnorm(3, m) > adicnorm(3, n) then
	  return Ordering.GT
   elseif adicnorm(3, m) < adicnorm(3, n) then
	  return Ordering.LT
   else
	  return Ordering.EQ
   end
end

result = result and adic3order(3,1) == Ordering.LT
result = result and adic3order(2,1) == Ordering.GT
result = result and adic3order(4,28) == Ordering.GT

-- sort
xs = list.bubble_sort(xs, list.order_canonical)

result = result and list.get_value(xs, 0) == 2
result = result and list.get_value(xs, 1) == 2
result = result and list.get_value(xs, 2) == 4

-- median
xs = list.push_front(xs, 7)
xs = list.push_front(xs, 6)
xs = list.push_front(xs, 9)
xs = list.push_front(xs, 10)

result = result and list.get_median(xs, list.order_opposite, list.bubble_sort) == 6
result = result and list.get_median(xs, adic3order, list.bubble_sort) == 4

-- clean the list
list.destroy(xs)

-- write the test result
write_result(result)
