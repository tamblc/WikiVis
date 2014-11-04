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
		sentenial = 0;
		while(!sentenial){
			if(!itr){//start of our line
				itr = strtok(readbuf,"(");
				printf("on line %d",i);
#ifdef DEBUG
			printf("%s\n",itr);
#endif			
			}else{//we have already advanced to the next element
				itr = strtok(NULL,"(");
				//means the previous operation itr = strtok(NULL,"\'");
				//reached the eob. so pl_title was cut off
				if(!itr){
					strcpy(pl_title_partial,placeholder);
					fgets(readbuf,READSIZE,readf);
					if(readbuf[0] != '\''){
						itr = strtok(readbuf,"\'");
						placeholder = readbuf;
						sprintf(writebuf,"%d,%d,%s%s\n",pl_from,pl_namespace,pl_title_partial,placeholder);
						fwrite(writebuf, sizeof(char), strlen(writebuf), currentwritefile);
						itr = strtok(NULL,"(");
					}else{//inclase the buffer stopped right before the '
						itr = strtok(readbuf+1,"\'");
					}
				}else{//write the data we have
					sprintf(writebuf,"%d,%d,%s\n",pl_from,pl_namespace,placeholder);
					fwrite(writebuf, sizeof(char), strlen(writebuf), currentwritefile);
				}
			}
			
			itr = strtok(NULL,",");
			//previous strtok(NULL,"(") reached eob
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				if(readbuf[0] == 'I'){
					sentenial = 1;
					return;
				}
				if(readbuf[0] != '('){
					itr = strtok(readbuf,"(");
					itr = strtok(NULL,",");
				}else{//incase the buffer stopped right before the (
					itr = strtok(readbuf+1,",");
				}
			}
			pl_from = atoi(itr);
			
			itr = strtok(NULL,",");
			//previous strtok(NULL,",") reached eob. So pl_from was cutoff
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				//printf("%s\n",readbuf);
				if(readbuf[0] != ','){
					itr = strtok(readbuf,",");
					scratch = atoi(readbuf);
					pl_from = pl_from*pow(10.0,(float)strlen(readbuf)) + scratch;
					itr = strtok(NULL,",");
					//printf("got here %d\n",pl_from);
				}else{//incase the buffer had stopped right before the ,
					itr = strtok(readbuf+1,",");
				}
			}
			pl_namespace = atoi(itr);
			
			itr = strtok(NULL,"\'");
			//previous strtok(NULL,",") reached eob our pl_name_space was cutoff
			if(!itr){
				fgets(readbuf,READSIZE,readf);
				if(readbuf[0] != ','){
					itr = strtok(readbuf,",");
					scratch = atoi(readbuf);
					pl_namespace = pl_namespace*pow(10.0,(float)strlen(readbuf)) + scratch;
					itr = strtok(NULL,"\'");
				}else{//incase the buffer had stopped right before the '
					itr = strtok(readbuf+1,"\'");
				}
			}		
			placeholder = itr;
		}
	}
}