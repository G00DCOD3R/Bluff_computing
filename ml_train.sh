g++ -O3 ml.cpp game.cpp modes.cpp -o ml

for((i=1;;i++)); do
	echo $i
	./ml > ./sample/best_strat1
	cp ./sample/best_strat1 ./sample/best_strat

done

