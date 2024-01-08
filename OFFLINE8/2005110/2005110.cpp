#include<iostream>
#include<bits/stdc++.h>

#define INT_MAX 999999;

using namespace std;


///Graph Class
class Graph
{
public:

    int** arr=nullptr;
    int length;
    int root;

    Graph()
    {
        root = 0;
    }


    int** Create_Random_Graph(int V)
    {
        length = V;
        arr = new int*[V];

        for(int i = 0; i < V; i++)
        {
            arr[i] = new int[V];

        }

        for(int i=0; i<V; i++)
        {
            for(int j=0; j<V; j++)
            {
                arr[i][j] = -1;
            }
        }

        for(int i = 0 ; i< V ; i++)
        {
            for(int j = 0; j<V ; j++)
            {
                if(i == j)
                {
                    arr[i][j] = 0;
                }
                else
                {
                    arr[i][j] = rand() % 151 + 50;

                    for(int k=0; k<V; k++)
                    {
                        if(arr[i][k] != -1 &&  arr[j][k]!= -1)
                        {
                            if(arr[i][k] + arr[j][k] < arr[i][j])
                            {
                                arr[i][j] = arr[i][k] + arr[j][k];
                            }
                        }

                    }
                }

            }
        }

        return arr;
    }

    void Make_EdgeList(vector<pair<double,pair<int,int>>>& edges)
    {
        for(int i=0 ; i<length ; i++)
        {
            for(int j =0 ; j <length ; j++)
            {
                int weight = arr[i][j];

                pair<int,int>edge=make_pair(i,j);
                edges.push_back(make_pair(weight,edge));
            }
        }
    }


};

///𝑬𝒙𝒂𝒄𝒕_𝑻𝑺𝑷(𝑮𝒓𝒂𝒑𝒉 𝑮)
int tsp(int mask, int pos, Graph G)
{
    int V = G.length;
    int VISITED_ALL = (1 << V) -1 ;

    if(mask==VISITED_ALL)
    {
        return G.arr[pos][0];
    }

    int ans = INT_MAX;
    int newAns=0;

    for(int city = 0 ; city < V ; city++)
    {
        if((mask & (1<<city))==0)
        {
            newAns = G.arr[pos][city] + tsp(mask| (1<< city),city,G);
            ans = min(ans, newAns);
        }
    }

    return ans;

}


int Exact_TSP(Graph G)
{
    int answer = tsp(1,0,G);
    return answer;

}



///make function assigns a vertex to it's parent itself
///keep initial size 1 for a vertex
void make(int v,int parent[],int setSize[],bool visit[])
{
    parent[v]=v;
    setSize[v]=1;
    visit[v] = false;
}


///finds a parent for a vertex using path compression
int Find(int v,int parent[])
{
    if(parent[v]==v)
        return v;
    return parent[v]=Find(parent[v],parent);
}



///makes two disjoint set into a single set using union by size
void Union(int a,int b,int parent[],int setSize[])
{

    if(setSize[a]<setSize[b])
        swap(a,b);
    parent[b]=a;
    setSize[a]+=setSize[b];

}

bool cmp(pair<double,pair<int,int>>&wtEdge1,pair<double,pair<int,int>>& wtEdge2)
{
    return wtEdge1.first < wtEdge2.first;
}

///Krushkals Algorithm
void krushkalAlgo(vector<pair<double,pair<int,int>>>& wtEdges,int parent[],int setSize[],vector<pair<int,int>>&mst,unordered_map<int,list<int>>&adj)
{
    sort(wtEdges.begin(),wtEdges.end(),cmp);///sorting time complexity: O(E logE)

    ///iterate through all the edges ,it would take O ( E alpha(V))
    ///where alpha (V) regarded as constant due to optimization of find and union function

    for(auto edge : wtEdges)
    {
        int u=edge.second.first;
        int v=edge.second.second;
        double edge_wt = edge.first;

        ///for each edge, the find and union operation take nearly constant time
        /// due to path compression and union by size techniques

        int u_parent=Find(u,parent);
        int v_parent=Find(v,parent);
        if(u_parent!=v_parent)
        {
            Union(u_parent,v_parent,parent,setSize);
            mst.push_back(make_pair(u,v));
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }


}

///DFS
void DFS(int vertex,unordered_map<int,list<int>>& adj, bool visit[], vector<int> &dfsPath)
{
    visit[vertex]=true;
    dfsPath.push_back(vertex);

    for(int child: adj[vertex])
    {
        if(!visit[child])
            DFS(child,adj,visit,dfsPath);
    }

}

///Metric_Approximation_TSP
vector<int> Metric_Approximation_TSP(Graph G)
{
    int root = G.root;
    int V = G.length;
    int parent[V];
    int setSize[V];
    bool visit[V];

    for(int i=0; i<V; i++)
    {
        make(i,parent,setSize,visit);
    }

    vector<int>dfsPath;
    unordered_map<int,list<int>>adj;
    vector<pair<double,pair<int,int>>>edges;
    vector<pair<int,int>>mst;

    G.Make_EdgeList(edges);
    krushkalAlgo(edges,parent,setSize,mst,adj);
    DFS(root,adj,visit,dfsPath);

    return dfsPath;
}


///Calculate Tour Length of a Tour T
int Calculate_Tour_Length(vector<int>T,Graph G)
{
    int totalWeight=0;
    for(int i=0 ; i< T.size()-1; i++)
    {
        totalWeight += G.arr[T[i]][T[i+1]];
    }
    totalWeight+= G.arr[T[T.size()-1]][G.root];
    return totalWeight;
}

int main()
{
    int X;
    cin >> X;

    int V = 4;
    int root =0;

    double exactL_arr[X];
    double apprxL_arr[X];


    for(int i=1; i<=X ; i++)
    {
        Graph G;
        vector<int>dfsPath;

        G.Create_Random_Graph(V);

        dfsPath = Metric_Approximation_TSP(G);
        int approximate_l=  Calculate_Tour_Length(dfsPath, G);

        int exact_l = Exact_TSP(G);


        exactL_arr[i] = double(exact_l);
        apprxL_arr[i] = double(approximate_l);
    }

    for(int i=1; i<=X; i++)
    {
        cout << "("  << i << "," << double(apprxL_arr[i]/exactL_arr[i]) << ")"<< endl;
    }




    return 0;

}
