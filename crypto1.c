#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>


// VARIABLES FOR TRACE/PRINTS
int p1 = 0 ; // for showing how 5 bits of random number were obtained
int p2 = 0 ; // for showing trace of not prime
int p3 = 0 ; // for showing trace of prime
int p4 = 0 ; // for showing trace of computing e
int p5 = 0 ; // for showing trace of computing d
int p6 = 0 ; // for showing trace of computing E(e,v)

/**************************************************************************
*  FUNCTION : mod 
*  INPUT : x , y
*  OUTPUT : x modulo y
*  DESCRIPTION : calculates x modulo y and returns the result
**************************************************************************/
uint32_t mod (uint32_t x, uint32_t y) {
   return (x -(y * (x/y)));
}

/**************************************************************************
*  FUNCTION : set_bit 
*  INPUT : number , position
*  OUTPUT : number
*  DESCRIPTION : sets bit(sets as 1) in 'number ' at position 'position' 
*  and returns the number . 
**************************************************************************/
// for the following LSB at position 0 and move towards MSB
uint32_t set_bit (uint32_t number, uint32_t position) {
   return (number | (1 << position));
}

/**************************************************************************
*  FUNCTION : unset_bit 
*  INPUT : number , position
*  OUTPUT : number
*  DESCRIPTION : unsets bit(sets as 0) in 'number ' at position 'position'
*  and returns the number . 
**************************************************************************/
uint32_t unset_bit (uint32_t number, uint32_t position) {
   return (number & (~(1 << position)));
}

/**************************************************************************
*  FUNCTION : get_bit 
*  INPUT : number , position
*  OUTPUT : number
*  DESCRIPTION : fetches bit at position 'position' in 'number ' 
*  and returns that bit . 
**************************************************************************/
uint32_t get_bit (uint32_t number, uint32_t position) {
   return ((number & (1 << position)) >> position);
}

/**************************************************************************
*  FUNCTION : find_msb_one 
*  INPUT : number 
*  OUTPUT : position 
*  DESCRIPTION : finds the position of the leftmost set bit 
*  and returns the position . 
**************************************************************************/
uint32_t find_msb_one (uint32_t number) {
   uint32_t position = 0;
   
   while (number >>= 1)
      position++ ;
   
   return (position);
}

/**************************************************************************
*  FUNCTION : printbits 
*  INPUT : a 1 byte number (v)
*  OUTPUT : NONE
*  DESCRIPTION : prints the binary represenation of a variable of size 1 byte 
**************************************************************************/
void printbits(uint8_t v) {
  int i; 
  for(i = 7; i >= 0; i--) putchar('0' + ((v >> i) & 1));
}

/**************************************************************************
*  FUNCTION : print32bits 
*  INPUT : a 4 byte number (v)
*  OUTPUT : NONE
*  DESCRIPTION : prints the binary represenation of a variable of size 4 byte 
**************************************************************************/
void print32bits(uint32_t v) {
   printbits((v>>24)&0xff);
   printbits((v>>16)&0xff);
   printbits((v>>8)&0xff);
   printbits((v)&0xff);
}

/**************************************************************************
*  FUNCTION : print_prime_trace 
*  INPUT : candidate, a, line
*  OUTPUT : returns 1 after printing prime, 0 for non prime
*  DESCRIPTION : prints trace of Miller-Rabin Algo for primaliy testing. We
*  identify if candidate is prime or not using input 'line' . Line 122 is for 
*  not prime and Line 126 is for prime. For not prime candidate, we have input 
*  a for which test fails and we print the trace. For a prime candidate, we 
*  have input a for which test succeeds and we print the trace. 
**************************************************************************/
uint32_t print_prime_trace (uint32_t candidate, uint32_t a, uint32_t line) {

   uint32_t i ;
   int32_t j ;
   uint32_t y ;
   uint32_t z ;
   uint32_t flag ;

      flag = 1; // to check where candidate fails if its non prime 
      
      printf("\nLine %d\nTrace : n = %d, a = %d\n",line,candidate,a);
      
      y = 1 ;
      // can use find_msb_one also
      //for (j = find_msb_one(candidate-1); j>=0; j--) {
      for (j = (uint32_t)((log(candidate-1))/log(2)); j>=0; j--) {
         z = y;
         y = mod((y*y),candidate);
         printf("j = %d, x = %d, z = %d, y1 = %d, ",j,get_bit((candidate-1),(uint32_t)j),z,y);
         if ((y == 1) && (z != 1) && (z != (candidate - 1))) {
            flag = 0;
            break;
         }
         if (get_bit((candidate-1),(uint32_t)j) == 1) {
            y = mod((y*a), candidate);
         }
         printf("y2 = %d\n",y);
      
      } 
     
   if (line == 126) {
      printf("candidate :%d is perhaps a prime\n",candidate);   
      return 1;
   }
   else {
      if (flag == 0) printf("\n");
      printf("candidate :%d is not a prime because ",candidate);   
      if (flag == 0){
         printf("%d mod %d = 1 and %d is not equal to 1 or %d\n",z,candidate,z,candidate-1);
      }
      else {
         printf("%d^%d mod %d != 1\n",a,candidate-1,candidate);   
      }
      return 0;
   }
}

