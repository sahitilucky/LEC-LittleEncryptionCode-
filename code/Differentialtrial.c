#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int poly[16];
int mul2[16];
int mul3[16];
int mul1[16];
double* powers;

void mulandred();
int power(int a,int n);
void MDS(int a1,int a2,int a3,int a4,int* res);
void calbias(int** Lintab,int* sblks_to_consider,double totalprop,int noofvars,int noofrounds,int** Path);
void dosubtitute(int** Lintab,int* sblks_to_consider,int indextosubtitute,double totalprop,int noofvars,int noofrounds,int** Path);
double max_propagation;
int** MaxPath;
int globalcount;
int main(){
	FILE *ptr=fopen("Differentialapproxtable.txt","r");
	int i,j,k;
	int** Lintab=(int**)malloc(sizeof(int*)*16);
	for(i=0;i<16;i++){
		Lintab[i]=(int*)malloc(sizeof(int)*16);
	}
	//Linapproxtab from file
	i=0;
	char* line=malloc(sizeof(char)*30);
	char* token=malloc(sizeof(char)*30);
	int read,len=81;
	while(((read = getline(&line, &len, ptr)) != -1)){
 		
		token = strtok(line," ");
		j=0;
		while( token!=NULL) {
      							
				Lintab[i][j]=atoi(token); 
				token = strtok(NULL," ");
				j++;
   		}
   		i++;
	}
	int max_noof_rounds=7;
	///////////////////Preproccesing
	powers=(double*)malloc(sizeof(double)*56);
	for(i=0;i<56;i++){
		powers[i]=pow(2,i);
	}
	mulandred();
	/////////////////

	MaxPath=(int**)malloc(sizeof(int*)*32);
	int** Path=(int**)malloc(sizeof(int*)*32);
	for(i=0;i<32;i++){
		Path[i]=(int*)malloc(sizeof(int)*8);
		MaxPath[i]=(int*)malloc(sizeof(int)*8);
	}		
	globalcount=0;
	int noofvars,noofrounds;
	max_propagation=0;
	double totalprop;
	int* sblks_to_consider;
	

	for(i=0;i<8;i++){
		noofvars=0;
		totalprop=1;
		sblks_to_consider=(int*)calloc(16,sizeof(int));
		for(j=1;j<16;j++){
			sblks_to_consider=(int*)calloc(16,sizeof(int));
			sblks_to_consider[i]=j;
			noofvars=0;
			noofrounds=0;
			for(k=0;k<8;k++){
				Path[(2*noofrounds)][k]=sblks_to_consider[k];
			}
			totalprop=1.0;
			calbias(Lintab,sblks_to_consider,totalprop,noofvars,noofrounds,Path);
			printf("Done with inner loop : %d\n",j);
		}
		printf("Done with outer loop: %d\n",i);
	}
	printf("Final max bias:%.60f  %d\n",max_propagation,globalcount);
	for(i=0;i<32;i++){
		for(j=0;j<8;j++){
			printf("%d ",MaxPath[i][j]);
		}
		printf("\n");
	}
}

void calbias(int** Lintab,int* sblks_to_consider,double totalprop,int noofvars,int noofrounds,int** Path){
	if(noofrounds==6){
		globalcount++;
		//double temp=totalprop*powers[noofvars-1];
		int i,j;
		
		if(globalcount%2==0){
			if(totalprop==(double)0){printf("How is it happening!!!"); }
			printf("global count:	%d  %.60f %d\n",globalcount,totalprop ,noofvars);
			
		}
		if(fabs(totalprop) > fabs(max_propagation)){
			//printf("%lf %d -what\n",totalprop,globalcount);
			for(i=0;i<32;i++){
				for(j=0;j<8;j++){
					MaxPath[i][j]=Path[i][j];
				}
				//MaxPath[i]=(int *)memcpy((void*)MaxPath[i],(void*)Path[i],8*(sizeof(int)));
			}
			max_propagation=totalprop;
		}	
		return;
	}
	else{
		dosubtitute(Lintab,sblks_to_consider,0,totalprop,noofvars,noofrounds+1,Path);
	}
}



