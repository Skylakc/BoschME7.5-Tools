/* EEPROM 95040 Checksum calculator
   By 360trev (C Conversion from C# & update, original by Julex)
    
   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
   OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/
#include <ctype.h>
#include "load_file.h"
#include "save_file.h"

static int pageDesc[32] = { 0xFF18,0x0017,0x0117,0x0207,0x0307,0x0437,0x0533,0x06B7,0x06F7,0x07B3,0x07F3,0x08B7,0x08F7,0x09B3,0x09F3,0x0AB3,0x0AF3,0x0B32,0x0B10,0x0B10,0x0B10,0x0C37,0x0D33,0x0E33,0x0F33,0x1033,0x1133,0x1233,0x1235,0x1235,0x13B7,0x13F7 };
static int CSMask = 0x01;
static int CBMask = 0x40;
static unsigned char dataOut[16*32];
	
int main(int argc, char **argv)
{
  int pageNumber = 0;
  unsigned char *pInPos,*pOutPos,chk_byte1,chk_byte2;
  unsigned char *pIn;
  unsigned char modified;
  int corrected=0;
	size_t filelen;
  int offset,result;
  unsigned short checksum;
	
		printf("EEPROM 95040 Checksum calculator 1.01\n\n");
    if (argc < 3) { printf("Usage: %s <infile> <outfile>",argv[0]);	return 1; }

		// load then check length is 512 bytes exactly
		pIn = load_file(argv[1],&filelen);

		// validate loaded size is correct
		if(filelen != 512) { printf("file must be 512 bytes\n"); if(pIn != 0) free(pIn); return 2; }

    printf("        : 0-------------------1-------|----|\n");
    printf("        : 0 1 2 3 4 5 6 7 8 9 0 1 2 3 |4 5 |\n");
    for(pageNumber=0;pageNumber < 32;pageNumber++)
    {
        printf("Block %-2.2d: ",pageNumber);
    	// calculate current in and out positions
			pInPos  = pIn     + (pageNumber*16);
			pOutPos = dataOut + (pageNumber*16);

      // copy 16 bytes, in to out
      memcpy(pOutPos,pInPos, 16);
    
    	// calc checksums
     	checksum = 0;
      if ((pageDesc[pageNumber] & CSMask) == CSMask)
      {
        for (offset = 0; offset <= 13; offset++) { 
        	printf("%-2.2x",pInPos[offset]);
        	checksum = checksum + (int)pInPos[offset]; 
        }
        checksum = checksum + pageNumber;
        if ((pageDesc[pageNumber] & CBMask) == CBMask) { checksum = checksum - 1; }
        checksum = checksum * -1;

       	printf("[%-2.2x%-2.2x]",pInPos[14],pInPos[15]);

				// display in ascii
				printf(" ");
        for (offset = 0; offset <= 15; offset++) { 
		    	if(isprint(((unsigned char)pInPos[offset]))) {
 		      	printf("%c", ((unsigned char)pInPos[offset]));
		      } else {
		      	printf(".");
		      }
	       	if(offset == 13) printf(" ");
        }
				chk_byte1 = (unsigned char)((checksum     ) & 0x00ff);
				chk_byte2 = (unsigned char)((checksum >> 8) & 0x00ff);
 	 	   	printf(" Desc %-4.4x, New Chksum [%-1.1x%-1.1x]",pageDesc[pageNumber],chk_byte1,chk_byte2);
 	 	   	
 				// updated corrected checksums and count number of changes
   	   	modified=0;
 	 	   	if(pInPos[14] == (chk_byte1) ) { printf(" "); } else { printf("*"); modified=1; pOutPos[14] = chk_byte1;}
 	   	 	if(pInPos[15] == (chk_byte2) ) { printf(" "); } else { printf("*"); modified=1; pOutPos[15] = chk_byte2;}
				if(modified==1) { corrected++; }
				printf("\n");
					
     	} else {
				// still display info for blocks we skip to re-calculate
        for (offset = 0; offset <= 13; offset++) { 
        	printf("%-2.2x",pInPos[offset]);
        }
       	printf("[%-2.2x%-2.2x]",pInPos[14],pInPos[15]);

				// display in ascii
				printf(" ");
        for (offset = 0; offset <= 15; offset++) { 
		    	if(isprint(((unsigned char)pInPos[offset]))) {
 		      	printf("%c", ((unsigned char)pInPos[offset]));
		      } else {
		      	printf(".");
		      }
	       	if(offset == 13) printf(" ");
        }
      	printf(" Desc %-4.4x, Checksum Skip\n",pageDesc[pageNumber]);
    	}
    }

		printf("\n");
		if(corrected == 0) {
			printf("No checksums where corrected, file is OK already, skipping save.\n");
		} else {
			printf("%d checksum(s) where corrected in this file.\n",corrected);
	 		// write out new file   
	    result = save_file((char *)argv[2],(unsigned char *)dataOut,(size_t)512);
		}

		// free file buffer
		if(pIn != 0) free(pIn);

		return 0;
}
