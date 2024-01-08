#include <algorithm>
#include <iostream>
#include<fstream>
#include <queue>
#include <vector>
using namespace std;

class  Node
{
public:
    int id;
    int team1,team2;

    Node()
    {

    }

    Node(int id)
    {
        this->id=id;
    }

    Node(int id,int team1,int team2)
    {
        this->id=id;
        this->team1=team1;
        this->team2=team2;
    }
};

class Team
{
public:
    string name;
    int wins, losses, left;
    int team_id;
};


class Graph
{

public:

    int totalTeam, count_nodes,total_matches,highestWin,maxPossibleWinningID;
    vector<Team> Teams;
    vector<vector<int>> match;
    vector<vector<int>> gameMatch;  // stores the node id for the pair of teams
    vector<vector<int>> residual_capacity;
    vector<vector<Node>> adjList;
    vector<bool> visited;
    Node source = {0},sink;

    Graph(int n)
    {
        totalTeam=n;
        Teams.resize(n + 2);
        match.assign(n+ 2, vector<int>(n + 2, 0));
        gameMatch.assign(n + 2, vector<int>(n + 2));
        total_matches = 0;
        highestWin = 0;
        maxPossibleWinningID = -1;
    }

    //initialize every node with a id-source node,match node,team node,sink node
    void initialization()
    {
        count_nodes = 1;//source node

        for (int i = 1; i <= totalTeam; i++)
        {
            Teams[i].team_id = count_nodes++;//team node
        }

        for (int i = 1; i <= totalTeam; i++)
        {
            for (int j = i + 1; j <= totalTeam; j++)
            {
                int p=count_nodes++;//match node
                gameMatch[i][j] = p;
                gameMatch[j][i] = p;
            }
        }

        sink.id = count_nodes++;
    }

    //make residual graph
    void create_graph(int present_id)
    {
        adjList.clear();
        adjList.resize(count_nodes + 2);
        residual_capacity.assign(count_nodes + 2, vector<int>(count_nodes + 2, 0));
        visited.assign(count_nodes + 2, false);

        for (int i = 1; i <=totalTeam; i++)
        {
            for (int j = i + 1; j <= totalTeam; j++)
            {
                if(i!=present_id && j!=present_id)
                {
                    int match_id = gameMatch[i][j];

                    //  edge between source and all pairs of teams

                    adjList[source.id].push_back({match_id, i, j});
                    adjList[match_id].push_back({source.id});

                    // edge between a pair of teams and their respective nodes
                    int team1_id = Teams[i].team_id;
                    int team2_id = Teams[j].team_id;

                    adjList[match_id].push_back({team1_id, i, i});
                    adjList[team1_id].push_back({match_id, i, j});

                    adjList[match_id].push_back({team2_id, j, j});
                    adjList[team2_id].push_back({match_id, i, j});
                }
            }
        }

        //edge between team nodes and sink
        for (int i = 1; i <= totalTeam ; i++)
        {
            int teamID = Teams[i].team_id;
            if(i!=present_id)
            {
                adjList[teamID].push_back({sink.id});
                adjList[sink.id].push_back({teamID, i, i});
            }
        }
    }

    void create_capacity(int present_id)
    {
        for (int i = 1; i <= totalTeam; i++)
        {
            for (int j = i + 1; j <= totalTeam; j++)
            {
                if(i!=present_id && j!=present_id)
                {
                    int match_id = gameMatch[i][j];

                    // edge_capacity between source and this pair node=
                    // no of matches played between these two teams
                    residual_capacity[source.id][match_id] = match[i][j];
                    residual_capacity[match_id][source.id] = 0;

                    // edge between a pair of teams and those teams' respective nodes
                    int team1_id= Teams[i].team_id;
                    int team2_id = Teams[j].team_id;

                    residual_capacity[match_id][team1_id] = total_matches;
                    residual_capacity[match_id][team2_id] = total_matches;

                    residual_capacity[team1_id][match_id] = 0;
                    residual_capacity[team2_id][match_id] = 0;
                }
            }
        }

        for (int i = 1; i <= totalTeam; i++)
        {
            if(i!=present_id)
            {
                int teamID = Teams[i].team_id;
                residual_capacity[teamID][sink.id] = Teams[present_id].wins + Teams[present_id].left - Teams[i].wins;
                residual_capacity[sink.id][teamID] = 0;
            }
        }
    }

    int bfs_travarse(int source, int sink,vector<int>& parent)
    {
        fill(parent.begin(),parent.end(),-1);

        parent[source]=-2;

        queue<pair<int,int>>q;
        q.push({source,total_matches});

        while(!q.empty())
        {
            int u=q.front().first;
            int capacity_u=q.front().second;

            q.pop();

            for(auto& adjNode: adjList[u])
            {
                if(u!=adjNode.id && parent[adjNode.id]==-1 && residual_capacity[u][adjNode.id]>0)
                {
                    parent[adjNode.id]=u;

                    int min_cap=min(capacity_u,residual_capacity[u][adjNode.id]);
                    if(adjNode.id==sink)
                        return min_cap;

                    q.push({adjNode.id,min_cap});
                }

            }
        }
        return 0;
    }



