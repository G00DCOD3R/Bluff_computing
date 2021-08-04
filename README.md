# Bluff_computing
AI for playing bluff (card game)
working Machine Learning!!!!!!!!!!!!!!!!

To compile: 
```
g++ -O3 a.cpp modes.cpp game.cpp -o a
```

# Machine Learning
This code uses machine learning. 
I even prepared a script called train.sh 
The only thing you have to do is:
```
bash train.sh
```
and it will do the rest!
You can stop it at any time and best progress won't be lost

# How does it work? (ML training) 
  So every time it randomizes few matrices, then compare them 
 (the best one which won most of the games stays) and do few iterations of minor changes
 (see perform_minor() in bot struct, game.cpp)
 
 Then it returns the best result it found (final strat)
 
 What I think could be improved is comparing strategies. 
 
 For example compare their win percentage against some random strats probably would bring better results
 
