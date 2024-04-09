#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_DIRS 10

// Structura pentru stocarea modificarilor
struct Date{
    char name[256]; // numele directorului
    long last_modified; // Data și ora ultimei modificări
    long size; // Dimensiunea fișierului
};

// Funcție pentru afișarea metadatelor unui fisier sau director
void printDate(struct Date *date) {

    printf("Name: %s\n", date->name);
    printf("Last Modified: %ld", date->last_modified);
    printf("Size: %ld bytes\n", date->size);
    printf("\n");
    
}

//verific daca procesorul a fost deja procesat
int director_procesat(char *dir,char *dirProcesat[],int count)
{
	for(int i=0;i<count;i++)
	{
		if(strcmp(dir,dirProcesat[i]==0))
			return 1;
	}
	return 0;
}

//functie pt a obt metadatele
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
	data.size = fileStat.st_size;
	data.last_modified=fileStat.st_mtime;
	
	return data;
}



// Funcție pentru a parcurge recursiv un director și a afișa metadatele fiecărei intrări
void parcurgere(char *nume_dir,char *output_dir,char *dirProcesat[],int nivel)
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
	
	//adaug directorul in lista de dir procesate
	dirProcesat[nivel]=strdup(nume_dir);
	nivel++;
	
	//nr maxim de directoare
	if(nivel>MAX_DIRS)
	{
		printf("a fost atins maximul de directoare");
		exit(EXIT_FAILURE);
	}
	
	printf("%sDIR %s:\n",spatii,nume_dir);
	
	/*while((in = readdir(dir))>0)
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
	closedir("\n");*/
	
	//cream directorul de iesire daca nu exista
	
	char outputDirPath[PATH_MAX];
	snprintf(outputDirPath,sizeof(outputDirPath),"%s %s",output_dir,nume_dir);
	mkdir(outputDirPath,0777);
	
	while((in==readdir(dir))!=NULL)
	{
		if(strcmp(nume,".")==0 || strcmp(nume,"..")==0)
			continue;
	}
	
	snprintf(cale,sizeof(cale),"%s %s",nume_dir,nume);
		
		if(lstat(cale,&info)<0)
		{
			printf("%s:",cale); fflush(stdout);
			perror("eroare la lstat");
			exit(1);
		}
	
	if(S_ISDIR(info.st_mode))
	{
		if(!isdirProcesat(cale,output_dir,dirProcesat,nivel))
		{
			parcurgere(cale,output_dir,dirProcesat,nivel);
		}
		else
		{
			char outputPath[PATH_MAX];
			snprintf(outputPath,sizeof(outputPath),"%s %s",outputDirPath,in->nume,".");
			struct Date fileData=getMetadate(cale);
			
			//copiem fisier in dir de iesire
			FILE *sourceFile=fopen(cale,"r");
			FILE *destinationFile=fopen(outputPath,"w");
			if(!sourceFile || !destinationFile)
			{
				perror("eroare la deschidere");
				exit(EXIT_FAILURE);
			}
			
			char ch;
			while((ch==fgetc(sourceFile))!=EOF)
			{
				fputc(ch,destinationFile);
			}
			fclose(sourceFile);
			fclose(destinationFile);
		}
	}
	closedir(dir);
	
	
}

/*void traverseDirectory(char *dir_path) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dir_path))) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    printf("Snapshot for directory: %s\n", dir_path);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char path[256];
                snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
                traverseDirectory(path);
            }
        } else {
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);
            struct Date file_data = getMetadate(file_path);
           // printDate(Date);
        }
    }

    closedir(dir);
}*/

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <directory1> <directory2> ... <direvtoryN> <outputDirectory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *outputDir = argv[argc-1];
    char processedDirs[MAX_DIRS];
    int nivel=0;
    
    for(int i=0;i<argc-1;++i)
    {
    	char *dir = argv[i];
    	parcurgere(dir,outputDir,processedDirs,nivel);
    }
   // char *dir_path = argv[1];
    //arcurgere(argv[1],0);
    return 0;
}

//2.se actualizeaza functionalitatea prog astfel incat sa primeasca oricate argumente dar nu mai mult de 10,cu mentionarea ca nici un argument nu se va repeta.programul proceseaza doar directoarele si nu alte tipuri de fisiere;
//prog prim un arg suplimentar care e directorul de iesire in care vor fi srocate toate snaphoturile arg date la intrare
