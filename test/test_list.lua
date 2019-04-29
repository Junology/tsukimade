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

-- sort
xs = list.bubble_sort(xs, list.compare_canonical)

result = result and list.get_value(xs, 0) == 2
result = result and list.get_value(xs, 1) == 2
result = result and list.get_value(xs, 2) == 4

-- median
xs = list.push_front(xs, 7)
xs = list.push_front(xs, 8)
xs = list.push_front(xs, 9)
xs = list.push_front(xs, 10)

result = result and list.get_median(xs, list.compare_opposite, list.bubble_sort) == 7

-- clean the list
list.destroy(xs)

-- write the test result
write_result(result)
