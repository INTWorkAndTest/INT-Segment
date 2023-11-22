#include<bits/stdc++.h>
#include <Windows.h>
#include<time.h>
#pragma comment(linker, "/STACK:10024000000,10024000000")
using namespace std;
#define MAXN 1000000
int Min_MTU=2147483647;
long long ave_Time=0;
int min_Time=0;
struct Link_record{
	int x,y,c,t;
};
bool cmp(Link_record a,Link_record b){
	return a.c<b.c || (a.c==b.c && a.t<b.t);
}
struct init_input{
	int n,m;
	vector<pair<pair<int,int>,pair<int,int>>> v;
	Link_record test[MAXN];
	void init(){
		freopen("input01.in","r",stdin);
		scanf("%d%d",&n,&m);
		for (int i=1;i<=m;i++){
			int x,y,z,t;
			scanf("%d%d%d%d",&x,&y,&z,&t);
			test[i].x=x;
			test[i].y=y;
			test[i].c=z;
			test[i].t=t;
			Min_MTU=min(Min_MTU,z);
			min_Time=max(min_Time,t);
			ave_Time+=t;
		} 
		ave_Time=ave_Time/m;
		sort(test+1,test+m+1,cmp);
		for (int i=1;i<=m;i++){
			v.push_back(make_pair(make_pair(test[i].x,test[i].y),make_pair(test[i].c,test[i].t)));
		} 
	}
} Init_Graph;

struct G_euler{
	int n,m;
	vector<int> v[MAXN];
	vector<int> vM[MAXN];
	vector<int> vT[MAXN];
	vector<int> fa[MAXN];
	vector<pair<int,int>> road;
	int ans_tot;
	vector<int> ans[MAXN];
	int ansTime[MAXN];
	vector<int> check;
	bool pd[MAXN];
	int tot[MAXN];
	int BFS_dis[MAXN];
	int F[MAXN];
	int F1[MAXN]; 
	int record[MAXN];
	
	int splite_len; 
	int findnext[MAXN];
	int f[MAXN];
	int f_t[MAXN];//Add after 
	int f_M[MAXN];//Add after 
	int maxT,maxL,SumT;
	
	void init(int nn,int mm,vector<pair<pair<int,int>,pair<int,int>>> vv){
		n=nn;
		m=mm;
		for (int i=0;i<=ans_tot;i++){
			ans[i].clear();
		}
		splite_len=0;
		for (int i=0;i<n+10;i++){
			v[i].clear();
			vM[i].clear();
			vT[i].clear();
			fa[i].clear();
			tot[i]=0;  
			pd[i]=0;
		}
		for (int i=0;i<=2*m;i++){
			f[i]=f_t[i]=f_M[i]=ansTime[i]=BFS_dis[i]=findnext[i]=record[i]=F1[i]=pd[i]=tot[i]=0;
		}
		road.clear();
		for (auto now:vv){
			road.push_back(now.first);
			v[now.first.first].push_back(now.first.second);
			v[now.first.second].push_back(now.first.first);
			
			vM[now.first.first].push_back(now.second.first);//Add after 
			vM[now.first.second].push_back(now.second.first);
			vT[now.first.first].push_back(now.second.second);
			vT[now.first.second].push_back(now.second.second);//Add after 
			
			fa[now.first.first].push_back(road.size()-1);
			fa[now.first.second].push_back(road.size()-1);
			tot[now.first.first]++;
			tot[now.first.second]++; 
		}
		check.clear();
		for (int i=1;i<=n;i++)
			if (tot[i]%2==1) check.push_back(i);
		for (int i=1;i<=n;i++)
			if (tot[i]%2==0) check.push_back(i);
	}
	vector<int> euler_ans;
	vector<int> euler_ans_MTU;//after
	vector<int> euler_ans_Time;//after 
	
	int tot_euler;
	
	stack<int> ss;
	stack<pair<int,int>> ssMT;//after
	
