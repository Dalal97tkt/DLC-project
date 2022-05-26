#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int runs=1000;
	char command[50];
	int counter_2=0,counter_1=0;
	for(int i=0;i<runs;i++)// runs changes according to our needs  
	{
		printf("itération %d \n",i);
		strcpy(command, "gcc crible.c -lgmp -lm -o crible" );
		system(command);
		strcpy(command, "./crible");
		system(command);
		strcpy(command, "gcc attaque.c -lgmp -lm -o attaque" );
		system(command);
		//strcpy(command, "./attaque");
		//system(command);

		FILE *fp;
		char path[1000];

		fp = popen("./attaque", "r");
		if (fp == NULL) {
    		printf("Failed to run command\n" );
    		exit;
		}
		/* Read the output a line at a time - output it. */
		while (fgets(path, sizeof(path), fp) != NULL) {
			printf("%s", path);
		}

		if(atoi(path)==2)
		{
			counter_2++;
		}
		else if(atoi(path)==1)
		{
			counter_1++;
		}
	}
	float x=1000;
	printf("proba >512|  %f\n",counter_1/x);
	printf("proba >553|  %f\n",counter_2/x);
}