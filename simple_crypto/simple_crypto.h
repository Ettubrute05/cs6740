#ifndef _SIMPLE_CRYPTO_H_
#define _SIMPLE_CRYPTO_H_

#define MAX_LEN 256

void cryptoStart();
void generateSubstitutionKey(char key, char *substitution);
void processMessage(const char *substitution, const char *inputFileName, const char *outputFileName, int encrypt);

#endif