	void euler(int now){
		
		while (!ss.empty()) ss.pop();
		while (!ssMT.empty()) ssMT.pop();//after
		
		ss.push(now);
		ssMT.push(make_pair(0,0));//after
		
		while (!ss.empty()){
			int dot=ss.top();
			pair<int,int> test=ssMT.top();
			while (findnext[dot]<v[dot].size() && pd[fa[dot][findnext[dot]]]==1) findnext[dot]++;
			if (findnext[dot]<v[dot].size()){
				ss.push(v[dot][findnext[dot]]);
				ssMT.push(make_pair(vM[dot][findnext[dot]],vT[dot][findnext[dot]]));//after
				tot_euler++;
				pd[fa[dot][findnext[dot]]]=1;
			}else{
				ss.pop();
				ssMT.pop();//after
				euler_ans.push_back(dot);
				euler_ans_MTU.push_back(test.first);//after
				euler_ans_Time.push_back(test.second);//after
			}
		}
	} 
	bool more_pathwork(int Maxtimetest){
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
					for (int i=0;i<v[cnm].size();i++){
						int next=v[cnm][i];
						if (f[next]==-1){    
							q.push(next);
							f[next]=cnm;
							f_t[next]=vT[cnm][i];//Add after
							f_M[next]=vM[cnm][i];//Add after
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
					
					vT[now.first].push_back(f_t[cnm]);//Add after
					vT[now.second].push_back(f_t[cnm]);
					vM[now.first].push_back(f_M[cnm]);
					vM[now.second].push_back(f_M[cnm]);//Add after
					
					fa[now.first].push_back(road.size()-1);
					fa[now.second].push_back(road.size()-1);
					tot[now.first]++;
					tot[now.second]++; 
					cnm=f[cnm];
				}
			}
		} 
		euler_ans.clear();
		euler(check[0]);
		int main_dot=check[0];
		for (int i=1;i<=n;i++){
			if (v[i].size()>v[main_dot].size()) main_dot=i;
			BFS_dis[i]=2147483647;
		}
		BFS_dis[main_dot]=0;
		queue<int> q;
		q.push(main_dot);
		while (!q.empty()){
			int now=q.front();
			q.pop();
			for (int i=0;i<v[now].size();i++){
				int next=v[now][i];
				if (BFS_dis[next]>BFS_dis[now]+vT[now][i]){
					BFS_dis[next]=BFS_dis[now]+vT[now][i];
					q.push(next);
				}
			}
		}
		int first_dot=0;
		for (int i=0;i<euler_ans.size();i++)
			if (euler_ans[i]==main_dot){
				first_dot=i;
				break;
			}
		vector<int> ansf;
		vector<int> ansT;
		vector<int> ansM;
		vector<int> sumT;
		ansf.push_back(0);
		ansT.push_back(0);
		ansM.push_back(0);
		for (int i=first_dot;i<euler_ans.size()-1;i++){
			ansf.push_back(euler_ans[i]);
			ansT.push_back(euler_ans_Time[i]);
			ansM.push_back(euler_ans_MTU[i]);
		}
		for (int i=0;i<=first_dot;i++){
			ansf.push_back(euler_ans[i]);
			ansT.push_back(euler_ans_Time[i]);
			ansM.push_back(euler_ans_MTU[i]);
		}
		int Sum=0;
		for (int i=0;i<ansT.size();i++){
			Sum+=ansT[i];
			sumT.push_back(Sum);
		}
		int MAXTime=Maxtimetest;
		
		F[1]=0;
		record[1]=-1;
		for (int i=2;i<ansf.size();i++){
			F[i]=2147483647;
			record[i]=0;
			int MAX_MTU=min(i-1,ansM[i-1]);
			for (int j=1;j<=MAX_MTU && i-j>=1;j++){
				MAX_MTU=min(MAX_MTU,ansM[i-j-1]);
				if (sumT[i-1]-sumT[i-j-1]>=MAXTime) break;
				if (F[i] >= F[i-j] + BFS_dis[ansf[i]] + sumT[i-1]-sumT[i-j-1]){
					F[i] = F[i-j] + BFS_dis[ansf[i]] + sumT[i-1]-sumT[i-j-1];
					record[i] = i - j;
				}
			}
		}
		int next=ansf.size()-1;
		ans_tot=0;
		while (record[next]>=1){
			ans[ans_tot].clear();
			ansTime[ans_tot]=0;
			for (int i=next;i>=record[next];i--){
				ans[ans_tot].push_back(ansf[i]);
				if (i!=next) ansTime[ans_tot]+=ansT[i];
			}
			ans_tot++;
			next=record[next];
		}
		
		return 1;
	}
	bool work(int Maxtimetest){
		tot_euler=0;
		ans_tot=0;
		return more_pathwork(Maxtimetest); 
	}
	void output(){
		cout<<"总共需要生成"<<ans_tot<<"条路径"<<endl; 
		cout<<"总路径长度为"<<euler_ans.size()-1<<endl; 
		for (int i=0;i<ans_tot;i++){
			for (auto now:ans[i]){
				printf("%d ",now);
			}
			printf("Time:%d ms\n",ansTime[i]);
		}
		cout<<endl;
	}
	void output1(){
		cout<<"总共需要生成"<<ans_tot<<"条路径"<<endl; 
		cout<<"总路径长度为"<<euler_ans.size()-1<<endl; 
		map<int,int> Tot;
		maxT=0,maxL=0,SumT=0;
		for (int i=0;i<ans_tot;i++){
			maxT=max(maxT,ansTime[i]);
			SumT+=ansTime[i];
			if (Tot.find(ans[i][0])!=Tot.end()) Tot[ans[i][0]]++;
			else Tot[ans[i][0]]=1;
		}
		printf("节点数量：%d\n",Tot.size());
		printf("遥测数据收集时间：%d\n",maxT);
		for (int i=0;i<ans_tot;i++){
			maxL=max(maxL,(int)(ans[i].size()-1));
		}
		printf("最长路径长度：%d\n",maxL);
	}	
} INT_low_cost;


int main(){
	double dur;
	LARGE_INTEGER freq,start,end;
	Init_Graph.init();
	double TimeRecord;
	INT_low_cost.init(Init_Graph.n,Init_Graph.m,Init_Graph.v);
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	INT_low_cost.work(ave_Time*Min_MTU*3/2);
	QueryPerformanceCounter(&end); 
    TimeRecord=(end.QuadPart-start.QuadPart)*1.0/freq.QuadPart;
    INT_low_cost.output1();
    printf("运行时间：%.8lf\n\n",TimeRecord);
    int ansMaxcnm=INT_low_cost.ans_tot*INT_low_cost.maxT;
} 