/**************************************************************************
*  FUNCTION : print_known_not_prime_trace 
*  INPUT : None
*  OUTPUT : None
*  DESCRIPTION : finds a known non prime(we try all even numbers) for which 
*  primality test fails, finds candidate, a and then calls 
*  print_prime_trace to print the trace. 
**************************************************************************/
void print_known_not_prime_trace () { 

   uint32_t i ;
   int32_t j ;
   uint32_t a ;
   uint32_t y ;
   uint32_t z ;
   uint32_t flag ;
   uint32_t candidate;

   for (candidate = 4; ; candidate += 2) {
      for (i = 0; i < 20; i++) {
         flag = 1; 
         
         do { a = mod((uint32_t)rand() , candidate); } while (a == 0 );
         
         y = 1 ;
         // can use find_msb_one also
         //for (j = find_msb_one(candidate-1); j>=0; j--) 
         for (j = (uint32_t)((log(candidate-1))/log(2)); j>=0; j--) {
            z = y;
            y = mod((y*y), candidate);
            if ((y == 1) && (z != 1) && (z != (candidate - 1))) {
               flag = 0;
               break;
            }
            if (get_bit(candidate-1,(uint32_t)j) == 1) {
               y = mod((y*a), candidate);
            }
         
         } // check for particular a complete
         if ((flag == 0) || (y != 1) )  {
            //not prime 
            flag = 0;
            break;
         }
      }
      if (flag == 0) {
         // found a not prime candidate so we break 
         break;
      }
   }
   print_prime_trace(candidate,a,122);
   return ;
}


/**************************************************************************
*  FUNCTION : is_prime 
*  INPUT : candidate that is to be checked 
*  OUTPUT : 1 if candidate is prime else 0
*  DESCRIPTION : uses Miller-Rabin Algo for primality testing for 
*  different values of a(max 20).  
**************************************************************************/
uint32_t is_prime (uint32_t candidate) { // using Miller-Rabin Algo

   uint32_t i ;
   int32_t j ;
   uint32_t a ;
   uint32_t y ;
   uint32_t z ;
   uint32_t flag ;

   for (i = 0; i < 20; i++) {
      // Miller rabin code for particular value of a
      flag = 1; // assume it a prime & test will pass for this a
      
      do { a = mod((uint32_t)rand(), candidate); } while (a == 0 );      
      
      y = 1 ;
      // can use find_msb_one also
      //for (j = find_msb_one(candidate-1); j>=0; j--) 
      for (j = (uint32_t)((log(candidate-1))/log(2)); j>=0; j--) {
         z = y;
         y = mod((y*y), candidate);
         if ((y == 1) && (z != 1) && (z != (candidate - 1))) {
            flag = 0;
            break;
         }
         if (get_bit((candidate-1),(uint32_t)j) == 1) {
            y = mod((y*a), candidate);
         }
      
      } // check for a particular a complete
      if ((flag == 0) || (y != 1) )  {
         
         flag = 0;
         break;
      }
     
   }
   if (flag == 0) { // Case : not prime
      if ((p2 == 0) && (p3 == 0)) { 
         print_prime_trace(candidate, a, 122); // Line 122 -for not prime trace
         p2 = 1;
      }
      return 0;
   }
   else { // Case : perhaps prime

      if ((p3 == 0) && (p2 == 1)) {
         // Case : not prime trace already printed, only prime trace to print
         print_prime_trace(candidate, a, 126); // Line 126 - for prime trace
         p3 = 1;
      }
      else if ((p3 == 0) && (p2 == 0)) {
         // Case : both trace(not prime & prime) to be printed 
         print_known_not_prime_trace(); 
         p2 = 1;
         print_prime_trace(candidate, a, 126); // Line 122- for prime trace
         p3 = 1;
      }
      return 1;
   }
}


