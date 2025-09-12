//if md5 header is not defined, define it
#ifndef MD5_H
#define MD5_H

//create a data type of type union uwb called WBunion
//unsure of what this is for right now
typedef union uwb
{
    unsigned w; //unsigned is short for unsigned int
    unsigned char b[4];
} WBunion;

//create a data type of type unsigned int[4] called digest
typedef unsigned Digest[4];

//function f0 returning an unsigned integer
unsigned f0(unsigned abcd[]);

//function f1 returning an unsigned integer
unsigned f1(unsigned abcd[]);

//function f2 returning an unsigned integer
unsigned f2(unsigned abcd[]);

//function f3 returning an unsigned integer
unsigned f3(unsigned abcd[]);

//create a new data type called DgstFctn
typedef unsigned (*DgstFctn)(unsigned a[]);

//function calcKs that returns a pointer to an unsigned integer
unsigned *calcKs(unsigned *k);

/**
 * function that performs a left binary shift
 * - v is the data we are performing the shift operation on
 * - amt is the number of bits we are shifting by
**/
unsigned rol(unsigned v, short amt);

/**
 * function that performs the MD5 hash
 * - msg is the string we are hashing
 * - mlen is the length of the string
**/
unsigned *md5(const char *msg, int mlen);

#endif