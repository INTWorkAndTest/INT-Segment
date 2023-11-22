#ifndef ALOGROITHM_GRAPH_5
#define ALOGROITHM_GRAPH_5
#include<bits/stdc++.h>
using namespace std; 
typedef long long ll;
/*
	INT-optimize算法 
*/
namespace Work{
	const ll OO=1LL<<40;
	const int MAXNN=1005;
	ll w[MAXNN][MAXNN], g[MAXNN][MAXNN];
	int match[MAXNN], path[MAXNN], p[MAXNN], len;
	ll d[MAXNN];
	bool v[MAXNN];
	int n, m, k;
	bool dfs(int i) {
    	path[len++] = i;
    	if (v[i]) return true;
    	v[i] = true;
    	for (int j = 0; j < k; ++j) {
      		if (i != j && match[i] != j && !v[j]) {
    			int kok = match[j];
    			if (d[kok] < d[i] + w[i][j] - w[j][kok]) {
      				d[kok] = d[i] + w[i][j] - w[j][kok];
      				if (dfs(kok)) return true;
    			}
      		}
    	}
    	--len;
    	v[i] = false;
    	return false;
  	}
	ll solve() {
    	if (k&1) 
      	return -1;
    	for (int i = 0; i < k; ++i) p[i] = i, match[i] = i ^ 1;
    	int cnt = 0;
    	for (;;) {
      		len = 0;
      		bool flag = false;
      		memset(d, 0, sizeof(d));
      		memset(v, 0, sizeof(v));
      		for (int i = 0; i < k; ++i) {
    			if (dfs(p[i])) {
	      			flag = true;
	      			int t = match[path[len - 1]], j = len - 2;
	      			while (path[j] != path[len - 1]) {
	        			match[t] = path[j];
	        			swap(t, match[path[j]]);
	        			--j;
	      			}
	      			match[t] = path[j];
	      			match[path[j]] = t;
	      			break;
    			}
      		}
      		if (!flag) {
    			if (++cnt >= 3) break;
    			random_shuffle(p, p+k);
      		}
    	}
    	ll ans = 0;
    	for (int i = 0; i < k; ++i) {
      		ll t = w[i][match[i]];
      		if (t == 0) {
    			return -1;
      		}
      		ans += OO - t;
    	}
    	return ans/2;
  	}
}
struct graph_alg_5{
	int n,m;
	vector<int> v[MAXN];
	vector<int> fa[MAXN];
	vector<pair<int,int>> road;
	int ans_tot;
	vector<int> ans[MAXN];
	vector<int> check;
	int splite_len;
	bool pd[MAXN];
	int tot[MAXN];
	void init(int nn,int mm,vector<pair<int,int>> vv){
		n=nn;
		m=mm;
		ans_tot=0;
		for (int i=0;i<n+10;i++){
			v[i].clear();
			fa[i].clear();
			tot[i]=0;
			pd[i]=0;
		}
		road.clear();
		for (auto now:vv){
			road.push_back(now);
			v[now.first].push_back(now.second);
			v[now.second].push_back(now.first);
			fa[now.first].push_back(road.size()-1);
			fa[now.second].push_back(road.size()-1);
			tot[now.first]++;
			tot[now.second]++; 
		}
		for (int i=1;i<=n;i++)
			if (tot[i]%2==1) check.push_back(i);
		for (int i=1;i<=n;i++)
			if (tot[i]%2==0) check.push_back(i);
	}
	vector<int> euler_ans;
	int tot_euler;
	/*void euler(int now){
		for (int i=0;i<v[now].size();i++){
			if (!pd[fa[now][i]]){
				pd[fa[now][i]]=1;
				tot_euler++;
				tot[now]--;
				tot[v[now][i]]--;
				euler(v[now][i]);
			}
		}
		euler_ans.push_back(now);
	}*/
	stack<int> ss;
	int findnext[MAXN];
	void euler(int now){
		while (!ss.empty()) ss.pop();
		ss.push(now);
		while (!ss.empty()){
			int dot=ss.top();
			while (findnext[dot]<v[dot].size() && pd[fa[dot][findnext[dot]]]==1) findnext[dot]++;
			if (findnext[dot]<v[dot].size()){
				ss.push(v[dot][findnext[dot]]);
				tot_euler++;
				pd[fa[dot][findnext[dot]]]=1;
			}else{
				ss.pop();
				euler_ans.push_back(dot);
			}
		}
	}  
	int w[1005][1005];
	int f[MAXN];
	bool more_pathwork(){
		for (int i=1;i<=n;i++)
			for (int j=1;j<=n;j++)
				w[i][j]=1e6;
		for (int i=1;i<=n;i++)
		 	for (int j=0;j<v[i].size();j++){
		 		w[i][v[i][j]]=w[v[i][j]][i]=1;
			 }
		for (int k=1;k<=n;k++)
    		for (int i=1;i<=n;i++)
      			for (int j=1;j<=n;j++)
    				w[i][j]=min(w[i][j],w[i][k]+w[k][j]);
    	int now=0;
    	while (now<check.size() && tot[check[now]]%2==1)
    		now++;
    	now--;
    	Work::k=now+1;
    	for (int i=0;i<=now;i++){
    		for (int j=0;j<=now ;j++){
      			Work::w[i][j]=(Work::OO)-w[check[i]][check[j]];
		  	}
      	}
      	ll anscnm=Work::solve();
	/*	for (int i=0;i<=now;i++)
			cout<<i<<' '<<Work::match[i]<<endl; */		
		for (int i=0;i<=now;i++){
			if (tot[check[i]]%2==1){
				for (int i=1;i<=n;i++)
					f[i]=-1;
				int cnm=check[i];
				f[check[i]]=check[i];
				queue<int> q;
				q.push(check[i]);
				while (!q.empty()){
					cnm=q.front();
					q.pop();
					for (auto next:v[cnm]){
						if (f[next]==-1){
							q.push(next);
							f[next]=cnm;
						}
					}
				}
				cnm=check[Work::match[i]];
				while (cnm!=f[cnm]){
					m++;
					pair<int,int> now=make_pair(cnm,f[cnm]);
					road.push_back(now);
					v[now.first].push_back(now.second);
					v[now.second].push_back(now.first);
					fa[now.first].push_back(road.size()-1);
					fa[now.second].push_back(road.size()-1);
					tot[now.first]++;
					tot[now.second]++; 
					cnm=f[cnm];
				}
			}
		}
		euler(check[0]);
		if (tot_euler!=m) return 0;
		ans_tot++;
		ans[ans_tot].clear();
		for (auto now:euler_ans)
			ans[ans_tot].push_back(now);
		splite_len=0;
		int splite_dot=0;
		for (int i=1;i<=n;i++)
			if (v[i].size()>v[splite_dot].size()) splite_dot=i;
		int lenx=0,begin;
		bool cnm=0;
		for (auto now:euler_ans){
			if (now==splite_dot){
				if (cnm==0) cnm=1,begin=lenx;
				splite_len=max(splite_len,lenx); 
				lenx=1;
			}else{
				lenx++;
			}
		}
		splite_len=max(splite_len,lenx+begin);
		return 1;
	}
	bool work(){
		tot_euler=0;
		ans_tot=0;
		if (tot[check[0]]%2==0){
			euler(check[0]);
			if (tot_euler!=m) return 0;
			ans_tot++;
			ans[ans_tot].clear();
			for (auto now:euler_ans)
				ans[ans_tot].push_back(now);
			splite_len=0;
			splite_len=0;
		
		int splite_dot=0;
		for (int i=1;i<=n;i++)
			if (v[i].size()>v[splite_dot].size()) splite_dot=i;
		int lenx=0,begin;
		bool cnm=0;
		for (auto now:euler_ans){
			if (now==splite_dot){
				if (cnm==0) cnm=1,begin=lenx;
				splite_len=max(splite_len,lenx); 
				lenx=1;
			}else{
				lenx++;
			}
		}
		splite_len=max(splite_len,lenx+begin);
			return 1;
		}else{
			if (check.size()>2 && tot[check[2]]%2==0){
				euler(check[0]);
				if (tot_euler!=m) return 0;
				ans_tot++;
				ans[ans_tot].clear();
				for (auto now:euler_ans)
					ans[ans_tot].push_back(now);
				return 1;
			}else{
				return more_pathwork(); 
			}
		}
		return 0;
	}
	void output(){
		cout<<"总共需要生成"<<ans_tot<<"条路径"<<endl; 
		cout<<"总路径长度为"<<tot_euler<<endl; 
		int maxn=-1;
		double ss=0,s=0;
		for (int i=1;i<=ans_tot;i++){
			int now=ans[i].size();
			maxn=max(maxn,now-1);
			s=s+ans[i].size()-1;
		}
		s=s/ans_tot;
		for (int i=1;i<=ans_tot;i++)
			ss=ss+(s-ans[i].size()+1)*(s-ans[i].size()+1);
		ss=ss/ans_tot;
		cout<<"最长路径长度"<<maxn<<endl; 
		printf("路径平均长度%.4lf\n",s);
		printf("路径长度方差%.4lf\n",ss);
		printf("拆分后长度%d\n",splite_len);
		/*for (int i=1;i<=ans_tot;i++){
			cout<<"第"<<i<<"条:";
			for (int j=0;j<ans[i].size();j++) 
				printf("%d%c",ans[i][j],j!=ans[i].size()-1?'-':'\n');
		}*/ 
	}
};
#endif
