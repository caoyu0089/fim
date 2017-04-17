#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;
/*****Input program******/
# define N 40// number of items
# define M 1000 //umber of transactions
# define MinSup 0.20 
#define  nb_bats 100 // represents the number of bats on the given swarm
int IMAX=8;
typedef struct {int trans[N]; int nb;} ligne;
typedef struct {int solution[N]; float cost;} bat;
vector<ligne> dataset(M);
vector<bat> pop(nb_bats);
vector<bat> best_local(nb_bats);
vector<bat> bestlocal(nb_bats);
bat best_globale;
int vitesse[nb_bats];
/****************************prototype declaration of BAT********************/
void population_initialization(); // this function allows to initialize population
void generation(); // update the position of the particles 
void reproduction(); // reproduize the new population
float average_fitness(); // average fitness of the current population
/**************prototype declaration of ARM problem*******/
void read_trans();// this function allows to read the transactional data base et insert it into the dataset vector
float support_rule(int s[]); // this function calculates the support of the entire solution s
float support_antecedent(int s[]); // this function computes the support of the antecedent of the solution s
float confidence(int s[]); // it calculates the confidence of the rule
float fitness1(int s[]); // computes the fitness of a given solution s
void copy(int t[], int v[]); // it copies the vector t in the vector v
void display_dataset(); //this function allows to display the transactional data base
void display_solution(bat S); // this function display the current solution with its cost
void display_pop(); // display solutions

/****************************principal program*********/
int main(void)
{
int i,j;
float moy=0;

read_trans();
//long topcrono = System.currentTimeMillis();
//long x=topcrono;
	time_t start,end; 
	 clock_t t1,t2;
      //start = time(NULL);
     t1=clock();
     population_initialization();
     //display_pop();
     i=0;
     while(i<IMAX)
     {
     generation();
     //display_pop();
     //printf("\nthe next generation\n");
     //reproduction();
     i++;    
     }
     //printf("the average fitness is: %f", average_fitness());
     //display_pop();
     t2=clock();
      float diff ((float)t2-(float)t1);
	  float seconds = diff / CLOCKS_PER_SEC;
      printf("\npopsize is %d the average fitness is: %f", nb_bats, average_fitness());
      printf("\nthe exexution time is%f\n",seconds);
 int x; scanf("%d",&x);
 return 0;
}
/****************BAT*******/
void population_initialization()
{

for (int i=0;i<nb_bats;i++)
{
   for (int j=0;j<N;j++)
 {
  pop[i].solution[j]=rand()%2;
  bestlocal[i].solution[j]=pop[i].solution[j];
 }
 pop[i].cost=fitness1(pop[i].solution);
bestlocal[i].cost=pop[i].cost;
}

 for (int j=0;j<nb_bats;j++)
 {     
  vitesse[j]=rand()%N;
 }
}

/*********************************/
void best()
{
float cost=pop[0].cost;
int indice=0;   
for (int i=1; i<nb_bats;i++)
{
if (cost<pop[i].cost)
{
indice=i;
cost=pop[i].cost;                     
}        
}
for (int j=0;j<N;j++)
{
best_globale.solution[j]=pop[indice].solution[j];
}
best_globale.cost=pop[indice].cost; 
}
/********************************/
void generation()
{
 for (int i=0;i<nb_bats;i++)
  {
   int indice=vitesse[i];
   pop[i].solution[indice]=best_local[i].solution[indice];  
   indice=(indice*2)%N;
   pop[i].solution[indice]=best_globale.solution[indice];
   pop[i].cost=fitness1(pop[i].solution);
   if (best_local[i].cost<pop[i].cost)
     {
        for (int j=0;j<N;j++)
         {
          best_local[i].solution[j]=pop[i].solution[j];        
         }
       best_local[i].cost=pop[i].cost;                    
     } 
     
  }
  best();
   
}

void combinaison1(bat A, bat B, bat C)
{
int index; 
bool trouve=false;
 for (int i=0;i<N; i++)     
  {
  C.solution[i]=A.solution[i];
  if (B.solution[i]==1 && A.solution[i]==0 && trouve==false)
  {
  index=i;
  trouve=true;                     
  }        
  }
C.solution[index]=1;     
}
void combinaison2(bat A, bat B)
{
int index; 
bool trouve=false;
int i=0;
while (i<N && trouve==false)     
  {
       if (A.solution[i]==0 && B.solution[i]==1)  
             {
             trouve=true;
             index=i;               
             }
       i++;      
  }
  
A.solution[i]=1;

trouve=false;
i=0;
while (i<N && trouve==false)     
  {
       if (A.solution[i]==1)  
             {
             trouve=true;
             A.solution[i]=0;               
             }
       i++;      
  }
 for (i=0; i<N; i++)
 {
 B.solution[i]=A.solution[i];    
 }  
    B.cost=fitness1(B.solution);
       
}


