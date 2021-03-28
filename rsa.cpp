#include "rsa.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;
using std::string;
using std::cout;
using std::endl;

const int ROUNDS = 10; //completely arbitrary, figure out how exactly this affects accuracy

InfInt powmodulo(InfInt baseOG, InfInt expOG, InfInt modOG) {
  //int thing = time(0);
  InfInt base = baseOG;
  InfInt exp = expOG;
  InfInt mod = modOG;

  InfInt result = 1;
  while (exp != 0) {
    if (exp % 2 == 1) result = (result * base) % mod;
    exp /= 2;
    base = (base * base) % mod;
  }
  //cout << "time taken is: "<<std::to_string(time(0) - thing) << endl;
  return result;
}

InfInt infpow(InfInt base, InfInt exp){
  InfInt result = 1;
  while(exp != 0){
    if(exp % 2 == 1) result = result*base;
    exp /= 2;
    base *= base;
  }
  return result;
}

rsa_pubkey rsa_privkeyToPubkey(rsa_privkey privkey){ //so funny lmao
  return get<1>(privkey);
}

rsa_ciphertext rsa_encrypt(rsa_plaintext plaintext, rsa_pubkey pubkey){
  //string toEncrypt = pad(plaintext); //TODO
  string toEncrypt = plaintext;

  InfInt m = 0;
  for(int i = 0; i < toEncrypt.length(); i++){
    m += int(toEncrypt[i]);
    m *= 0b100000000; //p sure this is right :P
  }
  m /= 0b100000000;
  cout << "plaintext as int" << endl;
  cout << m.toString() << endl; //delete later

  InfInt n = get<0>(pubkey); //p unnecessary just for clarity's sake
  InfInt e = get<1>(pubkey);
  return powmodulo(m, e, n);
}

rsa_plaintext rsa_decrypt (rsa_ciphertext ciphertext,rsa_privkey privkey){
  rsa_pubkey pubkey = get<1>(privkey);
  InfInt d = get<0>(privkey);
  InfInt n = get<0>(pubkey);

  InfInt decrypted = powmodulo(ciphertext, d, n);
  cout << "n is: " << n.toString() << endl;
  cout << "decrypted int" << endl;
  cout << decrypted.toString() << endl;

  string plaintext = "";
  while(decrypted > 0){
    //char c = decrypted & 0b11111111;
    char c = atoi((decrypted % 0b100000000).toString().c_str());
    decrypted /= 0b100000000;
    plaintext.insert(0, string(1, c));
  }
  //c = 0;
  //plaintext.insert(0, c);
  /*
  cout << "decrypted plaintext" << endl;
  cout << plaintext << endl; //check if this works lol
  */

  return plaintext;
}




string pad(rsa_plaintext){
  //LMAO USE A GENERIC AES TO PAD AS WELL
  //TODO
}

tuple<InfInt,InfInt> gcdExtended(InfInt a, InfInt b) {
   if (a == 0)
      return make_tuple(0, 1);
   auto x1y1 = gcdExtended(b%a, a);
   auto x1 = get<0>(x1y1);
   auto y1 = get<1>(x1y1);
   InfInt x = y1 - (b/a) * x1;
   InfInt y = x1;
   return make_tuple(x,y);
}

bool miller_rabin(InfInt testPrime){
  InfInt d = testPrime-1;
  int r;
  for(r = 0; d % 2 == 1; r++)
    d /= 2;

  srand(time(0));
  for(int i = 0; i < ROUNDS; i++){ //make sure 1 < e < totient(n) (the totient is (p-1)(q-1))
    InfInt random = rand();
    InfInt a = (random % testPrime-4) + 2; // 2 < a < testPrime-2

    InfInt x = powmodulo(a, d, testPrime);
    if(x == 1 || x == testPrime-1)
      continue;
    for(int j = 0; j < r; j++){
      x = powmodulo(x, 2, testPrime);
      if(x == testPrime - 1)
        continue;
      else
        return false;
    }
  }
  return true;
}

InfInt generatePrime(InfInt bits){
  InfInt MaybePrime = 0;

  for(InfInt i = 1; i < infpow(2, bits); i *= RAND_MAX){ //generating number in approximate range
    MaybePrime *= RAND_MAX;
    MaybePrime += rand();
  }

  if(MaybePrime % 2 == 0){ //evens aren't prime
    MaybePrime --;
  }

  cout << std::to_string(MaybePrime.size()) << endl;

  cout << "entering Miller Rabin Loop with : " << MaybePrime << endl;


  for(; !miller_rabin(MaybePrime); MaybePrime-= 2){
    cout << "trying: " << MaybePrime << endl;
  }
  return MaybePrime;
}

rsa_privkey rsa_keygen(int bits, InfInt e){
  int bitsOver2 = bits/2;
  cout << "trying to generate p " << endl;
  InfInt p = generatePrime(bitsOver2);
  cout << "generated p: " << p << endl;
  cout << "miller_rabin p: " << miller_rabin(p) << endl;
  cout << "miller_rabin p: " << miller_rabin(p) << endl;
  cout << "miller_rabin p: " << miller_rabin(p) << endl;
  cout << "miller_rabin p: " << miller_rabin(p) << endl;
  cout << "miller_rabin p: " << miller_rabin(p) << endl;
  cout << "trying to generate q" << endl;
  InfInt q = generatePrime(bitsOver2);
  cout << "generated q: " << q << endl;

  InfInt n = p*q;
  InfInt totient = (p-1)*(q-1);


  cout << "trying to find d "<< endl;
  InfInt d = get<0>(gcdExtended(e, totient));

  rsa_pubkey pubkey = make_tuple(n, e);
  rsa_privkey privkey = make_tuple(d, pubkey);
  return privkey;
}
