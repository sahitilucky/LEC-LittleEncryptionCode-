#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
//int X[16];
int Y[16];
int prop_ratio[16][16];
int i,j;
FILE* fout;
FILE* fin;
for(i=0;i<15;i++)
	for(j=0;j<15;j++)
		prop_ratio[i][j]=0;
fin=fopen("actinp.txt","r");
char* buf=malloc(sizeof(char)*10);
int len=3;
int count1=0;
int st;
while(st=getline(&buf,&len,fin)!=-1)
{
	Y[count1]=atoi(buf);
	count1++;
}
int x,y;
int xstar[16];
int xdash;
int s[16];
int sstar[16];


for(x=0;x<16;x++)
	
{
	for(xdash=0;xdash<16;xdash++)
			xstar[xdash]=x^xdash;
	for(i=0;i<16;i++)
{
	s[i]=Y[i];	
	sstar[i]=Y[xstar[i]];
	s[i]=s[i]^sstar[i];
}
int count=0;
 	for(y=0;y<16;y++)
 	{
 		count=0;
 		for(i=0;i<16;i++)
 		if(s[i]==y)
 			count=count+1;
 		prop_ratio[x][y]=count;	
 	}
} 	
fout=fopen("actout.txt","w");
for(i=0;i<16;i++)
{
	for(j=0;j<16;j++)
	{
		fprintf(fout,"%d",prop_ratio[i][j]);
		fprintf(fout," ");
	}
	fprintf(fout,"\n");
}
return 0;
}
