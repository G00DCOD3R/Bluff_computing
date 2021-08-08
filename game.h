#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

ll random(ll a, ll b);
ld drandom(ld a, ld b);

int name_to_code();
void code_to_name(int code);

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


struct bot
{
	#define N 382
	#define mat_size 65
	//  const int mat_size = 65;
	
	int id, wins;
	//  ld moves[N][mat_size][mat_size]; // I think it's useless now, but I can be wrong
	ld chances[mat_size];
	ld mvs[8][2][5];
	
	void set_random();
	void perform_minor();
	void print_moves();
	void read_moves(string where);
	inline ld choosing_problem(const vector <vector <ll>> & C, int n, int a, int b, int c);
	void calc_chances(const vector <vector <ll>> & C, const vi & sizes, Game & G, int & bet);
	int choose_move(Game & G, int & bet);
	void react(int x);
	
	
	#undef N
	#undef mat_size
	
};

pair<int, ll> cards_used(int code);
