/* Simple load file into memory
 
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
#include "load_file.h"

/* load a file into memory and return buffer */
unsigned char *load_file(char *filename, size_t *filelen)
{
	FILE *fp;
	unsigned char *data;
	size_t size,bytesRead;

	/* open file */
	printf("þ Opening '%s' file\n",filename);
	if ((fp = (FILE *)fopen(filename, "rb")) == NULL){ printf("\nCan't open file \"%s\".", filename); return(0); }

	/* get file length */
	printf("þ Getting length of '%s' file\n",filename);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(size <= 0) { printf("Error: Problem with seeking filesize\n"); fclose(fp); return(0); }
		
	*filelen = size;		/* return size of file to caller */

	/* alloc buffer for file */
	printf("þ Allocating buffer of %d bytes\n",size);
	data = (unsigned char *)malloc(size);
	if(data == 0) { printf("\nfailed to allocate memory to load module\n"); fclose(fp); return 0; }

	/* load file into buffer */
	printf("þ Reading file to buffer\n");
	bytesRead = fread(data, 1, size, fp);

	/* validate it all loaded correctly */
	printf("þ Validating size correct %d=%d\n",bytesRead,size);
	if(bytesRead != size) { printf("\nfailed to load module into buffer\n"); free(data); fclose(fp); return 0; }

	/* close the file */
	printf("þ Closing file\n\n");
	fclose(fp);
	return(data);
}
