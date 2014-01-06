#include "keys.h"
#include "Structs.h"

void readBlocks( u8* inputData );
void resignFile( u8* fdata, u64 fsize );

int main(int argc, char* args[]){

  u8* fileBuffer = ReadFile( args[1] ); //savegame such as RAGE.SAV AFTER PS3/XBOX encryption
  
  readBlocks( &fileBuffer );
  
  resignFile( &fileBuffer, filesize( args[1] ) );
  
  return 0;
}

void readBlocks( u8* inputData ){
  u8* decData = AES_DECRYPT( ECB, 128, 256, PS3_SAVE_KEY, &inputData ); //this is just your decrypted data
  block blocks[5];
  u8* blocksData[5];
  u32 pos = 0, i = 0;
  
  for(; i<5; i++){
  	blocks[i].name = ES32(*(u32 *)(&decData[ pos ])); //ES32 == endian swap 32bit
  	blocks[i].size = ES32(*(u32 *)(&decData[ pos + 4]));
  	blocksData[i]  = &decData[ pos + 8 ];
  	pos += blocks[i].size;
  }
}

u32 get_save_hash(u8* buf, u32 len)
{
    u32 hash = 0x3FAC7125, i = 0;
    if (len != 0)
    {
      for (; i < len; ++i)
      {
        hash += (signed char)buf[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
	    }
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/*
NOTE: IN ORDER FOR THIS TO WORK YOU MUST IGNORE THE PADDING, 
AND ALSO SET THE VALUES OF LENGTH AND HASH IN CHKS TO 0.
YOU CAN DO THAT WITH THE FOLLOWING:
*/

void resignFile( u8* fdata, u64 fsize ){
  memset(&fdata[fsize-0xC], 0, 8); //null out size + hash
	u32 new_hash = get_save_hash(fdata, fsize); //default hash key
	u32 nfsize, nhash;
	nfsize = ES32(fsize); //endian swap
	nhash = ES32(new_hash);
	memcpy( &fdata[fsize-0xC], (u32*)&nfsize, 4); //write the new size (should be the same)
	memcpy( &fdata[fsize-0x8], (u32*)&nhash, 4); //write the new hash..
}
