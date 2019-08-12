#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "directoryfunctions.h"
#include "tokenizer.h"
#include <pwd.h>

#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>

typedef struct pNode{
	char *word;
	float memd;
	struct pNode *next;
} pNode;

pNode *createpNode(char *word);
void addToEnd(pNode **head, pNode *target);

//int main(void) {
int main(int argc, char **argv){
	//listdir(argv[1], 0);
    	DIR *dir = opendir("/proc");
	struct dirent *entry;
    	/*	
	if(entry == NULL){
		printf("Couldn't open directory\n");
		return 0;
	}
	*/

	//get memtotal (present memory available on machine)
	float memtotal;
	char c;
	int cnt = 0;
	int mfd = open("/proc/meminfo", O_RDONLY, 0);
	while(read(mfd, &c, 1) != 0){
		if(c == '\n'){
			break;
		}
		if(isdigit(c)){
			cnt++;
			//printf("USER\t");
			//printf("%c",c);
		}
	}
	close(mfd);
	int cnt2 = 0;
	char memtotaldata[cnt];
	int mfd2 = open("/proc/meminfo", O_RDONLY, 0);
	while(read(mfd2, &c, 1) != 0){
		if(c == '\n'){
			break;
		}
		if(isdigit(c)){
			memtotaldata[cnt2] = c;
			cnt2++;	
		}
	}
	memtotaldata[cnt2] = '\0';
	printf("Memtotaldata = %s\n",memtotaldata);
	memtotal = atof(memtotaldata);
	printf("memtotal: %f\n",memtotal);
	close(mfd2);
	printf("%*s",3,"USER\tPID\tCPU  MEM\t VSZ\t\t   RSS\t\tTTY\t\tSTAT\t\tSTART  TIME COMMAND\n",3,"");
	//printf("%*s")
	//Loop though pid directories
	pNode *PID = NULL;
	pNode *CPU = NULL;
	pNode *MEM = NULL;
	pNode *VSZ = NULL;
	pNode *RSS = NULL;
	pNode *TTY = createpNode("?");
	pNode *STAT = createpNode("S");
	pNode *START = NULL;
	pNode *TIME = NULL;
	pNode *COMMAND = NULL;
	while((entry = readdir(dir)) != NULL){
		//printf("Starting 1st while ...\n");
		if(isdigit(entry->d_name[0])){
			//loop through folders within pid
			char pidDir[50];
			char path[50];
			char slash[50];
			strcpy(pidDir, entry->d_name);
			strcpy(path, "/proc/");
			strcpy(slash, "/");
			
			strcat(path,pidDir);
			strcat(path, slash);
			//path now contains proc/<PID>/
			//new---------------------
			char pathTemp[50];
			strcpy(pathTemp, path);
			//new---------------------
			//Traverse PID Folder
			DIR *dir2 = opendir(path);
			struct dirent *entry2;

			Node *letterLL = NULL;
			
			//printf("Ablut to start 2nd while...\n");
			
			if(dir2 == NULL){
   				//perror("Unable to open folder.");
   				continue;
			}
			//
			while((entry2 = readdir(dir2)) != NULL){
				//printf("Starting 2nd while ...\n");
				if(strcmp(entry2->d_name, "status") == 0){
					char c;
					char status[1000];
					strcpy(status, "status");
					strcat(path, status);
					
					int fd = open(path, O_RDONLY, 0);
					//printf("Trying to read from %s\n",path);
					//the following will read status 
					//letter by letter
					while(read(fd, &c, 1) != 0){
					//printf("Starting 3rd while ...\n");
						if(c != '\n') {
							Node *letterNode = createLetterNode(c);
							addToEndLetterNode(&letterLL, letterNode);
						} else {
							if (letterLL != NULL){
								char *innerWordString = convertLetterLLToArrayOfLetters(letterLL);

								letterLL = NULL;
								//printf("%s\n",innerWordString);
								//check beginning of word, see if it contains desired info
									
								if(strncmp(innerWordString, "Name",4) == 0){
								//	printf("%s\n",innerWordString);
								}
								//PID
								if(strncmp(innerWordString, "Pid",3) == 0){
								//	printf("%s\n",innerWordString);
									char piddata[50];
									int lenOfStr = strlen(innerWordString);
									int i = 5;
									int c = 0;
									while(i < lenOfStr){
										piddata[c] = innerWordString[i];
										c++;i++;
									}
									piddata[c] = '\0';
									//printf("piddata: %s\n",piddata);
									PID = createpNode(piddata);
									
									//USER part
									/*
									char *ptr;
									uid_t uid;

									uid = strtoul(piddata, &ptr, 10);
									printf("The number is %u\n", uid);
									*/
								}
								//VSZ
								if(strncmp(innerWordString, "VmRSS",5) == 0){
								//	printf("%s\n",innerWordString);
									char vszdata[50];
									int lenOfStr = strlen(innerWordString);
									int i = 11;
									int c = 0;
									while(i < lenOfStr){
										//printf("%c",innerWordString[i]);
										vszdata[c] = innerWordString[i];
										c++;i++;
									}
									vszdata[c-3] = '\0';
									//printf("vszdata: %s\n",vszdata);
									VSZ = createpNode(vszdata);
								}
								//RSS
								if(strncmp(innerWordString, "VmSize",6) == 0){
								//	printf("%s\n",innerWordString);
									char rssdata[50];
									int lenOfStr = strlen(innerWordString);
									int i = 10;
									int c = 0;
									while(i < lenOfStr){
										//printf("%c",innerWordString[i]);
										rssdata[c] = innerWordString[i];
										c++;i++;
									}
									rssdata[c-3] = '\0';
									//printf("rssdata: %s\n",rssdata);
									RSS = createpNode(rssdata);
									//MEM PART
									float MEMdata = atof(rssdata)/ memtotal;
									//printf("MEM data = %f\n",MEMdata);
									MEM = createpNode("");
									MEM->memd = MEMdata;
								}
								//STAT (S or s or s+)
								
								//START
								//
								//TIME
								//
								//COMMAND
								//-------------
								//struct passwd *userID = getpwuid(uid_t uid);
								//-------------
								
							}
						}
					}
					//printf("Just finished 3rd While\n");	
					close(fd);
				}

				else if(strcmp(entry2->d_name, "cmdline") == 0){
					char c;
					char cmdline[1000];
					strcpy(cmdline, "cmdline");
					strcat(pathTemp, cmdline);
					
					int fd = open(pathTemp, O_RDONLY, 0);
					char cmdlinedata[1000];
					int i = 0;
					while(read(fd, &c, 1) != 0){
						//printf("%c",c);
						cmdlinedata[i] = c;
						i++;
						if(i > 950){
							break;
						}
					}
					cmdlinedata[i] = '\0';
					//printf("cmdlinedata = %s\n",cmdlinedata);
					COMMAND = createpNode(cmdlinedata);
				}
			}
			
			//printf("Just finished 2nd While\n");	
			closedir(dir2);
		}
		//printf("%*s",3,"USER\tPID\tCPU  MEM\tVSZ\t\tRSS\t\tTTY\t\tSTAT\t\tSTART  TIME COMMAND\n",3,"");
		if(PID != NULL){
			printf("N/A\t",3,"");
			//printf("%s\t\t",PID->word);
			printf("%*s\t",3,PID->word);
		} else {
			//printf("PID  ");
			continue;
			//printf("PID\t\t");
		}
		
		//printf("CPU  ");
		printf("N/A  ");
		if(MEM != NULL){
			//printf("%.1f ",MEM->memd);
			printf("%.1f\t",MEM->memd);
		} else {
			//printf("0.0 ");
			printf("0.0\t");
		}
		
		if(VSZ != NULL){
			//printf("%s ",VSZ->word);
			printf("%*s\t\t",3,VSZ->word);
		} else {
			//printf("0 ");
			printf("%*s0\t\t",3,"");
		}
		if(RSS != NULL){
			//printf("%s ",RSS->word);
			printf("%*s\t\t",5,RSS->word);
		} else {
			//printf("0 ");
			printf("%*s0\t\t",5,"");
		}
		if(TTY != NULL){
			printf("%s\t\t",TTY->word);
		} else {
			printf("?\t\t");
		}
		if(STAT != NULL){
			//printf("%s    ",STAT->word);
			printf("%s\t\t",STAT->word);
		} else {
			//printf("S    ");
			printf("S\t\t");
		}
		//printf("START  ");
		//printf("%*s",3,"N/A     ");
		printf("N/A    ");
		//printf("TIME ");
		printf("N/A ");
		if(COMMAND != NULL){
			printf("%s\n",COMMAND->word);
		} else {
			printf("-bash\n");
		}
				
		PID = NULL;
		CPU = NULL;
		MEM = NULL;
		VSZ = NULL;
		RSS = NULL;
		STAT = NULL;
		START = NULL;
		TIME = NULL;
		COMMAND = NULL;

	}
	//printf("Just finished 1st While\n");	
	closedir(dir);

	return 0;
}

pNode *createpNode(char *word){
	pNode *tmp = malloc(sizeof(pNode));
	tmp->word = strdup(word);
	tmp->next = NULL;
	return tmp;
}


void addToEnd(pNode **head, pNode *target){
	if(*head == NULL){
		(*head) = target;
		return;
	}	
	pNode *temp = createpNode(target->word);
	pNode *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

/*
void listdir(const char *name, int indent){
    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir(name))){
        return;
	}

    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_DIR) {
            char path[1024];
			if(strcmp(entry->d_name, ".") == 0){
				continue;
			}
			if(strcmp(entry->d_name, "..") == 0){
				continue;
			}

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			//printf("%s\n", entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
			
            listdir(path, indent + 2);
        } else {
            printf("%*s- %s\n", indent, "", entry->d_name);
        }
    }
    closedir(dir);
}
*/


