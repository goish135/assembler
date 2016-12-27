#include<iostream>
#include<fstream>//使用到fstream的類別
#include<string>
#include<string.h>
#include<cstring>
#include<sstream>
#include<stdlib.h>//atoi() 字串轉數字 
using namespace std;
#define SIZE 100 
#include <iomanip> //setw(寬度)
char lable[100][100];
char instruction[100][100];
char symbol[100][100];
void chk_label();
void chk_opcode();
int Loc[100];//new
int toDec(int hex){
	int b,c,d;//b 除數 c 商數 d 餘數
	int i=1,ans=0;
	c = hex/10;//printf("%d cccccc\n",c);
	d = hex%10;//printf("%d dddddd\n",d);
	while(c!=0){
		ans+=d*i;
		//printf("%d d1\n",d);
		//printf("%d i1\n",i);
		//printf("%d ans\n",ans);
		i*=16; //printf("%d i2\n",i);
		d=c%10; //printf("%d d2\n",d);
		c/=10;  //printf("%d c2\n",c);
		if(c==0) {
			ans+=d*i;
			break;
		}
	}
	return ans; 
}
struct optab
{
	char code[10],objcode[10];
}MyOptab[59]={
{"ADD","18"},
{"ADDF","58"},
{"ADDR","90"},
{"AND","40"},
{"CLEAR","B4"},
{"COMP","28"},
{"COMPF","88"},
{"COMPR","A0"},
{"DIV","24"},
{"DIVF","64"},
{"DIVR","9C"},
{"FIX","C4"},
{"FLOAT","C0"},
{"HIO","F4"},
{"J","3C"},
{"JEQ","30"},
{"JGT","34"},
{"JLT","38"},
{"JSUB","48"},
{"LDA","00"},
{"LDB","68"},
{"LDCH","50"},
{"LDF","70"},
{"LDL","08"},
{"LDS","6C"},
{"LDT","74"},
{"LDX","04"}, 
{"LPS","D0"},
{"MUL","20"},
{"MULF","60"},
{"MULR","98"},
{"NORM","C8"},
{"OR","44"},
{"RD","D8"},
{"RMO","AC"},
{"RSUB","4C"},
{"SHIFTL","A4"},
{"SHIFTR","A8"},
{"SIO","F0"},
{"SSK","EC"},
{"STA","0C"},
{"STB","78"},
{"STCH","54"},
{"STF","80"},
{"STI","D4"},
{"STL","14"},
{"STS","7C"},
{"STSW","E8"},
{"STT","84"},
{"STX","10"},
{"SUB","1C"},
{"SUBF","5C"},
{"SUBR","94"},
{"SVC","B0"},
{"TD","E0"},
{"TIO","F8"},
{"TIX","2C"},
{"TIXR","B8"},
{"WD","DC"},
};
struct symtab{
	char symbol[100];
	int addr;
}mysymtab[100];//10-->100
int startaddr,locctr,symcount=0,length;
char programName[10];
char line[SIZE];
int row=0;
void Pass1()
{

	if(!strcmp(instruction[row],"START"))
	{
		startaddr=atoi(symbol[row]);
		locctr=toDec(startaddr);
		strcpy(programName,lable[row]);
		//cout<<lable[row]<<"XDD"<<endl;
		
		//cout<<programName<<"XDD"<<endl;
	}
	else
	{
		//programName="\0";
		startaddr=0;
		locctr=0;
	}
	//cout<<locctr<<" "<<lable[row]<<" "<<instruction[row]<<" "<<symbol[row]<<endl;//old
	Loc[0]=locctr;//new
	row++;
	int index=1;//new
	while(strcmp(instruction[row],"END")!=0)
	{
		
		//cout<<locctr<<" "<<lable[row]<<" "<<instruction[row]<<" "<<symbol[row]<<endl;//Old
		Loc[index]=locctr;//new
		index++;
		if(lable[row]!=" ")
		chk_label();
		////////////////
		chk_opcode();
		row++;
	}		
} 
void chk_label()
{
	int k,dupsym=0;
	for(k=0;k<symcount;k++)
	if(!strcmp(lable[row],mysymtab[k].symbol))
	{
		mysymtab[k].addr=-1;
		dupsym=1;
		break;
	}
	///////////
	if(!dupsym)
	{
		strcpy(mysymtab[symcount].symbol,lable[row]);//拷貝 
		//cout<<lable[row]<<"XDDDD"<<endl; 
		//mysymtab[symcount].symbol=lable[row];
		mysymtab[symcount++].addr=locctr;
	}
}
void chk_opcode()
{
	int k,found=0;
	for(k=0;k<59;k++)
	if(!strcmp(instruction[row],MyOptab[k].code))
	{
		locctr+=3;
		found=1;
		break;
	}
	if(!found)
	{
		if(!strcmp(instruction[row],"WORD"))
		locctr+=3;
		else if(!strcmp(instruction[row],"RESW"))
		locctr+=(3*atoi(symbol[row]));
		else if(!strcmp(instruction[row],"RESB"))
		locctr+=atoi(symbol[row]);
		else if(!strcmp(instruction[row],"BYTE"))
		{
			int len = strlen(symbol[row])-3;
			if(symbol[row][0]=='X')
			{
			  len/=2;//1
			  //printf("%d",len);
			  locctr = locctr + len;
		    }
			if(symbol[row][0]=='C')
			{
			 	locctr = locctr +len;//3
			 	//printf("%d",len);
		    }
			/*
			if(!strcmp(symbol[row],"C'EOF'"))
			locctr+=3;
			if(!strcmp(symbol[row],"X'F1'"))
			locctr+=1;
			*/
		} 
	}
}	
int record=0;//design for C'???'
int opCount;//no mind
char objectCode1[100][100];
int output[100][100]; 
int row2=1;
void Pass2(){
	while(strcmp(instruction[row2],"END")!=0)//不等於end的情況 
	{
		if(strcmp(instruction[row2],"BYTE")!=0&&strcmp(instruction[row2],"WORD")!=0&&strcmp(instruction[row2],"RESW")!=0&&strcmp(instruction[row2],"RESB")!=0)
		{
				
		for(int i22=0;i22<59;i22++)//0~58 助憶 opcode 共 59個 
		{
			if(strcmp(instruction[row2],MyOptab[i22].code)==0)
			{
					strcpy(objectCode1[row2],MyOptab[i22].objcode);
					
					
					break;
			}
	    }
    
    // ------op----------
    
		
		for(int j=0;j<symcount;j++)//symcount 算 symble table 裡共幾個元素 
		{
			
			if(strcmp(symbol[row2],mysymtab[j].symbol)==0)
			{
				
					int ans; int count=0;
					ans=mysymtab[j].addr;
					output[row2][count]=ans;
					//printf("%d\n",output[row2][count]);
					
			    }
			    if(strcmp(symbol[row2],"")==0)
			    {
			    		int count = 0 ;
						output[row2][count]=0;
						
			    		
			    }//---RSUB-----
			    if(strcmp(symbol[row2],"BUFFER,X")==0)
			    {
			    	for(int i=0;i<symcount;i++)//搜尋symbolTable 
			    	{
			    		if(strcmp(mysymtab[i].symbol,"BUFFER")==0)//+8(2) +8000(16) --> +32768(10) 
			    		{
			    			int count = 0;
			    			//output[row2][count]=mysymtab[i].addr+32768;-->fatal
			    			//cout<<mysymtab[i].addr<<endl;//4153
			    			int special=mysymtab[i].addr+32768;
			    			output[row2][count]=special;
			    			break; 			
						}
					}
				}//----BUFFER,X----EX:Line160,225
		    }
		    
	
    
    // -----後4碼-------
		}//end if  go--> resw resb BYTE WORD ---- 
		else if(strcmp(instruction[row2],"WORD")==0)
		{
			int count=0;
			strcpy(objectCode1[row2],"00");
			int temp=atoi(symbol[row2]);
			output[row2][count]=temp;
		}//end if --WORD---
		else if(strcmp(instruction[row2],"RESW")==0||strcmp(instruction[row2],"RESB")==0)
		{
			int count=0;
			objectCode1[row2][count]='\0';
			output[row2][count]=-1;//做的假設 
			
		}//--RESW,RESB---
		
		else if(strcmp(instruction[row2],"BYTE")==0)
		{
			if(symbol[row2][0]=='C')
			{
				int col=1;
				int count=1;
				
				for(col;symbol[row2][col]!='\0';col++)
				{
					
					if(symbol[row2][col]!='\'')
					{
						
						int acs=symbol[row2][col]; //printf("%X\n",acs);//cout<<"acs"<<acs<<endl;
							output[row2][0]=-3;
							output[row2][count]=acs; //cout<<"output:"<<output[row2][count]<<endl;
							objectCode1[row2][count]='\0';
							record=count;
							count++;
					}
				}
			}
			else if(symbol[row2][0]=='X')
			{
				int col1=1;	int count=0; int col11=0;
				for(col1;symbol[row2][col1]!='\0';col1++){
					 
				if(symbol[row2][col1]!='\''){
			   		
					   output[row2][count]=-2;//design   
					   //strcpy(objectCode1[row2],symbol[row2]);
					   char temp = symbol[row2][col1]; //cout<<"temp:"<<temp<<endl<<endl; OK
					    objectCode1[row2][col11]= temp; //cout<<"objectCode1[row][col1]"<<objectCode1[row2][col1]<<endl<<endl; OK
					    col11++;
					   		
			    }
			
		        }
		    }
	    }
		row2++;
}
	cout<<Loc[0]<<" "<<lable[0]<<" "<<instruction[0]<<" "<<symbol[0]<<endl;
	for(int i=1;i<row;i++)
	{
		cout<<Loc[i]<<" "<<lable[i]<<" "<<instruction[i]<<" "<<symbol[i]<<" "<<objectCode1[i];//New
		if(output[i][0]!=-1&&output[i][0]!=-2&&output[i][0]!=-3)
		{
			printf("%04X\n",output[i][0]);
		}
		else if(output[i][0]==-3){
			for(int j=1;j<=record;j++)
				printf("%X",output[i][j]);
				////////////
				cout<<endl;
		}
		else cout<<endl;
	}
	cout<<"    "<<lable[row]<<" "<<instruction[row]<<" "<<symbol[row]<<endl;

}
int main(){
    fstream fin;//宣告 一個 fstram的物件 file  
    fin.open("Figure2.1.txt",ios::in);//在讀取的方式下 開啟 2.1檔
	int i=0;
	
	int flag=0;

    int mod = 0;	
	char first[100][100];
	char second[100][100];
	char third[100][100];
    while(fin.getline(line,sizeof(line),'\n')){
			istringstream sin(line);
    		sin>>first[i]>>second[i]>>third[i];
    		mod = 1;
    		if(strstr(line,third[i]) == NULL || strcmp(line, strstr(line,third[i])) == 0){
    			mod = 2;
    		}	
    		if(strstr(line,second[i]) == NULL || strcmp(line, strstr(line,second[i])) == 0){
    			mod = 3;
    		}
    		//cout<<line<<endl;
    				if(mod==1){
    				strcpy(lable[i],first[i]);
    				strcpy(instruction[i],second[i]);
    				strcpy(symbol[i],third[i]);
				}
				if(mod==2)
				{
					strcpy(instruction[i],first[i]);
					strcpy(symbol[i],second[i]);
				}
				if(mod==3){
					strcpy(instruction[i],first[i]);
				}
				
        	//cout<<"Line "<<i<<" lable:"<<lable[i]<<" instruction:"<<instruction[i]<<" symbol:"<<symbol[i]<<" "<<endl;
        	i++;
        	opCount=i;
        	
    //system("pause");
    
}
Pass1();
length=locctr-toDec(startaddr);
Pass2();
//cout<<programName<<endl;
//cout<<"program length:"<<length<<endl;

// test 區 symbol table ----  共17個
/*for(int i=0;i<symcount;i++)
{
	cout<<mysymtab[i].symbol<<endl;
	cout<<mysymtab[i].addr<<endl;
	cout<<"Number:"<<i<<endl;
} */
//cout<<MyOptab[3].code<<" "<<MyOptab[3].objcode<<endl;
//cout<<opCount<<endl<<endl;
FILE *fout;
fout = fopen("object_list.txt","w+t");
fprintf(fout,"H%s  %06d%06X\n",programName,startaddr,length);//Head record
//int startlocation = startaddr;
int cnt=0;
bool headloc=true;
int begin=1;
int enterTime=1,counter=1;
for(int i=1;i<=row;i++)
{
		//fprintf(fout,"%dXDD\n",cnt);
		//cout<<Loc[i]<<" "<<lable[i]<<" "<<instruction[i]<<" "<<symbol[i]<<" "<<objectCode1[i];
		//int begin;
		if((cnt%60==0&&cnt!=0)||output[i][0]==-1||(cnt>54&&cnt!=60)||(i==row))
		{
			if(output[i][0]!=-1&&(i!=row))
			{
      		    
      		    fprintf(fout,"%2X",cnt/2);
			    for(int j=begin;j<i;j++)
			    {
			    	if(strcmp(instruction[j],"BYTE")==0)
					fprintf(fout,"%s",objectCode1[j]);
					else
					fprintf(fout,"%s%04X",objectCode1[j],output[j][0]);
					//fprintf(fout,"XDD");
			    }
			    fprintf(fout,"\n");
			    begin=i;
			    headloc = true;
			    cnt=0;
		    }
		    else if(i==row)
		    {
		    	fprintf(fout,"%02X",cnt/2);
		    	
		    	for(int j=begin;j<i;j++)
			    {
			    	if(strcmp(instruction[j],"BYTE")==0)
					fprintf(fout,"%s",objectCode1[j]);
					else
					fprintf(fout,"%s%04X",objectCode1[j],output[j][0]);
					//fprintf(fout,"XDD");
			    }
			    fprintf(fout,"\n");
		    	break;	
			} 
		    else
			{
				if(enterTime==1)
				{
					fprintf(fout,"%02X",cnt/2);
			        cnt=0;
					for(int z=begin;z<i;z++)
			    	{
						if(output[z][0]!=-1&&output[z][0]!=-2&&output[z][0]!=-3)
						{
							fprintf(fout,"%s",objectCode1[z]);
							fprintf(fout,"%04X",output[z][0]);
							//cnt+=6;
						}
		else if(output[z][0]==-3){
			for(int j=1;j<=record;j++)
				fprintf(fout,"%X",output[z][j]);
				////////////
				//cout<<endl;
				//fprintf(fout,"\n");
				//cnt+=6;
		}
		//else cout<<endl; -1 的情況
		else if(output[z][0]==-2) 
		{
			fprintf(fout,"%s",objectCode1[z]);
			//cnt+=2;
		}
			    }
					fprintf(fout,"\n");
					enterTime++;
					counter++;
			    }
				//break;
				if(enterTime==counter)
				{
					if(output[i+1][0]!=-1) 
					{
						begin=i+1;
						headloc=true;
				    }
					else
					{
					  counter++;
					  enterTime++;
				    }
				} 
				
		    }
	}
	
		if(headloc)
		{
			fprintf(fout,"T%06X",Loc[begin]);
			headloc=false;
		}
		
		if(output[i][0]!=-1&&output[i][0]!=-2&&output[i][0]!=-3)
		{
			//fprintf(fout,"%s",objectCode1[i]);
			//fprintf(fout,"%04X",output[i][0]);
			cnt+=6;
		}
		else if(output[i][0]==-3){
			//for(int j=1;j<=record;j++)
				//fprintf(fout,"%X",output[i][j]);
				////////////
				//cout<<endl;
				//fprintf(fout,"\n");
				cnt+=6;
		}
		//else cout<<endl; -1 的情況
		else if(output[i][0]==-2) 
		{
			//fprintf(fout,"%s",objectCode1[i]);
			cnt+=2;
		}
		
}

fprintf(fout,"E%06d",startaddr);//End record  
	return 0;
}








