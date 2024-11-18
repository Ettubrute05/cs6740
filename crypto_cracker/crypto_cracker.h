
#ifndef CRYPTO_CRACKER_H
#define CRYPTO_CRACKER_H

#define MAX_TEXT_SIZE 21000
#define MAX_KEY_LENGTH 12
#define ALPHABET_SIZE 26

void cryptoCrackerStart();
void cryptoCracker();
int readFile(char* filename, char *content, int maxlength);
int detectCaesarCipher(const char *text);
void decryptCaesarCipher(const char *text, int key, char *output);
int detectVigenereCipher(const char *text);
void findVigenereKey(const char *text, int keyLength, char *key);
void decryptVigenereCipher(const char *text, const char *key, char *output);
int detectColumnarCipher(const char *text);
int decryptColumnarCipher(const char *text, char *output);
int containsCommonWords(const char *text);

#endif //CRYPTO_CRACKER_H
