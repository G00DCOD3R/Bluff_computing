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

using namespace std;
mt19937 rng1(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng1)


ll random(ll a, ll b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_int_distribution<ll>(a,b)(rng1);
}
ld drandom(ld a, ld b)  // double randomization
{
	//  assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_real_distribution<ld>(a,b)(rng1);
}

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


// BOT STRUCT DECLARATION

#define N 382
#define mat_size 65

void bot::set_random() // set random matrices for moves
{
	for(int g=0;g<8;g++) for(int i=0;i<2;i++) for(int j=0;j<5;j++) mvs[g][i][j] = drandom(0.0, 2.0);
	//  for(int i=0;i<N;i++) for(int j=0;j<mat_size;j++) for(int g=0;g<mat_size;g++) moves[i][j][g] = drandom(0.0, 2.0);
}
void bot::perform_minor() // performing minor changes on moves matrices
{
	// for small matrices
	for(int g=0;g<8;g++)
	{
		for(int i=0;i<2;i++) 
		{
			for(int j=0;j<5;j++) 
			{
				int change = random(0, 1);
				if(change == 0) continue;
				change = random(0, 1);
				if(change) mvs[g][i][j] += drandom(0.01, 0.5);
				else mvs[g][i][j] -= drandom(0.01, 0.5);
				mvs[g][i][j] = max(mvs[g][i][j], (ld)0.0);
				mvs[g][i][j] = min(mvs[g][i][j], (ld)2.0);
			}
		}
	}
	// for large matrices
	//  for(int i=0;i<N;i++) 
	//  {
		//  for(int j = 0; j < mat_size; j++) 
		//  {
			//  for(int g = 0; g < mat_size; g++) 
			//  {
				//  int change = random(0, 5);
				//  if(change < 4) continue;
				//  if(change == 4) moves[i][j][g] += drandom(0.01, 0.6);
				//  else moves[i][j][g] -= drandom(0.01, 0.6);
				//  moves[i][j][g] = min((ld)2.0, moves[i][j][g]);
				//  moves[i][j][g] = max((ld)0.0, moves[i][j][g]);
			//  }
		//  }
	//  }
}
void bot::print_moves() // print those matrices
{
	for(int g=0;g<8;g++)
	{
		for(int i=0;i<2;i++) 
		{
			for(int j=0;j<5;j++) cout << setprecision(6) << fixed << mvs[g][i][j] << " ";
			cout << "\n";
		}
	}
	//  for(int i=0;i<N;i++)
	//  {
		//  for(int j=0;j<mat_size;j++)
		//  {
			//  for(int g=0;g<mat_size;g++) cout << setprecision(6) << fixed << moves[i][j][g] << " ";
			//  cout << "\n";
		//  }
		//  cout << "\n";
	//  }
}
void bot::read_moves(string where) // reads matrices from input
{
	ifstream myfile;
	myfile.open(where);
	for(int g=0;g<8;g++) for(int i=0;i<2;i++) for(int j=0;j<5;j++) myfile >> mvs[g][i][j];
	//  for(int i=0;i<N;i++) for(int j=0;j<mat_size;j++) for(int g=0;g<mat_size;g++) myfile >> moves[i][j][g];
	myfile.close();
}
inline ld bot::choosing_problem(const vector <vector <ll>> & C, int n, int a, int b, int c)
{
	// problem:
	//  we have n cards to deal, c special, we choose b of them
	//  what is probability that exactly a cards will be within those b cards 
	if(b - a > n - b) return 0;
	if(b < a) return 0;
	return (ld) C[n-c][b-a] / C[n][b] * C[c][a];
}
void bot::calc_chances(const vector <vector <ll>> & C, const vi & sizes, Game & G, int & bet) // calculating initial chances, with hand given
{
	ll my_hand = G.get_cards(id);
	int b = 0;
	for(int i=0;i<G.players;i++)
	{
		if(i == id) continue;
		b += sizes[i];
	}
	
	for(int i=0;i<mat_size;i++) chances[i] = 0.0;
	ll mask = 15;
	for(int i=0;i<13;i++)
	{
		ll tmp = (mask & my_hand);
		int cur = ppc(tmp);
		
		for(int j = cur; j < 5; j++)
		{
			chances[j + i * 5] = choosing_problem(C, 52 - sizes[id], j-cur, b, 4 - cur);
		}
		//  for(int j = 2; j >= cur; j--) chances[j + i * 4] += chances[j+1 + i * 4];
		
		mask <<= 4;
	}
	//  for(int i=0;i<mat_size;i++) cerr << setprecision(4) << fixed << chances[i] << " ";
	//  cerr << "\n";
}
int bot::choose_move(Game & G, int & bet)
{
	// thand stands for temporary hand :)) 
	ld hand_chances[N];
	//  for(int i=0;i<bet;i++) hand_chances[i] = 0.0;
	//  cerr << "hand_chances: \n";
	for(int hand = 0; hand < N; hand++)
	{
		hand_chances[hand] = 0.0;
		if(hand < 13)
		{
			// highest card;	
			for(int i=1;i<5;i++) hand_chances[hand] += chances[hand * 5 + i];
		}
		else if(hand < 26)
		{
			// pair
			int thand = hand -  13;
			for(int i=2;i<5;i++) hand_chances[hand] += chances[thand * 5 + i];
		}
		else if(hand < 182)
		{
			// double pair
			int thand = hand -  26;
			int a = thand / 12;
			int b = thand - a * 12;
			if(b >= a) b++;
			else continue;
			ld A, B;
			A = B = 0.0;
			for(int i=2;i<5;i++) A += chances[a * 5 + i];
			for(int i=2;i<5;i++) B += chances[b * 5 + i];
			hand_chances[hand] = A * B;
		}
		else if(hand < 195)
		{
			// three of a kind
			int thand = hand -  182;
			for(int i=3;i<5;i++) hand_chances[hand] += chances[thand * 5 + i];
		}
		else if(hand < 204)
		{
			// straight
			int thand = hand -  195;
			hand_chances[hand] = 1.0;
			for(int i=0;i<5;i++)
			{
				hand_chances[hand] *= (1 - chances[(thand+i) * 5]);
			}
		}
		else if(hand < 360)
		{
			// full house
			int thand = hand -  204;
			int a = thand / 12;
			int b = thand - a * 12;
			if(b >= a) b++;
			ld A, B;
			A = B = 0.0;
			for(int i=3;i<5;i++) A += chances[a * 5 + i];
			for(int i=2;i<5;i++) B += chances[b * 5 + i];
			hand_chances[hand] = A * B;
			
		}
		else if(hand < 373)
		{
			// four of a kind
			int thand = hand -  360;
			for(int i=4;i<5;i++) hand_chances[hand] += chances[thand * 5 + i];
		}
		else if(hand < 382)
		{
			// straight flush
			hand_chances[hand] = hand_chances[hand - 178] / 256;
			
		}
		//  cerr << setprecision(5) << fixed << hand_chances[hand] << " ";
	}
	//  cerr << "\n";
	
	ld better = -1.0;
	for(int i=bet+1; i < N; i++) better = max(better, hand_chances[i]);
	if(bet > -1 && 1.0 - hand_chances[bet] > better) return -1;
	vector <pair<ld, int>> best;
	for(int i = bet + 1; i < N; i++) best.pb(mp(hand_chances[i], i));
	sort(all(best));
	reverse(all(best));
	int K = 9;
	
	if(bet > -1)
	{
		//  cerr << "equalizing chances\n";
		for(int i=0;i<K;i++)
		{
			if(best[i].fi < 1.0 - hand_chances[bet]) 
			{
				K = i;
				break;
			}
		}
	}
	
	//  for(int i=0;i<K;i++) 
	//  {
		//  cerr << "prob = " << setprecision(3) << fixed << best[i].fi << ", ";
		//  code_to_name(best[i].se);
		//  // //  last = best[i].fi;
	//  }
	//  cerr << "\n";
	
	for(int i=0;i<K;i++) best[i].fi = min((ld)1, best[i].fi * best[i].fi * 1.8);
	for(int i=1;i<K && i < (int)best.size();i++) best[i].fi += best[i-1].fi;
	
	//  debug
	
	//  cerr << "certain: \n";
	//  for(auto e: certain) code_to_name(e);
	//  cerr << "considered options:\n";
	//  ld last = 0.0;
	
	
	
	
	//  for(int i = bet + 1; i < N; i++) hand_chances[i] += hand_chances[i-1];
	
	ld choice = drandom(0, best[K-1].fi);
	for(int i=0; i < K; i++) if(best[i].fi >= choice) return best[i].se;
	
	
}
void bot::react(int x) // react to bet x
{
	ld upd[mat_size];
	//  for(int i=0;i<mat_size;i++) upd[i] = .0;
	//  for(int i = 0; i < mat_size; i++)
	//  {
		//  upd[i] = 0.0;
		//  for(int j=0;j < mat_size; j++) 
		//  {
			//  upd[i] += moves[x][i][j] * chances[j];
		//  }
	//  }
	pair<int, ll> cur = cards_used(x); // current bet (0 - 7) and cards used (mask)
	
	for(int i=0;i<13;i++) 
	{
		int bit = (cur.se&(1<<i));
		if(bit > 0) bit = 1;
		for(int j=0;j<5;j++) upd[i * 5 + j] = chances[i * 5 + j] * mvs[cur.fi][bit][j];
	}
	
	for(int i=0;i<13;i++) 
	{
		ld sum = 0.0;
		for(int j=0;j<5;j++)
		{
			sum += upd[i * 5 + j];
		}
		if(sum < 0.00000001) continue;
		for(int j=0;j<5;j++) chances[j + i * 5] = upd[j + i * 5] / sum;
	}
	//  for(int i=0;i<mat_size;i++) cerr << setprecision(3) << fixed << chances[i] << " ";
	//  cerr << "\n";
}

