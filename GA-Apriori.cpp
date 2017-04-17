/*********************************************************Readme**************************
This program implements GAApriori approach that combines Apriori and the genetic algorithm for solving FIM (Frequent 
Itemsets Mining) problem, it employs Apriori heuristics in exploring the solution's space by the genetic mechanism. 
We use the genetic algorithm in the exploration step. It exisits some basic functions like: 
initialization, crossover, mutation and reproduction for the GA. We also developed other intelligent functions such as 
Iinitialization, Icrossover, and Imutation for GA-Apriori algorithm. There is other functions for FIM problem, 
such as fitness1, support, confidence. Finally, there is some functions to manage input and output data like: 
copy, read_trans, display_dataset, display_pop and display_solution.

This  program is developed by Dr. Djenouri Youcef  and supervised by Pr. Marco Comuzzi under the project 
funded by UNIST University, Ulsan, South Korea 
Contact: y.djenouri@gmail.com 
**************************************************************************************************/
/********************************************Improtant Note:********************************************
 Since the size() of a vector is an int, which is a 32-bit signed integer, 
 the maximum number of elements is 2**31-1 or roughly 2.1 billion elements. Thus, I suggest who want to use this program, 
 that respect this constraint, that means the maximum number of transactions that could be handled are 
 2.1 billion transactions with limited number of items. Otherwise, 
 you have to use more appropriate structure for handling with big transactional database
 *******************************************************************************************************/
 

#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;
/*****Input program******/
# define N 40 // number of items
# define M 1000 //number of transactions
# define MinSup 0.20 // minimum support constraint
int pop_size=100; // represents the number of individuals on the given population
int IMAX=8; // represents the maximum number of iterations
typedef struct {int trans[N]; int nb;} ligne; // structure that reprensets the transaction
typedef struct {int solution[N]; float cost;} indiv; // structure that represents one solution in the solution's space
vector<ligne> dataset(M); // represents the transactional database
vector<indiv> pop(pop_size); // represents the current population
vector<indiv> new_pop(pop_size); // represents the new population
/****************************prototype declaration of genetic algorithm********************/
void population_initialization(); // this function allows to initialize population
void Ipopulation_initialization(); // this function allows to initialize population intelligently using Apriori Heuristic
void crossover(); // crossover the individiduals 
void Icrossover(); // crossover the individiduals intelligently using Apriori Heuristic 
void mutation(); // mutate the individuals
void Imutation(); //  mutate the individuals intelligently using Apriori Heuristic
void reproduction(); // reproduce the new population from the current population
float average_fitness(); // average fitness of the current population
/**************prototype declaration of ARM problem*******/
void read_trans();// this function allows to read the transactional database et inserts it into the dataset vector
float support_rule(int s[]); // this function calculates the support of the entire solution s
float support_antecedent(int s[]); // this function computes the support of the antecedent of the solution s
float confidence(int s[]); // it calculates the confidence of the rule
float fitness1(int s[]); // computes the fitness of a given solution s
void copy(int t[], int v[]); // it copies the vector t in the vector v
void display_dataset(); //this function allows to display the transactional database
void display_solution(indiv S); // this function display the current solution with its cost
void display_pop(); // display solutions

/****************************principal program*********/
int main(void)
{
int i,j;
float moy=0;

read_trans();
//long topcrono = System.currentTimeMillis(); To compute the runtime 
//long x=topcrono;
	time_t start,end; 
	 clock_t t1,t2;
      //start = time(NULL);
     t1=clock();
     Ipopulation_initialization();
     //display_pop();
     i=0;
     while(i<IMAX)
     {
     Icrossover();
     Imutation();
     //display_pop();
     //printf("\nthe next generation\n");
     reproduction();
     i++;    
     }
     //printf("the average fitness is: %f", average_fitness());
     //display_pop();
     t2=clock();
      float diff ((float)t2-(float)t1);
	  float seconds = diff / CLOCKS_PER_SEC;
      printf("\npopsize is %d the average fitness is: %f", pop_size, average_fitness());
      printf("\nthe execution time is%f\n",seconds);
 int x; scanf("%d",&x);
 return 0;
}
/****************Genetic algorithm*******/
void population_initialization()
{

for (int i=0;i<pop_size;i++)
{
   for (int j=0;j<N;j++)
 {
  pop[i].solution[j]=rand()%2;
 }
}
}

