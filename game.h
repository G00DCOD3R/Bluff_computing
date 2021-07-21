#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

class Game
{
	public:
	vector <int> deck; 
	int players;
	vector <ll> cards;
	void __init();
	void give_cards(const vi & sizes);
	ll get_cards(int who);
	void set_my_cards(ll my_cards);
	ll cards_on_table();
};
