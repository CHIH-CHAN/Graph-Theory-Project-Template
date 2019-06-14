#include <iostream>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();
string names[10];
int adjacency[10][10]={0};
int degree[10]={0},odd_connect[10]={0},odd_connect_temp[10]={0};
int MinDistance(int*, int*); 
vector<string> outpath;
int ok=0;

int Dijkstra(int start, int end, int path[]) ;
int MinDistance(int* dist, int* Blackened) ;
void Record(int* parent, int _d, int path[], int c) ;
void DFS (int start, int length, int visit[], string names[], int size);
void adjacency_print();
int find_shortest(int odd[10],int oddnum,int oddnum_total);
void degree_update();




int main(int argc, char** argv){

    //input file and print
	nm->interpret("topo.txt");
	nm->print_all_e();
    nm->print_all_v();
	ofstream fout;

	//make a adjacency matrix and count degree
	Vertex *list =  nm->get_all_nodes();
	Edge *edge = nm->elist;

    while(list!=NULL){
		names[nm->switch_num] = list->name;
       	cout << "name : " << nm->switch_num << " = " << names[nm->switch_num] << endl;
		list=list->next;
		nm->switch_num = nm->switch_num + 1 ;
	}
	cout << "switch_num : "<< nm->switch_num << endl;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(i>=nm->switch_num||j>=nm->switch_num)
				adjacency[i][j] = 0 ;
			else if(!nm->connected(names[i],names[j]) )
				adjacency[i][j] = 1 ;
		}
	}
	degree_update();
	//print adjacency and degree
	adjacency_print();
	int path_test[10],odd[10],oddnum=0;
	for(int i=0;i<10;i++) 
		odd[i]=99;
	//print odd degree vertex
	for(int i=0;i<10;i++) 
		if(degree[i]%2==1)
		{
			odd[oddnum] = i ;
			cout<<"odd["<<oddnum<<"]="<<odd[oddnum]<<endl;
			oddnum++;
		}
	//print odd vertex matching and repeat path length
	cout<<endl<<"shortest_path:"<<find_shortest(odd,oddnum,oddnum)<<endl;
	cout<<"odd vertex matching:"<<endl;
	for(int i=0;i<oddnum;i++)
	{
		if(i%2==0)
			cout<<odd_connect[i];
		else
			cout<<"->"<< odd_connect[i]<<endl;
	}
	//add repeat path to adjacency matrix
	for(int i=0;i<10;i=i+2)
	{
		if(odd_connect[i]==odd_connect[i+1])
			break;
		for(int k=0;k<10;k++)
			path_test[k]=-1;
		Dijkstra(odd[i],odd[i+1],path_test);
		for(int j=1;j<10;j++)
		{
			if(path_test[j]!=(-1))
			{
				adjacency[path_test[j]][path_test[j-1]] ++;
				adjacency[path_test[j-1]][path_test[j]] ++;
			}
			else
			{
				adjacency[odd[i]][path_test[j-1]] ++;
				adjacency[path_test[j-1]][odd[i]] ++;
				break;
			}		
		}
	}
	degree_update();
	adjacency_print();
	int pathlength_final=0,visit[10]={0};
	//count the path length and visit number
	for(int i=0;i<10;i++)
	{
		pathlength_final+= degree[i]/2;
		visit[i]= degree[i]/2;
		if(i==0)
			visit[0]++;
	}
	//use DFS to find path
	DFS(0,pathlength_final,visit,names,nm->switch_num);
	fout.open("output.txt");
	fout<<"Final Path :\n"<< names[0]<<"->";
	for(int i=0;i<outpath.size();i++)
	{
		if(i!=outpath.size()-1)
			fout<<outpath[i]<<"->";
		else
			fout<<outpath[i];
	}
	fout<<endl;
	fout.close();
	//Gplot
	Gplot *gp = new Gplot();
    gp->gp_add(nm->elist);
    gp->gp_dump(true);
    gp->gp_export("plot");

	
    return 0;
}



