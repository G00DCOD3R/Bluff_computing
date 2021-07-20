#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}
#define ppc(...) __builtin_popcountll(__VA_ARGS__)

#include <bits/stdc++.h>
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

int random(int a, int b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_int_distribution<int>(a,b)(rng);
}

vector <PII> deck;

void __init(); // initialize deck of cards
void show_card(PII & a); // show single card
void show_deck(const vector <PII> & deck); // shows deck of cards
bool check(ll cards, int hand); // checks if hand exist in cards (cards is mask, hand is code from 0 to 381
ll cards_to_mask(const vector <PII> & v); // converts cards to mask
void show_int(ll x); // shows long long x in binary form
int name_to_code(); // converts name of hand to its code
void code_to_name(int code); // converts code of hand to its name (writes it)



int main()
{
	__init();
	
	
	//  while(true)
	//  {
		//  ll x;
		//  random_shuffle(all(deck));
		//  cin >> x;
		//  vector <PII> tmp;
		//  tmp.resize(x);
		//  for(int i=0;i<x;i++) tmp[i] = deck[i];
		//  show_deck(tmp);
		//  ll cards = conv(tmp);
		//  int hand = 0;
		//  while(hand != -1)
		//  {
			//  cin >> hand;
			//  cout << check(cards, hand) << "\n";
		//  }
		
		//  cout << "\n";
	//  }
	
	
	
	
	return 0;
}


void __init()
{
	for(int i=2;i<=14;i++)
	{
		for(int j=0;j<4;j++) deck.pb(mp(i, j));
	}
}

void show_card(PII & a)
{
	// showing cards 2 - 14 (higher than 10 are J, Q, K, A. 
	// colors are: C, D, H, S (0 - 3)
	
	assert(a.fi >= 2 && a.fi <= 14 && a.se >= 0 && a.se < 4);
	if(a.fi <= 10) printf("%d", a.fi);
	else 
	{
		vector <char> opts {'J', 'Q', 'K', 'A'};
		printf("%c", opts[a.fi - 11]);
	}
	vector <char> color {'C', 'D', 'H', 'S'};
	printf("%c ", color[a.se]);
}

void show_deck(const vector <PII> & deck)
{
	for(auto e: deck) show_card(e);
	puts("");
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
		if(x & (1LL << i)) printf("1 ");
		else printf("0 ");
	}
	puts("");
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
	puts("CHECKING NON-EXISTING HAND");
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
			puts("No such card!");
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
				puts("No such card (second)!");
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
				puts("No such strit");
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
				puts("No such card (second)!");
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
				puts("No such poker");
				continue;
			}
			return 373 + who;
		}
		puts("no such poker hand\ntry for example:");
		printf("jedna X\npara X\ndwie X Y\ntrzy X\nstrit X\nfull X Y\nkareta X\npoker X\n\nWhere X Y signify cards\n");
	}
	
}
void code_to_name(int code)
{
	vector <string> opts {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	if(code < 0) 
	{
		cout << "no such code (" << code << ")\n";
		return;
	}
	if(code < 13)
	{
		// highest card;
		cout << "jedna " << opts[code] << "\n";
		
	}
	else if(code < 26)
	{
		// pair
		cout << "para " << opts[code - 13] << "\n";
	}
	else if(code < 182)
	{
		// double pair
		code -= 26;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		cout << "dwie " << opts[a] << " " << opts[b] << "\n";
	}
	else if(code < 195)
	{
		// three of a kind
		code -= 182;
		cout << "trzy " << opts[code] << "\n";
	}
	else if(code < 204)
	{
		// straight
		code -= 195;
		cout << "strit " << opts[code] << "\n";
	}
	else if(code < 360)
	{
		// full house
		code -= 204;
		int a = code / 12;
		int b = code - a * 12;
		if(b >= a) b++;
		cout << "full " << opts[a] << " " << opts[b] << "\n";
		
	}
	else if(code < 373)
	{
		// four of a kind
		code -= 360;
		cout << "kareta " << opts[code] << "\n";
	}
	else if(code < 382)
	{
		// straight flush
		code -= 373;
		cout << "pokerzysko " << opts[code] << "\n";
	}
}