float fitness1(int s[])
{
	return support_rule(s);
}

float average_fitness()
{
 float sum=0;
 for (int i=0;i<nb_bats;i++)
  {
       sum=sum+pop[i].cost;                  
  }          
  return(sum/nb_bats);   
}
/***********ARM problem*****************/
/*******************display dataset***********************/
void display_dataset()
{
  for (vector<ligne>::size_type i = 0; i < dataset.size(); i++)
    {
                               
              for ( int j=0;j<dataset[i].nb;j++)                
              printf ("%d ",dataset[i].trans[j]);
              printf("\n");
   }    
}
/**********************copry t in v********/
void copy(int t[], int v[])
{
for (int i=0;i<N; i++)
{
v[i]=t[i]; 
}     
}
/**************************display_solution*****************/
void display_solution(particle S)
{
for (int i=0;i<N;i++)
{
    printf("%d ", S.solution[i]);
}
//printf ("cost is:%f",S.cost);
printf("\n");
}
/*******read transactional data bass and insert it in the data set structure********************************/
void read_trans()
{
int c ;
int val;
char t[10];
int j;
int i=0;
int l=0;
FILE *f=NULL;
f=fopen("C://dataset.txt","r");
if (f!=NULL) {printf("the file is succefully opened\n");} 
j=0;
while (c!=EOF)
{
   
        c=fgetc(f); 
        if (c==' ') {t[j]='\0';dataset[i].trans[l]=atoi(t);l++;/*printf("\n\t%d",val);*/j=0;}
        if (c=='\n'){dataset[i].nb=l;l=0;i++;j=0;}
        if (c!=' ' && c!='\n'){t[j]=c;j++;}
}    
printf("\n");
fclose(f); 
}
/*************************compute the support of the solution s**********/
float support_rule(int s[])
{
float compt=0;
		for (int i=0; i<M; i++)
		{
		bool appartient=true;
		
		int j=0;
		while (j<N)
		{
		 if (s[j]!=0)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
				if (dataset[i].trans[l]==j+1)
				{existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
       //printf("support rule is:%f\n",compt);
       compt=compt/M;

	return compt;
}
/*****************************support antecedent computing*****************************/
float support_antecedent(int s[])
	{
		float compt=0;
		
		for (int i=0; i<M; i++)
		{
		bool appartient=true;
		int j=0;
		while (j<N)
		{
		 if (s[j]==1)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
					if (dataset[i].trans[l]==j+1)
				        {existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
		 //printf("support antecedente is:%f\n",compt);
	   compt=compt/M;
	//if(compt!=0)System.out.println("antecedent"+compt);
	   return compt;
}
/****************************condifence computing**************************/
float confidence(int s[])
{
	float conf=0;
	if (support_antecedent(s)!=0)
	conf=support_rule(s)/support_antecedent(s);
	else conf=0;
	//nf=support_rule(sol);
	//if (conf==1){display_sol_SAT(s);}
	//if (conf==0){conf=1;}
    return conf;
}
/***********************evaluation of the solution s******/

bool verify(int t[], int v[])
{
bool exist=true;
int i=0;
while (i<N && exist==true)
{
  if (t[i]!=v[i]){exist=false;}
  i++;    
}
return exist;  
}
/*****************************display T_dance************/
void display_pop()
{
//f=fopen("C://Users//Dzcomp//Desktop//benchmark//dataset.txt","a");
//if (f==NULL) {printf("the file is succefully opened\n");} 
for (int i=0;i<nb_particules;i++)
{
    for (int j=0;j<N;j++)
    {
    printf ("%d-",pop[i].solution[j]);   
    }
    printf (" %f\n",pop[i].cost); 
}
/*printf("\ncrossover step \n");
for (int i=0;i<pop_size;i++)
{
    for (int j=0;j<N;j++)
    {
    printf ("%d-",pop[i].solution[j]);   
    }
    printf (" %f\n",new_pop[i].cost); 
}
*/
}
