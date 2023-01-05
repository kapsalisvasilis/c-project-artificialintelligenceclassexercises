#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 5
#define poss 0.7  // diladi pithanotita ebodiou 70%

int LAVIR1[N][N],LAVIR2[N][N]; // LAVIR1=NxN pinakas poy exei 0 H 1
// LAVIR2=NxN pinakas me tis times (1, 2, 3, 4)
struct K
{ struct K *MA,*par;
  float kostos;
  int ver,hor;
};
struct K *s_top=NULL,*s_bottom=NULL;

int S_ver,S_hor,G1_ver,G1_hor,G2_ver,G2_hor;
float real(void)
{ return(rand()/((float)RAND_MAX+1));
}

int randominteger(int low,int high)
{   double m;
    int s;
    m=(double)rand()/ ((double)RAND_MAX +1);
    s=(int)(s*(high-low+1));
    return(low+m);
}

void initialize(void)
{ struct K *p=(struct K *)malloc(sizeof(struct K)); // p = S (struct)
  int x,y;
  float z;

  s_top=p;
  s_bottom=p;

  printf("Grapse tis sintetagmenes tou S:\n");
  scanf("%d %d",&S_ver,&S_hor);
  printf("Grapse tis sintetagmenes tou G1:\n");
  scanf("%d %d",&G1_ver,&G1_hor);
  printf("Grapse tis sintetagmenes tou G2:\n");
  scanf("%d %d",&G2_ver,&G2_hor);

  for(x=0;x<N;x++)
  { for(y=0;y<N;y++)
    { LAVIR1[x][y]=0;
      if((x!=S_ver || y!=S_hor) && (x!=G1_ver || y!=G1_hor) && (x!=G2_ver || y!=G2_hor)) // gia na MIN na mpei empodio se kapoia apo tis S,G1,G2
      { z=real();
        if(z < poss)
          LAVIR1[x][y]=1;
      }
    }
  }
  
  for(x=0;x<N;x++)
  { for(y=0;y<N;y++)
    { if(LAVIR1[x][y]==0)
      { LAVIR2[x][y]=randominteger(1,4);
      }
    }
  }

  p->MA=NULL;
  p->par=NULL;
  p->kostos=0;
  p->ver=S_ver;
  p->hor=S_hor;
}

void print(struct K *p)
{ int x,y;

  for(x=0;x<N;x++)
  { for(y=0;y<N;y++)
    { if(x==S_ver && y==S_hor)
       printf(" S/%d ",LAVIR2[x][y]);
      else if(x==G1_ver && y==G1_hor)
       printf(" A/%d ",LAVIR2[x][y]);
      else if(x==G2_ver && y==G2_hor)
       printf(" B/%d ",LAVIR2[x][y]);
      else if(LAVIR1[x][y]==1)
        printf(" X ");
      else
        printf("-/%d ",LAVIR2[x][y]);
     }
     printf("\n");
  }
}

struct K *extr(void)
{ float minimum,m;
  struct K *d,*wanted=s_top,*pass=s_top,*back=NULL;

  minimum=pass->kostos;

  while(pass!=NULL)  //sigrisi me ta "g" twn epomenon katastaseon 
  { d=pass;
    if(pass->MA!=NULL)
    { pass=pass->MA;
      m=pass->kostos;
      if(m<minimum)
      { minimum=m;
        wanted=pass;
        back=d;
      }
    }
    else
      break;
  }
  if(back==NULL)
   s_top=wanted->MA;
  else
   back->MA=wanted->MA;

  if(wanted->MA==NULL)
   s_bottom=back;
  wanted->MA=NULL;
  return(wanted);
}
int fin(struct K *p)
{ int apostasi1,apostasi2;

  apostasi1=abs(p->ver-G1_ver)+abs(p->hor-G1_hor);
  apostasi2=abs(p->ver-G2_ver)+abs(p->hor-G2_hor);

  if(apostasi1==0)
   return(1);
  if(apostasi2==0)
   return(1);

  return(0);
}
void route(struct K *p)
{ struct K *fin=p,*t;

  printf("Kostos anazitisis = %f\n",fin->kostos);

  while(p->par!=NULL)
  { t=p;
    p=p->par;
    p->MA=t;
  }

  printf("kaliteri diadromi: (%d,%d) -> ",p->ver,p->hor);
  while(t!=fin)
  { printf("(%d,%d) -> ",t->ver,t->hor);
    t=t->MA;
  }
  printf("(%d,%d)\n",t->ver,t->hor);
}

void make(struct K *p,int mv,int mh,float kos)
{ struct K *t;

  if(p->ver+mv >=0 && p->hor+mh >=0 && p->ver+mv <N && p->hor+mh <N)
  { if(LAVIR1[p->ver+mv][p->hor+mh]==0)
    { t=(struct K *)malloc(sizeof(struct K));
      t->ver=p->ver+mv;
      t->hor=p->hor+mh;
      t->kostos=p->kostos + abs(LAVIR2[p->ver][p->hor]-LAVIR2[t->ver][t->hor]) +kos;
      t->par=p;
      t->MA=NULL;
      if(s_top==NULL)
      { s_top=t;
        s_bottom=s_top;
      }
      else
      { s_bottom->MA=t;
        s_bottom=t;
      }
    }
  }
}

void new_K(struct K *p)  // ftiaxnei tous 8 geitones-paidia
{
  make(p,1,0,1);
  make(p,-1,0,1);
  make(p,0,1,1);
  make(p,0,-1,1);
  make(p,-1,-1,0.5);
  make(p,1,1,0.5);
  make(p,-1,1,0.5);
  make(p,1,-1,0.5);
}    

int main()
{ int extends=0,stopped=0,m;
  struct K *p;

  srand((int)time(NULL));
  initialize();
  print(s_top);

  do
  { if(s_top==NULL)
    { printf("UNSOLVED\n\n\n");
      break;
    }
    p=extr();
    if(fin(p)==1)
    { route(p);
      stopped=1;
    }
    else
    { extends++;
      new_K(p);
    }
  }
  while(stopped==0);
  printf("epektaseis %d\n\n\n",extends);
  scanf("%d",&m);
}
