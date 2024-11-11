
#ifndef CRYPTO_CRACKER_H
#define CRYPTO_CRACKER_H

void cryptoCrackerStart();
void cryptoCracker();
int readFile(char* filename, char *content, int maxlength);
int detectCaesarCipher(const char *text);
void decryptCaesarCipher(const char *text, int key, char *output);
int detectVigenereCipher(const char *text);
void findVigenereKey(const char *text, int keyLength, char *key);
void decryptVigenereCipher(const char *text, const char *key, char *output);
int detectColumnarCipher(const char *text);
void decryptColumnarCipher(const char *text, int keyLength, char *output);

#endif //CRYPTO_CRACKER_H
