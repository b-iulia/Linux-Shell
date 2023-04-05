#include "my_chroot.h"
#include <stdio.h>
#include <unistd.h>

int my_chroot(int argc, char *argv[]) {
    printf("command chroot\n");

    if (argc < 2) {
        // Print usage and exit if no directory is provided
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    // Change the root directory
    if (chdir(argv[1]) != 0) {
        perror("chdir");
        return 1;
    }
    if (chroot(argv[1]) != 0) {
        perror("chroot");
        return 1;
    }

    // Print the current working directory (should be the new root)
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("Current working directory: %s\n", cwd);

    return 0;
}
int main(int argc, char *argv[]) {
    int r = my_chroot(argc, argv);
    return r;
}