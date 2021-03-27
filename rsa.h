#ifndef rsa_h
#define rsa_h

#include <string>

typedef int rsa_privkey;
typedef int rsa_pubkey;
typedef std::string rsa_plaintext;
typedef std::string rsa_encrypted;

rsa_privkey rsa_keygen();
rsa_pubkey  rsa_privkeyToPubkey(rsa_privkey);
rsa_encrypted rsa_encrypt (rsa_plaintext,rsa_pubkey);
rsa_plaintext rsa_decrypt (rsa_encrypted,rsa_privkey);

#endif
