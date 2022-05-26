#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// a function to calculate the size of the file
//this function retun the number of characters +1 inside the file
int file_size(FILE*fp)
{
  int size;

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  return size;
}
//noise
double gauss(void)
{
  static double x0, x1;
  static unsigned int nb_ready;

  double u, v, w, z;

  if (nb_ready == 0)
  {
    u = ((double) rand()) / RAND_MAX;
    v = ((double) rand()) / RAND_MAX;
    w = sqrt(-2.0 * log(u));
    z = 2 * M_PI * v;
    x0 = w * cos(z);
    x1 = w * sin(z);

    nb_ready = 1;
    return x0;
  }
  else
  {
    nb_ready = 0;
    return x1;
  }
}
void add_noise(double vect[], int size)
 {
  for(int i=0;i<size-1;i++)
   {
    vect[i]=vect[i]+gauss();
   }
  }
void CRT (mpz_t a_x, int S_xx[], int eq_x[], int size )
{
  //here size is the cardinal of S_x
  mpz_t N,L,x,y,g,rop1,rop2;
  mpz_inits(N,L,x,y,g,rop1,rop2,NULL);
  mpz_set_ui(N,1);

  //N=product of all elements in S_x
  for(int i=0;i<size-1;i++)
  {
    //gmp_printf("calculating(%d)- N=%Zd\n",i,N);
    mpz_mul_ui(N,N,S_xx[i]);
  }
  //calculating a_x
  //la boucle va jusqu'a size-1 car on a ajouté "2" à S_p et à S_q mais on n'a aucune congruence pour 2
  for(int i=0;i<size-1;i++)
  {
    //printf("eq_x[%d]=%d\n",i,eq_x[i]);
    mpz_cdiv_q_ui(L,N,S_xx[i]);
    mpz_set_ui(rop1,S_xx[i]);
    mpz_gcdext(g,x,y,L,rop1);
    //gmp_printf("extended_gcd(%Zd,%d,x,y)=%Zd\n",L,S_xx[i],x);
    mpz_mul(rop2,L,x);
    mpz_mul_ui(rop2,rop2,eq_x[i]);
    mpz_add(a_x,a_x,rop2);
    mpz_mod(a_x,a_x,N);
    //printf("testing the a_x\n");
   // gmp_printf("a_x=%Zd\n",a_x);
  }
}
//calculating the coefficient of correlation
double Coef(double x[], double y[], int n)
     {
     double r,xx[n],xy[n],yy[n],nr=0,dr_1=0,dr_2=0,dr_3=0,dr=0;
     float sum_y=0,sum_yy=0,sum_xy=0,sum_x=0,sum_xx=0;
     for(int i=0;i<n;i++)
     {
     for(i=0;i<n;i++)
        {
         xx[i]=x[i]*x[i];
         yy[i]=y[i]*y[i];
        }
        for(i=0;i<n;i++)
        {
         sum_x+=x[i];
         sum_y+=y[i];
         sum_xx+= xx[i];
         sum_yy+=yy[i];
         sum_xy+= x[i]*y[i];     
        }
        nr=(n*sum_xy)-(sum_x*sum_y);
        float sum_x2=sum_x*sum_x;
        float sum_y2=sum_y*sum_y;
        dr_1=(n*sum_xx)-sum_x2;
        dr_2=(n*sum_yy)-sum_y2;
        dr_3=dr_1*dr_2;
        dr=sqrt(dr_3);
        r=(nr/dr);
      }
       return r;
       
     }

void ChooseVect(double vect[], int n, int index_j, double temp[], int k)
 {
  int jump=index_j;
  for(int i=0; i<n;i++)
   {
    temp[i]=vect[jump];
    jump+=k;
  }
 } 
//this function works perfectly, it gives back the index of the element with the highest value 
int ChooseMax(double vect[],int threshhold)
 {
  double max=-1000;
  int index=0;
  for(int i=0;i<threshhold;i++)
   {
    if(max<vect[i])
     {
      max=vect[i];
      index=i;
     }
    //printf("rho\n");
   }
  return index;
 }
void attack(int primes[], int k, double vect[],int n, mpz_t z_x)
 {
  double m[n];
  int candidate[k];
  unsigned long int  box;
  double  temp[n+1];
  double rho[primes[k-1]-1];
  mpz_t bla,z_box;
 mpz_inits(bla,z_box,NULL);
  for(int j=1;j<k;j++)
   {
    
   for(int h=1;h<primes[j];h++)
     {
     
      for(int i=0;i<n;i++)
       {
        box=(h-(n-i-1)*2 );
        mpz_set_ui(z_box,box);
        m[i]=(mpz_hamdist (z_box, bla) )%primes[j];
        
       
        }
        ChooseVect(vect,n, j, temp, k);
        //printf("problem in coefffor?\n");
        rho[h]=Coef( m, temp,n);
        //printf("rho[%d]=%f\n",h,rho[h]);
     }
      //printf("problem in choose max\n");
      candidate[j]=ChooseMax(rho,primes[j]);
      
      }

   mpz_clears(bla,z_box,NULL);
   
   CRT (z_x,primes, candidate,k);
 }
int main()
 {
 FILE *f1, *f2;
 int k=54,u;//k can be changed
  int primes[k];

  //printf("test1\n");
  f1=fopen("traces_p","r");

  if(!f1)
    printf("Erreur d'ouverture du fichier p\n");


  f2=fopen("traces_q","r");

  if(!f2)
    printf("Erreur d'ouverture du fichier q\n");
  //printf("test2\n");
  int size_p=file_size(f1), size_q=file_size(f2);
  int n_p=((size_p/2)-1)/(k-1),n_q=((size_q/2)-1)/(k-1);
  //printf("size_p=%d, k=%d, n_p=%d\n",size_p,k,n_p);
  double vect_p[size_p/2],vect_q[size_q/2];

  for(int i=0;i<size_p/2;i++)
   {
    fscanf(f1,"%le",&vect_p[i]);
   
    //printf("vect_p[%d]=%d\n",i,vect_p[i]);//this is just for testing 
   }
   add_noise(vect_p, size_p/2);
  for(int i=0;i<size_q/2;i++)
   {
     fscanf(f2,"%le",&vect_q[i]);
   }
   add_noise(vect_q,size_q/2);


  //printf("test3\n");
  fclose(f1);
  fclose(f2);
  //printf("test4\n");
  mpz_t p,q,z_p,z_q;
  mpz_inits(p,q,z_p,z_q,NULL);
 
//generating the primes
for(int i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
	}


//printf("test5\n");
attack(primes, k, vect_q,n_p,z_p);
gmp_printf("size_p'=%ld\n",mpz_sizeinbase(z_p,2));
//printf("test6\n");
attack(primes, k, vect_p,n_q,z_q);
//gmp_printf("q'=%Zd\n",z_q);
  mpz_clears(p,q,z_p,z_q,NULL);
}
  
