#include <stdio.h>
#include <stdlib.h>

int xorwithin(int x){
	int i,acc;
	acc=x;
	for(i=1;i<4;i++){
		acc=acc ^ (x<<i);
		//acc=acc & 15;
		
	}

	acc=(acc & 8);
	acc=acc >> 3;
	return acc;
}


int main()
{
		
	FILE *ptr=fopen("S_boxnumbers2.txt","r");
	int len=81,i,x=0,j,k;
	char* line=malloc(sizeof(char)*30);
	char* token=malloc(sizeof(char)*30);
	int* xes=(int*)malloc(sizeof(int)*16);
	int* yes=(int*)malloc(sizeof(int)*16);
	int read;
	while(((read = getline(&line, &len, ptr)) != -1)){
 		
		token = strtok(line," ");
		i=0;
		while( i<2) {
      							
				if(i==0){
					xes[x]=atoi(token);
				}
				else{
					yes[x]=atoi(token);
				} 
				token = strtok(NULL," ");
				i++;
   		}
		x++;
		
	}
	int check=0,temp1,temp2,temp3;
	
	int** linappmatrix=(int**)malloc(sizeof(int*)*16);
	for(i=0;i<16;i++){
		linappmatrix[i]=(int*)malloc(sizeof(int)*16);
		for(j=0;j<16;j++){
			linappmatrix[i][j]=0;
			for(k=0;k<16;k++){
				
				check=xorwithin((i & xes[k]) ^ (j & yes[k])); 
				temp1=(i & xes[k]);
				temp2=(j & yes[k]);
				temp3=temp1^ temp2;
				

				if(xorwithin((i & xes[k]) ^ (j & yes[k]))==0){
					
					linappmatrix[i][j]++;
				}
			}		
		}
	}
		
	FILE *fp = fopen ("Linearapproxtable.txt", "w+");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			fprintf(fp,"%d ",linappmatrix[i][j]);

		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	
}

