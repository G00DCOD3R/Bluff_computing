#include <bits/stdc++.h> 

#ifndef gameh
#define gameh
#include "game.h"
#endif


using namespace std;

void one_turn(Game & G, int & bet, vi & sizes, bot & me, int & player_turn, const vector <vector <ll>> & C); // simulates one turn XD // modes
void make_move(bool checking, Game & G, int & bet, bot & me, vi & sizes, int & player_turn); // bot makes move, checking is variable for determining who won (after check)
void let_make_move(bool checking, Game & G, int & bet, vi & sizes, int & player_turn); // human makes move
void compvhuman_oneturn(Game & G, int & bet, bot & me, const vector <vector <ll>> & C, vi & sizes, int & player_turn); // simulates one turn human vs computer, human deals cards // mode 
void botvbot(vi & sizes, Game & G, int & bet, vector <bot> & bots, int & player_turn, const vector <vector <ll> > & C); // mode
void computer_deals(vi & sizes, Game & G, int & bet, bot & me, int & player_turn, const vector<vector<ll>> & C); // mode
void human_deals(vi & sizes, Game & G, int & bet, bot & me, int & player_turn, const vector<vector<ll>> & C); // mode
void bot_move(int id, Game & G, int & bet, vector <bot> & bots, vi & sizes, int & player_turn); // for mode 

void show_card(int a);
void show_deck(ll deck);
ll cards_to_mask(const vector <PII> &v);
void show_int(ll x);
bool check(ll cards, int hand);
