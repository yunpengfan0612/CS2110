// Yunpeng Fan

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

//define prototype

int main(int argc, char *argv[]);

// This is the array into where you will load the raw data from the file
char data_arr[0x36 + 240 * 160 * 4];
char *fileName;
int width;
int height;

int main(int argc, char *argv[]) {

	// 1. Make sure the user passed in the correct number of arguments

	if (argc != 2) {
		printf("%s\n", "You passed in wrong number of arguments!");
		return 0;
	}
	
	// 2. Open the file; if it doesn't exist, tell the user and then exit

	fileName = argv[1];
	FILE *openFile = fopen(fileName, "r");
	if (!openFile) {
		printf("The file does not exsit.\n");;
	}

	// 3. Read the file into the buffer then close it when you are done

	fread(data_arr, 1, 0x36 + 240 * 160 * 4, openFile);
	fclose(openFile);


	// 4. Get the width and height of the image

	width = *(int*) (data_arr + 0x12);
	height = *(int*) (data_arr + 0x16);
	//malloc allocate the requested memory and returns a pointer to it
	
	unsigned short *img_arr = malloc(sizeof(unsigned short) * width * height);
	//beginning of the pixel
	char *begPixel = data_arr + 0x36;
	unsigned short blue;
	unsigned short green;
	unsigned short red;
	//covert image into GBA mode 3 format but reversed order
	for (int i = 0; i < width * height; i++) {
		blue = (begPixel[i * 4] >> 3) & 0x001f;
		green = (begPixel[i * 4 + 1] >> 3) & 0x001f;
		red = (begPixel[i * 4 + 2] >> 3) & 0x001f;
		img_arr[i] = ((blue << 10) | (green << 5) | (red));
	}

	//reverse row order to get correct image information
	unsigned short *temp = malloc(sizeof(unsigned short) * width * height);
	//memcpy copy n bytes from source memory area to destination memory area
	for (int i = 0; i < height; i++) {
		memcpy(& temp[i * width], & img_arr[(height - i - 1) * width],
			sizeof(unsigned short) * width);
	}
	memcpy(img_arr, temp, sizeof(unsigned short) * width * height);
	//tell the machine the memory of temp area is not being used anymore
	free(temp);

	// 5. Create header file, and write header contents; close it

	
	char header[500];
	char outHfile[500];
	char outCfile[500];
	char cFile[500];

	/*The  strcpy()  function  copies the string pointed to by src, including
       the terminating null byte ('\0'), to the buffer  pointed  to  by  dest.
       The  strings  may  not overlap, and the destination string dest must be
       large enough to receive the copy.  Beware  of  buffer  overruns!   (See
       BUGS.)
	
       The  strcat() function appends the src string to the dest string, over‐
       writing the terminating null byte ('\0') at the end of dest,  and  then
       adds  a  terminating  null  byte.  The strings may not overlap, and the
       dest string must have enough space for the  result.   If  dest  is  not
       large  enough, program behavior is unpredictable; buffer overruns are a
       favorite avenue for attacking secure programs.*/

	strcpy(outHfile,fileName);
	strcat(outHfile,".h");
	strcpy(outCfile,fileName);
	strcat(outCfile,".c");

	/* print out the header and stored in .h file */
	FILE *hFIlept = fopen(outHfile, "w");
	sprintf(header, "#define IMAGE_WIDTH %d\n#define IMAGE_HEIGHT %d\nconst unsigned short IMAGE_DATA[%d];\n",
			width, height, width * height);
	fwrite(header, 1, strlen(header), hFIlept);
	fclose(hFIlept);

	// 6. Create c file, and write pixel data; close it

	FILE *cFIlept = fopen(outCfile, "w");
	sprintf(cFile, "const unsigned short IMAGE_DATA[%d] = {",
		width * height);
	fwrite(cFile, 1, strlen(cFile), cFIlept);

	int i = 0;
	while(i < width * height){
		sprintf(cFile, "0x%04x, ", img_arr[i]);
		fwrite(cFile, 1, strlen(cFile), cFIlept);
		i++;
		if(!(i % 8)){
			sprintf(cFile, "\n");
			fwrite(cFile, 1, strlen(cFile), cFIlept);
		}
	}

	sprintf(cFile, "};");
	fwrite(cFile, 1, strlen(cFile), cFIlept);

	fclose(cFIlept);
	free(img_arr);
	return 0;
}

