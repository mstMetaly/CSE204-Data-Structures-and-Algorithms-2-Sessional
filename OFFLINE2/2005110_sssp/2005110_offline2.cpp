#include<iostream>
#include<queue>
#include<unordered_map>
#include<vector>
#include<list>
#include<fstream>
#define INF  2147483647

using namespace std;

typedef pair<int, int>wtVertex;

///Dijkstra Algorithm---

int dijkstraAlgo(int n,int source,int dest, unordered_map<int,list<pair<int,int>>>&adjList,vector<int>& parent,vector<int>&key)
{
    //keep track for the vertex whether it is popped from the queue or not
    vector<bool>visit(n,false);

    //priority queue keeps pair of (wt,vertex) in ascending order according to their weight
    priority_queue< wtVertex, vector <wtVertex>, greater<wtVertex> > priority_q;

    priority_q.push(make_pair(key[source],source));

    while(!priority_q.empty())
    {
        pair<int,int>wtVertex=priority_q.top();
        priority_q.pop();

        int u=wtVertex.second;

        if(!visit[u])
        {
            visit[u]=true;

            for(auto adjV : adjList[u])
            {
                int edgeWt=adjV.second;
                int v=adjV.first;

                if(visit[v]==false && key[v]>edgeWt+key[u])
                {
                    key[v]=edgeWt + key[u];
                    priority_q.push(make_pair(key[v],v));
                    parent[v]=u;
                }
            }
        }

    }

    return key[dest];

}


///Bellman-Ford Algorithm----

bool bellmanFord(int n,int source,int dest,vector<pair<int,pair<int,int>>>&edges,vector<int>&parent,vector<int>&key)
{
    int u,v,wt;

    //relaxation for every edge will be called for n-1 th times
    for(int i=1; i<n; i++)
    {
        for(auto edge: edges)
        {
            u=edge.first;
            v=edge.second.first;
            wt=edge.second.second;

            //relaxation--takes O(1) time
            if(key[u]+wt < key[v] && key[u]!=INF)
            {
                key[v]=key[u]+wt;
                parent[v]=u;
            }

        }
    }

    //if for any of edge relaxation is possible,then it means the graph has negative edge cycle
    //then it will return false
    for(auto edge : edges)
    {
        u=edge.first;
        v=edge.second.first;
        wt=edge.second.second;

        if(key[u]+wt < key[v] && key[u]!=INF)
            return false;
    }

    return true;



}

int main()
{
    ifstream infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("output.txt");

    int n,m,source,dest;
    infile >> n >> m;

    //adjacency list for the graph representation--
    //it keeps a list of pair of (adjacent vertex,edge wt) against all the vertex

    unordered_map<int,list<pair<int,int>>>adjList;
    vector<pair<int,pair<int,int>>>edges;
    vector<int>parent(n,-1);//store parent for all the vertex
    vector<int>dijkstra_key(n,INF);
    vector<int>bellman_key(n,INF);

    for(int i=1; i<=m; i++)
    {
        int u,v,wt;
        infile >> u >> v >> wt;

        adjList[u].push_back(make_pair(v,abs(wt)));

        pair<int,int>edge=make_pair(v,wt);
        edges.push_back(make_pair(u,edge));

    }

    infile >> source >> dest;

    bellman_key[source]=0;

    cout << "Bellman Ford Algorithm:----"<< endl;
    outfile << "Bellman Ford Algorithm:----"<< endl;

    bool cycleExist=bellmanFord(n,source,dest,edges,parent,bellman_key);

    if(!cycleExist)
    {
        cout << "Negative weight cycle present"<< endl;
        outfile << "Negative weight cycle present"<< endl;

    }
    else
    {
        cout << bellman_key[dest]<< endl;
        outfile << bellman_key[dest]<< endl;

        int temp=dest;
        vector<int>path;

        while(temp!=-1)
        {
            path.push_back(temp);
            temp=parent[temp];
        }

        for(int i=path.size()-1; i>=0; i--)
        {
            cout << path[i];
            outfile << path[i];
            if(i!=0)
            {
                cout << "->";
                outfile << "->";
            }
        }
        outfile << endl << endl;


    }

    outfile << endl<<endl;
    cout << endl<<endl;

    dijkstra_key[source]=0;
    int cost=dijkstraAlgo(n,source,dest,adjList,parent,dijkstra_key);

    cout << "Dijkstra's algorithm:----"<< endl;
    cout << "Total Cost : " << cost << endl;

    outfile << "Dijkstra's algorithm:----"<< endl;
    outfile << "Total Cost : " << cost << endl;


    int temp2=dest;
    vector<int>path2;

    while(temp2!=-1)
    {
        path2.push_back(temp2);
        temp2=parent[temp2];
    }

    for(int i=path2.size()-1; i>=0; i--)
    {
        cout << path2[i];
        outfile << path2[i];
        if(i!=0)
        {
            cout << "->";
            outfile << "->";
        }

    }


    return 0;
}
