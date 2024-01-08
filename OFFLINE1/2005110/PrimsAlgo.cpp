#include<iostream>
#include<queue>
#include<unordered_map>
#include<vector>
#include<list>
#include<fstream>
#define INF 1e10

using namespace std;

typedef pair<double, int>wtVertex;

int primsAlgorithm(int n,int source, unordered_map<int,list<pair<int,double>>>adjList,vector<int>& parent)
{
    double totalWeight=0;
    vector<double>key(n,INF);//stores key--(weight) for all the vertex
    vector<bool>mst(n,false);//keep track for the vertex whether it is included in MST or not
    priority_queue< wtVertex, vector <wtVertex>, greater<wtVertex> > priority_q;

    key[source]=0;
    priority_q.push(make_pair(key[source],source));

    while(!priority_q.empty())
    {
        pair<double,int>wtVertex=priority_q.top();
        priority_q.pop();

        int minValVertex=wtVertex.second;

        if(!mst[minValVertex])
        {
            mst[minValVertex]=true;

            for(auto v : adjList[minValVertex])
            {
                double edgeWt=v.second;
                int adjVtx=v.first;

                if(mst[adjVtx]==false && key[adjVtx]>edgeWt)
                {
                    key[adjVtx]=edgeWt;
                    priority_q.push(make_pair(edgeWt,adjVtx));
                    parent[adjVtx]=minValVertex;
                }
            }
        }

    }

    for(auto v : key)
    {
        totalWeight+=v;
    }

    return totalWeight;

}

int main()
{
    ifstream infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("prim.txt");

    int n,m,rootNode=0;
    infile >> n >> m;

    //adjacency list for the graph representation--
    //it keeps a list of pair of adjacent vertex and edge wt against all the vertex

    unordered_map<int,list<pair<int,double>>>adjList;
    vector<int>parent(n,-1);//store parent for all the vertex

    for(int i=1; i<=m; i++)
    {
        int u,v;
        double wt;
        infile >> u >> v >> wt;
        adjList[u].push_back(make_pair(v,wt));
        adjList[v].push_back(make_pair(u,wt));

    }


    double totalWeight=primsAlgorithm(n,rootNode,adjList,parent);

    cout << "Prim's algorithm:----------"<< endl;
    cout << "Total Weight: " <<  totalWeight << endl;
    cout << "Root Node : "<< rootNode<< endl;


    outfile<<  "Prim's algorithm:----------"<< endl;
    outfile << "Total Weight: " <<  totalWeight << endl;
    outfile << "Root Node : "<< rootNode<< endl;

    for(int i=1; i<n; i++)
    {
        cout << parent[i] << "  "<< i << endl;
        outfile << parent[i] << "  "<< i << endl;

    }

    return 0;
}