int Dijkstra(int start, int end, int path[]) 
{ 
	int min_distance; 
	int dist[10],parent[10]; 
	int Blackened[10]={0},pathlength[10]={0}; 

	parent[start] = -1; 

	for (int i = 0; i < 10; i++) 
		dist[i] = 99; 

	dist[start] = 0; 
	for (int i = 0; i < 9; i++) { 
		min_distance = MinDistance(dist, Blackened); 
		if (min_distance == 99) 
			break; 
		else 
		{ 
			Blackened[min_distance] = 1; 
			for (int j = 0; j < 10; j++) 
			{ 
				if (!Blackened[j] && adjacency[min_distance][j] 
					&& dist[min_distance] + adjacency[min_distance][j] < dist[j]) 
				{ 
					parent[j] = min_distance; 
					pathlength[j] = pathlength[parent[j]] + 1; 
					dist[j] = dist[min_distance] + adjacency[min_distance][j]; 
				} 
				else if (!Blackened[j] && adjacency[min_distance][j] 
						&& dist[min_distance] + adjacency[min_distance][j] == dist[j] 
						&& pathlength[min_distance] + 1 < pathlength[j]) 
				{ 
					parent[j] = min_distance; 
					pathlength[j] = pathlength[min_distance] + 1; 
				} 
			} 
		} 
	} 
	if (dist[end] != 99) 
		Record(parent, end, path, -1); 
	else
		return 99;
	return	dist[end];
			
} 

int MinDistance(int* dist, int* Blackened) 
{ 
	int min = 99, min_index, v; 
	for (v = 0; v < 10; v++) 
		if (!Blackened[v] && dist[v] < min) 
		{ 
			min = dist[v]; 
			min_index = v; 
		} 
	return (min == 99) ? 99 : min_index; 
} 

void Record(int* parent, int _d, int path[], int c) 
{ 
	if (parent[_d] == -1)
		return; 
	c++;
	Record(parent, parent[_d], path, c); 	
	path[c] = _d;

}

void adjacency_print()
{
	cout<<"  ";
	for(int i=0;i<nm->switch_num;i++)
		cout<<names[i]<<" ";
	
	cout<<"degree"<<endl;
	for(int i=0;i<nm->switch_num;i++)
	{
		cout<<names[i]<<" ";
		for(int j=0;j<nm->switch_num;j++)
			cout<<adjacency[i][j]<<" ";
		cout<<degree[i]<<endl;
	}	
}

int find_shortest(int odd[10],int oddnum,int oddnum_total)
{
	int path_test[10]={0};
	if(oddnum==2)
	{
		odd_connect[0]=odd[0];
		odd_connect[1]=odd[1];

		return Dijkstra(odd[0],odd[1],path_test);
	}
	else
	{
		int new_odd[10]={0},shortest=999,temp,shortest_odd[10];
		for(int i=1;i<oddnum;i++)
		{
			for(int j=1,index=0;index<oddnum-2;j++)
			{
				if(i!=j)
				{
					new_odd[index]=odd[j];
					index++;
				}
			}
			temp = 	Dijkstra(odd[0],odd[i],path_test) +
					find_shortest(new_odd,oddnum-2,oddnum_total);
			if(temp<shortest)
			{
				shortest = temp ;
				odd_connect_temp[oddnum-2]=odd[0];
				odd_connect_temp[oddnum-1]=odd[i];
				for(int k=0;k<10;k++)
					shortest_odd[k]=new_odd[k];
			}
		}
		odd_connect[oddnum-2]=odd_connect_temp[oddnum-2];
		odd_connect[oddnum-1]=odd_connect_temp[oddnum-1];
		find_shortest(shortest_odd,oddnum-2,oddnum_total-2);

		return shortest;
	}
}
void degree_update()
{
	for(int i=0;i<10;i++)
	{
		degree[i]=0;
		for(int j=0;j<10;j++)
			degree[i]+=adjacency[i][j];
	}
}
void DFS (int start, int length, int visit[], string names[], int size)
{
	visit[start] = visit[start] - 1 ;
	for(int i = 0; i < size; i++)
	{
		if (adjacency[start][i] != 0 && visit[i] != 0)
		{
			adjacency[start][i]--;
			adjacency[i][start]--;
			outpath.push_back(names[i]);
			length--;			
			if (length > 0)
				DFS(i, length, visit, names, size);
			else if (i == 0)
			{
				ok = 1;
				return;
			}
			if (ok == 1)  
				return;
			length++;
			visit[i]++;
			outpath.pop_back();
			adjacency[start][i]++;
			adjacency[i][start]++;
		}	
	}
}