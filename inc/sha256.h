#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
static char *gen_wp(char *string, char *salt)
{
        int i;
        unsigned char digest[SHA256_DIGEST_LENGTH];
        static char buf[200];
        SHA256_CTX wp;
        SHA256_Init(&wp);
        SHA256_Update(&wp, string, strlen(string));
        SHA256_Update(&wp, salt, strlen(salt));
        SHA256_Final(digest, &wp);
        for(i=0; i<SHA256_DIGEST_LENGTH; i++)
        snprintf(&(buf[i*2]), SHA256_DIGEST_LENGTH * 2, "%02x", digest[i]);
        return (char *)buf;
}
