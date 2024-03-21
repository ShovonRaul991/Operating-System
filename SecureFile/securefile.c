#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/* function to change the file permission (Removing read permission from Group level and other level)*/
void secure_file(char *file_path)
{
    //to store file path
    char file_location[2048];
    strcpy(file_location, file_path);

    struct stat old_status;
    if (stat(file_location, &old_status) == -1)
    {
        perror("Error in getting file old_status");
        exit(EXIT_FAILURE);
    }

    //printing the permission of the file before changing it
    printf(" Old Permissions: ");
    printf((old_status.st_mode & S_IRUSR) ? "r" : "-");
    printf((old_status.st_mode & S_IWUSR) ? "w" : "-");
    printf((old_status.st_mode & S_IXUSR) ? "x" : "-");
    printf((old_status.st_mode & S_IRGRP) ? "r" : "-");
    printf((old_status.st_mode & S_IWGRP) ? "w" : "-");
    printf((old_status.st_mode & S_IXGRP) ? "x" : "-");
    printf((old_status.st_mode & S_IROTH) ? "r" : "-");
    printf((old_status.st_mode & S_IWOTH) ? "w" : "-");
    printf((old_status.st_mode & S_IXOTH) ? "x" : "-");

    //assigned the changed permission
    old_status.st_mode &= ~(S_IRGRP | S_IROTH);

    //to change the permissions of a file specified by file_location to the permissions specified in old_status.st_mode.
    chmod(file_location, old_status.st_mode);

    struct stat new_status = old_status;

    if (stat(file_location, &new_status) == -1)
    {
        perror("Error in getting file status");
        exit(EXIT_FAILURE);
    }

    //updated file permission
    printf(" \t\t\tNEW Permissions: ");
    printf((new_status.st_mode & S_IRUSR) ? "r" : "-");
    printf((new_status.st_mode & S_IWUSR) ? "w" : "-");
    printf((new_status.st_mode & S_IXUSR) ? "x" : "-");
    printf((new_status.st_mode & S_IRGRP) ? "r" : "-");
    printf((new_status.st_mode & S_IWGRP) ? "w" : "-");
    printf((new_status.st_mode & S_IXGRP) ? "x" : "-");
    printf((new_status.st_mode & S_IROTH) ? "r" : "-");
    printf((new_status.st_mode & S_IWOTH) ? "w" : "-");
    printf((new_status.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}


//function to access all the subfolders and files
void ListOfFiles(const char *dir_name, const char *extn) {
    //pointer to a directory stream
    DIR *dir;
    //information about the individual entries (files or subdirectories) within a directory.
    struct dirent *entry;
    //file status information
    struct stat fileStat;
    char path[1024];

    // Open the directory
    if ((dir = opendir(dir_name)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Ignore special directories . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Construct the full path of the file
        strcpy(path, dir_name);  // Copy the directory name to the path
        strcat(path, "/");       // Concatenate a slash to the path
        strcat(path, entry->d_name); // Concatenate the file/directory name to the path
        printf("\nOPENING %s\n", path);

        // Get file status
        if (stat(path, &fileStat) < 0) {
            perror("stat");
            continue;
        }

        // Check if the entry is a regular file and has the desired extension
        if (S_ISREG(fileStat.st_mode) && strstr(entry->d_name, extn) != NULL) {
            
            secure_file(path);
            printf("Permissions of file %s modified.\n", entry->d_name);
        }
        else{
            printf("Not the expected file to change it's permission.\n");
        }

        // If the entry is a directory, recursively process it
        if (S_ISDIR(fileStat.st_mode))
            ListOfFiles(path, extn);
    }

    // Close the directory
    closedir(dir);
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir_name> <extn>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // Extract directory name and extension from command line arguments
    const char *dir_name = argv[1];
    const char *extn = argv[2];

    // Secure the files
    ListOfFiles(dir_name, extn);

    return 0;
}
