#include "deploy.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include "math.h"  
#include "time.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

//算法实现函数！！
using namespace std;

/*构造一个带权值的无向图*/
typedef int  edgetype;    //边的类型
#define MAXVEX 800      //最大顶点数
#define INFINITY 65535     //用65535代表无穷

class MGraph
{
	public:
		MGraph(int numVertexes_,int numEdges_)
			:numVertexes(numVertexes_),numEdges(numEdges_){};
		edgetype arc[MAXVEX][MAXVEX]; //邻接矩阵，看作边表
		int numVertexes, numEdges;   //图中当前的定点数和边数 
		~MGraph(){};
	private:
		
};

typedef int Patharc[MAXVEX];    /* 用于存储最短路径下标的数组 */
typedef int ShortPathTable[MAXVEX];/* 用于存储到各点最短路径的权值和 */

int must_pastpoint_1, must_pastpoint_2;//存储必经节点1和2 
int must_pastpath_1_start,must_pastpath_1_end;//存储必经边1的起始节点和结束节点 
int must_pastpath_2_start,must_pastpath_2_end;//存储必经边2的起始节点和结束节点 
int ban_pastpath_start,ban_pastpath_end;//存储禁止经过边的起始节点和结束节点

//Dijkstra算法 
void ShortestPath_Dijkstra(MGraph *G, int v0, Patharc *P, ShortPathTable *D)
{    
	int v,w,k,min;    
	int final[MAXVEX];/* final[w]=1表示求得顶点v0至vw的最短路径 */
	for(v=0; v<G->numVertexes; v++)    /* 初始化数据 */
	{        
		final[v] = 0;			/* 全部顶点初始化为未知最短路径状态 */
		(*D)[v] = G->arc[v0][v];/* 将与v0点有连线的顶点加上权值 */
		(*P)[v] = -1;				/* 初始化路径数组P为-1  */       
	}

	(*D)[v0] = 0;  /* v0至v0路径为0 */  
	final[v0] = 1;    /* v0至v0不需要求路径 */        
	/* 开始主循环，每次求得v0到某个v顶点的最短路径 */   
	for(v=1; v<G->numVertexes; v++)   
	{
		min=INFINITY;    /* 当前所知离v0顶点的最近距离 */        
		for(w=0; w<G->numVertexes; w++) /* 寻找离v0最近的顶点 */    
		{            
			if(!final[w] && (*D)[w]<min)             
			{                   
				k=w;                    
				min = (*D)[w];    /* w顶点离v0顶点更近 */            
			}        
		}        
		final[k] = 1;    /* 将目前找到的最近的顶点置为1 */
		for(w=0; w<G->numVertexes; w++) /* 修正当前最短路径及距离 */
		{
			/* 如果经过v顶点的路径比现在这条路径的长度短的话 */
			if(!final[w] && (min+G->arc[k][w]<(*D)[w]))   
			{ /*  说明找到了更短的路径，修改D[w]和P[w] */
				(*D)[w] = min + G->arc[k][w];  /* 修改当前路径长度 */               
				(*P)[w]=k;        
			}       
		}   
	}
}

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    int netnodeNum = 0;
    int linkNum = 0;
    char *c;
    char *c1;
    int spaceCount = 0;
    c = topo[0];
    MGraph *G = new MGraph(0,0);//构造图G，用来计算S->E的费用最少的路径
	   
    while (*c != '\0' && *c != '\n' && *c != '\r') 
    {
        if (*c == ' ') 
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 1) 
        {
            linkNum = *c - '0' + linkNum * 10;
        }
        if (spaceCount == 0) 
        {
            netnodeNum = *c - '0' + netnodeNum * 10;
        }
        c++;
    }
	G->numVertexes=netnodeNum;
    G->numEdges=linkNum;
 
	for (int i = 0; i < G->numVertexes; i++)
	{
		for ( int j = 0; j < G->numVertexes; j++)
		{
			if (i==j)
			{
				G->arc[i][j] = 0;		
			}
			else
			{
				G->arc[i][j] = G->arc[j][i]= INFINITY;
			}		
		}
	}
	
	must_pastpoint_1 = must_pastpoint_2 = 0;
	spaceCount = 0;
    c = topo[2];
    while (*c != '\0' && *c != '\n' && *c != '\r') 
    {
        if (*c == ' ') 
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 1) 
        {
            must_pastpoint_2 = *c - '0' + must_pastpoint_2 * 10;
        }
        if (spaceCount == 0) 
        {
            must_pastpoint_1 = *c - '0' + must_pastpoint_1 * 10;
        }
        c++;
    }

    must_pastpath_1_start = must_pastpath_1_end = 0;
    must_pastpath_2_start = must_pastpath_2_end = 0;
	spaceCount = 0;
    c = topo[4];
    while (*c != '\0' && *c != '\n' && *c != '\r') 
    {
        if (*c == ' ') 
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 0) 
        {
            must_pastpath_1_start = *c - '0' + must_pastpath_1_start * 10;
        }
        if (spaceCount == 1) 
        {
            must_pastpath_1_end  = *c - '0' + must_pastpath_1_end  * 10;
        }
        if (spaceCount == 2) 
        {
            must_pastpath_2_start = *c - '0' + must_pastpath_2_start * 10;
        }
        if (spaceCount == 3) 
        {
            must_pastpath_2_end = *c - '0' + must_pastpath_2_end * 10;
        }
        c++;
    }

    ban_pastpath_start = ban_pastpath_end = 0;
	spaceCount = 0;
    c = topo[6];
    while (*c != '\0' && *c != '\n' && *c != '\r') 
    {
        if (*c == ' ') 
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 1) 
        {
            ban_pastpath_end = *c - '0' + ban_pastpath_end * 10;
        }
        if (spaceCount == 0) 
        {
            ban_pastpath_start = *c - '0' + ban_pastpath_start * 10;
        }
        c++;
    }
    
    MGraph *G1 = new MGraph(*G); //构造图G1，用来计算S->E的经过最少节点的路径
    int linkstart = 0,linkend = 0,unitcost = 0;
    for(int i = 0;i < G->numEdges; i++)
    {
       spaceCount = 0;
       c = topo[i+8];
       linkstart = 0,linkend = 0,unitcost = 0;
        while (*c != '\0' && *c != '\n' && *c != '\r') 
       {
    	
        if (*c == ' ') 
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 2) 
        {
            unitcost = *c - '0' + unitcost * 10;
        }
        if (spaceCount == 1) 
        {
           linkend = *c - '0' + linkend * 10;
        }
        if (spaceCount == 0) 
        {
           linkstart = *c - '0' + linkstart * 10;
        }
        c++;
       }
       	G->arc[linkstart][linkend] = unitcost;
       	G->arc[linkend][linkstart] = unitcost;
		G1->arc[linkstart][linkend] = 1;
       	G1->arc[linkend][linkstart] = 1;	
    }
    G->arc[ban_pastpath_start][ban_pastpath_end] = INFINITY;
    G->arc[ban_pastpath_end][ban_pastpath_start] = INFINITY;
    G1->arc[ban_pastpath_start][ban_pastpath_end] = INFINITY;
    G1->arc[ban_pastpath_end][ban_pastpath_start] = INFINITY;

    string res;
    char a[50];
    MGraph *G2 = new MGraph(*G); //构造图G2，用来暂时存储图G 
    
    //运行两次主算法，第一次计算费用最低路径，得到最优解或次优解；第二次计算经过节点最少的路径，得到次优解或者给出一定的提示 
	for(int replica=0;replica<2;replica++)
    {
	    if(replica==1) 
		  *G=*G1;
		  
		//运行中间必经点2+必经边2*2+起始结束节点2=8个点的Dijkstra算法 
		Patharc P[8]; /* 用于存储最短路径下标的数组 */   
		ShortPathTable D[8];/* 用于存储到各点最短路径的权值和 */ 
	
		ShortestPath_Dijkstra(G, 0, &P[0], &D[0]); 
		ShortestPath_Dijkstra(G, must_pastpoint_1, &P[1], &D[1]); 
		ShortestPath_Dijkstra(G, must_pastpoint_2, &P[2], &D[2]); 
		ShortestPath_Dijkstra(G, must_pastpath_1_start, &P[3], &D[3]); 
		ShortestPath_Dijkstra(G, must_pastpath_1_end, &P[4], &D[4]); 
		ShortestPath_Dijkstra(G, must_pastpath_2_start, &P[5], &D[5]); 
		ShortestPath_Dijkstra(G, must_pastpath_2_end, &P[6], &D[6]); 
		ShortestPath_Dijkstra(G, netnodeNum-1, &P[7], &D[7]); 
		
		int paths[8];
		paths[0] = 0;	
		paths[1] = must_pastpoint_1;
		paths[2] = must_pastpoint_2;
		paths[3] = must_pastpath_1_start;	
		paths[4] = must_pastpath_1_end;
		paths[5] = must_pastpath_2_start;
		paths[6] = must_pastpath_2_end;
		paths[7] = netnodeNum-1;
		
	 
	   
	    int mincost_paths[8];
	    int cost_paths[8];
	    int mincost = INFINITY;
	    int pathcost = 0;
	    int pathcount = 0;
	    
	    mincost_paths[0]=0;
	    mincost_paths[7]=7;
	   
	    for(int i=1;i<=6;i++)
	    {
		  for(int j=1;j<=6;j++)
	         if(j!=i)
	         for(int k=1;k<=6;k++)
	            if((k!=j)&&(k!=i))
	            for(int s=1;s<=6;s++)
	                if((s!=j)&&(s!=i)&&(s!=k))
	                for(int l=1;l<=6;l++)
					     if((l!=j)&&(l!=i)&&(l!=k)&&(l!=s))
					     for(int m=1;m<=6;m++)
					            if((m!=j)&&(m!=i)&&(m!=k)&&(m!=s)&&(m!=l))
					            {
									 pathcost = D[0][paths[i]]+D[i][paths[j]]+D[j][paths[k]]+D[k][paths[s]]+D[s][paths[l]]+D[l][paths[m]]+D[m][paths[7]];
									 cost_paths[1]=i;cost_paths[2]=j;cost_paths[3]=k;
									 cost_paths[4]=s;cost_paths[5]=l;cost_paths[6]=m;
									 int check = 0;
									 for(int ij=1;ij<=6;ij++)
									 {
									    if((cost_paths[ij]==3)&&(cost_paths[ij+1]!=4)&&(cost_paths[ij-1]!=4))
									       check = 1;
										if((cost_paths[ij]==4)&&(cost_paths[ij+1]!=3)&&(cost_paths[ij-1]!=3))
									       check = 1;
										if((cost_paths[ij]==5)&&(cost_paths[ij+1]!=6)&&(cost_paths[ij-1]!=6))
									       check = 1;
										if((cost_paths[ij]==6)&&(cost_paths[ij+1]!=5)&&(cost_paths[ij-1]!=5))
									       check = 1;								 	
									 }
	
									 if((pathcost<mincost)&&(check == 0))
									 {
									 	mincost = pathcost;
									 	mincost_paths[1]=i;mincost_paths[2]=j;mincost_paths[3]=k;
										mincost_paths[4]=s;mincost_paths[5]=l;mincost_paths[6]=m;
									 }
					            }
		}
	
		if(replica==0)
		{
			cout<<"Conclusion: "<<endl; 
			cout<<"Part1: Only consider the mincost."<<endl; 
			cout<<"       The path is: ";   
            sprintf(a, "%s","Conclusion: \nPart1: Only consider the mincost.\n       The path is: ");
			res += a;
		    
			int j;
			int pastpoint = 0;
			for(int i=0;i<7;i++)
		    {
		    	j = paths[mincost_paths[i]];
		    	cout<<j<<" -> ";
		    	sprintf(a, "%d %s",j,"-> ");
		    	res += a;
		    	pastpoint++;
				while(P[mincost_paths[i+1]][j]!=-1)
		        {
			      j=P[mincost_paths[i+1]][j];
		 	      cout<<j<<" -> ";
		 	      sprintf(a, "%d %s",j,"-> ");
		    	  res += a;
		    	  pastpoint++;
				}	
		    }
		    cout<<netnodeNum-1<<endl; 
		    sprintf(a, "%d",netnodeNum-1);
		    res += a;
		    res += "\n";
		    pastpoint++;
		    cout<<"       The number of points passed is: "<<pastpoint<<endl;
		    cout<<"       The mincost is: "<<mincost<<endl;
		    sprintf(a, "%s%d%s%d","       The number of points passed is: ",pastpoint,"\n       The mincost is: ",mincost);
			res += a;
			res += "\n";

		    if(pastpoint>9) 
			{
				sprintf(a, "%s","       This is a reference path.");
			    res += a;
			    res += "\n";
			} 
		    else 
		    {
		    	sprintf(a, "%s","       This is the optimal path.");
			    res += a;
			    res += "\n";
			}
		    
		    if(pastpoint>9)  cout<<"       This is a reference path."<<endl;
		    else  cout<<"       This is the optimal path."<<endl;	
			cout<<endl;	
			res += "\n";
		}
		if(replica==1)
		{
			cout<<"Part2: Only consider the min_number points passed."<<endl; 
			cout<<"       The path is: ";   
            sprintf(a, "%s","Part2: Only consider the min_number points passed.\n       The path is: ");
			res += a;	
			int min_point_path[MAXVEX];
			int min_point_path_count = 0;
			int min_point_cost = 0;
			int j;
			int pastpoint = 0;
			for(int i=0;i<7;i++)
		    {
		    	j = paths[mincost_paths[i]];
		    	cout<<j<<" -> ";
		    	sprintf(a, "%d %s",j,"-> ");
		    	res += a;
		    	pastpoint++;
                min_point_path[min_point_path_count]=j;
                min_point_path_count++;
				while(P[mincost_paths[i+1]][j]!=-1)
		        {
			      j=P[mincost_paths[i+1]][j];
		 	      cout<<j<<" -> ";
		 	      sprintf(a, "%d %s",j,"-> ");
		    	  res += a;
		    	  pastpoint++;
		    	  min_point_path[min_point_path_count]=j;
                  min_point_path_count++;
				}	
		    }
		    cout<<netnodeNum-1<<endl; 
		    sprintf(a, "%d",netnodeNum-1);
		    res += a;
		    res += "\n";
		    pastpoint++;
		    
		    min_point_path[min_point_path_count]=netnodeNum-1;
            min_point_path_count=0;
            
            while(min_point_path[min_point_path_count]!=netnodeNum-1)
            {
            	min_point_cost += G2->arc[min_point_path[min_point_path_count]][min_point_path[min_point_path_count+1]];
            	min_point_path_count++;
			}

			cout<<"       The number of points passed is: "<<pastpoint<<endl;
		    cout<<"       The mincost is: "<<min_point_cost<<endl;
		    sprintf(a, "%s%d%s%d","       The number of points passed is: ",pastpoint,"\n       The mincost is: ",min_point_cost);
			res += a;
			res += "\n";

		    if(pastpoint>9) 
			{
				sprintf(a, "%s","       This is a reference path, but it is not the optimal path. In this case, no optimal path exists.");
			    res += a;
			    res += "\n";
			} 
		    else 
		    {
		    	sprintf(a, "%s","       This is sub-optimal path. In this case, a sub-optimal path exists.");
			    res += a;
			    res += "\n";
			}
		    
		    if(pastpoint>9)  cout<<"       This is a reference path, but it is not the optimal path. In this case, no optimal path exists."<<endl;
		    else  cout<<"       This is sub-optimal path. In this case, a sub-optimal path exists"<<endl;	
			cout<<endl;	
			res += "\n";		    
		}			    			
	}
     
	delete G;
	delete G1;	
	delete G2;
    char * topo_file = (char *)res.c_str();
    write_result(topo_file, filename);
}
