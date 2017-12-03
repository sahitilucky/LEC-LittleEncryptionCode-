#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "decryptfuncs.h"
int poly[16];
int S_box[16];
int mulx2[16];
int mulx3[16];
int mulx[16];
int mul1[16];

//int tempx2[7];
//int tempx3[7];
//int tempx[16];
//int temp1[16];

int mule[16];
int mulb[16];
int muld[16];
int mul9[16];

int T0[16][4];
int T1[16][4];//look up tables
int T2[16][4];
int T3[16][4];
void mulandred();
int power(int a,int n);
//void MDS(int a1,int a2,int a3,int a4,int res[]); 
void mixcolumns(int a[4][2],int res[4][2]);

void decrypt(int input[4][2] ,int output[4][2],int Keys[8][8]){
	int i,j,k;	
	for(i=0;i<4;i++){
		input[i][0]=Keys[0][2*i]^input[i][0];
		input[i][1]=Keys[0][(2*i)+1]^input[i][1];
	}
	for(i=0;i<7;i++){
		int temp1=output[0][0];
		int temp2=output[0][1];
		int temp3=output[1][0];
		int temp4=output[1][1];
		//Permutation
		output[0][0]=output[2][0];
		output[1][0]=output[2][1];
		output[2][0]=output[3][0];
		output[3][0]=output[3][1];
		output[0][1]=temp1;
		output[1][1]=temp2;
		output[2][1]=temp3;
		output[3][1]=temp4;
		
		if(i!=0){
			mixcolumns(output,output);
		}		
		for(j=0;j<4;j++){
			if(i==0){
				output[j][0]=S_box[input[j][0]];
				output[j][1]=S_box[input[j][1]];
			}			
			else{
				output[j][0]=S_box[output[j][0]];
				output[j][1]=S_box[output[j][1]];
			}
		}
		for(j=0;j<4;j++){
			output[j][0]=Keys[i+1][2*j]^output[j][0];
			output[j][1]=Keys[i+1][(2*j)+1]^output[j][1];
		}
	
	}
	
}