/**************************************************************************
*  FUNCTION : get_prime 
*  INPUT : NONE
*  OUTPUT : a prime number
*  DESCRIPTION : chooses a candidate randomly, checks if its prime and  
*  returns the prime on finding a successful candidate.  
**************************************************************************/
uint32_t get_prime () {
   
   uint32_t candidate;
   int random_number;
   int i;

   do {        
      candidate = 0x00000041; // initialise candidate, set bit 0 and 7 as 1
      if (p1 == 0 ){
         printf("Line 106\nBit 0 : set as 1\n");   
      }
      for (i=1 ; i<=5 ; i++) {
         
         random_number = rand();
         if ( mod((uint32_t)random_number, 2) == 0 ) { // even number ;LSB is 0
            if (p1 == 0 ){
               printf("Bit %d : number ",i);
               print32bits(random_number);
               printf(" extracts 0\n");   
            }
            candidate = unset_bit(candidate,i); // set ith bit in p as 0
         }
         else { // odd number; so LSB is 1
            if (p1 == 0 ){
               printf("Bit %d : number ",i);
               print32bits(random_number);
               printf(" extracts 1\n");   
            }
            candidate = set_bit(candidate,i); // set ith bit in p as 1
         }
      }

      if (p1 == 0 ){
         printf("Bit 7 : set as 1\n");
         printf("random number formed: %d= ",candidate);
         print32bits(candidate);
         printf("\n");
         p1++;
      }
   } while (!is_prime(candidate));
   
   return candidate;

} // end get_prime

/**************************************************************************
*  FUNCTION : find_keys 
*  INPUT : pointer to encryption key(e),pointer to decryption key(d),
*  euler_value
*  OUTPUT : NONE
*  DESCRIPTION : uses the extended Euclidean Algo to find e and d, on finding  
*  them updates the e and d pointers. If tried all values, still don't find
*  a 'e' , return e=d=0  
**************************************************************************/
void find_keys(uint32_t *e, uint32_t *d, uint32_t euler_value) {
   uint32_t i,k;
   uint32_t r1,r2,r3;
   uint32_t q,q1,q2;
   int32_t s,s1,s2;
   int32_t t,t1,t2;
   uint32_t flag = 0; // value 1 indicates found keys

   if (p4 == 0) {
      printf("\nLine 145");
   }   

   for (k = 3; k < euler_value; k++  ) {
      // Initialisations :
      r1 = euler_value;
      r2 = k ; // try k as possible e value
      s1 = 1;
      s2 = 0;
      t1 = 0;
      t2 = 1;
     
      if (p4 == 0) {
         printf("\ntrying e = %d \n",k);
      }

      i = 1;
      while (1) {
         q = r1 / r2;
         r3 = mod(r1, r2);
         if (i == 1) {
            s = s1;
            t = t1;
            q1 = q;
         }
         else if(i == 2) {
            s = s2;
            t = t2;
            q2 = q;
         }
         else {
            if (mod(i, 2) == 0) {
               s = s2 - (q2 * s1);
               s2 = s;
               t = t2 - (q2 * t1);
               t2 = t;
               q2 = q;
            }
            else {
            
               s = s1 - (q1 * s2);
               s1 = s;
               t = t1 - (q1 * t2);
               t1 = t;
               q1 = q;
            }
         }
         if (p4 == 0) {
            printf("i =%d, qi=%d, ri=%d, r(i+1)=%d, r(i+2)=%d",i,q,r1,r2,r3);
            printf(", si=%d, ti=%d\n",s,t);
         }

         r1 = r2 ;
         r2 = r3 ;
         i++ ;
         if (r2 == 0) { 
            if (i == 1) {
               s = s1;
               t = t1;
            }
            else if(i == 2) {
               s = s2;
               t = t2;
            }          
               
            else { 
               if (mod(i , 2) == 0) {
                  s = s2 - (q2 * s1);
                  t = t2 - (q2 * t1);
               }
               else {
                  s = s1 - (q1 * s2);
                  t = t1 - (q1 * t2);
               }
            }
            if (p4 == 0) {
               printf("i =%d, ri=%d, si=%d, ti=%d\n",i,r1,s,t);
            }
            break ;
         }
      }
      if (r1 == 1) {
         flag = 1; // found keys 
         break;
      }
   } // end for
   if (flag == 1) {
      *e = k;
      p4 = 1; // trace complete for possible e values(for Alice only)
      if (t > 0) {
         *d = t;
      }
      else { 
         *d = t + euler_value;
      }
      if(p5 == 0){
         printf("\nLine 155\nd = %d \n",*d);
         p5 = 1;
      }
   }
   else {
      *e = 0;
      *d = 0;
   }
   return ;
}

