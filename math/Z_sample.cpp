

#include<Z.hpp>


int main()
{
   Z a, b, c, d;

   a = "10";
   b = -6;
   c = 23;

   d = a*(b^3) + a*(b^2) + (a*b) + (c/2);
   std::cout << d << std::endl;



   return 0;
}


