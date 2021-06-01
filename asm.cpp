/*
Name - Saubhik Kumar
Roll - 1801CS44
Declaration of Authorship
This cpp file, asm.cpp, is part of the assignment of CS321 at the
department of Computer Science and Engg, IIT Patna .
*/


#include <bits/stdc++.h>
using namespace std;

#define ll signed long long int

//File pointers
ifstream fp1("");  // File to be processed  ---- Input File containing SIMPLE ASSEMBLY CODE
ofstream fp2("bubble.log"); // Error File
ofstream fp3("bubble.o");  // Machine Codes
ofstream fp4("bubble.l"); // Listing File

//All function declaration
void Z();    // simply goes to next line, only for testing purpose
pair<string,int> check_label(string s);
string refine(string s);
pair<string,int> find_pnemonic(string s);
bool only_comment(string s);
bool valid_pnemonic(string s);
int get_opcode(string s);
string category(string s);
string find_operand(string s,int idx);
string int_to_hex(ll n,int len);
bool call_func(string s);
bool check_value_only(string s);
void pass1();
void pass2();

// refine(s) : removes all spaces from beginning
// check_label(s) : checks if there is a label definition, returns it else returns "!"
// Find pnemonic and operand

//All data structures
string mnemonic[19]={"ldc","adc","ldl","stl","ldnl","stnl","add","sub","shl","shr","adj","a2sp",
                    "sp2a","call","return","brz","brlz","br","halt"};
string alone[8]={"add","sub","shl","shr","a2sp","sp2a","return","halt"};
string with[13]={"data","ldc","adc","ldl","strl","ldnl","stnl","adj","call","brz","brlz","br","set"};
string value_only[2]={"ldc","adc"};
unordered_set<string> labels;
vector<vector<string> > info(1000,vector<string> (3,""));
map<int,int> location;   // maps a line to its address
map<string,int> lable_address; // maps label to its address
map<int,string> mc;
map<string,int> vis;

//All useful constants
int line=0;
int errors=0;
int pc=0;


// Functions implementation

void Z()
{
    return ;
	cout<<endl;
	return ;
}

pair<string,int> check_label(string s)   // Returns if there is label, also if there is only label definition
{
	int n=s.length(),idx=-1;
	for(int i=0;i<n;i++)
	{
       if(s[i]==':')
       	{
       		idx=i;
       		break;
       	}
	}
	if(idx==-1) return {"!",0};  

	string label;
	for(int i=0;i<idx;i++) label+=s[i];

	// Refining the label
    label=refine(label);
    reverse(label.begin(),label.end());
    label=refine(label);
    reverse(label.begin(),label.end());

	string check;
    for(int i=idx+1;i<n;i++)
    {
    	if(s[i]==';') break;
    	if(s[i]!=' ' and s[i]!=9)
    		{
    			//cout<<"Something at "<<line<<endl;
    			check+=s[i];
    		}
    }
   // cout<<check<<" "<<check.length()<<endl;
    check=refine(check);
    reverse(check.begin(),check.end());
    check=refine(check);
    reverse(check.begin(),check.end());
   // cout<<check<<" length is "<<check.length()<<endl;
    if(refine(check).length()==0)
    {
    	return {label,1};
    }
	return {label,0};
}

string refine(string s)
{
	string ret;
	int n=s.length();
	int i=0;
	while(s[i]==' ' or s[i]==9) i++;
	for(int j=i;j<n;j++)
	{
		if(s[j]==';') break;
		ret+=s[j];
	}
	for(int i=0;i<ret.length();i++)
	{
		if(ret[i]>=65 and ret[i]<=90) ret[i]+=32;
	}
	return ret;
}

pair<string,int> find_pnemonic(string s)    // returns pnemonic and its end position
{
	int n=s.length(),last=n;
   string ret;
   if(check_label(s).first=="!")
   {
   	   s=refine(s);
       for(int i=0;i<n;i++)
       {
       	if(s[i]==' ')
       		{
       			last=i;
       			break;
       		}
       	ret+=s[i];
       }
       return {ret,last};
   }
   else
   {
   	  int i;
   	  for(i=0;i<n;i++)
   	  	{
   	  		if(s[i]==':') break;
   	  	}
   	  for(int j=i+1;j<n;j++)
   	  {
          if(s[j]==' ' and ret.length()>0)
          	{
          		last=j;
          		break;
          	}
         if(s[j]!=' ') ret+=s[j];
   	  }
   	  ret=refine(ret);
   	  return {ret,last};
   }
}

