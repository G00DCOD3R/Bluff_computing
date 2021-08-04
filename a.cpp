#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}
#define ppc(...) __builtin_popcountll(__VA_ARGS__)

#include <bits/stdc++.h>

#ifndef gameh
#define gameh
#include "game.h"
#endif

#ifndef modesh
#define modesh
#include "modes.h"
#endif

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng)

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;
constexpr ll nax = 2e5+6969, INF = 2e9+6969;

/*
ll random(ll a, ll b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_int_distribution<ll>(a,b)(rng);
}
ld drandom(ld a, ld b)  // double randomization
{
	//  assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_real_distribution<ld>(a,b)(rng);
}
//  Game G;
*/



void show_card(int a); // show single card // gui 
void show_deck(ll deck); // shows given deck of cards (as a mask) // gui
bool check(ll cards, int hand); // checks if hand exist in cards (cards is mask, hand is code from 0 to 381
//  ll cards_to_mask(const vector <PII> & v); // converts cards to mask. Bruh I don't even use it 
void show_int(ll x); // shows long long x in binary form. // gui, but mostly for debugging
int name_to_code(); // converts name of hand to its code // gui
void code_to_name(int code); // converts code of hand to its name (writes it) // gui 

pair<int,ll> cards_used(int code); // returns which cards are used in particular hand



vi sizes; // sizes[i] signify how many cards have player i 
int player_turn = 0;

bot me, opp;
vector <bot> bots(30);
vector <vector <ll>> C(60, vector<ll>(60, 0)); // newton's binomial

