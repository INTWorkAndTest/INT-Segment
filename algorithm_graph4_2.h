#ifndef ALOGROITHM_GRAPH_4_2
#define ALOGROITHM_GRAPH_4_2
#include<bits/stdc++.h>
using namespace std; 
/*
	INT-low-cost算法 
*/ 
struct graph_alg_4_2{
	int n,m;
	vector<int> v[MAXN];
	vector<int> fa[MAXN];
	vector<pair<int,int>> road;
	int ans_tot;
	vector<int> ans[MAXN];
	vector<int> check;
	bool pd[MAXN];
	int tot[MAXN];
	int splite_len; 
	int findnext[MAXN];
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
/*	void euler(int now){
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
	}
*/
	stack<int> ss;
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
	int f[MAXN];
	bool more_pathwork(){
		for(int i=0;i<check.size();i++){
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
					if (cnm!=check[i] && tot[cnm]%2==1){
						break;
					}
				}
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
				ans_tot++;
				if (cnm==0) cnm=1,begin=lenx;
				splite_len=max(splite_len,lenx); 
				lenx=1;
			}else{
				lenx++;
			}
		}
		splite_len=max(splite_len,lenx+begin);
		ans_tot--;
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
		ans_tot=0;
		for (auto now:euler_ans){
			if (now==splite_dot){
				ans_tot++;
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
	}
};
#endif
