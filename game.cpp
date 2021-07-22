#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
#include "game.h"
using namespace std;
mt19937 rng1(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng1)

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;


	//  vector <int> deck; 
	//  int players;
	//  vector <ll> cards;
	
void Game::__init()
{
	//  cerr << "enter number of players (> 1)\n";
	//  printf("enter number of players (> 1)\n");
	//  scanf("%d", &players);
	
	assert(players > 1 && players <= 20);
	cards.resize(players);
	for(int i=0;i<52;i++) deck.pb(i);
	random_shuffle(all(deck));
}
void Game::give_cards(const vi & sizes)
{
	assert((int)sizes.size() == players);
	random_shuffle(all(deck));
	int cnt = 0; 
	for(int j = 0; j < (int)sizes.size(); j++) 
	{
		ll cur = 0;
		for(int i=0;i<sizes[j];i++) 
		{
			assert(cnt < 52);
			cur |= (1LL << deck[cnt]);
			cnt++;
		}
		cards[j] = cur;
	}
}
ll Game::get_cards(int who) 
{
	assert(who >= 0 && who < players);
	return cards[who];
}
void Game::set_my_cards(ll my_cards) // only when we don't know other's cards
{
	cards[0] = my_cards;
}
ll Game::cards_on_table()
{
	ll w = 0;
	for(auto e: cards) w |= e;
	return w;
}
