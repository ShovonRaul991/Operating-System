#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//function to print the permission
void printPermissions(const char *path, const char *action) {
    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    printf("%s permissions for %s: %c%c%c%c%c%c%c%c%c\n", action, path,
           (S_ISDIR(file_stat.st_mode)) ? 'd' : '-',
           (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
           (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
           (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
           (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
           (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
           (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
           (file_stat.st_mode & S_IROTH) ? 'r' : '-',
           (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
           (file_stat.st_mode & S_IXOTH) ? 'x' : '-');
}

void modifyPermissions(const char *dir_name, const char *extn) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if (!(dir = opendir(dir_name))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        if (stat(path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            modifyPermissions(path, extn);
        } else {
            char *extension = strrchr(entry->d_name, '.');
            if (extension && strcmp(extension, extn) == 0) {
                printPermissions(path, "Previous");
                chmod(path, file_stat.st_mode & ~S_IRGRP & ~S_IROTH);
                printPermissions(path, "Modified");
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir_name> <extn>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *dir_name = argv[1];
    const char *extn = argv[2];

    modifyPermissions(dir_name, extn);

    return EXIT_SUCCESS;
}
