#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<map>
using namespace std;
//struct to contain notaion(reduced minterm) and its corresponding minterm
bool in(string a, std::vector<string> v);
struct ep
{
    string nt;//notation
    vector<string> minterms;
};
//deal with '-' in minterm
void deal_min(string str, vector<string>& minterm)
{
    size_t posi = str.find("-");
    if (str.find("-") != string::npos)
    {
        str[posi] = '0';
        deal_min(str, minterm);
        str[posi] = '1';
        deal_min(str, minterm);
    }
    else
    {
    	if(!in(str, minterm))
        	minterm.push_back(str);
    }
}
// check if there is only one different digit
bool gCode(string a, string b)
{
	int diff = 0;
	for(int i = 0; i < a.length(); ++ i)
	{
		if(a[i] != b[i])
			diff++;
	}

	return (diff == 1);
}
//replace digit that is dontcare
string replace_d(string a, string b)
{
	string str = ""; 
	for(int i = 0; i < a.length(); ++i)
	{
		if(a[i] != b[i])
		{
			str+='-';
		}
		else
			str+=a[i];
	}
	return str;
}
//check if a is in b
bool in(string a, std::vector<string> v)
{
	for(int i = 0;  i < v.size(); ++i)
	{
		if(v[i].compare(a) == 0)
			return true;
	}
	return false;
}
//reduce minterm
vector<string> reduce_min(vector<string> mt, vector<string> dontcare)
{
	vector<string> nmt;
	int len = mt.size();
	int check[1000] = {0};
	for(int i = 0; i < len; ++i)
	{
		for(int j = 0; j < len ; ++j)
		{
            if(!in(mt[i], dontcare))
            {
    			if(gCode(mt[i], mt[j]))
    			{
    				check[i] = 1;
    				check[j] = 1;
    				if(!in(replace_d(mt[i], mt[j]), nmt))
    				{
    					nmt.push_back(replace_d(mt[i], mt[j]));
    				}
    			}
            }
		}
	}
	for(int i=0;i<len;i++)
    {      
        if(check[i] != 1 && !in(mt[i], nmt))
        {
            nmt.push_back(mt[i]);
        }
    }
    return nmt;
}
//check if vectors are equal
bool eq(vector<string> a,vector<string> b)
{
   if(a.size() != b.size())
   {
      return false;
   }

    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    for(int i=0;i<a.size();i++)
    {
        if(a[i]!=b[i])
        {
        	return false;
        }
    }
    return true;
}

//main fucntion
int main(int argc, char const *argv[])
{
	ifstream i(argv[1], ios::in);
    ofstream out(argv[2], ios::out);
    int v; //nums of var
    int o;//nums of output file
    int n;//num of minterms or dontcare
    string line, input_buffer[50];
    vector<string> minterm, dontcare;
    int cnt = 0;
    //deal with file input
    while(getline(i,line,'\n'))
    {
    	input_buffer[cnt++] = line;
    	if(line[0] == '.')
    	{
	    	if(line[1] == 'i' && line[2] == ' ')
	    	{
	    		v = (line[3]- '0');
	    	}
	    	else if(line[1] == 'p')
	    	{
	    		string temp;
	    		for(int i = 3; i < line.length(); ++i)
	    		{
	    			temp+=line[i];
	    		}
	    		n = stoi(temp);
	    	}
    	}
    	else
    	{
    		if(line[v + 1] == '1')
    		{
    			string temp = "";
    			for(int i = 0; i < v; ++i)
    			{
    				temp += line[i];
    			}
   
    			deal_min(temp, minterm);
    		}
    		else
    		{
    			string temp = "";
    			for(int i = 0; i < v; ++i)
    			{
    				temp += line[i];
    			}
   				deal_min(temp, minterm);
    			deal_min(temp, dontcare);

    		}
    	}
    }
    i.close();
    vector<string> mtemp;
    //dci = dontcare.size();
    for(int i = 0;i< minterm.size(); ++i)
    {
        if(!in(minterm[i], dontcare))
            mtemp.push_back(minterm[i]);
    }
    //


    sort(minterm.begin(),minterm.end());
    do{ 
         minterm = reduce_min(minterm, dontcare);
         sort(minterm.begin(),minterm.end());
     }while(!eq(minterm, reduce_min(minterm, dontcare)));
     //build a list to find essential prime
    std::map<string, vector<string> >  m;
    vector<struct ep> epl(minterm.size());
    vector<string> esP;//store essential prime
    for(int i = 0; i < minterm.size(); ++i)
    {
        epl[i].nt =  minterm[i];
        deal_min(minterm[i], epl[i].minterms);
        for(int j = 0; j < epl[i].minterms.size(); ++j)
        {
            m[epl[i].minterms[j]].push_back(minterm[i]);
        }
    }
    for(int i = 0; i < mtemp.size(); ++i)
    {
        if(m[mtemp[i]].size() ==1 && !in(m[mtemp[i]][0] + " 1", esP))
            esP.push_back(m[mtemp[i]][0] + " 1");
    }
    //output
    for(int i =0; i < 4; ++i)
    {
        out << input_buffer[i] <<"\n";
    }
    out << ".p " << esP.size() <<"\n";
    for(int i = 0; i < esP.size(); ++i)
    {
        out << esP[i] <<"\n";
    }
    out <<".e\n";
	return 0;
}

