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

void __init();
void show_card(PII & a);
void show_deck(const vector <PII> & deck);
bool check(ll cards, int hand);
ll conv(const vector <PII> & v);
void show_int(ll x);

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
ll conv(const vector <PII> & v)
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
		if(ppc(mask) >= 2) return true;
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
		if(ppc(mask) >= 3) return true;
		return false;
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
		if(ppc(mask) >= 4) return true;
		return false;
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
