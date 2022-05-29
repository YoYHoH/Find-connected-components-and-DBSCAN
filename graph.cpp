#include<iostream>
#include <random>//for random
#include <iomanip>//for random
#include <string>
#include <fstream>
#include <cmath>

//V = set of points, E = set of edges directed
//we use number to represent V, i.e. V= {0,1,2,...,n-1} we do not consider the (real meaning)value of point, but only the number
//every point u in V has a Adjlist
//Adjlist is a list which stores the number of points
//in the Adjlist of u, we store every point v such that (u,v) is in E

typedef struct Edge{
	int startpoint;
	int endpoint;
}Edge;

typedef struct Adjlist{
	int pointnumber;
	Adjlist *next;
}Adjlist;

//add a point pointN to the adjlist s
void addpoint(Adjlist *&s,int pointN){
	Adjlist *x=new Adjlist;
	x->pointnumber=pointN;
	x->next=s;
	s=x;
}

typedef struct Data
{
	int n_node;
	int n_edge;
	Edge *edges;
}Data;

//get the data from a file which stores the informations of edges, the structure of the file is strict
//return a Data which stores the informations of edges
Data getData(char* fileName){
	Data data;
	int n;
	int m;
	Edge *edges;
	std::ifstream file(fileName);
	if(!file.is_open()){
		std::cout << "open file error" << std::endl;
		return data;
	}
	std::string buf;
	// get the amount of nodes
	file >> buf;
	if(buf == "node"){
		file >> n;
		data.n_node = n;
	}
	else{
		std::cout << "node amount error" <<std::endl;
		return data;
	}
	// get the amount of edges 
	file >> buf;
	if(buf == "edge"){
		file >> m;
		data.n_edge = m;
	}
	else{
		std::cout << "edge amount error" <<std::endl;
		return data;
	}
	// get data of edges
	file >> buf;
	if(buf == "data"){
		edges = new Edge[data.n_edge];
	}
	else{
		std::cout << "edge data error" <<std::endl;
		return data;
	}
	int i = 0;
	while(! file.eof()){
		file >> edges[i].startpoint;
		file >> edges[i].endpoint;
		i+=1;
		continue;
	}
	data.edges = edges;
	file.close();
	return data;
}

//G=(V,E)
class Graph{
public:
	int n;// number of points
	int m;// number of edges
	Adjlist **head;//head[i] is the Adjlist* of points i.
	Graph(){
			this->n=0;
			this->m=0;
			this->head=NULL;
		}
// create a graph with n points, m edges
	Graph(Edge *edges,int n,int m){
			this->n=n;
			this->m=m;
			head=new Adjlist*[n];
			for(int i=0;i<n;i++)
				head[i]=NULL;
			for(int i=0;i<m;i++){
				addpoint(head[edges[i].startpoint],edges[i].endpoint);
				
			}
		}
// create a random graph with N points, every edge e, P(e in E)= p
	Graph(int N,double p){
			this->n=N;
			this->m=0;
			this->head=new Adjlist*[N];
			for(int i=0;i<N;i++)
				head[i]=NULL;
			
			std::random_device rd;
		    	std::default_random_engine eng(rd());
			std::uniform_real_distribution<float> distr(0, 1);
			for(int i=0;i<N;i++){
				for(int j=0;j<N;j++){
					if(j==i)continue;
					if (distr(eng)	<p){
					addpoint(head[i],j);
					m++;
					}			
				}
			}
		}		
	~Graph(){
			for(int i=0;i<n;i++){
				delete[] head[i];
			//	delete[] head;
			}
		}		
// print all points v in the Adjlist of point i
	void showlist(int i){
			std::cout<<"Adjlist of point "<<i<<": ";
			Adjlist *s=head[i];
			while(s!=NULL){
			std::cout << "-"<< s->pointnumber;
			s=s->next;
			}
			std::cout<<"\n";
		}	
	void showalllist(){
	
		for(int i=0;i<n;i++)
			showlist(i);
	}
};

//Graph1 is used for the question1,2 
#include <stack>

