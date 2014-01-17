/*
   Copyright (c) 2006 Daniel Lerch Hostalot <dlerch@gmail.com>

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

/* For more information visit www.daniellerch.me */

/*
   Solve matrix in GF2
   ================================

	Needs a file with format:

   10101010010100100111000
   10001010101010010101001
   11101010100101001010101
   10101010100111111111110


	Dependences:
   http://www.shoup.net/ntl/

	Comilation:
	g++ -Wall matrix_solve_GF2.cpp -lntl -o matrix_solve_GF2


   REFERENCES:
   http://www.shoup.net/ntl/
   http://www.pgnfs.org/   

*/


#include <iostream>
#include <fstream>
#include <vector>
#include <NTL/mat_GF2.h>


// {{{ get_freecols()
NTL::vec_GF2 get_freecols(NTL::mat_GF2& M) 
{
   NTL::vec_GF2 freecols;
   freecols.SetLength(M.NumCols());

   for(int i=0, j=0; (i<M.NumRows())&&(j<M.NumCols()); i++, j++) 
   {
      if(M[i][j]==0) 
      {
         int k=i+1;
         while((k<M.NumRows())&&(M[k][j]==0)) k++;

         if(k>=M.NumRows()) 
         {
            freecols[j]=1;
            i--;
         }
      }
   }	

   return freecols;
}	
// }}}

// {{{ get_solution()
NTL::vec_GF2 get_solution(NTL::mat_GF2* M, NTL::vec_GF2& freecols, int n) 
{
   int j=-1;
   int i=n;
   int k;
   NTL::vec_GF2 res;
   res.SetLength(freecols.length());

   while(i>0) 
   {
      j++;
      while(freecols[j]==0) j++;
      i--;
   }

   res[j]=1;
   for(i=0; i<M->NumRows(); i++) 
   {
      if((*M)[i][j]==1) 
      {
         k=i;
         while(k<j) 
         {
            if((*M)[i][k]==1) 
            {
               res[k]=1;
               break;
            }
            k++;
         }
      }
   }

   return res;
}
// }}}

// {{{ get_GF2_matrix_from_file()
/*
	Read from file:

   10101010010100100111000
   10001010101010010101001
   11101010100101001010101
   10101010100111111111110
*/
NTL::mat_GF2 get_GF2_matrix_from_file(const char *file)
{
   std::string line;
   NTL::mat_GF2 M;
   long filesize;
   long numcols;
   long numrows;
   long begin;
   long end;

   std::ifstream matrix_file(file);
   if(!matrix_file)
   {
      std::cerr << "Error opening " << file << std::endl;
      return M;
   }

   begin = matrix_file.tellg();
   matrix_file.seekg (0, std::ios::end);
   end = matrix_file.tellg();
   filesize=end-begin;
   
   matrix_file.seekg (0, std::ios::beg);
   getline(matrix_file, line);

   numcols = line.size();
   numrows = filesize/numcols;

   M.SetDims(numrows, numcols);

   int i=0;
   while(!matrix_file.eof())
   {
      for(int j=0; j<numcols; j++)
      {
         int value = 0;
         if(line[j]=='1') value=1;
         M[i][j]=value;
      }
      
      getline(matrix_file, line);
      i++;
   }

   matrix_file.close();
   return M;
}
// }}}

// {{{ MAIN
int main(int argc, char *argv[]) 
{
   using namespace std;

   if (argc!=3) 
   {
      cerr << "Usage: " << argv[0] 
           << " <input file (matrix)> <output file>"  << endl;
      
      return 1;
   }
   
   ofstream output(argv[2]);
   if(!output)
   {
      cerr << "Error opening " << argv[2] << endl;
      return 0;
   }

  	NTL::mat_GF2 M = get_GF2_matrix_from_file(argv[1]);

   NTL::gauss(M);
	
	NTL::vec_GF2 freecols = get_freecols(M);

   long n=1;
	for(long i=0; i<freecols.length(); i++)
   {
		if(freecols[i]==1) 
      {
         NTL::vec_GF2 V =get_solution(&M, freecols, n++);
         for(long j=0; j<V.length(); j++)
            output << V[j];
         output << endl;
      }
   } 

   output.close();
   return 0;
}
// }}}