//Comlpetes one round subtitution,Linear transformation,permutation 
void dosubtitute(int** Lintab,int* sblks_to_consider,int indextosubtitute,double totalprop,int noofvars,int noofrounds,int** Path){
	if(indextosubtitute==8){
		//int *original=(int*)malloc(sizeof(int)*8);
		//original=(int *)memcpy((void*)(Path[2*noofrounds]),(void*)sblks_to_consider,8*sizeof(int));
		int* output1=(int*)malloc(sizeof(int)*4);
		int* output2=(int*)malloc(sizeof(int)*4);
		int i;
		for(i=0;i<8;i++){
			Path[(2*noofrounds)-1][i]=sblks_to_consider[i];
		}
		//Path[(2*noofrounds)-1]=(int *)memcpy((void*)(Path[2*noofrounds-1]),(void*)sblks_to_consider,8*sizeof(int));
		//MDS 
		MDS(sblks_to_consider[0],sblks_to_consider[2],sblks_to_consider[4],sblks_to_consider[6],output1);
		MDS(sblks_to_consider[1],sblks_to_consider[3],sblks_to_consider[5],sblks_to_consider[7],output2);
		
		//Permutation
		//int *temp=(int*)malloc(sizeof(int)*4);
		for(i=0;i<8;i++){
			if(i<4){
				//temp[i]=sblks_to_consider[i];
				sblks_to_consider[i]=output2[i];
			}
			else{
				sblks_to_consider[i]=output1[i-4];
			}
		}
		free(output1);
		free(output2);
		
		for(i=0;i<8;i++){
			Path[(2*noofrounds)][i]=sblks_to_consider[i];
		}
		calbias(Lintab,sblks_to_consider,totalprop,noofvars,noofrounds,Path);
		
		for(i=0;i<8;i++){
			sblks_to_consider[i]=Path[(2*noofrounds)-1][i];
		}
		return;
	}
	else{
		int a1;
		if(sblks_to_consider[indextosubtitute]!=0){
			int temp=sblks_to_consider[indextosubtitute];
			int twos=0,max_index,heuristic=1;
			for(a1=0;a1<16;a1++){
				double temp2=(double)(((double)Lintab[sblks_to_consider[indextosubtitute]][a1])/(double)16);
				if(temp2>=0.25 || (temp2 == 0.125 && twos<heuristic)){
					if(temp2==0.125){twos++;}
					sblks_to_consider[indextosubtitute]=a1;
					if(totalprop==(double)0){printf("How is it happening!!!"); }
					temp2=totalprop*temp2;
					//temp3=temp2*powers[noofvars-1];
					if((temp2)<(max_propagation) ){
						sblks_to_consider[indextosubtitute]=temp;
						continue;	
					}
					//if(temp2<1){printf("How is it happening!!! %f \n",temp2);}
					dosubtitute(Lintab,sblks_to_consider,indextosubtitute+1,(temp2),noofvars+1,noofrounds,Path);
					sblks_to_consider[indextosubtitute]=temp;
				}

			}
			sblks_to_consider[indextosubtitute]=temp;
		}
		else{
			dosubtitute(Lintab,sblks_to_consider,indextosubtitute+1,totalprop,noofvars,noofrounds,Path);
		}
		return;
	} 	

}

