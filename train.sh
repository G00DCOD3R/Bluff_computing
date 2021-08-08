g++ -O3 game.cpp modes.cpp a.cpp -o a

for((i=1;;i++)); do
	echo $i
	#  ./a < ./best_strat > ./best_strat1
	./a > ./best_strat1
	cp ./best_strat1 ./best_strat
done
