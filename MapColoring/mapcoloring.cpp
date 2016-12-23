#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<vector>
#include<queue>
#define dist(a,b) ((a)<(b)?(b)-(a):(a)-(b))
using namespace std;

int N, M, grp[25][25], pir[777];

namespace trivial {
	int col[25], ans[25], mcol;

	bool dfs(int ix){
		if(ix>=N){
			memcpy(ans, col, sizeof(ans));
			return 1;
		}
		int chk=0;
		for(int i=0; i<ix; i++) if(grp[ix][i]) chk |= 1<<col[i];
		for(int c=1; c<=mcol; c++){
			if(chk & (1<<c))continue;
			col[ix]=c;
			if(dfs(ix+1))return 1;
			col[ix]=0;
		}
		return 0;
	}

	void trace(){
		int c;
		for(c=1;; c++){
			memset(col, 0, sizeof(col));
			mcol = c;
			if(dfs(0))break;
		}
		printf("Minimum Color: %d, ", c);
		for(int i=0; i<N; i++)printf("%d ", ans[i]);
		puts("");
	}
};

namespace bitmask {
	int exp3;

	struct bm {
		unsigned bts[1200000];
		bm(){memset(bts,0,sizeof(bts));}
		void clear(){memset(bts,0,sizeof(bts));}
		void set(){memset(bts,15,sizeof(bts));}
		void cover(int ix){bts[ix/30] |= 1<<(ix%30);}

		bm operator* (const bm& c) const {
			bm res;
			for(int i=0; i<exp3; i++){
				int tmp=i, a=0, b=0;
				for(int j=0; j<N; j++){
					if(tmp%3 == 1)a |= 1<<j;
					if(tmp%3 == 2)b |= 1<<j;
					tmp /= 3;
				}
				if((bts[a/30] & (1<<(a%30))) && (c.bts[b/30] & (1<<(b%30))))res.cover(a|b);
			}
			return res;
		}
	}isok[5], tmp;

	void trace(){
		exp3 = 1;
		for(int i=N; i--;)exp3 *= 3;
		for(int i=0; i<(1<<N); i++){
			int j;
			for(j=0; j<M; j++){
				if((pir[j] & i) == pir[j])break;
			}
			if(j==M)isok[0].cover(i);
		}

		for(int i=1; i<5; i++)isok[i] = isok[i-1]*isok[i-1];
		tmp = isok[0];

		int c;
		for(c=1;; c++){
			int xx = (1<<N)-1;
			if(tmp.bts[xx/30] & (1<<(xx%30)))break;
			tmp = tmp*isok[0];
		}
		printf("Minimum Color: %d\n", c);
	}
};

int main(){
	FILE *in = fopen("input.txt","r");
	fscanf(in, "%d%d", &N, &M);
	for(int i=0; i<M; i++){
		int s, e;
		fscanf(in, "%d%d", &s, &e), s--, e--;
		grp[s][e] = grp[e][s] = 1;
		pir[i] = (1<<s) | (1<<e);
	}
	fclose(in);

	puts("Trivial Algorithm");
	trivial::trace();
	puts("");

	puts("Bitmask Algorithm");
	bitmask::trace();
	puts("");
	return 0;
}
