/* Simple write file from memory
 
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
#include "save_file.h"

/* load a file into memory and return buffer */
int save_file(char *filename, unsigned char *filebuf, size_t filelen)
{
	FILE *fp;
	size_t bytesWritten;

	/* open file */
	printf("þ Opening '%s' file for writing\n",filename);
	if ((fp = (FILE *)fopen(filename, "wb")) == NULL){ printf("\nCan't open file \"%s\".", filename); return(-1); }

	/* load file into buffer */
	printf("þ Writing to file\n");
	bytesWritten = fwrite((void *)filebuf, (size_t)1, (size_t)filelen, fp);

	/* validate it all loaded correctly */
	printf("þ Validating size correct %d=%d\n",(int)bytesWritten,(int)filelen);
	if(bytesWritten != filelen) { printf("\nfailed to write buffer\n"); fclose(fp); return(-2); }

	/* close the file */
	printf("þ All OK, closing file\n\n");
	fclose(fp);

	return(0);
}
