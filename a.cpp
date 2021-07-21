#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}
#define ppc(...) __builtin_popcountll(__VA_ARGS__)

#include <bits/stdc++.h>
#include "game.h"
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

ll random(ll a, ll b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_int_distribution<ll>(a,b)(rng);
}
ld drandom(ld a, ld b)
{
	//  assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_real_distribution<ld>(a,b)(rng);
}
Game G;


void show_card(int a); // show single card
void show_deck(ll deck); // shows given deck of cards (as a mask)
bool check(ll cards, int hand); // checks if hand exist in cards (cards is mask, hand is code from 0 to 381
ll cards_to_mask(const vector <PII> & v); // converts cards to mask
void show_int(ll x); // shows long long x in binary form
int name_to_code(); // converts name of hand to its code
void code_to_name(int code); // converts code of hand to its name (writes it)
void one_turn(); // simulates one turn XD
void make_move(bool checking);
void let_make_move(bool checking);
void compvhuman_oneturn();



vi sizes;
int player_turn = 0, bet;
const int N = 382, mat_size = 65;
ll C[60][60];

struct bot
{
	int id;
	ld moves[N][mat_size][mat_size], chances[mat_size];
	void set_random()
	{
		for(int i=0;i<N;i++) for(int j=0;j<mat_size;j++) for(int g=0;g<mat_size;g++) moves[i][j][g] = drandom(0.0, 2.0);
	}
	void print_moves()
	{
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<mat_size;j++)
			{
				for(int g=0;g<mat_size;g++) cout << moves[i][j][g] << " ";
				cout << "\n";
			}
			cout << "\n";
		}
	}
	void read_moves()
	{
		for(int i=0;i<N;i++) for(int j=0;j<mat_size;j++) for(int g=0;g<mat_size;g++) cin >> moves[i][j][g];
	}
	inline ld choosing_problem(int n, int a, int b, int c)
	{
		// problem:
		//  we have n cards to deal, c special, we choose b of them
		//  what is probability that exactly a cards will be within those b cards 
		if(b - a > n - b) return 0;
		if(b < a) return 0;
		return (ld) C[n-c][b-a] / C[n][b] * C[c][a];
	}
	void calc_chances() // calculating initial chances, with hand given
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
				chances[j + i * 5] = choosing_problem(52 - sizes[id], j-cur, b, 4 - cur);
			}
			//  for(int j = 2; j >= cur; j--) chances[j + i * 4] += chances[j+1 + i * 4];
			
			mask <<= 4;
		}
		//  for(int i=0;i<mat_size;i++) cerr << setprecision(4) << fixed << chances[i] << " ";
		//  cerr << "\n";
	}
	int choose_move()
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
		vi certain;
		int cnt = 0;
		for(int i=0;i < best.size() && abs(best[i].fi - 1.0) <= 0.00000001; i++)
		{
			certain.pb(best[i].se);
			cnt = i;
		}
		cnt++;
		int K = 9;
		for(int i=1;i<K && cnt + i < best.size();i++) best[i+cnt].fi += best[i-1+cnt].fi;
		
		//  debug
		
		//  cerr << "certain: \n";
		//  for(auto e: certain) code_to_name(e);
		//  cerr << "considered options:\n";
		//  ld last = 0.0;
		
		//  for(int i=0;i<K;i++) 
		//  {
			//  cerr << "prob = " << setprecision(3) << fixed << best[i+cnt].fi - last << ", ";
			//  code_to_name(best[i+cnt].se);
			//  last = best[i+cnt].fi;
		//  }
		//  cerr << "\n";
		
		
		//  for(int i = bet + 1; i < N; i++) hand_chances[i] += hand_chances[i-1];
		
		ld go_certain = drandom(0.4, 0.8) - drandom(0.0, 1.0);
		
		if(go_certain >= 0.0 && certain.size() > 0) 
		{
			//  cerr << "with certainty: " << go_certain << "\n";
			return certain[random(0, certain.size() - 1)];
		}
		
		ld choice = drandom(0, best[K-1+cnt].fi);
		for(int i=0; i < K; i++) if(best[i+cnt].fi >= choice) return best[i+cnt].se;
		
		
	}
	void react(int x) // react to bet x
	{
		ld upd[mat_size];
		//  for(int i=0;i<mat_size;i++) upd[i] = .0;
		for(int i = 0; i < mat_size; i++)
		{
			upd[i] = 0.0;
			for(int j=0;j < mat_size; j++) 
			{
				upd[i] += moves[x][i][j] * chances[j];
			}
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
};

bot me, opp;

int main()
{
	for(int i=0;i<=52;i++) C[i][0] = 1;
	for(int i=1;i<=52;i++)
	{
		for(int j=1;j<=i;j++)
		{
			C[i][j] = C[i-1][j] + C[i-1][j-1];
		}
	}
	
	
	
	//  vector <int> deck;
	//  for(int i=0;i<52;i++) deck.pb(i);
	//  random_shuffle(all(deck));
	
	//  while(true)
	//  {
		//  show_deck(random(0, (1LL<<52)-1));
		
		//  cout << "\n";
	//  }
	
	G.__init();
	sizes.resize(G.players, 1);
	me.id = 0;
	
	
	
	
	
	
	
	while(true) // computer with human, human deals cards
	{
		cerr << "give my cards (" << sizes[me.id] << ")\n";
		ll w = 0;
		while(true)
		{
			for(int i=0;i<sizes[me.id];i++) 
			{
				string num, suit;
				cin >> num >> suit;
				vector <string> col {"C", "D", "H", "S"};
				vector <string> numb {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
				int a, b;
				for(int i=0;i<4;i++) if(suit == col[i]) 
				{
					b = i;
					break;
				}
				for(int i=0;i<13;i++) 
				{
					if(num == numb[i])
					{
						a = i;
						break;
					}
				}
				w |= ((1LL << b) << (a * 4));
				cerr << "(number, suit) now: " << a << " " << b << "\n";
			}
			cerr << "are you sure? \n";
			string tak;
			cin >> tak;
			if(tak == "yes" || tak == "y") break;
		}
		G.set_my_cards(w);
		
		compvhuman_oneturn();
		
	}
	
	while(true) // computer with human, computer deals cards
	{
		one_turn();
		string wait;
		cin >> wait;
	}
	return 0;
	
	G.__init();
	sizes.resize(G.players, 1);
	me.id = 0;
	me.set_random();
	
	
	
	opp.id = 1;
	opp.set_random();
	
	for(int sim = 0; ; sim++)
	{
		cerr << "Simulation " << sim << " now\n";
		
		
		
		//  cout << G.players << "\n";
		//  for(auto e: sizes) cout << e << " ";
		//  return 0;
		
		int lost = -1;
		while(lost == -1)
		{
			//  lost = botvbot(); // one turn of bot vs bot
			if(lost != -1)
			{
				if(lost == 1)
				{
					me.print_moves();
				}
				else opp.print_moves();
			}
		}
	}
	
	while(true)
	{
		one_turn();
		string wait;
		cin >> wait;
	}
	
	
	return 0;
}

void compvhuman_oneturn()
{
	//  G.give_cards(sizes);
	me.calc_chances();
	
	// for debug, you know each other's cards
	
	cerr << "I have " << sizes[me.id] << " cards\n";
	
	//  cerr << "my cards\n";
	//  show_deck(G.get_cards(0));
	//  cerr << "your cards\n";
	//  show_deck(G.get_cards(1));
	
	//  me.calc_chances();
	//  code_to_name(me.choose_move());
	//  return;
	
	bet = -1;
	while(true)
	{
		if(player_turn == 0) // my turn
		{
			make_move(0);
		}
		else
		{
			let_make_move(0); // opp's turn
			//  if(bet > -1) me.react(bet);
		}
		if(bet == -1) 
		{
			int won;
			cerr << "Was there such hand?\n";
			cin >> won;
			if(won == 0) 
			{
				int n = G.players;
				sizes[(player_turn + n - 1) % n]++;
				player_turn = (player_turn + n - 1) % n;
				cerr << "doesn't exist\n";
			}
			else
			{
				sizes[player_turn]++;
				cerr << "exists\n";
			}
			return;
		}
		player_turn++;
		if(player_turn == G.players) player_turn = 0;
	}
}


void let_make_move(bool checking)
{
	while(true)
	{
		int cur = name_to_code();
		if(cur == -1 && checking)
		{
			ll table = G.cards_on_table();
			show_deck(table);
			if(check(table, bet))
			{
				sizes[player_turn]++;
				cerr << "exists\n";
			}
			else
			{
				int n = G.players;
				sizes[(player_turn + n - 1) % n]++;
				player_turn = (player_turn + n - 1) % n;
				cerr << "doesn't exist\n";
			}
			bet = -1;
			return;
		}
		if(cur == -1 && checking == 0)
		{
			bet = -1;
			return;
		}
		if(cur <= bet) 
		{
			cerr << "INVALID MOVE, current bet is: ";
			code_to_name(bet);
			continue;
		}
		else
		{
			bet = cur;
			return;
		}
	}
}
void make_move(bool checking)
{
	//  cerr << "all my bets (last is valid)\n";
	int cur = -1;
	//  for(int i=0;i<10;i++)
	//  {
		cur = me.choose_move();
		code_to_name(cur);
	//  }
	cerr << "\n";
	if(cur == -1 && checking)
	{
		ll table = G.cards_on_table();
		show_deck(table);
		if(check(table, bet))
		{
			sizes[player_turn]++;
			cerr << "exists\n";
		}
		else
		{
			int n = G.players;
			sizes[(player_turn + n - 1) % n]++;
			player_turn = (player_turn + n - 1) % n;
			cerr << "doesn't exist\n";
		}
		bet = -1;
		return;
	}
	if(cur == -1 && checking == 0)
	{
		bet = -1;
		return;
	}
	bet = cur;
}

void one_turn()
{
	G.give_cards(sizes);
	me.calc_chances();
	
	// for debug, you know each other's cards
	
	cerr << "I have " << sizes[me.id] << " cards\n";
	
	//  cerr << "my cards\n";
	//  show_deck(G.get_cards(0));
	cerr << "your cards\n";
	show_deck(G.get_cards(1));
	
	me.calc_chances();
	//  code_to_name(me.choose_move());
	//  return;
	
	bet = -1;
	while(true)
	{
		if(player_turn == 0) // my turn
		{
			make_move(1);
		}
		else
		{
			let_make_move(1); // opp's turn
			//  if(bet > -1) me.react(bet);
		}
		if(bet == -1) return;
		player_turn++;
		if(player_turn == G.players) player_turn = 0;
	}
}

void show_card(int a)
{
	// showing cards 2 - 14 (higher than 10 are J, Q, K, A. 
	// colors are: C, D, H, S (0 - 3)
	
	assert(a >= 0 && a < 52);
	PII cur = mp(a / 4, a % 4);
	if(cur.fi <= 8) cerr << cur.fi + 2;
	else 
	{
		vector <char> opts {'J', 'Q', 'K', 'A'};
		cerr << opts[cur.fi - 9];
	}
	vector <char> color {'C', 'D', 'H', 'S'};
	cerr << color[cur.se] << " ";
}

void show_deck(ll deck)
{
	for(int i=0;i<52;i++) if(deck&(1LL<<i)) show_card(i);
	cerr << "\n";
}

ll cards_to_mask(const vector <PII> & v)
{
	ll w = 0;
	for(auto &e: v) w |= ((1LL << e.se) << ((e.fi - 2) * 4));
	return w;
}

void show_int(ll x)
{
	for(int i=0;i<52;i++)
	{
		if(x & (1LL << i)) cerr << "1 ";
		else cerr << "0 ";
	}
	cerr << "\n";
}

bool check(ll cards, int hand)
{
	// cards is mask of cards in order: 
	//  2C, 2D, 2H, 2S, ... , AC, AD, AH, AS (0 - 51)
	if(hand < 0) return false;
	if(hand < 13)
	{
		// highest card;
		ll mask = 15;
		mask <<= hand * 4;
		mask &= cards;
		if(mask > 0) return true;
		return false;
	}
	else if(hand < 26)
	{
		// pair
		ll mask = 15;
		mask <<= (hand - 13) * 4;
		mask = (cards&mask);
		int tmp = ppc(mask);
		if(tmp >= 2) return true;
		return false;
		
	}
	else if(hand < 182)
	{
		// double pair
		hand -= 26;
		int a = hand / 12;
		int b = hand - a * 12;
		if(b >= a) b++;
		ll mask = (15LL << (a * 4));
		mask &= cards;
		if(ppc(mask) < 2) return false;
		mask = (15LL << (b * 4));
		mask &= cards;
		if(ppc(mask) < 2) return false;
		return true;
	}
	else if(hand < 195)
	{
		// three of a kind
		hand -= 182;
		ll mask = (15LL << (hand * 4));
		mask &= cards;
		if(ppc(mask) < 3) return false;
		return true;
	}
	else if(hand < 204)
	{
		// straight
		hand -= 195;
		ll mask = (15LL << (hand * 4));
		for(int i=0;i<5;i++) 
		{
			ll tmp = mask & cards;
			if(ppc(tmp) == 0) return false;
			mask <<= 4;
		}
		return true;
	}
	else if(hand < 360)
	{
		// full house
		hand -= 204;
		int a = hand / 12;
		int b = hand - a * 12;
		if(b >= a) b++;
		ll mask = (15LL << (a * 4));
		mask &= cards;
		if(ppc(mask) < 3) return false;
		mask = (15LL << (b * 4));
		mask &= cards;
		if(ppc(mask) < 2) return false;
		return true;
		
	}
	else if(hand < 373)
	{
		// four of a kind
		hand -= 360;
		ll mask = (15LL << (hand * 4));
		mask &= cards;
		//  if(ppc(mask) >= 4) return true;
		//  return false;
		if(ppc(mask) < 4) return false;
		return true;
	}
	else if(hand < 382)
	{
		// straight flush
		hand -= 373;
		for(int i=0;i<4;i++) 
		{
			bool ok = 1;
			ll mask = ((1LL << i) << (hand * 4));
			for(int j=0;j<5;j++) 
			{
				ll tmp = mask & cards;
				if(ppc(tmp) == 0) 
				{
					ok = 0;
					break;
				}
				mask <<= 4;
			}
			if(ok) return true;
		}
		return false;
	}
	cerr << "CHECKING NON-EXISTING HAND\n";
	return false;
}

int name_to_code()
{
	// available names: (in polish)
	
	//  jedna X
	//  para X
	//  dwie X Y
	//  trzy X
	//  strit X
	//  full X Y
	//  kareta X
	//  poker X
	
	
	string s;
	while(true)
	{
		cin >> s;
		if(s == "check") return -1;
		string type;
		cin >> type;
		vector <string> opts {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
		int who = -1;
		for(int i=0;i<opts.size();i++)
		{
			if(opts[i] == type)
			{
				who = i;
				break;
			}
		}
		if(who == -1)
		{
			cerr << "No such card!\n";
			continue;
		}
		if(s == "jedna")
		{
			return who;
		}
		if(s == "para")
		{
			return 13 + who;
		}
		if(s == "dwie")
		{
			cin >> type;
			int sec = -1;
			for(int i=0;i<opts.size();i++)
			{
				if(opts[i] == type)
				{
					sec = i;
					break;
				}
			}
			if(sec == -1) 
			{
				cerr << "No such card (second)!\n";
				continue;
			}
			if(sec > who) sec--;
			return 26 + who * 12 + sec;
		}
		if(s == "trzy")
		{
			return 182 + who;
		}
		if(s == "strit")
		{
			if(who > 8) 
			{
				cerr << "No such strit\n";
				continue;
			}
			return 195 + who;
		}
		if(s == "full")
		{
			cin >> type;
			int sec = -1;
			for(int i=0;i<opts.size();i++)
			{
				if(opts[i] == type)
				{
					sec = i;
					break;
				}
			}
			if(sec == -1) 
			{
				cerr << "No such card (second)!";
				continue;
			}
			if(sec > who) sec--;
			return 204 + who * 12 + sec;
		}
		if(s == "kareta")
		{
			return 360 + who;
		}
		if(s == "poker")
		{
			if(who > 8)
			{
				cerr << "No such poker\n";
				continue;
			}
			return 373 + who;
		}
		cerr << "no such poker hand\ntry for example:\n";
		cerr << "jedna X\npara X\ndwie X Y\ntrzy X\nstrit X\nfull X Y\nkareta X\npoker X\n\nWhere X Y signify cards\n";
	}
	
}
void code_to_name(int code)
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
		cerr << "jedna " << opts[code] << "\n";
		
	}
	else if(code < 26)
	{
		// pair
		cerr << "para " << opts[code - 13] << "\n";
	}
	else if(code < 182)
	{
		// double pair
		code -= 26;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		cerr << "dwie " << opts[a] << " " << opts[b] << "\n";
	}
	else if(code < 195)
	{
		// three of a kind
		code -= 182;
		cerr << "trzy " << opts[code] << "\n";
	}
	else if(code < 204)
	{
		// straight
		code -= 195;
		cerr << "strit " << opts[code] << "\n";
	}
	else if(code < 360)
	{
		// full house
		code -= 204;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		cerr << "full " << opts[a] << " " << opts[b] << "\n";
		
	}
	else if(code < 373)
	{
		// four of a kind
		code -= 360;
		cerr << "kareta " << opts[code] << "\n";
	}
	else if(code < 382)
	{
		// straight flush
		code -= 373;
		cerr << "pokerzysko " << opts[code] << "\n";
	}
}
