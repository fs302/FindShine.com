#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;
#define SCALE 500000
struct state
{
    int obj,t;
};

int N;
vector<state> view[SCALE+1];
int influence[SCALE+1];
bool cmp(state a,state b)
{
    return a.obj<b.obj;
}
void update(int st,int ed,int t,int delta_t)
{
    //Step 1:view[st] self update
    bool ExistSt = false;
    for(int i=0;i<view[st].size();i++)
    {
        if (view[st][i].obj == st)
        {
            view[st][i].t = t;
            ExistSt = true;
            break;
        }
    }
    if (!ExistSt)
    {
        state Initial_st;
        Initial_st.obj = st;
        Initial_st.t = t;
        view[st].push_back(Initial_st);
    }
    //Step2:Using st to update ed
    //printf("Using %d to update %d...\n",st,ed);
    sort(view[st].begin(),view[st].end(),cmp);
    sort(view[ed].begin(),view[ed].end(),cmp);
    vector<state> temp;//临时存储新增view
    int j = 0;
    for(int i=0;i < view[st].size();i++)
    {
        int flag = 0;
        while(j<view[ed].size())
        {
            if(view[ed][j].obj<view[st][i].obj)
            {
                j++;
                continue;
            }
            else if (view[ed][j].obj==view[st][i].obj)
            {
                if (view[ed][j].t < view[st][i].t && t-view[st][i].t<=delta_t)
                    view[ed][j].t = view[st][i].t;
                flag = 1;
                j++;
            }
            break;
        }
        if (flag == 0 && t-view[st][i].t<=delta_t)//ed中不存在view[st][i]的记录
            temp.push_back(view[st][i]);
    }
    for(int i=0;i<temp.size();i++)
        view[ed].push_back(temp[i]);
}
void calculate_influence(int t)
{
    memset(influence,0,sizeof(influence));
    for(int i=1;i<=N;i++)
    {
        //cout << i << " " << view[i].size() << endl;
        for(int j=0;j<view[i].size();j++)
        {
            int k = view[i][j].obj;
            //cout << view[i][j].obj << " "<<view[i][j].t << endl;
            influence[k] += 1;
        }
    }
    double P = 0;
    int V = 0;
    for(int k=1;k<=N;k++)
    {
        if (influence[k]>0)
        {
            P += influence[k]-1;
            V ++;
        }

    }
    P = P/V;
    printf("%d\t%lf\n",t,P);
}
int main()
{
    int u1,u2,t = 0;
    string t1,t2;
    freopen("Network.txt","r",stdin);
    cin >> N;
    //freopen("Influence Power.txt","w",stdout);
    printf("t\tInfluence Power\n");
    int dt = 32000,flagt = 0;
    int count = 0;
    while(scanf("%d%d%d",&t,&u1,&u2)!=EOF)
    {
        //printf("%d\n",t);
        update(u1,u2,t,dt);
        update(u2,u1,t,dt);

        if (t%100000==0 && t!=flagt)
        {
            calculate_influence(t);
            flagt = t;
        }
        //printf("%d\n",t);

    }

    return 0;
}
