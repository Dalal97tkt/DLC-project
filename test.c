#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void ordering(int vect[], int size)
 {
  int temp;
  for(int i=0;i<size;i++)
   {
    for(int j=i+1;j<size;j++)
     {
      if(vect[i]>vect[j])
       {
        temp=vect[i];
        vect[i]=vect[j];
        vect[j]=temp;
       }
      }
     printf("vect[%d]=%d\n",i,vect[i]);
    }
  }
int mediann( int vect[], int size)
 {
  int sum=0;
  for(int i=0;i<size;i++)
   {
    sum+=vect[i];
   }
  return sum/size;
 }  
void other_half( int vect[], int vect2[], int size)
 {
  for(int i=0;i<size/2;i++)
   {
    vect2[i]=vect[(size/2)+i];
   }
  }
void quartile( int quart[], int vect[], int size)
 {
  quart[1]=mediann(vect, size);
  quart[0]=mediann(vect, size/2);
  int vect2[size/2];
  other_half(vect, vect2, size);
  quart[2]=mediann(vect2, size/2);
  printf("quar1=%d\nquart2=%d\nquart3=%d\n",quart[0],quart[1],quart[2]);
 }
int main()
{
 int runs=2000;//change to 2000
 char command[50];
 int x=0;
  FILE *fp;
  int status, counter_2=0,counter_1=0;
  char path[1035];
  int vect[runs];
  int vect2[3];
 for(int i=0;i<runs;i++)//don't forget to change runs
 {
  strcpy( command, "gcc pr_gn2.c -lgmp -lm -o pr_gn2" );//use correct names
  system(command);
 

  fp = popen("./pr_gn2", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit;
  }
 //printf("testing\n");
  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    printf("%s", path);
  }
  vect[i]=atoi(path);
  printf("vect[%d]=%d\n",i,vect[i]);
  status = pclose(fp);
  if (status == -1) {
  printf("error\n");
    
  }

  
 }
 ordering(vect, 2000);
 quartile(vect2, vect, runs);
}