void mulandred()//input for mul and red should be substitution matrix
{
	S_box[0]=8;
	S_box[1]=12;
	S_box[2]=1;
	S_box[3]=0;
	S_box[4]=14;
	S_box[5]=4;
	S_box[6]=15;
	S_box[7]=6;
	S_box[8]=10;
	S_box[9]=11;
	S_box[10]=13;
	S_box[11]=3;
	S_box[12]=5;
	S_box[13]=7;
	S_box[14]=9;
	S_box[15]=2;
	int x;int j;int i;
	int pol[16];
	for(i=0;i<16;i++)
		pol[i]=i;
	int tempx[7];
	int tempx2[7];
	int tempx3[7];
	int bin[4];
	for(i=0;i<16;i++)
	muld[i]=0;
for(i=0;i<16;i++)
	mule[i]=0;	
for(i=0;i<16;i++)
	mul9[i]=0;
for(i=0;i<16;i++)
	mulb[i]=0;
	
for(i=0;i<16;i++)
	mul1[i]=i;
for(i=0;i<16;i++)
	mulx[i]=0;	
for(i=0;i<16;i++)
	mulx2[i]=0;
for(i=0;i<16;i++)
	mulx3[i]=0;
		for(i=0;i<16;i++)
			{
				x=pol[i];
				for(j=3;j>-1;j--)
			{
				//printf("%d ",power(2,j));
				bin[j]=x/power(2,j);
				x=x%power(2,j);
				//i=i-bin[j]*power(2,j);
			}
			for(j=6;j>2;j--)
				tempx3[j]=bin[j-3];	
				tempx3[2]=0;
				tempx3[1]=0;
				tempx3[0]=0;	//left shifting
				
			for(j=5;j>1;j--)
				tempx2[j]=bin[j-2];	
				tempx2[6]=0;
				tempx2[1]=0;
				tempx2[0]=0;	//left shifting				
		    	
			for(j=4;j>0;j--)
				tempx[j]=bin[j-1];	
				tempx[5]=0;
				tempx[6]=0;
				tempx[0]=0;	//left shifting		
				
				int dectempx3=0;
				int dectempx2=0;
				int dectempx=0;
				
				for(j=6;j>-1;j--)
		{
			dectempx3=dectempx3+tempx3[j]*power(2,j);
			dectempx2=dectempx2+tempx2[j]*power(2,j);
			dectempx=dectempx+tempx[j]*power(2,j);
		}
				//reducing mulx3 		
	
		int pos;int quo;
	int dividend[7];
		for(j=0;j<7;j++)
			dividend[j]=tempx3[j];
		for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		int div[5];
		div[4]=1;
		div[3]=1;
		div[2]=0;
		div[1]=0;
		div[0]=1;
		//int quo;
		int pro[7];	
		int rem=dectempx3;
	while(quo>-1)
{				
		if(quo==2)
	{
			for(j=6;j>1;j--)
				pro[j]=div[j-2];
			pro[1]=0;
			pro[0]=0;
	}		
	   if(quo==1)
	  	{
	  		pro[6]=0;
			for(j=5;j>0;j--)
				pro[j]=div[j-1];
			//pro[1]=0;
			pro[0]=0;
		}
			
	if(quo==0)
	{
		pro[6]=0;
		pro[5]=0;
		
			for(j=4;j>-1;j--)
				pro[j]=div[j];
	}	
	//converting each into decimal pr,dividend
	int decpr=0;
	int decdiv=0;
	for(j=6;j>-1;j--)
		{
			decpr=decpr+pro[j]*power(2,j);
			decdiv=decdiv+dividend[j]*power(2,j);
		}
	 rem=decpr^decdiv;
	//int binrem[7];
	int s=rem;
	for(j=6;j>-1;j--)
	{
		dividend[j]=s/power(2,j);
				s=s%power(2,j);
	}
	for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		
}								
mulx3[i]=rem;//
	
	for(j=0;j<7;j++)
			dividend[j]=tempx2[j];
		for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		rem=dectempx2;
		//int quo;
		//int pro[7];	
	while(quo>-1)
{			
	
		
		
		if(quo==2)
	{
			for(j=6;j>1;j--)
				pro[j]=div[j-2];
			pro[1]=0;
			pro[0]=0;
	}		
	   if(quo==1)
	  	{
	  		pro[6]=0;
			for(j=5;j>0;j--)
				pro[j]=div[j-1];
			//pro[1]=0;
			pro[0]=0;
		}
			
	if(quo==0)
	{
		pro[6]=0;
		pro[5]=0;
		
			for(j=4;j>-1;j--)
				pro[j]=div[j];
	}	
	//converting each into decimal pr,dividend
	 int decpr=0;
	 int decdiv=0;
	for(j=6;j>-1;j--)
		{
			decpr=decpr+pro[j]*power(2,j);
			decdiv=decdiv+dividend[j]*power(2,j);
		}
	 rem=decpr^decdiv;
	//int binrem[7];
	int s=rem;
	for(j=6;j>-1;j--)
	{
		dividend[j]=s/power(2,j);
				s=s%power(2,j);
	}
	for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		
}								
mulx2[i]=rem;//
	
	for(j=0;j<7;j++)
			dividend[j]=tempx[j];
		for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		rem=dectempx;
	//	int quo;
	//	int pro[7];	
	while(quo>-1)
{			
	
		
		
		if(quo==2)
	{
			for(j=6;j>1;j--)
				pro[j]=div[j-2];
			pro[1]=0;
			pro[0]=0;
	}		
	   if(quo==1)
	  	{
	  		pro[6]=0;
			for(j=5;j>0;j--)
				pro[j]=div[j-1];
			//pro[1]=0;
			pro[0]=0;
		}
			
	if(quo==0)
	{
		pro[6]=0;
		pro[5]=0;
		
			for(j=4;j>-1;j--)
				pro[j]=div[j];
	}	
	//converting each into decimal pr,dividend
	 int decpr=0;
	 int decdiv=0;
	for(j=6;j>-1;j--)
		{
			decpr=decpr+pro[j]*power(2,j);
			decdiv=decdiv+dividend[j]*power(2,j);
		}
	 rem=decpr^decdiv;
	//int binrem[7];
	int s=rem;
	for(j=6;j>-1;j--)
	{
		dividend[j]=s/power(2,j);
				s=s%power(2,j);
	}
	for(j=6;j>-1;j--)
		{
			if(dividend[j]!=0)
			{
				pos=j;
				break;
			}
		}
		quo=pos-4;
		
}								
mulx[i]=rem;//
}
for(j=0;j<16;j++)
	{
		mule[j]=mulx3[j]^mulx2[j]^mulx[j];
		mulb[j]=mulx3[j]^mulx[j]^mul1[j];
		muld[j]=mulx3[j]^mulx2[j]^mul1[j];
		mul9[j]=mulx3[j]^mul1[j];
	}


		
	for(i=0;i<16;i++)
		{
			//for(j=0;j<4;j++)
			T0[i][0]=mule[i];
			T0[i][1]=mul9[i];
			T0[i][2]=muld[i];
			T0[i][3]=mulb[i];
			
			T1[i][0]=mulb[i];
			T1[i][1]=mule[i];
			T1[i][2]=mul9[i];
			T1[i][3]=muld[i];
			
			T2[i][0]=muld[i];
			T2[i][1]=mulb[i];
			T2[i][2]=mule[i];
			T2[i][3]=mul9[i];
			
			T3[i][0]=mul9[i];
			T3[i][1]=muld[i];
			T3[i][2]=mulb[i];
			T3[i][3]=mule[i];
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

void mixcolumns(int a[4][2],int res[4][2])//round ke is divided into  4 b 2 matrix
{
	int act1[4];int i;
	int act2[4];
	for(i=0;i<4;i++)
	{
	act1[i]=T0[a[0][0]][i]^T1[a[1][0]][i]^T2[a[2][0]][i]^T3[a[3][0]][i];
	act2[i]=T0[a[0][1]][i]^T1[a[1][1]][i]^T2[a[2][1]][i]^T3[a[3][1]][i];
	}
	for(i=0;i<4;i++)
	{
	res[i][0]=act1[i];
	res[i][1]=act2[i];
	}
}