int main()
{
	
	Game G;
	int bet = -1;
	G.players = 2;
	G.__init();
	#warning this doesnt read number of players!!!
	
	for(int i=0;i<=52;i++) C[i][0] = 1;
	for(int i=1;i<=52;i++)
	{
		for(int j=1;j<=i;j++)
		{
			C[i][j] = C[i-1][j] + C[i-1][j-1];
		}
	}
	// preprocessing end
	
	//  while(true)
	//  {
		//  int tmp = name_to_code();
		//  pair<int, ll> cur = cards_used(tmp);
		//  cout << cur.fi << "\n";
		//  show_int(cur.se);
		//  cout << "\n";
	//  }
	//  return 0;
	
	//  bots[0].read_moves("noreact_strat");
	//  bots[1].read_moves("best_strat");
	
	
	computer_deals(sizes, G, bet, me, player_turn, C);
	return 0;
	
	//  G.players = 2;
	//  G.__init();
	//  sizes.resize(2);
	//  if(G.players == 2) 
	//  {
		//  sizes[0] = sizes[1] = 1;
	//  }
	//  else for(auto & e: sizes) e = 1;
	//  for(int i=0;i<G.players;i++) bots[i].id = i;
	//  // one simulation
	//  for(int turn = 0; turn < 15; turn++)
	//  {
		//  botvbot();
		//  string wait;
		//  cin >> wait;
	//  }
	//  int M = 0;
	//  for(int i=1;i<G.players;i++) if(sizes[i] < sizes[M]) M = i;
	//  bots[M].wins++;
	//  cout << M << "\n";
	//  return 0;
	
	
	// Bots tournament implementation (for ML)
	
	
	
	for(int i=0;i<G.players;i++) 
	{
		bots[i].id = i;
		bots[i].set_random();
	}
	sizes.resize(G.players);
	
	clock_t ST = clock();
	bots[0].read_moves("best_strat");
	
	int last_winner = 0, reps = 20001, reps_done = reps;
	
	for(int turnaments = 0; turnaments < 5; turnaments++)
	{
		reps_done = reps;
		
		for(int i=0;i<G.players;i++) 
		{
			bots[i].wins = 0;
			if(i == last_winner) continue;
			bots[i].set_random();
		}
	
		//  bots[0].print_moves();
		//  return 0;
		
		
		
		for(int sim = 0; sim < reps; sim++)
		{
			if(G.players == 2) 
			{
				sizes[0] = sizes[1] = 1;
			}
			else for(auto & e: sizes) e = 1;
			// one simulation
			for(int turn = 0; turn < 15; turn++)
			{
				botvbot(sizes, G, bet, bots, player_turn, C);
			}
			int M = 0;
			for(int i=1;i<G.players;i++) if(sizes[i] < sizes[M]) M = i;
			bots[M].wins++;
			
			if(sim > 100 && (ld) bots[M].wins / (sim+1) > .90)
			{
				reps_done = sim + 1;
				break;
			}
			
		}
		
		for(int i=0;i<G.players;i++) 
		{
			if(bots[i].wins > bots[last_winner].wins) last_winner = i;
		}
	}
	for(int i=0;i<G.players;i++)
	{
		if(i == last_winner) continue;
		for(int j = 0; j < 8; j++) 
		{
			for(int g = 0; g < 2; g++) 
			{
				for(int h = 0; h < 5; h++)
				{
					bots[i].mvs[j][g][h] = bots[last_winner].mvs[j][g][h];
				}
			}
		}
	}
	
	clock_t EN = clock();
	cerr << "one simulation (rand): " << (ld) (EN - ST) / CLOCKS_PER_SEC << "\n";
	cerr << "win % = " << setprecision(4) << fixed << (ld) bots[last_winner].wins / reps_done << "\n";
	reps_done = reps;
	
	for(int turnaments = 0; turnaments < 5; turnaments++)
	{
		reps_done = reps;
		
		for(int i=0;i<G.players;i++) 
		{
			bots[i].wins = 0;
			if(i == last_winner) continue;
			bots[i].perform_minor();
		}
	
		//  bots[0].print_moves();
		//  return 0;
		
		
		
		for(int sim = 0; sim < reps; sim++)
		{
			if(G.players == 2) 
			{
				sizes[0] = sizes[1] = 1;
			}
			else for(auto & e: sizes) e = 1;
			// one simulation
			for(int turn = 0; turn < 11; turn++)
			{
				botvbot(sizes, G, bet, bots, player_turn, C);
			}
			int M = 0;
			for(int i=1;i<G.players;i++) if(sizes[i] < sizes[M]) M = i;
			bots[M].wins++;
			
			if(sim > 100 && (ld) bots[M].wins / (sim+1) > .90)
			{
				reps_done = sim + 1;
				break;
			}
			
		}
		
		for(int i=0;i<G.players;i++) 
		{
			if(bots[i].wins > bots[last_winner].wins) last_winner = i;
		}
	}
	
	EN = clock();
	cerr << "one simulation (minor): " << (ld) (EN - ST) / CLOCKS_PER_SEC << "\n";
	cerr << "win % = " << setprecision(4) << fixed << (ld) bots[last_winner].wins / reps_done << "\n";
	
	
	bots[last_winner].print_moves();
	
	//  computer_deals();
	//  human_deals();
	
	
	
	//  vector <int> deck;
	//  for(int i=0;i<52;i++) deck.pb(i);
	//  random_shuffle(all(deck));
	
	//  while(true)
	//  {
		//  show_deck(random(0, (1LL<<52)-1));
		
		//  cout << "\n";
	//  }
	
	
	
	//  // bot vs bot part
	//  opp.id = 1;
	//  for(int turns = 0; turns < 20; turns++)
	//  {
		//  botvbot();
	//  }
	
	
	
	//  return 0;
	return 0;
	
	while(true)
	{
		one_turn(G, bet, sizes, me, player_turn, C);
		string wait;
		cin >> wait;
	}
	
	
	return 0;
}


//  TEMPLATE OF CODES (HANDS):
void templates(int code)
{
if(code == -1) 
	{
		cerr << "check\n";
		return;
	}
	vector <string> opts {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	if(code < 0) 
	{
		cerr << "no such code (" << code << ")\n";
		return;
	}
	if(code < 13)
	{
		// highest card;
		
	}
	else if(code < 26)
	{
		// pair
		code -= 13;
	}
	else if(code < 182)
	{
		// double pair
		code -= 26;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		
	}
	else if(code < 195)
	{
		// three of a kind
		code -= 182;
		
	}
	else if(code < 204)
	{
		// straight
		code -= 195;
		
	}
	else if(code < 360)
	{
		// full house
		code -= 204;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		
		
	}
	else if(code < 373)
	{
		// four of a kind
		code -= 360;
		
	}
	else if(code < 382)
	{
		// straight flush
		code -= 373;
	}
}


