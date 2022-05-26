#include<stdio.h>
#include"gmp.h"
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/resource.h>
#include<unistd.h>

unsigned long int cputime()
{
	struct rusage rus;
	getrusage (0, &rus);
	return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}


int nombre_k_bits(mpz_t n,int b, gmp_randstate_t mon_generateur){
	mpz_t z_n;
	mpz_inits(z_n,NULL);
	mpz_ui_pow_ui(z_n,2,b-1);
	mpz_urandomb(n, mon_generateur, b-1);
	mpz_add(n,n,z_n);
	return 1;

}


int crible_simple(int b,int k, int t, mpz_t n, gmp_randstate_t mon_generateur, int primes[], FILE* f)
{

    do
    {
        nombre_k_bits(n, b, mon_generateur);
    } while (mpz_divisible_ui_p(n, 2) != 0);
    int isPrime = 1;
    do
    {
        isPrime = 1;
        int i;
        for (i = 1; i < k; i++)
        {
        	//tests de divisions de n par les k premiers nombres premiers
			//gmp_printf("n=%Zd \n",n);
			//printf("prime[%d]=%d \n",i,primes[i]);
			//printf("D_%d \n",i);
            if (mpz_divisible_ui_p(n, primes[i]) != 0)
            {
                isPrime = 0;
                mpz_add_ui(n, n, 2);
                //printf("G \n"); //G = q=q+2 et retour a la ligne 2
                fputc('B',f);
                break;
            }
            //printf("I \n");//incrémentation
            fputc('A',f);
        }

        if (isPrime)
        {
        	//gmp_printf("T[%Zd} \n",n);
        	fputc('C',f);
			if(mpz_probab_prime_p(n,t)==0){
				mpz_add_ui(n,n,2);
				fputc('B',f);
				isPrime=0;
			}
			else{
				isPrime=1;
			}
        }
    } while (!isPrime);


    return isPrime;
}

int main(int argc, char * argv[])
{
	int r,b,k,t,u;
	int seed,i,j,rep;
	mpz_t n,p,q;
	mpz_t N;
	mpz_inits(n,N,p,q,NULL);
	seed = time(NULL);

	FILE* f;
	FILE* g;
	FILE* val_N;



	//ce programme genere 2 nombres : p et q 
	r=2;
	//de taille exactement 512 bits chacun, puis de 1024 bits chacun
	b=1024;
	//non divisible par les k premiers nombres premiers
	//on aura trois cas à étudier pour b=512 (T1, k = 54), (T2, k = 60), (T3, k = 70)
	//on aura trois cas à étudier pour b=1024 (T1, k = 100), (T2, k = 110), (T3, k = 120)
	k=120;
	//avant d’etre testé par la méthode de Miller-Rabin avec t bases
	t=20;
	

	int primes[k];

	for(i=0;i<k;i++){
		mpz_nextprime(p,q);
		u=mpz_get_ui(p);
		primes[i]=u;
		mpz_set(q,p);
	}

	gmp_randstate_t mon_generateur; //Declaration du générateur
	gmp_randinit_default(mon_generateur); //Initialisation du génératuer
	gmp_randseed_ui(mon_generateur, seed);

	
	mpz_set_ui(N,1);
	long int starttime = cputime();

	f=fopen("traces_de_p.txt","w+");
	if(f==NULL){
		printf("Erreur d'ouverture du fichier \n");
	}
	rep=crible_simple(b,k,t,p,mon_generateur,primes,f);
	fclose(f);

	g=fopen("traces_de_q.txt","w+");
	if(g==NULL){
		printf("Erreur d'ouverture du fichier \n");
	}
	rep=crible_simple(b,k,t,q,mon_generateur,primes,g);
	fclose(g);

	mpz_mul(N,p,q);
	//gmp_printf("p = %Zd \n",p);
	//gmp_printf("q = %Zd \n",q);
	//gmp_printf("N = %Zd \n",N);

	//N est une valeur publique, donc on l'écrit dans un fichier pour s'en servir lors de l'attaque
	val_N=fopen("valeur_de_N=p.q.txt","w+");
	if(val_N==NULL){
		printf("Erreur d'ouverture du fichier \n");
	}
	mpz_out_str(val_N, 10, N);
	fclose(val_N);

	long int endtime = cputime();
	//printf("temps d'execution : %ld milli-secondes \n", endtime - starttime);

	gmp_randclear(mon_generateur);
    mpz_clears(n,N,p,q, NULL);
    
}
