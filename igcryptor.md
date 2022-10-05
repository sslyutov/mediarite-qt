[TOC]
##igcrypto library and utilty

###required files
|file|decsriptio|
|---|---|
|igcrypto.h|include this file into your sources|
|igcrypto.lib|link this file to your project|
|igcryptor.exe|command line utiltity|

## igcrypto library. 
the workspace is ns_igcrypto\n
###to encrypt a buffer of data call ns_igcrypto::encrypt
void encrypt(const char* origd, const uint32_t origc, char** cryptd, uint32_t* cryptc, char passkey[32]);\n
origd - pointer to the original data \n
origc - number of bytes in the buffer with the original data\n
cryptd - double pointer. pass a pointer to the char * which will provide an access to the encrypted data.\n
&emspIt is a caller responcibility to free memory returned over this parameter.
cryptc - pointer to uint32_t which receives number of bytes containing in the resulting data buffer.\n
passkey - cahr[32] passworrd. the same password has to be applied to for success full encruption and decryption.\n

###to deccrypt a buffer of data call ns_igcrypto::encrypt
void decrypt(char* cryptd, uint32_t cryptc, char** decryptd, uint32_t* decryptc, char passkey[32]);\n
cryptd - pointer to the buffer with the encrypted data.\n
cryptc - number of byte in the buffer pointed by cryptd\n
decryptd - double pointer. pointer to char* variable recieving an address of the buffer with the resultng data\n
decryptc - number of byte in the resultinng data buffer.\m
passkey - password used for encryption of data.


## igcryptor utility
igcryptor utility provides TEA file encryption functions\n
\n
format: igcryptor [operation] -in [filename] -out [filename] -pass [password]\n
[operation] - enc to ecrypt \n
              dec to decrypto\n
-in [filename] - specifies input file\n
-out[filename] - specifies output file\n
-pass[password] - password to crypting \n
all parameters are required\n
password upto 32 bytes leangth \n
                                                                               
examples:\n
&emspigcryptor enc - in original.txt -out encrypted.enc.txt -pass yourpassword\n
&emspigcryptor dec - in encrypted.txt -out decrypted.dec.txt -pass yourpassword\n
 

appendix\n
sources: https://en.m.wikipedia.org/wiki/Tiny_Encryption_Algorithm\n
