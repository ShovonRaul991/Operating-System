#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SUPER_BLOCK_SIZE 4096
#define FILE_DESCRIPTOR_SIZE 29
#define FILE_NAME_SIZE 20
#define BLOCK_SIZE 4096

//structure of the file descriptor block
typedef struct {
    char file_name[FILE_NAME_SIZE];
    int first_block_number;
    int size;
    time_t modification_time;
} FileDescriptor;

//function to create a file based on filename, blocksize and block numbers;
int init_File_dd(const char *fname, int bsize, int bno) {
    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error creating file");
        return -1;
    }
    // Initialize Super Block
    int n = bno;
    int ubn = 0;
    int fbn = bno;
    char ub[bno]; // assuming enough memory to hold bitmap
    memset(ub, '0', bno); // Initially, all blocks are free

    // Write Super Block to file
    fwrite(&n, sizeof(int), 1, file);
    fwrite(&bsize, sizeof(int), 1, file);
    fwrite(&ubn, sizeof(int), 1, file);
    fwrite(&fbn, sizeof(int), 1, file);
    fwrite(ub, sizeof(char), bno, file);

    // Initialize file descriptor block
    fseek(file, SUPER_BLOCK_SIZE, SEEK_SET);
    FileDescriptor fd;
    memset(&fd, 0, sizeof(FileDescriptor));
    fwrite(&fd, sizeof(FileDescriptor), 1, file);

    fclose(file);
    return 0;
}



int read_block(const char *fname, int bno, char *buffer) {
    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, SUPER_BLOCK_SIZE + (bno * BLOCK_SIZE), SEEK_SET);
    fread(buffer, sizeof(char), BLOCK_SIZE, file);
    fclose(file);
    return 0;
}

int write_block(const char *fname, int bno, char *buffer) {
    FILE *file = fopen(fname, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, SUPER_BLOCK_SIZE + (bno * BLOCK_SIZE), SEEK_SET);
    fwrite(buffer, sizeof(char), BLOCK_SIZE, file);
    fclose(file);
    return 0;
}

int get_freeblock(const char *fname) {
    FILE *file = fopen(fname, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, 2 * sizeof(int), SEEK_SET); // Move to ubn position
    int ubn;
    fread(&ubn, sizeof(int), 1, file);
    if (ubn == 0) {
        fclose(file);
        return -1; // No free blocks
    }

    char ub[ubn];
    fread(ub, sizeof(char), ubn, file);

    // Find first free block
    int i;
    for (i = 0; i < ubn; i++) {
        if (ub[i] == '0') {
            ub[i] = '1'; // Mark as used
            fseek(file, SUPER_BLOCK_SIZE + sizeof(int) * 2 + i, SEEK_SET);
            fwrite("1", sizeof(char), 1, file); // Update bitmap
            break;
        }
    }

    fclose(file);
    return i; // Returning block number
}

int free_block(const char *fname, int bno) {
    FILE *file = fopen(fname, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fseek(file, 2 * sizeof(int), SEEK_SET); // Move to ubn position
    int ubn;
    fread(&ubn, sizeof(int), 1, file);

    fseek(file, SUPER_BLOCK_SIZE + sizeof(int) * 2 + bno, SEEK_SET);
    fwrite("0", sizeof(char), 1, file); // Update bitmap

    fclose(file);
    return 1;
}

int readblock(const char *fname, char *myfile_name, int bno, char *buffer) {
    // Implementation of this function depends on how files are stored within myfs
    // It would need to traverse file descriptors to find the relevant blocks
    // and then use read_block to read the data into buffer
    return -1; // Placeholder return value
}

int writeblock(const char *fname, char *myfile_name, int bno, char *buffer) {
    // Similar to readblock, depends on the filesystem structure
    // Needs to find the myfile_name and write data to the specified block
    // If the file doesn't exist, create it, and allocate blocks as needed
    return -1; // Placeholder return value
}

int check_fs(const char *fname) {
    // Implementation of this function depends on the specific checks you want to perform
    // It would involve reading the super block, checking values, and inspecting block usage
    // Return 0 if everything is consistent, otherwise return 1
    return -1; // Placeholder return value
}

void create_myfs() {
    char fname[50];
    int bsize, bno;

    printf("Enter filename for myfs: ");
    scanf("%s", fname);

    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error creating file");
        return;
    }
    fclose(file); // Close the file immediately after creating

    printf("Enter block size for myfs: ");
    scanf("%d", &bsize);
    printf("Enter number of blocks for myfs: ");
    scanf("%d", &bno);

    if (init_File_dd(fname, bsize, bno) == 0)
        printf("myfs created successfully.\n");
    else
        printf("Failed to create myfs.\n");
}


void mount_myfs() {
    // Implement mounting procedure here
    printf("Mounting myfs...\n");
}

void list_files() {
    // Implement listing procedure here
    printf("Listing files of myfs...\n");
}

void copy_files() {
    // Implement file copying procedure here
    printf("Copying a file from myfs to another myfs...\n");
}

int main() {
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Create myfs\n");
        printf("2. Mount myfs\n");
        printf("3. List files of myfs\n");
        printf("4. Copy file from myfs to another myfs\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_myfs();
                break;
            case 2:
                mount_myfs();
                break;
            case 3:
                list_files();
                break;
            case 4:
                copy_files();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