    int Edmonds_Karp_algo()    // O(VE^2)
    {
        vector<int>parent(count_nodes+2);
        int min_cap=0,max_flow=0;

        while(min_cap=bfs_travarse(source.id,sink.id,parent))
        {
            max_flow+=min_cap;

            int u=sink.id;

            while(u!=source.id)
            {
                int v=parent[u];
                residual_capacity[u][v]+=min_cap;
                residual_capacity[v][u]-=min_cap;
                u=v;
            }
        }

        return max_flow;
    }

    void dfs(int node)
    {
        visited[node] = true;
        for (auto& adjNode : adjList[node])
        {
            if (!visited[adjNode.id] && residual_capacity[node][adjNode.id] > 0)
                dfs(adjNode.id);
        }
    }


    ///Details of a Team--For Bonus

    void details_Of_A_Team(int x,ofstream& outfile)
    {
        int total_win = 0, total_played_between= 0;
        cout << Teams[x].name << " is eliminated."<<endl;
        outfile << Teams[x].name << " is eliminated."<<endl;
        cout << "They can win at most " << Teams[x].wins << " + " << Teams[x].left
             << " = " << Teams[x].wins + Teams[x].left << " games."<<endl;
        outfile << "They can win at most " << Teams[x].wins << " + " << Teams[x].left
                << " = " << Teams[x].wins + Teams[x].left << " games."<<endl;

        vector<int> visitedTeamList;

        if (maxPossibleWinningID != x && highestWin > Teams[x].wins + Teams[x].left)
            visitedTeamList.push_back(maxPossibleWinningID);

        else
        {
            fill(visited.begin(), visited.end(), false);
            dfs(source.id);
            for (int j = 1; j <= totalTeam; j++)
                if (visited[Teams[j].team_id]) visitedTeamList.push_back(j);
        }

        const int list_size = visitedTeamList.size();

        for (int i = 0; i < list_size; i++)
        {
            cout << Teams[visitedTeamList[i]].name;
            outfile << Teams[visitedTeamList[i]].name;
            total_win += Teams[visitedTeamList[i]].wins;
            for (int j = 0; j < list_size; j++)
                total_played_between += match[visitedTeamList[i]][visitedTeamList[j]];
            if (i < list_size - 2)
            {
                cout << ", ";
                outfile << ", ";
            }

            else if (i == list_size - 2 && list_size > 1)
            {
                cout << " and ";
                outfile<< " and ";
            }

            else
            {
                cout << " ";
                outfile << " ";
            }

        }

        total_played_between =total_played_between/ 2;

        cout << (list_size == 1 ? "has" : "have") << " won a total of " << total_win<< " games."<<endl;
        outfile << (list_size == 1 ? "has" : "have") << " won a total of " << total_win<< " games."<<endl;
        cout << "They play each other " << total_played_between << " times."<<endl;
        outfile << "They play each other " << total_played_between << " times."<<endl;
        cout << "So on average, each of the teams wins "<< total_win + total_played_between << "/" << list_size << " = "<<
             float((total_win + total_played_between) / list_size) << " games.";
        cout << endl<< endl;
        outfile << "So on average, each of the teams wins "<< total_win + total_played_between << "/" << list_size << " = "<<
                float((total_win + total_played_between) / list_size) << " games.";
        outfile<< endl<< endl;

    }

};



int main()
{
    ifstream  infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("output.txt");

    int numberOfTeams;
    infile >> numberOfTeams;

    Graph g(numberOfTeams);

    for (int i = 1; i <= numberOfTeams; i++)
    {
        infile >> g.Teams[i].name >> g.Teams[i].wins >> g.Teams[i].losses >> g.Teams[i].left;
        if (g.Teams[i].wins > g.highestWin)
        {
            g.highestWin = g.Teams[i].wins;
            g.maxPossibleWinningID = i;
        }
        for (int j = 1; j <= numberOfTeams; j++)
        {
            infile >> g.match[i][j];
            if (i < j)
                g.total_matches += g.match[i][j];
        }
    }

    g.initialization();

    for (int i = 1; i <= numberOfTeams; i++)
    {
        if (g.highestWin > g.Teams[i].wins + g.Teams[i].left)
        {
            g.details_Of_A_Team(i,outfile);
        }

        else
        {
            g.create_graph(i);
            g.create_capacity(i);
            int returnVal = g.Edmonds_Karp_algo();
            int maximumVal = *max_element(g.residual_capacity[0].begin(), g.residual_capacity[0].end());
            if (maximumVal > 0) g.details_Of_A_Team(i,outfile);
        }

    }

    return 0;
}
