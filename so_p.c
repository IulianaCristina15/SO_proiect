#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>

#define MAX_DIRS 10

//structura pt stocarea modificarilor
struct Date
{
  char name[256];//nume director
  long last_modified;//data si ora ulrimei modificari
  long size;
};

//afisare metadate
void printDate(struct Date *date)
{
  printf("Name:%s\n",date->name);
  printf("Last Modified:%ld",date->last_modified);
  printf("Size:%ld bytes\n",date->size);
  printf("\n");
}

//verific daca procesorul a fost deja procesat
int director_procesat(char *dir,char *dirProcesat[],int count)
{
  for(int i=0;i<count;i++)
    {
      if(strcmp(dir,dirProcesat[i]==0))
	{
	  return 1;
	}
    }
  return 0;
}

//functie pt a obtine metadatele
struct Date getMetadate(char *path)
{
  struct Date data;
  struct stat fileStat;

  if(stat(path,&fileStat)==-1)
    {
      perror("error getting file stats");
      exit(EXIT_FAILURE);
    }
  strcpy(data.name,path);
  data.size=fileStat.st_size;
  data.last_modified=fileStat.st_mtime;

  return data;
}

//fct pt a parcurge un director si a afisa metadatele fiecarei intrai
void parcurgere(char *nume_dir,int nivel)
{
  	DIR *dir;
	struct dirent *in;
	char *nume;
	struct stat info;
	char cale[PATH_MAX],cale_link[PATH_MAX+1],spatii[PATH_MAX];
	int n;
	
	memset(spatii,2,2*nivel);
	spatii[2*nivel]='\0';
	
	if(!(dir=opendir(nume_dir)))
	{
		printf("%s:",nume_dir); fflush(stdout);
		perror("opendir");
		exit(1);
	}

	printf("%sDIR %s:\n,spatii,nume_dir");

	while((in = readdir(dir))>0)
	{
		nume = in->d_name;
		
		if(strcmp(nume,".")==0 || strcmp(nume,"..")==0)
			continue;
		snprintf(cale,sizeof(cale),"%s %s",nume_dir,nume);
		
		if(lstat(cale,&info)<0)
		{
			printf("%s:",cale); fflush(stdout);
			perror("eroare la lstat");
			exit(1);
		}
		
		if(S_ISDIR(info.st_mode))
			parcurgere(cale,nivel+1);
		else
			if(S_ISLNK(info.st_mode))
			{
				n=readlink(cale,cale_link,sizeof(cale_link));
				cale_link[0]='\0';
				printf("%s %s -> %s\n",spatii,cale_link);
			}
			else
			{
				printf("%s %s",spatii,cale);
				if(info.st_mode & S_IXUSR || info.st_mode & S_IXGRP || info.st_mode & S_IXOTH);
				printf("*");
				printf("\n");
			}
	}
	closedir("\n");

}

void incarcare_snapshot(const char *dir,struct Date **fileInfo,int *numarFisier){
  FILE *snapshotFile=fopen("snapshot.txt","r");
  if(snapshotFile == NULL)
    {
      perror("Eroare la deschidere snapshot!");
      exit(1);
    }

  int capacitate=11;
  *fileInfo=malloc(capacitate *sizeof(struct Date));
  if(*fileInfo == NULL)
    {
      perror("eroare la alocarea memoriei");
      exit(1);
    }

  *numarFisier=0;
  char nume_d[PATH_MAX];
  long int ultima_modificare;
  while(fscanf(snapshotFile,"%s\t%ld",nume_d,&ultima_modificare)!=EOF)
    {
      if(*numarFisier == capacitate)
	{
	  capacitate*=2;
	  *fileInfo=realloc(*fileInfo,capacitate *sizeof(struct Date));
	  if(fileInfo==NULL)
	    {
	      perror("eroare la realocarea memoriei");exit(1);
	    }
	}
      strcpy((*fileInfo)[*numarFisier]->nume_d,nume_d);
      (*fileInfo)[*numarFisier].ultima_modificare=ultima_modificare;
      ++(*numarFisier);
    }
}

int main(int argc,char *argv[])
{
  if(argc != 2)
    {
      printf("Mod de utilizare:%s director\n",argv[0]);
      exit(1);
    }
for(int i=0;i<argc;i++)
{
  parcurgere(argv[1],0);
}

for (int i = 1; i < argc; i++) {
        pid_t pid = fork(); // Crearea unui proces copil

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Proces copil
            printf("Child process %d started for directory %s\n", getpid(), argv[i]);
            traverseDirectory(argv[i], 0); // Parcurgere director în procesul copil
            printf("Child process %d finished\n", getpid());
            exit(EXIT_SUCCESS); // Ieșire din procesul copil
        }
    }

    // Proces părinte
    int status;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0) { // Așteptare terminare a tuturor proceselor copil
        if (WIFEXITED(status)) {
            printf("Child process %d exited with status %d\n", wpid, WEXITSTATUS(status));
        } else {
            printf("Child process %d exited abnormally\n", wpid);
        }
    }


  return 0;
}
