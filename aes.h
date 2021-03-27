#ifndef aes_h
#define aes_h

#include <string>

typedef int aes_key;
typedef std::string aes_plaintext;
typedef std::string aes_encrypted;

aes_key aes_keygen();
aes_encrypted aes_encrypt(aes_plaintext,aes_key);
aes_plaintext aes_decrypt(aes_encrypted,aes_key);

#endif
