#include "types.h"

/*

After being decrypted, the file structure goes as following blocks.
They seem to always have the same blocks with CHKS (checks) being the last one.

*/

typedef struct{
    u32 name;
    u32 size; //note: this is the fullsize of the header of block + block data
} block;



/* There are 5 blocks... PSIN, PSIG, X, X, CHKS (I can't remember X's on the top of my head) */

/*
PSIN:
This block contains data relating to each player such as skills, money, and health.
It also contains data for cars.
The only thing I could find is a table of data which goes as the following:
*/

typedef struct{
    u32 id;
	u32 val;
} entry;

/*
  Entry table is possibly defined as:
  
  entry table[X]; //where X is total number of entries.. (also not sure if const value)
*/



/* 
CHKS:
This block just contains a check for file size (without padding), and a checksum of all the blocks in 1.
The chucksum used (for PS3) is jenkins one at a time hash algorithm.
It's 32bit integer. It can be calculated using the following function (see funcs.cpp)
*/

u32 get_save_hash(u8* buf, u32 len);