/**************************************************************************
*  FUNCTION : generate_rsa 
*  INPUT : pointer to prime1(p), pointer to prime2(q),pointer to n,
*  pointer to euler_value,pointer to encryption key(e),
*  pointer to decryption key(d)
*  OUTPUT : NONE
*  DESCRIPTION : generates the RSA public/private cryptosystem and on finding   
*  all parameters(p,q,n,euler_value,e,d) updates all pointers.  
**************************************************************************/
void generate_rsa (uint32_t *p, uint32_t *q, uint32_t *n, \
                  uint32_t *euler_value, uint32_t *e, uint32_t *d) {
    
   do {
   
      *p = get_prime();
      do { *q = get_prime();} while (*p == *q);  // p and q should be different
      
      *n = (*p) * (*q);
      *euler_value = ( (*p) - 1 ) * ( (*q) - 1 );
      find_keys(e, d, *euler_value) ;
   
   } while ((*e == 0) && (*d == 0));  // check till we get two keys
   
   return;
}

/**************************************************************************
*  FUNCTION : fast_exponentiation 
*  INPUT : a, x, n
*  OUTPUT :  y = (a raise to power x ) mod n
*  DESCRIPTION : calculates y = (a raise to power x ) mod n using fast
*  exponentiation and returns y  
**************************************************************************/
uint32_t fast_exponentiation(a, x, n ) {
   
   int i ;
   uint32_t y = 1;
   if (p6 == 1) {
      printf ("\nLine 226\nTrace : a = %d, x = %d, n = %d\n",a,x,n);
   } 
   for (i = find_msb_one(x); i>=0; i-- ) {
      y = mod( (y*y) , n );
      if (p6 == 1) {
         printf("i = %d, x = %d, y1 = %d, ",i,get_bit(x,(uint32_t)i),y);
      }
      if (get_bit(x,(uint32_t)i) == 1) {
         y = mod((y * a) , n);
      }
      if (p6 == 1) {
         printf("y2 = %d\n",y);
      }
   }
   
   return y ;
}


