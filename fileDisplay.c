#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <crtdbg.h>


int length = 0;
int height = 0;

int map(int sum, int minOne, int maxOne, int minTwo, int maxTwo) {
  int index = -1;
  int diffFactor = (maxOne - minOne)/(maxTwo-minTwo);
  int temp = maxOne;
  for (int i = maxTwo-1; i >= minTwo && sum < temp; i--) {
    //printf("%i\n",temp);
    if (sum >= temp - diffFactor) {
      index = i;
    }
    temp -= diffFactor;
  }
  return index;
  
}


char ** pgmRead (char * name) {
  int ret;
  
  FILE * frame = fopen(name,"rb");
  
  if (frame == NULL) {
    printf("Error opening file");
  }
  //printf("file opened\n");

  char magicNumber[3];
  fread(magicNumber,sizeof(char),2,frame);
  magicNumber[2] = '\0';
  //printf("%s\n",magicNumber);
  //printf("magicNumber read\n");
  

  fgetc(frame);//skips reading \n char

  char nextLine[11];
  int curIndex = 0;
  char c = 0;
  while (c != '\n') {
    c = fgetc(frame);
    nextLine[curIndex] = c;
    curIndex++;
  }
  nextLine[curIndex] = '\0';
  //printf("nextLine filled\n");
  
  int spaceIndex = -1;
  char lenArray[5];
  char heightArray[5];
  for (int i = 0; i <= curIndex; i++) {
    if (spaceIndex < 0){
      if (nextLine[i] == ' ') {
	spaceIndex = i;
	lenArray[i] = '\0';
      } else {	
	lenArray[i] = nextLine[i];
      }
    } else {
      heightArray[i - (spaceIndex+1)] = nextLine[i];
    }
  }
  length = atoi(lenArray);
  height = atoi(heightArray);
  //printf("length and width stored\n");
  ret = fseek(frame,4,SEEK_CUR);
  //printf("skipped color value\n");
  
  char ** fileBuffer = (char**) malloc(height * sizeof(char *));
  for (int i = 0; i < height; i++) {
    fileBuffer[i] = (char *) malloc(length * sizeof(char));
  }
  //printf("file malloced\n");
  
  //int max = -1;
  //int cur;
  for (int i = 0; i < height; i++) {
    ret = fread(fileBuffer[i],sizeof(char),length,frame);
    if (ret < 0) {
      printf("Error reading file");
      break;
    }
    
    
    /*
    for (int j = 0; j < length; j++) {
      cur = abs(fileBuffer[i][j]);
      if (max < cur) {
	max = cur;
	printf("%i\n",max);
      }
    }
    */
    
  }
  fclose(frame);
  //printf("file read\n");
  return fileBuffer;
}
int freeFile (char ** fileBuffer) {
  for (int i = 0 ; i<height; i++) {
    free(fileBuffer[i]);
  }
  free(fileBuffer);
  return 1;
}

int compress(char ** fileBuffer, int blockSize) {
  
  FILE * textFrame = fopen("ASCIIframe.txt","w+");

  if (textFrame == NULL) {
    printf("Error opening file");
    return -1;
  }
  char newBuffer[height/blockSize][(length/(blockSize/2)) + 2];

  char density[] = "Ñ@#W$9876543210?!abc;:+=-,._ ";
  int densityLen = strlen(density);
  int x = 0;
  
  for (int i = 0; i < height; i+=blockSize) {
    int y = 0;
    for (int j = 0; j < length; j+=blockSize/2) {
      int sum = 0;
      for (int k = 0; k < blockSize; k++) {
	for (int z = 0; z < blockSize/2; z++) {
	  //printf("[%i, %i]",i+k,j+z);
	  
	  sum += fileBuffer[i+k][j+z];
	}
	//printf("\n");
      }
      //printf("%i ",sum);
      int arrayIndex = map(sum,-128*8,127*8,0,densityLen);
      //printf("%i,%i: %i,%c\n",x,y,sum,density[arrayIndex]);
      if (y > 320) {
	printf("Error");
      }
      newBuffer[x][y] = density[arrayIndex];
      /*
      if (sum < -8) {
	newBuffer[x][y] = '_';
      } else {
	newBuffer[x][y] = '@';
      }*/
      //printf("%i\n",y);
      //printf("%i,%i\n",i,j);
      y++;
    }
    newBuffer[x][y+1] = '\0';

    //printf("[%i, %i]\n",x,y);
    
    
    //fprintf(textFrame,"%s\n",newBuffer[x]);
    printf("%s\n",newBuffer[x]);
    
    x++;
   
  }
  
  //fprintf(textFrame,"%c",'\0');
  fclose(textFrame);
  return 1;
}

int main (int argc, char ** argv) {
    char ** file = pgmRead("alpha.pgm");
    printf("\033[s");
    compress(file,4);
    printf("\033[u");
    freeFile(file);
  
  
}

