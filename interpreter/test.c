#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define _FILE_OFFSET_BITS 64

int main(int argc,char ** argv){
  FILE *readf, *config;
  off_t offset;
  char writebuf[20];
  int ret;

  readf = fopen(argv[1],"r");
  offset = atoll(argv[2]);
  ret = fseeko(readf,offset,SEEK_SET);
  if(ret < 0){
    printf("reached end of file?\n");
  }
  config = fopen(".linksconfig.txt","w");
  fgets(writebuf,2,readf);
  printf("got %c\n",writebuf[0]);

  offset = ftello(readf);
  sprintf(writebuf,"%jd",offset);
  printf("writing %jd\n",offset);
  fwrite(writebuf,sizeof(char),strlen(writebuf),config);
  close(config);
  close(readf);

  printf("continueing\n");
  readf = fopen(argv[1],"r");
  config = fopen(".linksconfig.txt","r");
  fgets(writebuf,20,config);
  offset = atoll(writebuf);
  printf("got back %jd\n",offset);
  ret = fseeko(readf,offset,SEEK_SET);
  if(ret < 0){
    printf("reached end of file?\n");
  }
  fgets(writebuf,2,readf);
  printf("got %c\n",writebuf[0]);
}