bool only_comment(string s)
{
	s=refine(s);
	for(int i=0;i<s.length();i++)
	{
		if(s[i]!=' ') return false;
	}
	return true;
}

bool valid_pnemonic(string s)
{
	if(s.length()==0) return false;
	if(s=="data" or s=="set") return true;
	for(int i=0;i<22;i++) if(s==mnemonic[i]) return true;
	return false;
}

int get_opcode(string s)
{
	for(int i=0;i<18;i++) if(s==mnemonic[i]) return i;
	return 0;
}

string category(string s)
{
	for(string  v: alone) if(s==v) return "alone";
	return "with";
}

string find_operand(string s,int idx)
{
	 string ret;
     int n=s.length();
     int i=idx;
     while(s[i]==' ') i++;
     for(int j=i;j<n;j++)
     {
         ret+=s[j];
     }
     reverse(ret.begin(),ret.end());
     ret=refine(ret);
     reverse(ret.begin(),ret.end());
     for(int i=0;i<ret.length();i++)
     {
     	if(ret[i]==' ' or ret[i]==',')
     	{
     		errors++;
     		cout<<"Line "<<line<<": Extra character on this line\n";
     		break;
     	}
     }
     return ret;
}

ll transform(ll n)  // Transforms 32 bit negative integer to its 2-compliment
{
    if(n>=0) return n;
    ll ans=1,d=2,x=32;
    while(x)
    {
    	if(x%2) ans=ans*d;
    	d=d*d;
    	x/=2;
    }
    n=ans+n;
    return n;
}

string int_to_hex(ll n,int len)   // converts int to hex of length len
{
	if(n<0) n=(1<<(len*4))+n-1;
	string ret;
	while(n)
	{
        int x=n%16;
        if(x<10) ret=char(x+48)+ret;
        else ret=char(x+87)+ret;
        n/=16;
	}
	while(ret.length()<len)
	{
		ret='0'+ret;
	}
	return ret;
}

bool call_func(string s)     // checks if pnemonic is a callee
{
	return s=="call" or s=="br" or s=="brz" or s=="brlz";
}

int string_to_decimal(string s)
{
	int neg=1,itr=0;
	if(s[0]=='-')
	{
		neg=-1;
		itr=1;
	}
	if(s[0]=='+')
	{
		neg=1;
		itr=1;
	}
	int n=s.length(),ret=0;
	for(int i=itr;i<n;i++)
	{
           ret=ret*10+(s[i]-48);
	}
	return ret*neg;
}

int hex_to_decimal(string s)
{
	int n=s.length();
	int ans=0;
	for(int i=2;i<n;i++)
	{
          ans=ans*16+((s[i]<=57)?(s[i]-48):(s[i]-87));
	}
	return ans;
}

int octal_to_decimal(string s)
{
	int n=s.length();
	int ans=0;
	for(int i=1;i<n;i++)
	{
          ans=ans*8+(s[i]-48);
	}
	return ans;
}

int to_int(string s)
{
	// s can be octal,decimal or hex

	//Conditions for decimal
	if(s.length()==1 or s[0]=='-') return string_to_decimal(s);
	if(s[0]!='0' and s[1]!='x') return string_to_decimal(s);

    //Conditions for hex

    if(s[1]=='x') return hex_to_decimal(s);
    return octal_to_decimal(s);
}

