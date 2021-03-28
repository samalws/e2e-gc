#ifndef rsa_h
#define rsa_h
#include "InfInt.h"
#include<tuple>
using namespace std;
#include <string>

typedef tuple <InfInt, InfInt> rsa_pubkey; //first is n, second is e
typedef tuple <InfInt, rsa_pubkey> rsa_privkey;
typedef std::string rsa_plaintext;
typedef InfInt rsa_ciphertext;
typedef InfInt rsa_signature;

rsa_privkey rsa_keygen(int bits, InfInt e);
rsa_pubkey  rsa_privkeyToPubkey(rsa_privkey);
rsa_ciphertext rsa_encrypt (rsa_plaintext,rsa_pubkey);
rsa_plaintext rsa_decrypt (rsa_ciphertext,rsa_privkey);
rsa_signature rsa_sign (rsa_ciphertext,rsa_privkey,InfInt(*hash)(rsa_ciphertext));
bool rsa_verify (rsa_signature, rsa_pubkey, InfInt(*hash)(rsa_ciphertext));

#endif
