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

InfInt powmodulo(InfInt base, InfInt exp, InfInt mod) {
  //int thing = time(0);
  InfInt result = 1;
  while (exp != 0) {
    if (exp % 2 == 1) result = (result * base) % mod;
    exp /= 2;
    base = (base * base) % mod;
  }
  //cout << "time taken is: "<<std::to_string(time(0) - thing) << endl;
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



/*

string pad(rsa_plaintext){
  //TODO
}

rsa_privkey rsakeygen(){
  //TODO
}

bool miller_rabin(int testPrime){
  cur = testPrime-1; //do later
  prev = cur;
  while(cur > 0){
    prev = cur;
    cur = cur<<1;
  }

  for(int i = 0; i < ROUNDS; i++){ //make sure 1 < e < totient(n) (the totient is (p-1)(q-1))
      srand(time(0));
      int a = rand() % (testPrime-4) + 2;

      int x = powmodulo(a, )
  }
  return true;
}

*/