bool check_value_only(string s)
{
	for(auto pn: value_only)
	{
		if(s==pn) return true;
	}
	return false;
}
void pass1()
{

	string pne,label,operand;
	int operand_start; 
	while(!fp1.eof())
	{
		line++;
		string s;
		getline(fp1,s);

		if(only_comment(s)) continue;   // If only comment on line, continue to next line

		int n=s.length(); 
		s=refine(s);                     // remove spaces from beginning
       
	//	cout<<s.length()<<" "<<s<<endl;
		label=check_label(s).first;         // check if there is label definition
		//cout<<label<<" "<<check_label(s).second<<endl;
		if(check_label(s).second==1)
		{
			// cout<<label<<"  "<<"      --         --\n";
			if(labels.count(label)==1)
		    {
		    	errors++;
		        //cout<<"Line "<<line<<": Duplicate Label definition\n";
		        fp2<<"Line "<<line<<": Duplicate Label definition\n";
		    }
		    location[line]=pc+4;
		    info[line][0]=label;
		    info[line][1]="--";
		    info[line][2]="--";
		    labels.insert(label);
			lable_address[label]=pc+4;
			continue;
		}
		//cout<<s<<" label is "<<label<<endl;

        if(labels.count(label)==1)
        {
        	errors++;
        	//cout<<"Line "<<line<<":Duplicate Label definition\n";
        	fp2<<"Line "<<line<<":Duplicate Label definition\n";
        	continue;
        }
        if(label!="!")
    	{
    		labels.insert(label);
    		lable_address[label]=pc+4;
    	}
		pne=find_pnemonic(s).first;                // find pnemonic
		//cout<<"pnemonic is "<<pne<<endl;

        bool val_pne=valid_pnemonic(pne);
		if(!val_pne)
		{
			errors++;
			//cout<<"Line "<<line<<" Error : Invalid pnemonic\n";   // check if valid pnemonic
			fp2<<"Line "<<line<<" Error : Invalid pnemonic\n";
	    }
		else
		{
			//cout<<"valid_pnemonic\n";
		}
        if(!val_pne) continue;

        int opc=get_opcode(pne);
       // cout<<"opcode for "<<pne<<" is "<<opc<<endl;
        string opcode=int_to_hex(opc,2);
       // cout<<"machine opcode for "<<pne<<" is "<<opcode<<endl;

        operand_start=find_pnemonic(s).second;
       // cout<<operand_start<<endl;

        operand=find_operand(s,operand_start);

        if(category(pne)=="alone")   // checks if it is in alone or with operand
        {

        	if(operand.length()>0)
        	{
        		errors++;
        		info[line][0]=label;
        		info[line][1]=pne;
        		info[line][2]=operand;
        		//cout<<"Line "<<line<<" : Unexpected Operand\n";
        		fp2<<"Line "<<line<<" : Unexpected Operand\n";
        		continue;
        	}
        	else operand="--";
        }
        else
        {
        	// need to find if there are mutliple operands or there is invalid operand
            //cout<<"operand is "<<operand<<endl;
            if(operand.length()==0)
            {
            	errors++;
            	//cout<<"Line "<<line<<" : Operand Expected\n";
            	fp2<<"Line "<<line<<" : Operand Expected\n";
            }
        }
       info[line][0]=label;
       info[line][1]=pne;
       info[line][2]=operand;
       pc+=4;
       location[line]=pc;
       //cout<<label <<"      "<<pne<<"      "<<operand<<endl;
	}
	//printf("\nLables are : \n");
	//for(auto v : labels) cout<<v<<endl;
	for(int i=1;i<=line;i++)
	{
		string pnem=info[i][1];
		string oper=info[i][2];
		if(call_func(pnem))
		{
		    vis[oper]=1;
			if(labels.count(oper)==0)
			{
				errors++;
				//cout<<"Line "<<i<<" : No such label found\n";
				fp2<<"Line "<<i<<" : No such label found\n";
			}
			else vis[oper]=1;
		}
	}
	/*for(auto lab: lable_address)
	{
		if(vis.count(lab.first)==0)
		{
			cout<<"Warning : Unused Label "<<lab.first<<endl;
		}
	} */
//	cout<<"Program Counter at "<<pc<<endl;
}