void Ipopulation_initialization()
{

for (int i=0;i<pop_size;i++)
{
    int index=rand()%N;
    pop[i].solution[index]=1;
   for (int j=0;j<N && j!=index;j++)
 {     
  pop[i].solution[j]=0;
 }
}
}


void crossover()
{

 for (int i=0; i<pop_size-1;i=i+2)
  {
          int pc=rand()%N;        
   for (int j=0; j<pc;j++)
    {
      new_pop[i].solution[j]=pop[i].solution[j];
      new_pop[i+1].solution[j]=pop[i+1].solution[j];       
    } 
    for (int j=pc; j<N;j++)
    {
      new_pop[i].solution[j]=pop[i+1].solution[j];
      new_pop[i+1].solution[j]=pop[i].solution[j];       
    }  
    new_pop[i].cost=fitness1(new_pop[i].solution);    
    new_pop[i+1].cost=fitness1(new_pop[i+1].solution);    
  }     
}

void Icrossover()
{
int index1, index2;
 for (int i=0; i<pop_size-1;i=i+2)
  { 
          bool trouve1=false, trouve2=false;     
   for (int j=0; j<N;j++)
    {
      new_pop[i].solution[j]=pop[i].solution[j];
      if (pop[i].solution[j]!=0 && trouve1==false)
      {
      trouve1=true; 
      index1=j;                          
      }
      new_pop[i+1].solution[j]=pop[i+1].solution[j];  
      if (pop[i+1].solution[j]!=0 && trouve2==false)
      {
      trouve2=true; 
      index2=j;                          
      }     
    } 

      new_pop[i].solution[index2]=1;
      new_pop[i+1].solution[index1]=1;       
  
    new_pop[i].cost=fitness1(new_pop[i].solution);    
    new_pop[i+1].cost=fitness1(new_pop[i+1].solution);    
  }     
}

void mutation()
{
for (int i=0; i<pop_size;i=i+1)
  {
   int pm=rand()%N;        
    new_pop[i].solution[pm]=rand()%3;     
    new_pop[i].cost=fitness1(new_pop[i].solution);
   }        
}

void Imutation()
{
for (int i=0; i<pop_size;i=i+1)
  {
         
   if (new_pop[i].cost < MinSup) 
   { 
           bool trouve1=false, trouve2=false;            
           for (int j=0; j< N && (trouve1==false || trouve2==false); j++)
            {
            if (new_pop[i].solution[j]==1 && trouve1==false) 
            {
             new_pop[i].solution[j]==0;
             trouve1=true;                       
            }     
            
            if (new_pop[i].solution[j]==0 && trouve2==true) 
            {
             new_pop[i].solution[j]==1;
             trouve2=true;                       
            }     
            }            
   }            
    new_pop[i].cost=fitness1(new_pop[i].solution);
   }        
}

void reproduction()
{
for (int j=0;j<pop_size;j++)
{   
float min=pop[0].cost;
int min_i=0;
  for (int i=1;i<pop_size;i++)
  {
     if(pop[i].cost<min)
      {
       min=pop[i].cost;
       min_i=i;                  
      }
  }          
  if (pop[min_i].cost<new_pop[j].cost)
  {
  for (int l=0;l<N; l++)
  {
  pop[min_i].solution[l]=new_pop[j].solution[l];
  }
  pop[min_i].cost=new_pop[j].cost;                                    
  }
}
}

float fitness1(int s[])
{
	return support_rule(s);
}

float average_fitness()
{
 float sum=0;
 for (int i=0;i<pop_size;i++)
  {
       sum=sum+pop[i].cost;                  
  }          
  return(sum/pop_size);   
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
void display_solution(indiv S)
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
f=fopen("C://dataset.txt","r"); // this operation allow to recuperate the dataset
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
for (int i=0;i<pop_size;i++)
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
