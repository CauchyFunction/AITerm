#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<algorithm>
#include<vector>
#define sq(a) ((long long)(a)*(a))
using namespace std;

int N, K, poi[121212][3], Q, qry[121212][3];

long long dist(int x1, int y1, int x2, int y2){
	long long dx=x1-x2, dy=y1-y2;
	return dx*dx+dy*dy;
}

namespace trivial {
	int pix[121212];

	void init(){
		for(int i=0; i<N; i++)pix[i]=i;
	}

	int cx, cy;
	bool comp(const int& x, const int& y){
		return dist(poi[x][0],poi[x][1],cx,cy) < dist(poi[y][0],poi[y][1],cx,cy);
	}

	int query(int x, int y){
		cx=x, cy=y;
		sort(pix, pix+N, comp);

		int vote=0;
		for(int i=0; i<K; i++){
			if(poi[pix[i]][2])vote++;
			else vote--;
		}
		return vote>0?1:0;
	}
};

namespace kdtree {
	struct kdt {
		int xl, xh, yl, yh;
		int pcn[2];
	}node[1700000];

	int pts[121212];

	bool cmp1(const int& x, const int& y) {return poi[x][0] < poi[y][0];}
	bool cmp2(const int& x, const int& y) {return poi[x][1] < poi[y][1];}

	void make_kdtree(int ix, int s, int e, bool typ){
		if(s >= e)return;

		node[ix].xl = node[ix].xh = poi[pts[s]][0];
		node[ix].yl = node[ix].yh = poi[pts[s]][1];
		for(int i=s; i<e; i++){
			int j = pts[i];
			node[ix].xl = min(node[ix].xl, poi[j][0]);
			node[ix].xh = max(node[ix].xh, poi[j][0]);
			node[ix].yl = min(node[ix].yl, poi[j][1]);
			node[ix].yh = max(node[ix].yh, poi[j][1]);
			node[ix].pcn[poi[j][2]]++;
		}
		if(node[ix].xl == node[ix].xh && node[ix].yl == node[ix].yh)return;

		bool istall = (node[ix].xh-node[ix].xl < node[ix].yh-node[ix].yl);
		if(typ)sort(pts+s, pts+e, cmp1);
		else sort(pts+s, pts+e, cmp2);

		make_kdtree(2*ix, s, (s+e)/2, !typ);
		make_kdtree(2*ix+1, (s+e)/2, e, !typ);
	}

	int getrectype(int r, int xl, int xr, int yl, int yr){
		long long r2 = sq(r), midist;
		if(max(sq(xl),sq(xr)) + max(sq(yl),sq(yr)) <= r2)return -1; // inside
		long long xmi = min(sq(xl),sq(xr)), ymi = min(sq(yl),sq(yr));
		midist = xmi + ymi;

		long long xlr = (long long)xl*xr, ylr = (long long)yl*yr;
		if(xlr <= 0 && ylr <= 0)return 0;
		if(xlr <= 0)midist = ymi;
		if(ylr <= 0)midist = xmi;
		if(midist > r2)return 1; // outside
		return 0; // other
	}

	int nodes[2];
	void getcirc(int ix, int x, int y, int r){
		if(node[ix].pcn[0] + node[ix].pcn[1] == 0)return;
//		printf("%d, %d, (%d~%d, %d~%d)\n", ix, node[ix].pcn[0]+node[ix].pcn[1], node[ix].xl, node[ix].xh, node[ix].yl, node[ix].yh);

		int typ = getrectype(r, node[ix].xl-x, node[ix].xh-x, node[ix].yl-y, node[ix].yh-y);
		if(typ < 0){
			nodes[0] += node[ix].pcn[0];
			nodes[1] += node[ix].pcn[1];
		}
		if(typ > 0)return;
		getcirc(ix*2, x, y, r); getcirc(ix*2+1, x, y, r);
	}

	void init(){
		for(int i=0; i<N; i++)pts[i]=i;
		make_kdtree(1,0,N,0);
	}

	int query(int x, int y){
		int cha=0;
		int mi=0, mx=150000, md;
//		printf("%d %d\n", x, y);
		while(mi<=mx){
			md = (mi+mx)/2;
			nodes[0]=nodes[1]=0;
			getcirc(1, x, y, md);
			if(nodes[0]+nodes[1] >= K)cha = nodes[0]-nodes[1], mx = md-1;
			else mi = md+1;
		}
		return cha<0?1:0;
	}
};

namespace lsensitive {
	vector<int> pts[101010];

	int hash(int x, int y){
		if(x==y)return x;
		if(x>y)swap(x,y);
		int m=(x+y)/2;
		if(rand()%2)return hash(x,m);
		return hash(m+1,y);
	}