int main () {

   int i, j ;
   
   // Follwing define Alice's RSA system
   uint32_t p ; // first prime
   uint32_t q ; // second prime
   uint32_t n ; // = p*q
   uint32_t euler_value ; // = (p-1)(q-1)
   uint32_t e ; // encryption key
   uint32_t d ; // decryption key

   // Follwing define Trent's RSA system 
   uint32_t trent_p ; // first prime
   uint32_t trent_q ; // second prime
   uint32_t trent_n ; // = p*q
   uint32_t trent_euler_value ; // = (p-1)(q-1)
   uint32_t trent_e ; // encryption key
   uint32_t trent_d ; // decryption key
  
   // Following are for digital certificate for Alice
   uint8_t r[14] ;      // 14 byte array to store certificate
   uint32_t h_r = 0 ;   // stores hash of r, hash will be 1 byte only
   uint32_t s ;         // stores Trent's signature on r

   // Following are for Alice authenticates herself to Bob
   uint32_t u = 0 ;
   uint32_t k ;
   uint32_t h_u ; // stores hash of u , hash will be 1 byte only
   uint32_t v ; // = D (d, h(u))
   uint32_t w ;   // stores encrypted value of v

   srand(time(NULL)); 
 
   // Step 1 : generate RSA system 
   generate_rsa(&p, &q, &n, &euler_value, &e, &d);
   
   // Trace for Alice's RSA system :
   printf("\nLine 159\nInteger:\n");   
   printf("p:%d, q:%d, n:%d, ",p,q,n);
   printf("e:%d, d: %d\nBits:\n",e,d);
   printf("p: ");
   print32bits(p);
   printf("\n");
   printf("q: ");
   print32bits(q);
   printf("\n");
   printf("n: ");
   print32bits(n);
   printf("\n");
   printf("e: ");
   print32bits(e);
   printf("\n");
   printf("d: ");
   print32bits(d);
   printf("\n");
 
   generate_rsa(&trent_p, &trent_q, &trent_n, &trent_euler_value, \
                  &trent_e, &trent_d);
   
   // Step 2 : Creating digital certificate for Alice
   
   // Step 2.1 : compute the certificate i.e. r
   r[0] = 0; 
   r[1] = 65;  // ASCII for A 
   r[2] = 108; // ASCII for l
   r[3] = 105; // ASCII for i
   r[4] = 99;  // ASCII for c
   r[5] = 101; // ASCII for e
   
   r[6] = (n >> 24) & 0xff;  // high-order (leftmost) byte: bits 24-31
   r[7] = (n >> 16) & 0xff;  // next byte, counting from left: bits 16-23
   r[8] = (n >>  8) & 0xff;  // next byte, bits 8-15
   r[9] = n & 0xff;  // low-order byte: bits 0-7
   
   r[10] = (e >> 24) & 0xff;  // high-order (leftmost) byte: bits 24-31
   r[11] = (e >> 16) & 0xff;  // next byte, counting from left: bits 16-23
   r[12] = (e >>  8) & 0xff;  // next byte, bits 8-15
   r[13] = e & 0xff;  // low-order byte: bits 0-7
   
   // Step 2.2 : compute the hash of r i.e. h_r
   for (i = 0; i < 14; i++ ) {
      h_r ^= r[i];
   }

   // Step 2.3 : compute the signature on r i.e. s 
   //            by decrypting h_r using trent_d, trent_n
   s = fast_exponentiation(h_r, trent_d, trent_n);  
   
   // Trace for digital certificate
   printf("\nLine 188\nr = ");
   for (i = 0; i<14; i++){
      printbits(r[i]);
   }
   printf("\nh(r) = "); 
   print32bits(h_r) ;
   printf("\ns = "); 
   print32bits(s) ;
   
   printf("\n\nLine 190\nh(r) = %d\ns = %d\n",h_r,s);
 
   // Step 3 : Alice authenticates to Bob
   k = find_msb_one(n);
  
   // generate the random number u, s.t. u < n
   do {        
      u = 0 ;
      u = set_bit(u,k-1);
      for (i = 0 ; i< (k-1); i++) {
         
         j = rand();
         if (mod((uint32_t)j , 2) == 0) { // even number ; so LSB is 0
            u = unset_bit(u,i); // set ith bit in p as 0
         }
         else { // odd number; so LSB is 1
            u = set_bit(u,i); // set ith bit in p as 1
         }
      }
   } while (!is_prime(u));
  
   // Trace for k and u 
   printf("\nLine 214\nk= %d, u=%d\n",k,u);

   printf("\nLine 216\nu = ");
   print32bits(u);
   printf("\n");
  
   h_u = ((u >> 24) & 0xff) ^((u >> 16) & 0xff) ^((u >> 8) & 0xff) ^ (u & 0xff) ;
   v = fast_exponentiation(h_u, d, n);  
   w = fast_exponentiation(v, e, n);

   // Trace for u, h_u, v, w
   printf("\nLine 222\nu = %d : ",u);
   print32bits(u);
   printf("\nh(u) = %d : ",h_u);
   print32bits(h_u);
   printf("\nv = (D (d, h(u))) = %d : ",v);
   print32bits(v);
   printf("\nE(e,v) = %d : ",w);
   print32bits(w);
   printf("\n");

   // Trace for E(e,v)
   p6 = 1 ;
   fast_exponentiation(v, e, n);
   p6 = 0 ;
   printf("\n");
   
   //if (w == h_u ) printf("\nSuccess\n");

   return 0;
}   