/////////////////////////////////////////////////////////////////////
////////////////////////////For MDS//////////////////////////////////
/////////////////////////////////////////////////////////////////////
void mulandred()
{
	int i;int k;int x;
	for(i=0;i<16;i++)
		poly[i]=i;
	for(i=0;i<16;i++)
		mul1[i]=i;
	for(i=0;i<16;i++)
		mul2[i]=0;
	for(i=0;i<16;i++)
		mul3[i]=0;

	
	for(k=0;k<16;k++)
	{
		//int quo;
		//int rem;
		int b;
		x=k;int j;int bin[4];
		for(j=3;j>-1;j--)
			{
				//printf("%d ",power(2,j));
				bin[j]=x/power(2,j);
				x=x%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			int binmul[5];
		binmul[0]=0;
		int redpol[5];
		redpol[4]=1;
		redpol[3]=1;
		redpol[2]=0;
		redpol[1]=0;
		redpol[0]=1;
		int binred[4];
		for(i=1;i<5;i++)
			binmul[i]=bin[i-1];//multiply by 2
			if(binmul[4]!=0)
			{
				for(i=0;i<4;i++)	
					binred[i]=binmul[i]-redpol[i];//red by polynomial
			}
			else
				for(i=0;i<4;i++)
					binred[i]=binmul[i];
					
					
				for(i=0;i<4;i++)
				{	if(binred[i]==-1)
						mul2[k]=mul2[k]+(power(2,i));//red by 2
					else
						mul2[k]=mul2[k]+binred[i]*power(2,i);
				}
			
				for(i=0;i<4;i++)
					binmul[i]=bin[i]+binmul[i];//mul with 3
					
					for(i=0;i<5;i++)
						if(binmul[i]==2)
							binmul[i]=0;//red by 2
							
				if(binmul[4]!=0)
			{
				for(i=0;i<4;i++)	
					binred[i]=binmul[i]-redpol[i];//red by polynomial
			}
			else
				for(i=0;i<4;i++)
					binred[i]=binmul[i];
					for(i=0;i<4;i++)
				{	if(binred[i]==-1)
						mul3[k]=mul3[k]+power(2,i);//red by 2
					else
						mul3[k]=mul3[k]+power(2,i)*binred[i];
				}
								
					
	}
}
int power(int a,int n)
{
int i;
int ret=1;
for(i=0;i<n;i++)
	{
		ret=ret*a;
	}
	return ret;
}
void MDS(int a1,int a2,int a3,int a4,int* res)
{
	int bin4[4];
	int bin3[4];
	int bin2[4];
	int bin1[4];
	int x4=mul1[a4];
	int x3=mul1[a3];
	int x2=mul3[a2];
	int x1=mul2[a1];
	int i,j;
		for(j=3;j>-1;j--)
			{
				bin4[j]=x4/power(2,j);
				x4=x4%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		for(j=3;j>-1;j--)
			{
				bin3[j]=x3/power(2,j);
				x3=x3%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			for(j=3;j>-1;j--)
			{
				bin2[j]=x2/power(2,j);
				x2=x2%power(2,j);
				//i=i-bin[j]*power(2,j);
			}	
			for(j=3;j>-1;j--)
			{
				bin1[j]=x1/power(2,j);
				x1=x1%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			int binadd[4];
			for(i=0;i<4;i++)
		{
				//binadd[i]=0;
			
				binadd[i]=(bin4[i]+bin3[i]+bin2[i]+bin1[i])%2;//red by 2
		}
		res[0]=0;
		res[1]=0;
		res[2]=0;
		res[3]=0;
		for(i=0;i<4;i++)
				{	
						res[0]=res[0]+binadd[i]*power(2,i);
				}
				
				
				
			 x4=mul1[a4];
	 x3=mul3[a3];
	 x2=mul2[a2];
	 x1=mul1[a1];
	//int i,j;
		for(j=3;j>-1;j--)
			{
				bin4[j]=x4/power(2,j);
				x4=x4%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		for(j=3;j>-1;j--)
			{
				bin3[j]=x3/power(2,j);
				x3=x3%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			for(j=3;j>-1;j--)
			{
				bin2[j]=x2/power(2,j);
				x2=x2%power(2,j);
				//i=i-bin[j]*power(2,j);
			}	
			for(j=3;j>-1;j--)
			{
				bin1[j]=x1/power(2,j);
				x1=x1%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		//	int binadd[4];
			for(i=0;i<4;i++)
		{
				//binadd[i]=0;
			
				binadd[i]=(bin4[i]+bin3[i]+bin2[i]+bin1[i])%2;
		}
		//res[0]=0;
		//res[1]=0;
		//res[2]=0;
		//res[3]=0;
		for(i=0;i<4;i++)
				{	
						res[1]=res[1]+binadd[i]*power(2,i);
				}
				
				
				 x4=mul3[a4];
	x3=mul2[a3];
	 x2=mul1[a2];
	 x1=mul1[a1];
	//int i,j;
		for(j=3;j>-1;j--)
			{
				bin4[j]=x4/power(2,j);
				x4=x4%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		for(j=3;j>-1;j--)
			{
				bin3[j]=x3/power(2,j);
				x3=x3%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			for(j=3;j>-1;j--)
			{
				bin2[j]=x2/power(2,j);
				x2=x2%power(2,j);
				//i=i-bin[j]*power(2,j);
			}	
			for(j=3;j>-1;j--)
			{
				bin1[j]=x1/power(2,j);
				x1=x1%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		//	int binadd[4];
			for(i=0;i<4;i++)
		{
				//binadd[i]=0;
			
				binadd[i]=(bin4[i]+bin3[i]+bin2[i]+bin1[i])%2;
		}
		//res[0]=0;
		//res[1]=0;
		//res[2]=0;
		//res[3]=0;
		for(i=0;i<4;i++)
				{	
						res[2]=res[2]+binadd[i]*power(2,i);
				}
				
				
				
				
				 x4=mul2[a4];
	x3=mul1[a3];
	 x2=mul1[a2];
	 x1=mul3[a1];
	//int i,j;
		for(j=3;j>-1;j--)
			{
				bin4[j]=x4/power(2,j);
				x4=x4%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
		for(j=3;j>-1;j--)
			{
				bin3[j]=x3/power(2,j);
				x3=x3%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			for(j=3;j>-1;j--)
			{
				bin2[j]=x2/power(2,j);
				x2=x2%power(2,j);
				//i=i-bin[j]*power(2,j);
			}	
			for(j=3;j>-1;j--)
			{
				bin1[j]=x1/power(2,j);
				x1=x1%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			//int binadd[4];
			for(i=0;i<4;i++)
		{
				//binadd[i]=0;
			
				binadd[i]=(bin4[i]+bin3[i]+bin2[i]+bin1[i])%2;
		}
	//	res[0]=0;
	//	res[1]=0;
		//res[2]=0;
		//res[3]=0;
		for(i=0;i<4;i++)
				{	
						res[3]=res[3]+binadd[i]*power(2,i);
				}
	}

