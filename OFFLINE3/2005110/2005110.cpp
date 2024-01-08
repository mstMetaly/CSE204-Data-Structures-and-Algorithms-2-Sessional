#include<iostream>
#include<fstream>
#include<limits.h>

#define INF INT_MAX

using namespace std;

///matrix multiplication

void multiplication(int n,int **matrix)
{
    for(int row=1; row<=n; row++)
    {
        for(int col=1; col<=n; col++)
        {
            for(int k=1; k<=n; k++)
            {
                if(matrix[row][k]!=INF && matrix[k][col]!=INF)
                    matrix[row][col]=min(matrix[row][col], matrix[row][k] + matrix[k][col]);
            }
        }
    }
}

void apsp_matrixMultiplication(int n,int **matrix)
{
    int power=1;

    while(power <= n-1)
    {
       multiplication(n,matrix);
       power*=2;
    }
}


///b---Floyd Warshall algorithm

void floydWarshall(int n,int** matrix)
{
    for(int k=1; k<=n; k++)
    {
        for(int i=1; i<=n; i++)
        {
            for(int j=1; j<=n; j++)
            {
                if(matrix[i][k]!=INF && matrix[k][j]!=INF)
                {
                    int val=matrix[i][k]+matrix[k][j];
                    if(matrix[i][j]>val)
                        matrix[i][j]=val;
                }

            }
        }

    }


}

void print(int n,int **matrix,ofstream& outfile)
{
     cout << "Shortest distance matrix"<< endl;
    outfile << "Shortest distance matrix"<< endl;

    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            if(matrix[i][j]!=INF)
            {
                cout << matrix[i][j]<< "    ";
                outfile<< matrix[i][j]<< "    ";
            }
            else
            {
                cout << "INF"<< "  ";
                outfile<< "INF"<< "  ";
            }

        }
        cout << endl;
        outfile << endl;
    }
}

int main()
{
    ifstream infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("output.txt");

    int n,m;

    infile >> n >> m;

    int** matrix = new int*[n+1];

    for(int i=1; i<=n; i++)
    {
        matrix[i] = new int[n+1];
    }

    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            if(i==j)
                matrix[i][j]=0;
            else
                matrix[i][j]=INF;
        }
    }


    int u,v,wt;

    for(int i=1; i<=m; i++)
    {
        infile >> u >> v >> wt;
        matrix[u][v]=wt;
    }

    floydWarshall(n,matrix);
   // apsp_matrixMultiplication(n,matrix);

    print(n,matrix,outfile);

    return 0;

}
