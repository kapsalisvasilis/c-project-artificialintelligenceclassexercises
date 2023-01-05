#include <stdio.h>
#include <stdlib.h>

#define M 4
#define N 4
#define K 3
//EXOUME VALEI MIKROTERES TIMES STA M,N,K LOGO MNIMIS!!!
#define MAX 1
#define MIN -1
#define NO_WIN 0
#define END 100
#define CONTINUE 200


struct minmax
{  struct minmax *childs[N];
   struct minmax *best_move;
   int free_cells,drop_cells[N];
   char matrix[M][N];
};

int check_game(char matrix[M][N], char c, int drop_cells[N])
{  int i,j,y,flag;

   i=0;
   j=M-1;
   y=0;

   while (i < N) {
     if (i + K <= N) {
       while (drop_cells[i] < j) {
         flag = 0;
         while (y < K) {
           if (matrix[j][i + y] != c)
           {
             flag = 1;
             break;
           }
           y++;
         }
         if (flag == 0)
           return (END);
         j = j - 1;
       }
     }
     i++;
   }


   i=0;
   j = M -1;
   y=0;
   while (i < N) {
     while (drop_cells[i] < j) {
       if (j >= K - 1) {
         flag = 0;
         while (y < K) {
           if (matrix[j - y][i] != c) {
             flag = 1;
             break;
           }
           y++;
         }
         if (flag == 0)
           return (END);
       }
       j = j - 1;
     }
     i++;
   }

   i=0;
   j=M-1;
   y=0;

   while (i < 0) {
     while (drop_cells[i] < j) {
       if (j + K <= M && i + K <= N) {
         flag = 0;
         while (y < K) {
           if (matrix[j + y][i + y] != c) {
             flag = 1;
             break;
           }
           y++;
         }
         if (flag == 0)
           return (END);
       }
       j = j - 1;
     }
     i++;
   }

   i=0;
   j=M-1;
   y=0;

   while(i<N)
   {
     while(drop_cells[i]<j)
     {
       if (j>=K-1 && i+K<=N)
       {
         flag=0;
         while(y<K){
           if(matrix[j-y][i+y]!=c)
           {
             flag=1;
             break;
           }
           y++;
         }
         if(flag==0)
             return (END);
       }
       j = j - 1;
     }
     i++;
   }

   i=0;
   j=M-1;
   y=0;

   while (i < N) {
     while (drop_cells[i] < j) {
       if (j >= K - 1 && i + K <= N) {
         flag = 0;
         while (y < K) {
           if (matrix[j - y][i + y] != c) {
             flag = 1;
             break;
           }
           y++;
         }
         if (flag == 0)
           return (END);
       }
       j = j - 1;
     }
     i++;
   }
   
   return (CONTINUE);
}

void print_matrix(char A[M][N])
{  int i,j;
   for(i=0;i<M;i++)
   {  for(j=0;j<N;j++)
      {  printf("%c ",A[i][j]);
      }
      printf("\n");
   }
   printf("\n\n");
}

void copy1(int X[N],int Y[N])
{  int i;
   for(i=0;i<N;i++)
     Y[i]=X[i];

}

void copy2(char X[M][N],char Y[M][N])
{  int i,j;
   for(i=0;i<M;i++)
     for(j=0;j<N;j++)
        X[i][j]=Y[i][j];

}


int DENTRO_MINMAX(struct minmax *status,int seira);

int make_children(struct minmax *status,char c,int next,int test[N],struct minmax *n[N])
{  int position=0,j;

   for(j=0;j<N;j++)
   {  if (status->drop_cells[j]>=0)
      {  status->childs[position]= (struct minmax *)malloc(sizeof(struct minmax));
         status->childs[position]->free_cells= status->free_cells-1;
         copy2(status->matrix,status->childs[position]->matrix);
         status->childs[position]->matrix[status->drop_cells[j]][j]=c;
         copy1(status->drop_cells,status->childs[position]->drop_cells);
         status->childs[position]->drop_cells[j]= status->drop_cells[j]-1;
         test[position]=DENTRO_MINMAX(status->childs[position],next); // kaleitai i dfs gia na ftiaxei ta dika tou paidia
         n[position]=status->childs[position];
         position++;
      }
   }

   return position;
}

