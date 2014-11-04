#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void writedata(FILE *readf, int numtoread);

#define READSIZE 1000
#define	WRITESIZE 300
#define PL_TITLE_MAX 255
#define DEBUG

int main(int argc, char ** argv){
	FILE *readf;
	int numtoread;
	if(argc != 3){
		printf("error: need 2 arguments. Name of source file,num of lines to read\n");
		exit(EXIT_FAILURE);
	}
	readf = fopen(argv[1],"r");
	if(!readf){
		printf("error: source file does not exist\n");
		exit(EXIT_FAILURE);
	}

	numtoread = atoi(argv[2]);
	
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
	currentwritefile = fopen("0.txt","w");
	/*while(1){
		printf(readbuf);
		fgets(readbuf,READSIZE,readf);
		if(readbuf[0] == 'I'){
			return;
		}
	}*/
	for(i = 0; i < numtoread; ++i){
		itr = NULL;
		placeholder = NULL;
		sentenial = 0;
		while(!sentenial){
			if(!itr){//start of our line
				itr = strpbrk(readbuf,"(");
				*itr = '\0';
				++itr;
			}else{
				placeholder = itr;
				itr = strpbrk(itr,"(");
				if(!itr){//could not find (
					fgets(readbuf,READSIZE,readf);
					if(readbuf[0] == 'I'){
						sentenial = 1;
						break;
					}
					itr = strpbrk(readbuf,"(");
				}
				*itr = '\0';
				++itr;
				if(!itr){//meant that delimiter was last char in buffer
					fgets(readbuf,READSIZE,readf);
					itr = readbuf;
				}
			}
			
			placeholder = itr;
			itr = strpbrk(itr,",");
			if(!itr){//could not find delimiting , will load next
				//meant that pl_from is partially written
				pl_from = atoi(placeholder);
				fgets(readbuf,READSIZE,readf);
				itr = strpbrk(readbuf,",");
				*itr = '\0';
				++itr;
				scratch = atoi(readbuf);
				pl_from = pl_from*pow(10.0,(float)strlen(readbuf)) + scratch;
			}else{
				*itr = '\0';
				++itr;
				pl_from = atoi(placeholder);
				if(!itr){//meant that delimiter was last char in buffer
					fgets(readbuf,READSIZE,readf);
					itr = readbuf;
				}
			}
			
			placeholder = itr;
			itr = strpbrk(itr,",");
			if(!itr){//could not find delimiting , will load next
				//meant that pl_namespace is partially written
				pl_namespace  = atoi(placeholder);
				fgets(readbuf,READSIZE,readf);
				itr = strpbrk(readbuf,",");
				*itr = '\0';
				++itr;
				scratch = atoi(readbuf);
				pl_namespace  = pl_namespace *pow(10.0,(float)strlen(readbuf)) + scratch;
			}else{
				*itr = '\0';
				++itr;
				pl_namespace = atoi(placeholder);
				if(!itr){//meant that delimiter was last char in buffer
					fgets(readbuf,READSIZE,readf);
					itr = readbuf;
				}
			}
			
			placeholder = itr;		//find first '
			itr = strpbrk(itr,"\'");
			if(!itr){//pl_title starts from the next buffer
				fgets(readbuf,READSIZE,readf);
				itr = readbuf;
			}
			*itr = '\0';
			++itr;
			if(!itr){//meant that delimiter was last char in buffer
				fgets(readbuf,READSIZE,readf);
				itr = readbuf;
			}
			
			placeholder = itr;	   //find next '
			itr = strpbrk(itr,"\'");
			if(!itr){//pl_title is cutoff
				strcpy(pl_title_partial,placeholder);
				fgets(readbuf,READSIZE,readf);
				itr = strpbrk(readbuf,"\'");
				*itr = '\0';
				++itr;
				sprintf(writebuf,"%d,%d,%s%s\n",pl_from,pl_namespace,pl_title_partial,readbuf);
			}else{
				/*if(*(itr-1) == '\\'){
					++itr;
					itr = strpbrk(itr,"\'");
				}*/
				*itr = '\0';
				++itr;
				sprintf(writebuf,"%d,%d,%s\n",pl_from,pl_namespace,placeholder);
				if(!itr){//meant that delimiter was last char in buffer
					fgets(readbuf,READSIZE,readf);
					itr = readbuf;
				}
			}
			fwrite(writebuf, sizeof(char), strlen(writebuf), currentwritefile);
		}
	}
}