pair<int,ll> cards_used(int code) 
{
	if(code == -1) return mp(-1, 0);
	ll mask = 0;
	int type = 0;
	if(code < 13)
	{
		type = 0;
		// highest card;
		mask |= (1LL << code);
	}
	else if(code < 26)
	{
		// pair
		type = 1;
		code -= 13;
		mask |= (1LL << code);
	}
	else if(code < 182)
	{
		// double pair
		type = 2;
		code -= 26;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		//  cerr << "dwie " << opts[a] << " " << opts[b] << "\n";
		mask |= (1LL << a);
		mask |= (1LL << b);
	}
	else if(code < 195)
	{
		// three of a kind
		code -= 182;
		type = 3;
		mask |= (1LL << code);
		//  cerr << "trzy " << opts[code] << "\n";
	}
	else if(code < 204)
	{
		// straight
		type = 4;
		code -= 195;
		for(int i=0;i<5;i++) mask |= (1LL << (code + i));
	}
	else if(code < 360)
	{
		// full house
		code -= 204;
		type = 5;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		mask |= (1LL << a);
		mask |= (1LL << b);
		
	}
	else if(code < 373)
	{
		// four of a kind
		code -= 360;
		type = 6;
		mask |= (1LL << code);
	}
	else if(code < 382)
	{
		// straight flush
		code -= 373;
		type = 7;
		for(int i=0;i<5;i++) mask |= (1LL << (code + i));
	}
	return mp(type, mask);
}


#undef N
#undef mat_size
