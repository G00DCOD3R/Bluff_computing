#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}
#define ppc(...) __builtin_popcountll(__VA_ARGS__)

#include <bits/stdc++.h>

#ifndef modesh
#define modesh
#include "modes.h"
#endif

#ifndef gameh
#define gameh
#include "game.h"
#endif

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;
constexpr ll nax = 2e5+6969, INF = 2e9+6969;



void human_deals(vi & sizes, Game & G, int & bet, bot & me, int & player_turn, const vector<vector<ll>> & C)
{
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
				for(int j=0;j<4;j++) if(suit == col[j]) 
				{
					b = j;
					break;
				}
				for(int j=0;j<13;i++) 
				{
					if(num == numb[j])
					{
						a = j;
						break;
					}
				}
				w |= ((1LL << b) << (a * 4));
				cerr << "(number, suit) now: " << a << " " << b << "\n";
			}
			cerr << "are you sure? \n";
			string tak; // yes in polish 
			cin >> tak;
			if(tak == "yes" || tak == "y") break;
		}
		G.set_my_cards(w);
		
		compvhuman_oneturn(G, bet, me, C, sizes, player_turn);
		
	}
}
void computer_deals(vi & sizes, Game & G, int & bet, bot & me, int & player_turn, const vector<vector<ll>> & C)
{	
	
	sizes.resize(G.players, 1);
	me.id = 0;
	me.read_moves("./sample/best_strat");
	while(true) // computer with human, computer deals cards
	{
		one_turn(G, bet, sizes, me, player_turn, C);
		string wait;
		cin >> wait;
	}
}

void botvbot(vi & sizes, Game & G, int & bet, vector <bot> & bots, int & player_turn, const vector <vector <ll> > & C)
{
	G.give_cards(sizes);
	for(int i=0;i<G.players;i++) bots[i].calc_chances(C, sizes, G, bet);
	
	//  cerr << "first player's cards:\n";
	//  show_deck(G.get_cards(0));
	//  cerr << "second player's cards:\n";
	//  show_deck(G.get_cards(1));
	
	bet = -1;
	while(true) // betting in progress
	{
		bot_move(player_turn, G, bet, bots, sizes, player_turn);
		if(bet == -1) return;
		player_turn++;
		if(G.players == player_turn) player_turn = 0;
	}
	
}
void bot_move(int id, Game & G, int & bet, vector <bot> & bots, vi & sizes, int & player_turn) 
{
	int cur = bots[id].choose_move(G, bet);
	//  code_to_name(cur);
	//  cerr << "\n";
	if(cur == -1) 
	{
		ll table = G.cards_on_table();
		//  show_deck(table);
		if(check(table, bet))
		{
			sizes[player_turn]++;
			//  cerr << "exists\n";
		}
		else
		{
			int n = G.players;
			sizes[(player_turn + n - 1) % n]++;
			player_turn = (player_turn + n - 1) % n;
			//  cerr << "doesn't exist\n";
		}
		bet = -1;
		return;
	}
	else
	{
		for(int i=0;i<G.players;i++) if(i != id) bots[i].react(cur);
	}
	bet = cur;
	
}


void compvhuman_oneturn(Game & G, int & bet, bot & me, const vector <vector <ll>> & C, vi & sizes, int & player_turn)
{
	//  G.give_cards(sizes);
	me.calc_chances(C, sizes, G, bet);
	
	// for debug, you know each other's cards
	
	cerr << "I have " << sizes[me.id] << " cards\n";
	
	//  cerr << "my cards\n";
	//  show_deck(G.get_cards(0));
	//  cerr << "your cards\n";
	//  show_deck(G.get_cards(1));
	
	//  me.calc_chances();
	//  code_to_name(me.choose_move());
	//  return;
	
	bet = -1; // resetting bet
	while(true)
	{
		if(player_turn == 0) // my turn
		{
			make_move(0, G, bet, me, sizes, player_turn);
		}
		else
		{
			let_make_move(0, G, bet, sizes, player_turn); // opp's turn
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


void let_make_move(bool checking, Game & G, int & bet, vi & sizes, int & player_turn)
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
void make_move(bool checking, Game & G, int & bet, bot & me, vi & sizes, int & player_turn)
{
	//  cerr << "all my bets (last is valid)\n";
	int cur = -1;
	//  for(int i=0;i<10;i++)
	//  {
		cur = me.choose_move(G, bet);
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

void one_turn(Game & G, int & bet, vi & sizes, bot & me, int & player_turn, const vector <vector <ll>> & C)
{
	G.give_cards(sizes);
	me.calc_chances(C, sizes, G, bet);
	
	// for debug, you know each other's cards
	
	cerr << "I have " << sizes[me.id] << " cards\n";
	
	//  cerr << "my cards\n";
	//  show_deck(G.get_cards(0));
	cerr << "your cards\n";
	show_deck(G.get_cards(1));
	
	me.calc_chances(C, sizes, G, bet);
	//  code_to_name(me.choose_move());
	//  return;
	
	bet = -1;
	while(true)
	{
		if(player_turn == 0) // comp's turn
		{
			make_move(1, G, bet, me, sizes, player_turn);
		}
		else
		{
			let_make_move(1, G, bet, sizes, player_turn); // opp's turn
			if(bet > -1) me.react(bet);
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
