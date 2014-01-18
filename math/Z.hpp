
#include<iostream>
#include<cstdlib>
#include<gmp.h>

#ifndef _Z_HPP_
#define _Z_HPP_


// ---------------------------------------------------------------------------
// Multi Precision Integer
// ---------------------------------------------------------------------------
class Z
{
   public:

   Z()
   {
      mpz_init(value);
      mpz_set_ui(value, 0);
   }
   
   ~Z()
   {
      mpz_clear(value);
      mpz_set_ui(value, 0);
   }


   mpz_t value;  


   // Assign
   inline Z& operator=(const Z& a)
   {
      mpz_set(value, a.value);
      return *this;
   }

   inline Z& operator=(long int a)
   {
      if(a>=0)
         mpz_set_ui(value, a);
      else
         mpz_set_si(value, a);

      return *this;
   }

   inline Z& operator=(const char *str)
   {
      mpz_set_str(value, str, 10);

      return *this;
   }


   // Algorithms
   int to_int()
   {
      char str[mpz_sizeinbase(value, 10)+2];
      mpz_get_str(str, 10, value);

      return atoi(str);
   }

};

// ---------------------------------------------------------------------------
// Addition
// ---------------------------------------------------------------------------
inline Z operator+(const Z& a, const Z& b)
{
   Z result;

   mpz_add(result.value, a.value, b.value);
   return result;
}

inline Z operator+(const Z& a, unsigned long int b)
{
   Z result;

   mpz_add_ui(result.value, a.value, b);
   return result;
}

// ---------------------------------------------------------------------------
// Substraction
// ---------------------------------------------------------------------------
inline Z operator-(const Z& a, const Z& b)
{
   Z result;

   mpz_sub(result.value, a.value, b.value);
   return result;
}

inline Z operator-(const Z& a, unsigned long int b)
{
   Z result;

   mpz_sub_ui(result.value, a.value, b);
   return result;
}

// ---------------------------------------------------------------------------
// Multiplication
// ---------------------------------------------------------------------------
inline Z operator*(const Z& a, const Z& b)
{
   Z result;

   mpz_mul(result.value, a.value, b.value);
   return result;
}

inline Z operator*(const Z& a, long int b)
{
   Z result;

   if(b>=0)
      mpz_mul_ui(result.value, a.value, b);
   else
      mpz_mul_si(result.value, a.value, b);

   return result;
}

inline Z operator*(long int b, const Z& a)
{
   Z result;

   if(b>=0)
      mpz_mul_ui(result.value, a.value, b);
   else
      mpz_mul_si(result.value, a.value, b);

   return result;
}


// ---------------------------------------------------------------------------
// Division
// ---------------------------------------------------------------------------
inline Z operator/(const Z& a, const Z& b)
{
   Z result;

   mpz_tdiv_q(result.value, a.value, b.value);
   return result;
}

inline Z operator/(const Z& a, long int b)
{
   Z result;

   if(b>=0)
   {
      mpz_tdiv_q_ui(result.value, a.value, b);
   }
   else
   {
      mpz_tdiv_q_ui(result.value, a.value, b);
      result = result * (-1);
   }

   return result;
}

// ---------------------------------------------------------------------------
// Module
// ---------------------------------------------------------------------------
inline Z operator%(const Z& a, const Z& b)
{
   Z result;

   mpz_mod(result.value, a.value, b.value);
   return result;
}

inline Z operator%(const Z& a, long int b)
{
   Z result;

   if(b<0)
      mpz_mod_ui(result.value, a.value, -b);
   else
      mpz_mod_ui(result.value, a.value, b);

   return result;
}


// ---------------------------------------------------------------------------
// Power
// ---------------------------------------------------------------------------
inline Z operator^(const Z& a, unsigned long int b)
{
   Z result;

   mpz_pow_ui(result.value, a.value, b);
   return result;
}



// ---------------------------------------------------------------------------
// Compare
// ---------------------------------------------------------------------------
inline bool operator==(const Z& a, const Z& b)
{
   if(mpz_cmp(a.value, b.value)==0)
      return true;
      
   return false;
}

inline bool operator==(const Z& a, long int b)
{
   if(b>=0)
   {
      if(mpz_cmp_ui(a.value, b)==0)
         return true;
   }
   else
   {
      if(mpz_cmp_si(a.value, b)==0)
         return true;
   }
      
   return false;
}

