/*
Name - Saubhik Kumar
Roll - 1801CS44
Declaration of Authorship
This cpp file, emu.cpp, is part of the assignment of CS321 at the
department of Computer Science and Engg, IIT Patna .
*/


#include <bits/stdc++.h>
using namespace std;

#define ll signed long long int

// All data structures
ll memory[10000];
map<pair<int,string>,string> mnemonics;

// All variables
ll line;
ll a;
ll b;
ll sp;
ll pc;



void init()
{
       mnemonics[{0,"ldc"}]="value";
       mnemonics[{1,"adc"}]="value";
       mnemonics[{2,"ldl"}]="offset";
       mnemonics[{3,"stl"}]="offset";
       mnemonics[{4,"ldnl"}]="offset";
       mnemonics[{5,"stnl"}]="offset";
       mnemonics[{6,"add"}]="";
       mnemonics[{7,"sub"}]="";
       mnemonics[{8,"shl"}]="";
       mnemonics[{9,"shr"}]="";
       mnemonics[{10,"adj"}]="value";
       mnemonics[{11,"a2sp"}]="";
       mnemonics[{12,"sp2a"}]="";
       mnemonics[{13,"call"}]="offset";
       mnemonics[{14,"return"}]="";
       mnemonics[{15,"brz"}]="offset";
       mnemonics[{16,"brlz"}]="offset";
       mnemonics[{17,"br"}]="offset";
       mnemonics[{18,"halt"}]="";
       mnemonics[{-1,"data"}]="value";
       mnemonics[{-1,"set"}]="value";
}
ll hex_to_int(string s)
{
	ll ans=0;
	for(int i=2;i<10;i++)
	{
	   ll c;
	   if(s[i]>=97) c=s[i]-87;
	   else c=s[i]-48;
	   ans=ans*16+c;
	}
	return ans;
}

ll pw(ll n)
{
	ll ans=1,d=2;
	while(n)
	{
		if(n%2) ans=ans*d;
		d=d*d;
		n/=2;
	}
	return ans;
}

string find_pnemonic(ll op)
{
   for(auto mne :mnemonics)
   {
      if(mne.first.first==op) return mne.first.second;
   }
   return "";
}
// first 24 bits to operand , last 8 bits to opcode
int main(int argc, char *argv[])
{
	string command=argv[1];
	string file=argv[2];
	// All File pointers
    ifstream fp1(file);   
   init();
   line=0;
   while(!fp1.eof())
   {
   	  string x;
   	  getline(fp1,x);
   	  if(x=="") break;
   	  memory[line]=hex_to_int(x);
   	  line++;
   }
   if(command=="-before")
   {
      for(int i=1;i<=line;i++) printf("%08x\t%08x\n",i,memory[i]);
   }
   else if(command=="-trace" or command=="-after")
   {
	   sp=0;
	   pc=0;
	   while(1)
	   {
	         ll operand,opcode;
	         operand=memory[pc];
	         opcode=operand&255;
	         operand=operand>>8;
	         if((operand&pw(23))!=0)
	         {
	             operand=operand-pw(24);
	         }
	         if(command=="-trace")
	         {
	            printf("%08x\t%08x\t%08x\t%08x\t",pc,sp,a,b);
	            string x=find_pnemonic(opcode);
	            if(mnemonics[{opcode,x}].length()==0) cout<<x<<endl;
	            else 
	            {
	            cout<<x;
	            printf("\t%08x\n",operand); 
	            }
	         }
	         if(opcode==18) break;
	         if(opcode==0)
	         {
	         	b=a;
	            a=operand;
	         }
	         else if(opcode==1)
	         {
	         	a=a+operand;
	         }
	         else if(opcode==2)
	         {
	         	b=a;
	         	a=memory[sp+operand];
	         }
	         else if(opcode==3)
	         {
	         	memory[sp+operand]=a;
	         	a=b;
	         }
	         else if(opcode==4)
	         {
	         	a=memory[a+operand];
	         }
	         else if(opcode==5)
	         {
	         	memory[a+operand]=b;
	         }
	         else if(opcode==6)
	         {
	         	a=a+b;
	         }
	         else if(opcode==7)
	         {
	         	a=b-a;
	         }
	         else if(opcode==8)
	         {
	         	a=b<<a;
	         }
	         else if(opcode==9)
	         {
	         	a=b>>a;
	         }
	         else if(opcode==10)
	         {
	         	sp=sp+operand+1;
	         }
	         else if(opcode==11)
	         {
	         	sp=a;
	         	a=b;
	         }
	         else if(opcode==12)
	         {
	         	b=a;
	         	a=sp;
	         }
	         else if(opcode==13)
	         {
	         	b=a;
	         	a=pc;
	         	pc=(pc+operand)%(1<<24)+2;
	         }
	         else if(opcode==14)
	         {
	         	pc=a;
	         	a=b;
	         }
	         else if(opcode==15)
	         {
	         
	         	if(a==0)  
	         		{
	         			//pc=pc+operand;
	         			pc=(pc+operand)%(1<<24)+2;
	         		}
	         }
	         else if(opcode==16)
	         {
	         	
	         	if(a<0) 
	         		{
	         			//pc=pc+operand;
	         			pc=(pc+operand)%(1<<24)+2;
	         		}
	         }
	         else if(opcode==17)
	         {
	         	
	     			pc=(pc+operand)%(1<<24)+2;
	         }
	         else memory[1000]=operand;
	         
	         pc++;
	         if(pc<0 or pc>=line) break;
	   }
	   if(command=="-after")
	   {
	      for(int i=1;i<=line;i++) printf("%08x\t%08x\n",i,memory[i]);
	   }
   } 
   else if(command=="-isa")
   {
   	          int c=1;
   	       printf("Opcode\tMnemonic\tOperand\n");
              for(auto mne : mnemonics)
              {
              	cout<<mne.first.first<<"\t"<<mne.first.second<<"\t"<<mne.second<<endl;
              	c++;
              }
   }
   else 
   {
   	cout<<"Invalid Input Command : "<<command<<endl;
   }

   return 0;
}
