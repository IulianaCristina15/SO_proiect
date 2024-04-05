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
void traverseDirectory(char *dir_path) {
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
            struct Date file_data = getMetadata(file_path);
            printMetadata(file_data);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *dir_path = argv[1];
    traverseDirectory(dir_path);

    return 0;
}

//se actualizeaza functionalitatea prog astfel incat sa primeasca oricate argumente dar nu mai mult de 10,cu mentionarea ca nici un argument nu se va repeta.programul proceseaza doar directoarele si nu alte tipuri de fisiere;
//prog prim un arg suplimentar care e directorul de iesire in care vor fi srocate toate snaphoturile arg date la intrare