class Graph1:public Graph{
public:
	bool *marked;//used in the dfs
	std::stack <int> mystack;// used in the dfs
	int *cluster;// used for marking the strong connected component, cluster[i] is the connected component of point i
	int C;//used for marking the connected component
	Graph1():Graph(){
			this->marked=NULL;
			std::stack<int>mystack;
			this->cluster=NULL;
			C=0;
		}				
	Graph1(Edge *edges,int n,int m):Graph(edges,n,m){
			this->marked=new bool[n];
			for (int i=0;i<n;i++)
				this->marked[i]=false;
				
			std::stack<int> mystack;
			
			cluster=new int[n];
			for(int i=0;i<n;i++)
				cluster[i]=-1;
			C=0;
		}
//create random graph	
	Graph1(int n,double p):Graph(n,p){
			this->marked=new bool[n];
			for (int i=0;i<n;i++)
				this->marked[i]=false;
			std::stack<int> mystack;
			cluster=new int[n];
			for(int i=0;i<n;i++)
				cluster[i]=-1;
			C=0;
		}			
	~Graph1(){
		delete []marked;
		delete []cluster;
		}
//create(and return) the reverse graph G'=(V,E'), i.e.  (u,v) in E' if and only if (v,u) in E		
	Graph1 reverse(){
			Graph1 newg;
			newg.n=this->n;
			newg.m=this->m;
			newg.head=new Adjlist*[this->n];
			for(int i=0;i<this->n;i++)
				newg.head[i]=NULL;
			for(int i=0;i<this->n;i++){
				Adjlist *y=this->head[i];
				while(y!=NULL){
					addpoint(newg.head[y->pointnumber],i);
					y=y->next;
				}
			}
			newg.marked=new bool[n];
			for (int i=0;i<n;i++)
				newg.marked[i]=false;
				
			newg.cluster=new int[n];
			for(int i=0;i<n;i++)
				newg.cluster[i]=-1;		
			return newg;
		}
//depth-first search, begins at the point i, push i in the stack after recursion
	void dfs(int i){
			Adjlist*y=this->head[i];
			marked[i]=true;
			while(y!=NULL){
				if(!marked[y->pointnumber])
					dfs(y->pointnumber);
				y=y->next;
			}
			mystack.push(i);
		}
//do dfs to all points	
	void ALLdfs(){
			for(int i=0;i<n;i++)
				if(!marked[i])
					dfs(i);
		}

//depth-first search. Mark the cluster value before recursion
	void dfs2(int i){
			
			Adjlist*y=this->head[i];
			cluster[i]=C;
			while(y!=NULL){
				if(cluster[y->pointnumber]==-1)
					dfs2(y->pointnumber);
				y=y->next;
			}
		}		
// give the Strong Connected components, 
// After running this function, the points in the same strong connected component have the same cluster value
	void StrongConnectComp(){
			Graph1 newg=reverse();
			newg.ALLdfs();
			
			while(!newg.mystack.empty()){
				int a=newg.mystack.top();
				newg.mystack.pop();
				if(cluster[a]==-1){
					dfs2(a);C++;
				}
			}
		}
//print the cluster value of every point		
	void showcluster(){
		std::cout<<"Number of strong connected components: "<< C <<std::endl<<"Componnet of each point:"<<std::endl;
			for(int i=0;i<n;i++)
				std::cout<<cluster[i]<<" ";
			std::cout<<std::endl;
		}		
//write the data of graph in a file (after creating a random graph)
	void myfilewriter(char * filename){		
		std::ofstream outfile;
		outfile.open(filename);
		outfile<<"node"<<"\n";
		outfile<<n<<"\n";
		outfile<<"edge"<<"\n";
		outfile<<m<<"\n";
		outfile<<"data"<<"\n";
		Adjlist *y;			
		for(int i=0;i<n;i++){
			y=head[i];
			while(y!=NULL){
			outfile<<i<<" "<<y->pointnumber<<"\n";
			y=y->next;
			}
		}	
		outfile.close();
		std::cout<<"The data of graph is saved in \""<<filename<<"\""<<std::endl;
	}
//write the result of scc in a file
	void writeclusterinfile(char *filename){	
		std::ofstream outfile;
		outfile.open(filename);
		outfile<<"Result of strong connected components\n";
		outfile<<"node"<<"\n";
		outfile<<n<<"\n";
		outfile<<"point"<<"  component\n";
		for(int i=0;i<n;i++)
			outfile<<i<<" "<<cluster[i]<<"\n";
		outfile.close();
		std::cout<<"The result of strong connected components is saved in \""<<filename<<"\""<<std::endl;
	}
};

