#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mixcolumns.h"
int pol[16];
int mul2[16];
int mul3[16];
int mul1[16];
int T0[16][4];
int T1[16][4];//look up tables
int T2[16][4];
int T3[16][4];
int S_box[16];

void mulandred();
int power(int a,int n);
//void MDS(int a1,int a2,int a3,int a4,int res[]); 

/*
int main()
{
	/* this part is just for testing
	int i,j;
	int plaintext[4][2];
	plaintext[0][0]=14;
	plaintext[1][0]=2;
	plaintext[2][0]=4;
	plaintext[3][0]=5;

	plaintext[0][1]=3;
	plaintext[1][1]=4;
	plaintext[2][1]=1;
	plaintext[3][1]=6;

	for(i=0;i<16;i++)
		poly[i]=i;// a matrix
	for(i=0;i<16;i++)
		mul1[i]=i;
	for(i=0;i<16;i++)
		mul2[i]=0;
	for(i=0;i<16;i++)
		mul3[i]=0;
	mulandred(poly);///mul and generates lookup tables
	int res[4][2];
	//MDS(14,1,3,3,res);
	mixcolumns(plaintext,res);
	
	return 0;
}
*/
void encrypt(int input[4][2] ,int output[4][2],int Keys[8][8]){
	int i,j,k;	
	for(i=0;i<4;i++){
		input[i][0]=Keys[0][2*i]^input[i][0];
		input[i][1]=Keys[0][(2*i)+1]^input[i][1];
	}
	for(i=0;i<7;i++){
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
		
		if(i!=6){
			mixcolumns(output,output);
			
		}	
		int temp1=output[2][1];
		int temp2=output[3][1];
		int temp3=output[2][0];
		int temp4=output[3][0];
		//Permutation
		output[2][0]=output[0][0];
		output[2][1]=output[1][0];
		output[3][0]=temp3;
		output[3][1]=temp4;
		output[0][0]=output[0][1];
		output[0][1]=output[1][1];
		output[1][0]=temp1;
		output[1][1]=temp2;
				
		for(j=0;j<4;j++){
			output[j][0]=Keys[i+1][2*j]^output[j][0];
			output[j][1]=Keys[i+1][(2*j)+1]^output[j][1];
		}
		
	}
	
}
void mulandred()//input for mul and red should be substitution matrix
{
	
	//change by sahiti
	S_box[0]=3;
	S_box[1]=2;
	S_box[2]=15;
	S_box[3]=11;
	S_box[4]=5;
	S_box[5]=12;
	S_box[6]=7;
	S_box[7]=13;
	S_box[8]=0;
	S_box[9]=14;
	S_box[10]=8;
	S_box[11]=9;
	S_box[12]=1;
	S_box[13]=10;
	S_box[14]=4;
	S_box[15]=6;
	int i;int k;int x;
	
	for(i=0;i<16;i++)
		pol[i]=i;// a matrix
	for(i=0;i<16;i++)
		mul1[i]=i;
	for(i=0;i<16;i++)
		mul2[i]=0;
	for(i=0;i<16;i++)
		mul3[i]=0;
	/////
	for(k=0;k<16;k++)
	{
		//int quo;
		//int rem;
		int b;
		x=pol[k];int j;int bin[4];
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
	
	for(i=0;i<16;i++)
		{
			//for(j=0;j<4;j++)
			T0[pol[i]][0]=mul2[i];
			T0[pol[i]][1]=pol[i];
			T0[pol[i]][2]=pol[i];
			T0[pol[i]][3]=mul3[i];
			
			T1[pol[i]][0]=mul3[i];
			T1[pol[i]][1]=mul2[i];
			T1[pol[i]][2]=pol[i];
			T1[pol[i]][3]=pol[i];
			
			T2[pol[i]][0]=pol[i];
			T2[pol[i]][1]=mul3[i];
			T2[pol[i]][2]=mul2[i];
			T2[pol[i]][3]=pol[i];
			
			T3[pol[i]][0]=pol[i];
			T3[pol[i]][1]=pol[i];
			T3[pol[i]][2]=mul3[i];
			T3[pol[i]][3]=mul2[i];
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

void mixcolumns(int a[4][2],int res[4][2])//round key is divided into  4 by 2 matrix
{
	int act1[4];int i;
	int act2[4];
	//actual code	
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
	/*
	for(i=0;i<4;i++)
	{
		res[i][0]=T0[a[0][0]][i]^T1[a[1][0]][i]^T2[a[2][0]][i]^T3[a[3][0]][i];
		res[i][1]=T0[a[0][1]][i]^T1[a[1][1]][i]^T2[a[2][1]][i]^T3[a[3][1]][i];
	}
	*/
}

