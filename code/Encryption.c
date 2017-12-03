#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include "mixcolumns.h"

int main(int argc,char** argv){
	if(argc !=3){
		printf("Please give corect input format : ./encryption <filename>  <encrypt-filename>");
	} 
	time_t t;
	srand((unsigned) time(&t));
	FILE *ptr=fopen(argv[1],"r");
	FILE *ptr2=fopen(argv[2],"w");
	FILE *ptr3=fopen("round_keys.txt","w");
	if(ptr==NULL){
		printf("File doesn't exist");	
		exit(0);	
	}
	int i=0,j;
	char block[4];
	int input[4][2];
	int Keys[8][8];	
	int a =240,x;
	
	struct timespec start, finish;
    double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<8;i++){	
		/*		
		scanf("%d",&x);
		Keys[i][0]=(x & 15);
		Keys[i][1]=(x & 240)>>4;
		Keys[i][2]=(x & a<<4)>>8;
		Keys[i][3]=(x & a<<8)>>12;
		Keys[i][4]=(x & a<<12)>>16;
		Keys[i][5]=(x & a<<16)>>20;
		Keys[i][6]=(x & a<<20)>>24;
		Keys[i][7]=(x & a<<24)>>28;
		*/
		x=rand()%255;
		fprintf(ptr3,"%d\n",x);		
		Keys[i][0]=(x &15);
		Keys[i][1]=(x & 240)>>4;
		x=rand()%255;
		fprintf(ptr3,"%d\n",x);
		Keys[i][2]=(x &15);
		Keys[i][3]=(x & 240)>>4;
		x=rand()%255;
		fprintf(ptr3,"%d\n",x);
		Keys[i][4]=(x &15);
		Keys[i][5]=(x & 240)>>4;
		x=rand()%255;
		fprintf(ptr3,"%d\n",x);
		Keys[i][6]=(x &15);
		Keys[i][7]=(x & 240)>>4;
		
	}
	fflush(ptr3);
	fclose(ptr3);

	
	mulandred();	
	char outchar;
	i=0;
	while(!feof(ptr)){
		if(i%4==0 && i!=0){
			i=0;	
			encrypt(input,input,Keys);
			for(j=0;j<4;j++){			
				outchar=(char)((input[j][0]<< 4) +input[j][1]);
				//fprintf(ptr2,"%x%x",input[j][0],input[j][1]);
			
				fputc(outchar,ptr2);
			} 
			//break;			
		}
		block[i]=fgetc(ptr);
		if(!feof(ptr)){
			input[i][1]=(int)block[i] & 15;
			input[i][0]=((int)block[i] & 240)>>4;
			i++;
		}
	}
	if(i!=0){
		for(;i<4;i++){
			input[i][0]=0;input[i][1]=0;			
		}
		encrypt(input,input,Keys);
		for(j=0;j<4;j++){			
			outchar=(char)((input[j][0]<< 4) +input[j][1]);
			//fprintf(ptr2,"%x%x",input[j][0],input[j][1]);
		
			fputc(outchar,ptr2);
		} 
	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	fclose(ptr);
	fflush(ptr2);
	fclose(ptr2);
	
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf(" %f\n",elapsed);

}