int DENTRO_MINMAX(struct minmax *status,int seira)
{  int next;
   int position=0;
   struct minmax *n[N];
   int test[N];
   int megisti_timi;
   struct minmax *best;
   int i;
   char c;

   if(seira==MAX && check_game(status->matrix,'O',status->drop_cells)==END) //paizei o MAX kai exei dimiourgidei K 'O'
   {
	  return (MIN);   //(-1) afou kerdizei o min 
   }   
   else if(seira==MIN && check_game(status->matrix,'X',status->drop_cells)==END) // tha epaize o MIN kai exei dimiourgidei K-ada 'X'
   {
      return (MAX);   //(+1) afou kerdizei o max
   }
   else if(status->free_cells==0)   
   {  
      return (NO_WIN);   // isopalia (0)
   }
   if(seira==MAX)
   {  next=MIN;
      c='X';
   }
   else
   {  next=MAX;
      c='O';
   }
   position= make_children(status,c,next,test,n);

   if(seira==MAX)  // an eimai sto epipedo MAX, krataw ti megisti timi apo auta kai sto best to kalitero paidi
   {  megisti_timi=test[0];
      best=n[0];
      for(i=0;i<position;i++)
      {  if(test[i]>megisti_timi)
         {  megisti_timi=test[i];
            best=n[i];
         }
      }
   }
   else  // an eimai sto epipedo MIN, krataw ti megisti timi apo auta kai sto best to kalitero paidi
   {  megisti_timi=test[0];
      best=n[0];
      for(i=1;i<position;i++)
      {  if(test[i]<megisti_timi)
         {  megisti_timi=test[i];
            best=n[i];
         }
      }
   }
   status->best_move=best;
   return megisti_timi;
}


int main()
{  struct minmax *state=(struct minmax *)malloc(sizeof(struct minmax));
   int seira=MAX,i,j,grammi,stili;
   char haraktiras;

   state->free_cells=M*N;
   for(i=0;i<M;i++)
   {  for(j=0;j<N;j++)
      {   state->matrix[i][j]='-'; // pavla = keni 8esi 
      }
   }
   for(j=0;j<N;j++)
   {   state->drop_cells[j]=M-1;
   }

   printf("enarksi paixnidiou \n\n");
   print_matrix(state->matrix);
   

   while(1)
   {  if(seira==MAX && check_game(state->matrix,'O',state->drop_cells)==END)
      {  printf("nikises \n\n\n");
         break;
      }
      else if(seira==MIN && check_game(state->matrix,'X',state->drop_cells)==END)
      {  printf("nikise to cpu \n\n\n");
         break;
      }
      else if(state->free_cells==0)
      {  printf("isopalia \n\n\n");
         break;
      }
	  

      if(seira==MAX) //paizei o MAX 
      {  printf("cpu paizei \n\n");
         DENTRO_MINMAX(state,MAX); // dfs gia na ginei i veltisti kinisi
         state=state->best_move;
         print_matrix(state->matrix);
         seira=MIN;
      }
	  else  //paizei o MIN
      {  printf("paizeis \n\n");

         printf("epelekse stili: 1 eos %d ? \n",N);  // na patisei apo 1 ews N
         scanf("%d",&stili);

         printf("\n");

         state->matrix[state->drop_cells[stili-1]][stili-1]= 'O';
         state->drop_cells[stili-1]= state->drop_cells[stili-1]-1;
         state->free_cells= state->free_cells-1;

         print_matrix(state->matrix);
         seira=MAX;
      }
   }
}
