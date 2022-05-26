#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <sys/types.h>
#include <sys/resource.h>

//testing the divisibilty by all the items in the array, return 1 if n is divisble by anyone of the k primes 
int test_with_array(mpz_t z_n[],mpz_t n,int k,int arr[],FILE *fp)
 {
     mpz_t bla;
  mpz_init(bla);
  //int cntr=0;
  int res=0;
  for(int i=0;i<k;i++)
   {
    mpz_mod_ui(z_n[i],n,arr[i]);
    fprintf(fp,"%ld%s",mpz_hamdist (z_n[i], bla)," ");
    	res++;
   if(res==k)
    {
     res=0;
    }
    else
    {
    res=1;
    }
   }
      mpz_clear(bla);
  return res;
 }

//adding 2
void addingtwo(mpz_t z_n[],mpz_t n, int k, int arr[])
 {

   for(int i=0;i<k;i++)
    {
     mpz_add_ui(z_n[i],z_n[i],2);

     mpz_mod_ui(z_n[i],z_n[i],arr[i]);
    }
   mpz_add_ui(n,n,2);

 }

//the optimized sieve function
int optimized_sieve(mpz_t q, int k, gmp_randstate_t generator, int arr[], int b, int t,FILE *fp)
 {
  mpz_t a,z_n[k]; //declaring the variable n
  mpz_init(a); //initializing the variable n
  int cntr=0;
  for(int i=0;i<k;i++)
   {
    mpz_t z_n[i];
   }
  for(int i=0;i<k;i++)
   {
    mpz_init(z_n[i]);
   }
  
  mpz_ui_pow_ui (a, 2, b-1);
  do
  {
   mpz_urandomb (q, generator ,b-1 ); //generate an integer n of at most k bits
   mpz_add(q,q,a);
  }while(mpz_even_p (q ));
  while(((mpz_probab_prime_p(q,t)==0))&&(test_with_array(z_n,q,k,arr,fp)!=0))
   {
    addingtwo(z_n,q,k,arr);
    cntr++;
   }
   
   mpz_clear(a);
   for(int i=0;i<k;i++)
   {
    mpz_clear(z_n[i]);
   }
   return cntr;
 }
 

 
int main()
 {
  int b=512,k=54,t=5,u;
  mpz_t n,p,q,l;
  mpz_inits(n,p,q,l,NULL);
  int primes[k];
  FILE *f,*g;

	for(int i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		//printf("%d",u);
		mpz_set(q,p);
	}

  //the random generator 
  gmp_randstate_t generator ;//declaring a generator
  gmp_randinit_default(generator); // Initializing the generator
  gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed   
  //the results
  f=fopen("traces_p","w+");
	if(f==NULL){
		printf("Can't open  \n");
	}
	printf("%d\n",optimized_sieve(n,k,generator,primes,b, t,f));
	
	fclose(f);
 	//printf("testing q\n");
 	g=fopen("traces_q","w+");
	//g=fopen("traces_de_q.txt","w+");
	if(g==NULL){
		printf("can't open \n");
	}
	optimized_sieve(n,k,generator,primes,b, t,g);
	

  
  gmp_randclear (generator);//clearing the generator
  mpz_clears(l,p,q,n,NULL);
  return 0;
  }