// What I have done is : extracted label , pnemonic and operand
void pass2()
{

    string pnem,oper;
    int opcode,operand_code;
	for(int i=1;i<=line;i++)
	{
        pnem=info[i][1];
        oper=info[i][2];
        string opco,opera;
        //For opcode
        if(pnem[0]<97 or pnem[0]>122) continue;
        else opcode=get_opcode(pnem);
       // cout<<pnem<<"\t";
        opco=int_to_hex(opcode,2);


        //For operand

        if(pnem=="set" or pnem=="data")    // 32 bit operand
        {
        	ll num=to_int(oper);
        	//cout<<"num is "<<num<<" ";
            if(num<0) num=transform(num);
            opera=int_to_hex(num,8);
            opco="";
        }
        else if(oper[1]=='-') opera=int_to_hex(0,6);  // if no operand
        else if((oper[0]<=57 and oper[0]>=48) or oper[0]=='-' or oper[0]=='+')  // numerical operand
        {
        	int num=to_int(oper);
        	//cout<<"num is "<<num<<" ";
            if(num<0) num=(1<<24)+num;
            opera=int_to_hex(num,6);
        }
        else    // label operand
        {
         	// It is a label;
            bool value_only_operand=check_value_only(pnem);
            if(value_only_operand)
            {
                int lab_add=lable_address[oper];
        		opera=int_to_hex(lab_add/4,6);
            }
            else
            {
              	int lab_add=lable_address[oper]-location[i]-4;
        		opera=int_to_hex(lab_add/4,6);
        	}
        	//oper=to_string(lable_address[oper]);
        }
        mc[i]="0x"+opera+opco;
        //cout<<mc[i]<<endl;
        fp3<<mc[i]<<endl;
	}
}
int main()
{
	//printf("Pass1\n\n");
	pass1();
	if(errors)
	{
		return 0;
	}
	Z();
    Z();Z();

    Z();Z();

	//printf("\nPass2 Machine Code is \n\n");
    if(errors==0)
	{
		fp2<<"Success! No errors detected\n";
		pass2();
		//printf("\nListing File is\n");
		for(int i=1;i<=line;i++)
	    {
	   	    if(location[i]==0) continue;
			//cout<<int_to_hex(location[i]/4-1,8)<<"\t"<<"  "<<mc[i]<<"   "<<
			//info[i][0]<<"\t"<<info[i][1]<<"\t"<<info[i][2]<<endl;
			fp4<<int_to_hex(location[i]/4-1,8)<<"\t"<<"  "<<mc[i]<<"   "<<
			info[i][0]<<"\t"<<info[i][1]<<"\t"<<info[i][2]<<endl;
	    }
	}
	/*ifstream fp1("assembly1.txt");
	string pne,label;
	int operand_start;
	while(!fp1.eof())
	{
		string s;
		getline(fp1,s);

		if(only_comment(s)) continue;   // If only comment on line, continue to next line

		int n=s.length();
		s=refine(s);                     // remove spaces from beginning

	//	cout<<s.length()<<" "<<s<<endl;

		label=check_label(s);         // check if there is label definition
		//cout<<s<<" label is "<<label<<endl;
        if(label!="!") labels.insert(label);
		pne=find_pnemonic(s).first;                // find pnemonic
		cout<<"pnemonic is "<<pne<<endl;

        bool val_pne=valid_pnemonic(pne);
		if(!val_pne) cout<<"Error : Invalid pnemonic\n";   // check if valid pnemonic
		else cout<<"valid_pnemonic\n";
        if(!val_pne) continue;

        int opc=get_opcode(pne);
       // cout<<"opcode for "<<pne<<" is "<<opc<<endl;
        string opcode=int_to_hex(opc,2);
        cout<<"machine opcode for "<<pne<<" is "<<opcode<<endl;

        operand_start=find_pnemonic(s).second;
        cout<<operand_start<<endl;

        if(category(pne)=="alone")   // checks if it is in alone or with operand
        {

        	if(find_operand(s,operand_start).length()>0)
        	{
        		cout<<"Unexpected Operand\n";
        		continue;
        	}
        }
        else
        {
        	// need to find if there are mutliple operands or there is invalid operand
        	string ope=find_operand(s,operand_start);
            cout<<"operand is "<<ope<<endl;
        }


	}
	for(auto v : labels) cout<<v<<endl;*/

}
