#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<vector>
#include<queue>
#define dist(a,b) ((a)<(b)?(b)-(a):(a)-(b))
using namespace std;

const int HASH_MAX = 1<<16, HM1 = (1<<16)-1;
const int dx[4]={1,-1,0,0}, dy[4]={0,0,1,-1};

struct state {
	int arr[3][3];
	state(){}
	state(int arr_[3][3]){ memcpy(arr, arr_, sizeof(arr)); }
	state(int val){
		for(int i=0; i<3; i++) for(int j=0; j<3; j++) arr[i][j]=val&7, val>>=3;
	}
	int value(){
		int c=0, m=1;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++)c += m*arr[i][j], m <<= 3;
		}
		return c;
	}
	void print(){
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++)printf("%d ", arr[i][j]);
			puts("");
		}
	}
	void getnext(vector<int>& v){
		v.clear();
		for(int i=0; i<3; i++) for(int j=0; j<3; j++){
			if(arr[i][j])continue;
			for(int k=0; k<4; k++){
				if(i+dx[k]<0 || i+dx[k]>=3)continue;
				if(j+dy[k]<0 || j+dy[k]>=3)continue;
				if(arr[i+dx[k]][j+dy[k]] == 0)continue;
				state tmp = *this;
				swap(tmp.arr[i][j], tmp.arr[i+dx[k]][j+dy[k]]);
				v.push_back(tmp.value());
			}
		}
	}
};

struct marker {
	vector<pair<int,int> > chk[HASH_MAX];
	bool cache[HASH_MAX>>6];
	marker(){ memset(cache, 0, sizeof(cache)); }

	void mark(int ix, int value, bool add=false){
		if(!add && cache[ix&(HM1>>6)]){
			for(unsigned i=chk[ix&HM1].size(); i--;) if(chk[ix&HM1][i].first == ix){
				chk[ix&HM1][i].second = value;
				return;
			}
		}
		chk[ix&HM1].push_back(make_pair(ix, value)), cache[ix&(HM1>>6)]=1;
	}
	int getval(const int ix){
		if(!cache[ix&(HM1>>6)])return 0;
		for(unsigned i=chk[ix&HM1].size(); i--;) if(chk[ix&HM1][i].first == ix) return chk[ix&HM1][i].second;
		return 0;
	}
	void clear(){
		for(unsigned i=HASH_MAX>>6; i--;){
			if(!cache[i])continue;
			for(unsigned j=0; j<64; j++)chk[(i<<6)|j].clear();
		}
		memset(cache, 0, sizeof(cache));
	}
};

namespace astar {
	marker chk, pv; int eix;

	struct stt {
		int ix, val, fur;
		stt(){}
		stt(int ix_, int val_=0) : ix(ix_), val(val_) {
			fur = 0;
			state s1(ix), s2(eix);
			for(int x=1; x<=8; x++){
				int i, j;
				for(i=0; i<9; i++)if(s1.arr[i/3][i%3]==x)break;
				for(j=0; j<9; j++)if(s2.arr[j/3][j%3]==x)break;
				fur += dist(i/3,j/3) + dist(i%3,j%3);
			}
		}
		bool operator< (const stt& c) const {return val+fur > c.val+c.fur;} // priority_queue is max_heap
	};
	priority_queue<stt> pq;

	void backtrace(int ix){
		int prv = pv.getval(ix);
		if(prv != -1)backtrace(prv);
		state st(ix);
		st.print(), puts("");
	}

	void trace(int st, int en, int& counter){
		while(!pq.empty())pq.pop();
		chk.clear(); pv.clear();

		eix = en;
		pq.push(stt(st,1));
		chk.mark(st,1), pv.mark(st,-1);

		while(1){
			stt cur;
			while(!pq.empty()){
				cur=pq.top();
				if(chk.getval(cur.ix) == cur.val)break;
				pq.pop();
			}
			if(pq.empty() || cur.ix == en)break;
			pq.pop();

			state my(cur.ix);
			vector<int> nxt; my.getnext(nxt);
			for(unsigned i=nxt.size(); i--;){
				int jx=nxt[i], jv=chk.getval(jx);
				if(jv && jv <= cur.val+1)continue;
				chk.mark(jx, cur.val+1); pv.mark(jx, cur.ix);
				pq.push(stt(jx, cur.val+1));
			}
		}

		printf("# of steps = %d\n", chk.getval(en)-1);
		backtrace(en);
	}
};

namespace IDS {
	int eix, cnt;
	vector<state> hist;
	marker chk;

	bool dfs_dep(int ix, int step){
		cnt++;
		state st(ix);
		if(ix == eix){
			hist.push_back(st);
			return 1;
		}
		if(!step)return 0;
		chk.mark(ix, step, 1);

		vector<int> nxt;
		st.getnext(nxt);
		for(unsigned i=nxt.size(); i--;){
			int jx=nxt[i];
			if(chk.getval(jx) >= step-1)continue;
			if(dfs_dep(jx, step-1)){
				hist.push_back(st);
				return 1;
			}
		}
		return 0;
	}

	void trace(int st, int en, int& counter){
		eix=en, cnt=0;
		hist.clear();

		for(int i=0; !dfs_dep(st, i); i++)chk.clear();

		printf("# of steps = %d\n", hist.size()-1);
		for(unsigned i=hist.size(); i--;)hist[i].print(), puts("");
		counter = cnt;
	}
};

int main(){
	int start[3][3], finish[3][3];
	FILE *in = fopen("input.txt","r");
	for(int i=0; i<3; i++) for(int j=0; j<3; j++) fscanf(in, "%d", &start[i][j]);
	for(int i=0; i<3; i++) for(int j=0; j<3; j++) fscanf(in, "%d", &finish[i][j]);
	fclose(in);

	state st(start), en(finish);
	int cnt1=0, cnt2=0;

	puts("A* Algorithm");
	astar::trace(st.value(), en.value(), cnt1);
	puts("");

	puts("Iterative Deepening Search");
	IDS::trace(st.value(), en.value(), cnt2);
	puts("");
	return 0;
}