inline bool operator<(const Z& a, const Z& b)
{
   if(mpz_cmp(a.value, b.value)<0)
      return true;
      
   return false;
}

inline bool operator<(const Z& a, long int b)
{
   if(b>=0)
   {
      if(mpz_cmp_ui(a.value, b)<0)
         return true;
   }
   else
   {
      if(mpz_cmp_si(a.value, b)<0)
         return true;
   }
      
   return false;
}

inline bool operator<(long int b, const Z& a)
{
   if(b>=0)
   {
      if(mpz_cmp_ui(a.value, b)<0)
         return true;
   }
   else
   {
      if(mpz_cmp_si(a.value, b)<0)
         return true;
   }
      
   return false;
}




inline bool operator>(const Z& a, const Z& b)
{
   if(mpz_cmp(a.value, b.value)>0)
      return true;
      
   return false;
}

inline bool operator>(const Z& a, long int b)
{
   if(b>=0)
   {
      if(mpz_cmp_ui(a.value, b)>0)
         return true;
   }
   else
   {
      if(mpz_cmp_si(a.value, b)>0)
         return true;
   }
      
   return false;
}

inline bool operator>(long int b, const Z& a)
{
   if(b>=0)
   {
      if(mpz_cmp_ui(a.value, b)>0)
         return true;
   }
   else
   {
      if(mpz_cmp_si(a.value, b)>0)
         return true;
   }
      
   return false;
}


inline bool operator<=(const Z& a, const Z& b)
{
   if(a<b || a==b)
      return true;

   return false;
}

inline bool operator<=(const Z& a, long int b)
{

   if(a<b || a==b)
      return true;

   return false;
}

inline bool operator<=(long int b, const Z& a)
{

   if(a<b || a==b)
      return true;

   return false;
}




inline bool operator>=(const Z& a, const Z& b)
{
   if(a>b || a==b)
      return true;

   return false;
}

inline bool operator>=(const Z& a, long int b)
{

   if(a>b || a==b)
      return true;

   return false;
}

inline bool operator>=(long int b, const Z& a)
{

   if(a>b || a==b)
      return true;

   return false;
}

// ---------------------------------------------------------------------------
// I/O
// ---------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& s, const Z& a)
{
   char str[mpz_sizeinbase(a.value, 10)+2];
   mpz_get_str(str, 10, a.value);

   s << str;
   return s;
}

inline std::istream& operator>>(std::istream& s, Z& a)
{
   std::string str;

   if(s.good())
   {
      char c = s.get();
      if(c=='-' || isdigit(c))
      {
         str.append(1, c);
      }
      else
      {
         std::cerr << "input format error" << std::endl;
         exit(0);
      }
   }

   while(s.good())
   {
      char c = s.get();
      if(isdigit(c))
         str.append(1, c);
      else if (isspace(c))
         break;
      else 
      {
         std::cerr << "input format error" << std::endl;
         exit(0);
      }
   }

   mpz_set_str(a.value, str.c_str(), 10);

   return s;
}


// ---------------------------------------------------------------------------
// Algorithms
// ---------------------------------------------------------------------------
inline Z gcd(const Z& a, const Z& b)
{
   Z result;
   mpz_gcd(result.value, a.value, b.value);
   return result;
}

inline Z gcd(const Z& a, unsigned long int b)
{
   Z result;
   mpz_gcd_ui(result.value, a.value, b);
   return result;
}

inline const Z& min(const Z& a, const Z& b)
{
	if(a<=b)
		return a;
	else
		return b;
}

inline Z abs(const Z& a)
{
	Z result;

	result = a;

	if(result<0) 
		result = result*(-1);

	return result;
}

inline Z next_prime(const Z& p)
{
   Z result;

   mpz_nextprime(result.value, p.value);

   return result;
}

inline Z sqrt(const Z& a)
{
   Z result;

   mpz_sqrt(result.value, a.value);

   return result;
}

inline Z random(const Z& n)
{
   Z result;
   static gmp_randstate_t state;
   static bool initialized = false;

   if(!initialized)
   {
      gmp_randinit_default(state);
      initialized=true;
   }

   mpz_urandomm(result.value, state, n.value);

   return result;
}



#endif