//to represent a 2D point
typedef struct Point{
	double x;
	double y;
}Point;

//compute the distance^2 of two points a and b
double squaredist(Point a,Point b){
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

//For question3
//G=(V,E)
//undirected graph
//V =a set of 2D points
class Graph2{
public:
	Point* Plist;//a list of 2D points
	int n;//total number of points
	Adjlist **adjlist;//every point has a list, to store the points which are closed enough(<epsilon) to it
	int* adjlistlen;//adjlistlen[u] = the number of points in adjlist[u]
	int* cluster;//used for marking the cluster
	int C;//used for marking the cluster
	
	Graph2(char * filename){			
		std::ifstream file(filename);
		if(!file.is_open()){
			std::cout << "open file error" << std::endl;
			file.close();
		}
		// get the amount of nodes
		file >> n;
		C=0;
		cluster=new int[n];
			
		adjlist=new Adjlist*[n];
		for(int i=0;i<n;i++)
			adjlist[i]=NULL;
		
		adjlistlen=new int[n];
		for(int i=0;i<n;i++)
			adjlistlen[i]=0;
		
		cluster=new int[n];
		for(int i=0;i<n;i++)
			cluster[i]=-1;
		
		Plist=new Point[n];
		
		for(int i=0;i<n;i++){
		file>>Plist[i].x;
		file>>Plist[i].y;
		}
		file.close();
	}
	//create a graph, N is total number of points, plist if the list of 2D points. This constructor can be used to copy a graph
	Graph2(Point* plist,int N){
		n=N;	
		C=0;

		adjlist=new Adjlist*[n];
		for(int i=0;i<n;i++)
			adjlist[i]=NULL;
		
		adjlistlen=new int[n];
		for(int i=0;i<n;i++)
			adjlistlen[i]=0;
		
		cluster=new int[n];
		for(int i=0;i<n;i++)
			cluster[i]=-1;
		
		Plist=new Point[N];
		for(int i=0;i<N;i++){
			Plist[i].x=plist[i].x;
			Plist[i].y=plist[i].y;
		}
	}
	//create a random graph with n points, in the area (0,a)X(0,a)
	Graph2(int n,double a){
			
			Plist=new Point[n];
			
			std::random_device rd;
			std::default_random_engine eng(rd());
			std::uniform_real_distribution<float> distr(0, a);
			
			for(int i=0;i<n;i++){
			Plist[i].x=distr(eng);
			Plist[i].y=distr(eng);
			}
			
			adjlist=new Adjlist*[n];
			for(int i=0;i<n;i++)
				adjlist[i]=NULL;
			
			adjlistlen=new int[n];
			for(int i=0;i<n;i++)
				adjlistlen[i]=0;
			
			cluster=new int[n];
			for(int i=0;i<n;i++)
				cluster[i]=-1;	
			this->n=n;
			C=0;
		}
		
	~Graph2(){
			delete Plist;
			delete cluster;
		}
	//assign value to adjlist
	//add v into adjlist[u] if and only if dist(u,v)<epsilon	
	void dbscanprepare(double epsilon){
			epsilon=epsilon*epsilon;
			for(int i=0;i<n;i++){
				for(int j=i+1;j<n;j++){
					if(squaredist(Plist[i],Plist[j])<epsilon){
						addpoint(adjlist[i],j);
						addpoint(adjlist[j],i);					
						adjlistlen[i]++;
						adjlistlen[j]++;

					}
				}
			}
			
	}	
	//breadth-first search, begin at point i
	//and do the recursion(not really recursion, but by using stack) on point u only if the adjlist[u] has size bigger than M
	void bfs(int i,int M){

			std::stack<int> s;
			cluster[i]=C;
			s.push(i);
			Adjlist *y;
			int a;		
			while(!s.empty()){
				a=s.top();
				s.pop();
				y=this->adjlist[a];
				while(y!=NULL){
					if(cluster[y->pointnumber]==-1){
						cluster[y->pointnumber]=C;
						if(adjlistlen[y->pointnumber]>=M)
							s.push(y->pointnumber);
					}
					y=y->next;
				}
			}
			
		}
	//Do the algorithm of dbscan
	//After running this function, the array cluster shows the result of dbscan
	void dbscan(int M,double epsilon){
			dbscanprepare(epsilon);
			for(int i=0;i<n;i++){
				if(cluster[i]==-1&&adjlistlen[i]>=M){
					{
					bfs(i,M);C++;
					}
				}
			}
		
		}
	//Next two function are used to find the (strong) connected components of G, we only remove the restriction M
	void bfs2(int i){
			std::stack<int> s;
			cluster[i]=C;
			s.push(i);
			Adjlist *y;
			int a;		
			while(!s.empty()){
				a=s.top();
				s.pop();
				y=this->adjlist[a];
				while(y!=NULL){
					if(cluster[y->pointnumber]==-1){
						if(adjlistlen[y->pointnumber]>0){
						cluster[y->pointnumber]=C;
						s.push(y->pointnumber);}
					}
					y=y->next;
				}
			}
			
	}
	void StrongConnectComp(double epsilon){
		dbscanprepare(epsilon);
		for(int i=0;i<n;i++){
			if(cluster[i]==-1&&adjlistlen[i]>0){
				bfs2(i);C++;
			}
		}
	}
	//print the value of point i
	void showpoint(int i){
			std::cout<<"Point "<<i<<": "<<Plist[i].x<<" "<<Plist[i].y<<std::endl;
		}
	//print all points
	void showallpoint(){
			for(int i=0;i<n;i++)
			 showpoint(i);

		}	
	//print points in adjlist[i]
	void showlist(int i){
	
			std::cout<<"Adjlist of point "<< i<<": ";
			Adjlist *s=adjlist[i];
			while(s!=NULL){
			std::cout << "-"<< s->pointnumber;
			s=s->next;
			}
			std::cout<<std::endl<<"lenth:"<<adjlistlen[i]<<std::endl;
		}	
	//print all adjlist[i]	
	void showall(){
			for(int i=0;i<n;i++)
				showlist(i);
		}
	//show the cluster
	void showcluster(){
		std::cout<<"Number of cluster: "<< C <<std::endl<<"Cluster of each point:"<<std::endl;
			for(int i=0;i<n;i++)
				std::cout<<cluster[i]<<" ";
			std::cout<<std::endl;
		}	
	//write the result of dbscan in a file	
	void myfilewriter(double epsilon,int M,char *filename){
		std::ofstream outfile;
		outfile.open(filename);
		outfile<<epsilon<<" "<<M<<" "<<C<<"\n";
		for(int i=0;i<n;i++){
		outfile<<Plist[i].x<<" "<<Plist[i].y<<" "<<cluster[i]<<"\n";
		}
		outfile.close();
		
		std::cout<<"The result of dbscan is saved in \""<<filename<<"\""<<std::endl;
	}
	//write the result of (strong) connected component in a file
	void myfilewriter(double epsilon,char *filename){
		std::ofstream outfile;
		outfile.open(filename);
		outfile<<epsilon<<" "<<C<<"\n";
		for(int i=0;i<n;i++){
		outfile<<Plist[i].x<<" "<<Plist[i].y<<" "<<cluster[i]<<"\n";
		}
		outfile.close();
		std::cout<<"The result of (strong) connected components is saved in \""<<filename<<"\""<<std::endl;
	}
	//write the data of this graph in a file (after creating a random graph)
	void myfilewriter(char *filename){
		std::ofstream outfile;
		outfile.open(filename);
		outfile<<n<<"\n";
		for(int i=0;i<n;i++){
		outfile<<Plist[i].x<<" "<<Plist[i].y<<"\n";
		}
		outfile.close();
		std::cout<<"The data of graph is saved in \""<<filename<<"\""<<std::endl;
	}
};

//Please see README
//You can remove writefile function if it is not needed
int main(int argc,char **argv){

    if(argc > 3 || argc == 1){
        std::cout << "wrong format for test" << std::endl;
        return 0;
    }
  
    int n_qst = argv[1][0] - '0';

    if(n_qst < 1 || n_qst > 4){
        std::cout << "no such question" << std::endl;
        return 0;
    }
     
    if(n_qst == 1){
        Data data = getData(argv[2]);
        Graph1 g(data.edges, data.n_node, data.n_edge);
        g.StrongConnectComp();
	std::cout<<"Number of strong connected components: "<< g.C <<std::endl;
	
	char filename1[]="result_scc1";
	g.writeclusterinfile(filename1);	
    }
    
    else if(n_qst == 2){
        std::cout<<"Enter the number of point: ";
        int n;
	fscanf(stdin,"%d",&n);
	double p;
	std::cout<<"Enter the probability: ";
	fscanf(stdin,"%lf",&p);
	if(p<0||p>1){
		std::cout<<"probability error\n";
		return 0;
	}
	Graph1 g(n,p);
	g.StrongConnectComp();
	std::cout<<std::endl;
	
	g.showalllist();//comment if not needed
	std::cout<<std::endl;
	g.showcluster();//comment if not needed

	char filename[]="Mydatarandom2";
	g.myfilewriter(filename);
	
	char filename2[]="result_scc2";
	g.writeclusterinfile(filename2);
    }
    
    else if (n_qst==3){
        std::cout<<"Enter the number of point: ";
        int n;
	fscanf(stdin,"%d",&n);
	
	double p;
	std::cout<<"Enter the side length L(all points are in plan(0,L)X(0,L)): ";
	fscanf(stdin,"%lf",&p);
	
	int M;double epsilon;
	std::cout<<"Enter the epsilon and M(enter a space between two value): ";
	fscanf(stdin,"%lf %d",&epsilon,&M);
	Graph2 g(n,p);

	std::cout<<std::endl<<"Epsilon: "<< epsilon<<" M: "<<M<<std::endl<<std::endl;
	g.dbscan(M,epsilon);
	
	
	
	char filename1[]="Mydatarandom3";
	g.myfilewriter(filename1);
	
//	g.showallpoint();//you can uncomment if needed
	std::cout<<std::endl;
//	g.showall();//you can uncomment if needed 
//	std::cout<<std::endl;
	std::cout<<"Number of cluster: "<< g.C <<std::endl;
//	g.showcluster();//you can uncomment if needed


	char filename2[]="result_dbscan3";
	g.myfilewriter(epsilon,M,filename2);

//uncomment this part, if you want to see connected components of the graph

/*	Graph2 g2(g.Plist,g.n)	;
	g2.StrongConnectComp(epsilon);
	std::cout<<"Number of (strong) connected components: "<< g2.C <<std::endl;
	char filename3[]="result_scc3";
	g2.myfilewriter(epsilon,filename3);
*/		
    }
    else if(n_qst==4){

    	Graph2 g(argv[2]);
    	int M;double epsilon;

	std::cout<<"Enter the epsilon and M(enter a space between two value): ";
	fscanf(stdin,"%lf %d",&epsilon,&M);
	
	g.dbscan(M,epsilon);
	std::cout<<std::endl<<"Epsilon: "<< epsilon<<" M: "<<M<<std::endl<<std::endl;
	std::cout<<"Number of cluster: "<< g.C <<std::endl;
	
	char filename[]="result_dbscan4";
	g.myfilewriter(epsilon,M,filename);

//uncomment this part, if you want to see connected components of the graph
/*		
	Graph2 g2(g.Plist,g.n)	;
	g2.StrongConnectComp(epsilon);
	std::cout<<"Number of (strong) connected components: "<< g2.C <<std::endl;
	char filename3[]="result_scc4";
	g2.myfilewriter(epsilon,filename3);	*/
    }
	return 0;
}
