#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "time.h"
#include "decryptfuncs.h"
int hextodecimal(char c);
int main(int argc,char** argv){
	if(argc !=3){
		printf("Please give corect input format : ./encryption <encrypt-filename>  <decrypt-filename>");
	} 
	time_t t;
	srand((unsigned) time(&t));
	FILE *ptr=fopen(argv[1],"r");
	FILE *ptr2=fopen(argv[2],"w");
	FILE *ptr3=fopen("round_keys.txt","r");	
	if(ptr==NULL){
		printf("No such file exixting");
		exit(0);
	}
	
	char block[4];
	int input[4][2];
	
	int Keys[8][8];	
	char* line=malloc(sizeof(char)*30);
	int read;
	int len=81;
	int i=7,j=0;	
	while(((read = getline(&line, &len, ptr3)) != -1)){
		Keys[i][j]=atoi(line) & 15;
		j++;		
		Keys[i][j]=(atoi(line)&240)>>4;
		j++;
		if(j==8){
			j=0;i--;
		}
	}
	fclose(ptr3);
	mulandred();	
	char outchar;
	i=0;
	int cametoend=0;
	while(!feof(ptr)){
		if(i%4==0 && i!=0){
			i=0;
			block[i]=fgetc(ptr);	
			if(feof(ptr)){
				cametoend=1;			
			}			
			decrypt(input,input,Keys);
			for(j=0;j<4;j++){			
				outchar=(char)((input[j][0]<< 4) +input[j][1]);
				if(!(cametoend==1 && (int)outchar==0)){				
					fprintf(ptr2,"%c",outchar);
				}
				//fputc(outchar,ptr2);
			} 			
		}
		else{
			block[i]=fgetc(ptr);
		}		
		//input[i][0]=hextodecimal(block[i]);
		//block[i]=fgetc(ptr);
		//input[i][1]=hextodecimal(block[i]);
		int x=(int)block[i];
		if(x<0){x=x+256;}
		input[i][1]=(x) &15;
		input[i][0]=(x & 240)>>4;		
		i++;
	}
	fclose(ptr);
	fflush(ptr2);
	fclose(ptr2);
}

int hextodecimal(char c){
	int val;
	switch(c)  
        {  
            case '0':  
                val = 0;  
                break;  
            case '1':  
                val = 1;  
                break;  
            case '2':  
                val = 2;  
                break;  
            case '3':  
                val = 3;  
                break;  
            case '4':  
                val = 4;  
                break;  
            case '5':  
                val = 5;  
                break;  
            case '6':  
                val = 6;  
                break;  
            case '7':  
                val = 7;  
                break;  
            case '8':  
                val = 8;  
                break;  
            case '9':  
                val = 9;  
                break;  
            case 'a':  
            case 'A':  
                val = 10;  
                break;  
            case 'b':  
            case 'B':  
                val = 11;  
                break;  
            case 'c':  
            case 'C':  
                val = 12;  
                break;  
            case 'd':  
            case 'D':  
                val = 13;  
                break;  
            case 'e':  
            case 'E':  
                val = 14;  
                break;  
            case 'f':  
            case 'F':  
                val = 15;  
                break;  
	}
	return val;
}

