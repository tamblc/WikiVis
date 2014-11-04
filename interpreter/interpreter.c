#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void writedata(FILE *readf, int numtoread);

#define READSIZE 800
#define	WRITESIZE 300
#define PL_TITLE_MAX 255
#define DEBUG

int main(int argc, char ** argv){
	FILE *readf, *writef;
	char data;
	int numtoread;
	if(argc < 2){
		printf("error: need 2 arguments. Name of source file,num of lines to read\n");
		exit(EXIT_FAILURE);
	}
	readf = fopen(argv[1],"r");
	if(!readf){
		printf("error: source file does not exist\n");
		exit(EXIT_FAILURE);
	}

	writedata(readf,numtoread);
	fclose(readf);
	exit(EXIT_SUCCESS);
}

void writedata(FILE *readf, int numtoread){
	char readbuf[READSIZE], writebuf[WRITESIZE], *itr, *placeholder;
	FILE * currentwritefile = NULL;
	int curcat = 0;
	int i,pl_from,pl_namespace,scratch;
	char sentenial = 0, pl_title_partial[PL_TITLE_MAX];

	fgets(readbuf,READSIZE,readf);
	//ignore the first couple of lines that appear in the files
	while(readbuf[0] != 'I'){
		fgets(readbuf,READSIZE,readf);
	}
	currentwritefile = fopen("0.txt","w+");
	for(i = 0; i < numtoread; ++i){
		itr = NULL;
		sentenial = 0;
		while(!sentenial){
			//start our our line
			if(!itr){
				itr = strtok(readbuf,"(");
				printf("on line %d",i);
			}else{//we have already advanced to the next element
				itr = strtok(NULL,"(");
			}
			//last char in our buffer is either )or, we don't care for it.
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				if(readbuf[0] == 'I'){
					sentenial = 1;
					break;
				}
			}
#ifdef DEBUG
			printf("%s\n",itr);
#endif
			pl_from = atoi(itr);
			
			itr = strtok(NULL,",");
			//means our pl_from was cut
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				itr = strtok(readbuf,"(");
				scratch = atoi(readbuf);
				pl_from = pl_from*pow(10.0,(float)strlen(readbuf)) + scratch;
			}
			pl_namespace = atoi(itr);
			
			itr = strtok(NULL,"\'");
			//our pl_name_space was cut
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				itr = strtok(readbuf,"\'");
				scratch = atoi(readbuf);
				pl_namespace = pl_namespace*pow(10.0,(float)strlen(readbuf)) + scratch;
			}			
			placeholder = itr;
			
			itr = strtok(NULL,"\'");
			//our pl_title was cut
			if(!itr){
				strcpy(pl_title_partial,placeholder);
				fgets(readbuf,READSIZE,readf);
				itr = strtok(readbuf,"\'");
				placeholder = readbuf;
			}
			sprintf(writebuf,"%d,%d,%s\n",pl_from,pl_namespace,placeholder);
			fwrite(writebuf, sizeof(char), strlen(writebuf), currentwritefile);
		}
	}
}