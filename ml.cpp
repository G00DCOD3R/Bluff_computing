#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

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

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;
constexpr ll nax = 2e5+6969, INF = 2e9+6969;
constexpr ld eps = 0.09; // 0.5 - eps is percentage when strategy A still has potential to beat strategy B (has 0.5 - eps win ratio)
constexpr int winpr = 42; // this is % (as an int) such that A beats B if it has winpr / 100 win ratio


pair<ld,ld> test(int reps, Game & G, int & bet); // test strategy, preferably reps % 5 == 0

vi sizes; // sizes[i] signify how many cards have player i 
int player_turn = 0;

vector <bot> bots(30);
bot prev_best, best_best_best;
vector <vector <ll>> C(60, vector<ll>(60, 0)); // newton's binomial

int main()
{
	
	Game G;
	int bet = -1;
	G.players = 2;
	G.__init();
	sizes.resize(G.players);
	#warning this doesnt read number of players!!!
	
	for(int i=0;i<=52;i++) C[i][0] = 1;
	for(int i=1;i<=52;i++)
	{
		for(int j=1;j<=i;j++)
		{
			C[i][j] = C[i-1][j] + C[i-1][j-1];
		}
	}
	//  END OF PREPROCESSING
	
	ld highscore;
	ifstream myfile;
	myfile.open("./sample/highscore");
	myfile >> highscore;
	myfile.close();
	
	best_best_best.read_moves("./sample/best_strat");
	bots[0].read_moves("./sample/best_strat");
	//  bots[0].set_random();
	
	clock_t ST = clock();
	
	pair <ld,ld> S = test(3000, G, bet);
	ld score = S.fi;
	
	clock_t EN = clock();
	cerr << "took " << (ld)(EN - ST) / CLOCKS_PER_SEC << " seconds\n";
	cerr << "score: " << setprecision(4) << fixed << score << "\n";
	
	if(score + eps < highscore)
	{
		cerr << "too bad :(\n";
		best_best_best.print_moves();
		return 0;
	}
	
	for(int i=0;i<8;i++) for(int j=0;j<2;j++) for(int g=0;g<5;g++) prev_best.mvs[i][j][g] = bots[0].mvs[i][j][g];
	
	cerr << "real values:\n";
	
	ST = clock();
	S = test(10000, G, bet);
	score = S.fi;
	EN = clock();
	
	cerr << "took " << (ld)(EN - ST) / CLOCKS_PER_SEC << " seconds\n";
	cerr << "score: " << setprecision(4) << fixed << score << "\n";
	
	if(score + eps < highscore)
	{
		cerr << "too bad :(\n";
		best_best_best.print_moves();
		return 0;
	}
	
	ld sofar_score = score;
	ld toset_score = S.se;
	cerr << "improving:\n";
	
	for(int turns = 0; turns < 10; turns++)
	{
		for(int i=0;i<8;i++) for(int j=0;j<2;j++) for(int g=0;g<5;g++) bots[0].mvs[i][j][g] = prev_best.mvs[i][j][g];
		bots[0].perform_minor();
		
		ST = clock();
		S = test(3000, G, bet);
		score = S.fi;
		EN = clock();
		
		cerr << "took " << (ld)(EN - ST) / CLOCKS_PER_SEC << " seconds\n";
		cerr << "score: " << setprecision(4) << fixed << score << "\n";
		
		if(score + eps < sofar_score) continue;
		
		ST = clock();
		S = test(10000, G, bet);
		score = S.fi;
		EN = clock();
		
		cerr << "took " << (ld)(EN - ST) / CLOCKS_PER_SEC << " seconds\n";
		cerr << "score: " << setprecision(4) << fixed << score << "\n";
		
		if(score <= sofar_score) continue;
		
		toset_score = S.se;
		
		sofar_score = score;
		for(int i=0;i<8;i++) for(int j=0;j<2;j++) for(int g=0;g<5;g++) prev_best.mvs[i][j][g] = bots[0].mvs[i][j][g];
		
	}
	
	if(highscore < sofar_score)
	{
		cerr << "good :)\n";
		highscore = toset_score;
		ofstream hfile;
		hfile.open("./sample/highscore");
		hfile << highscore;
		hfile.close();
		prev_best.print_moves();
		//  prev_best.print_moves();
	}
	else
	{
		cerr << "next time :(\n";
		best_best_best.print_moves();
	}
	
	return 0;
}


pair<ld,ld> test(int reps, Game & G, int & bet)
{
	int cur_op = -1;
	int wins[5]; // wins against each of the startegies
	for(int i=0;i<5;i++) wins[i] = 0;
	vector <string> opponents {"s0", "s1", "s2", "s3", "best_strat"};
	
	int one_fifth = reps / 5;
	
	for(int sim = 0; sim < reps; sim++) 
	{
		if(sim % (reps / 5) == 0)
		{
			if(cur_op > -1)
			{
				if(100 * wins[cur_op] < one_fifth * winpr)
				{
					cerr << "lost to " << cur_op << ", (" << setprecision(3) << fixed << (ld) wins[cur_op] / one_fifth << ")\n";
					return mp(-1.0, -1.0); // it lost (< 45 % win ratio)
				}
			}
			cur_op++; // changes every reps / 5 iterations
			string opp_name = "./sample/" + opponents[cur_op];
			bots[1].read_moves(opp_name);
		}
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
		if(M == 0) wins[cur_op]++;
	}
	
	
	int special_wins = wins[4], normal_wins = 0;
	for(int i=0;i<4;i++) normal_wins += wins[i];

	if(100 * wins[4] < one_fifth * winpr)
	{
		cerr << "lost to " << 4 << ", (" << setprecision(3) << fixed << (ld) wins[4] / one_fifth << ")\n";
		return mp(-1.0, -1.0); // it lost (< 50 % win ratio)
	}
	
	cerr << "win %: " << setprecision(4) << fixed << (ld)normal_wins / (reps - one_fifth) << " " << (ld)special_wins / one_fifth << "\n";
	ld real_score = 0.7 * (ld)normal_wins / (reps - one_fifth) + 0.3 * special_wins / one_fifth; 
	ld score = 0.7 * (ld)normal_wins / (reps - one_fifth) + 0.3 * 0.5; 
	// 0.3 * 0.5, because same strat have to get similar results at the same test
	// thus winning probability is exactly 0.5
	// the first score is to compare between scores, second is to set (can be either lower or higher, than previous)
	cerr << "score: " << setprecision(4) << fixed << score << "\n";
	return mp(real_score, score);
}