	void init(){
		for(int i=0; i<N; i++){
			int h=hash(poi[i][0], poi[i][1]);
			pts[h].push_back(i);
		}
	}

	int query(int x, int y){
		int vote=0, cnt=K, h=hash(x,y);
		for(int i=0;; i++){
			if(h+i <= 100000){
				for(unsigned j=pts[h+i].size(); j--;){
					int ix=pts[h+i][j];
					if(poi[ix][2])vote++;
					else vote--;
					if(--cnt == 0)break;
				}
			}
			if(!cnt)break;

			if(h-i >= 0 && i>0){
				for(unsigned j=pts[h-i].size(); j--;){
					int ix=pts[h-i][j];
					if(poi[ix][2])vote++;
					else vote--;
					if(--cnt == 0)break;
				}
			}
			if(!cnt)break;
		}
		return vote>0?1:0;
	}
};

#include<time.h>
#define RAND() (rand()%32768)

int rnd(){
	return RAND()*32768+RAND();
}

int gettype(int x, int y, int typ){
	if(typ == 1)return 2*(y-50000) >= (x-50000) ? 1 : 0;
	if(typ == 2)return sq(x-20000) + sq(y-65432)*2 <= sq(12345) ? 1 : 0;
	if(typ == 3)return (long long)(x-2*y)*(2*x-y)*(100000-x-y) >= 0 ? 1 : 0;
	return 0;
}

void gen(int typ){
	printf("Input N, K: ");
	if(scanf("%d%d", &N, &K) != 2)exit(1);
	if(N!=100 && N!=1000 && N!=10000 && N!=100000)exit(1);
	for(int i=0; i<N; i++){
		poi[i][0] = rnd()%100001;
		poi[i][1] = rnd()%100001;
		poi[i][2] = gettype(poi[i][0], poi[i][1], typ);
	}
}

int main(){
	srand(1);
	int a=-1, st, cor;
	printf("Data Input Type : 0(file), 123(random) - ");
	scanf("%d", &a);
	if(a==0){
		FILE *in = fopen("input.txt","r");
		fscanf(in, "%d%d", &N, &K);
		for(int i=0; i<N; i++)fscanf(in, "%d%d%d", &poi[i][0], &poi[i][1], &poi[i][2]);
		fscanf(in, "%d", &Q);
		for(int i=0; i<Q; i++)fscanf(in, "%d%d", &qry[i][0], &qry[i][1]);
		fclose(in);
		if(N<K)return 1;
	}
	else if(a>=1 && a<=3)gen(a);
	else return 1;

	puts("Trivial Algorithm");
	st = clock();
	if(a>0)Q=cor=0;
	trivial::init();
	for(int i=0;; i++){
		if(a==0){
			if(i>=Q)break;
			int x=qry[i][0], y=qry[i][1];
			printf("(%d %d): %d\n", x, y, trivial::query(x, y));
		}
		else{
			int x=rnd()%100001, y=rnd()%100001;
			int typ=trivial::query(x,y);
			if(clock()-st > 2000000)break;
			Q++;
			if(typ == gettype(x,y,a))cor++;
		}
	}
	if(a>0)printf("Queries = %d, Success = %lf\n", Q, (double)cor/Q);
	puts("");

	puts("K-d Tree Algorithm");
	st = clock();
	if(a>0)Q=cor=0;
	kdtree::init();
	for(int i=0;; i++){
		if(a==0){
			if(i>=Q)break;
			int x=qry[i][0], y=qry[i][1];
			printf("(%d %d): %d\n", x, y, kdtree::query(x, y));
		}
		else{
			int x=rnd()%100001, y=rnd()%100001;
			int typ=kdtree::query(x,y);
			if(clock()-st > 2000000)break;
			Q++;
			if(typ == gettype(x,y,a))cor++;
		}
	}
	if(a>0)printf("Queries = %d, Success = %lf\n", Q, (double)cor/Q);
	puts("");

	puts("Local-Sensitive Hashing Algorithm");
	st = clock();
	if(a>0)Q=cor=0;
	lsensitive::init();
	for(int i=0;; i++){
		if(a==0){
			if(i>=Q)break;
			int x=qry[i][0], y=qry[i][1];
			printf("(%d %d): %d\n", x, y, lsensitive::query(x, y));
		}
		else{
			int x=rnd()%100001, y=rnd()%100001;
			int typ=lsensitive::query(x,y);
			if(clock()-st > 2000000)break;
			Q++;
			if(typ == gettype(x,y,a))cor++;
		}
	}
	if(a>0)printf("Queries = %d, Success = %lf\n", Q, (double)cor/Q);
	puts("");
	return 0;
}
