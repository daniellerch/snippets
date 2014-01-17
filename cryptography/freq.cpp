/*
   Copyright (c) 2007 Daniel Lerch Hostalot <dlerch@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a 
   copy of this software and associated documentation files (the "Software"), 
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
   and/or sell copies of the Software, and to permit persons to whom the 
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
   DEALINGS IN THE SOFTWARE.
*/



#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>


int main(int argc, char* argv[])
{
	using namespace std;

	if(argc!=3)
	{
		cout << "Usage: " << argv[0] << " [file] [max group size]" << endl;
		return 0;
	}

	for(int i=0; i<atoi(argv[2]); i++)
	{
		int c;
		map<string,int> freq;
		ifstream file(argv[1]);
		string group;
		while( (c=file.get()) && !file.eof() )
		{
			if(isspace(c))
				continue;
			group += string(1,c);

			if(group.size()>=i+1)
				freq[group.substr(group.size()-i-1, i+1)]++;
		}

		cout << endl << "-- FREQ SIZE " << i+1 << " --" << endl;
		int count = 0;
		map<string,int>::iterator it = freq.begin();    
		for(;it!=freq.end(); it++)
		{
			if((*it).second>1)
			{
				count ++;

				cout << (*it).first << ": " << (*it).second << "\t";

				if(count%6==0)
					cout << endl;
			}
		}
		cout << endl;
	}

	return 0;
}

