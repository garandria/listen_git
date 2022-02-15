#include <git2.h>
#include <stdio.h>
#include <stdlib.h>


#define URL   "https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git"
#define PATH  "linux"


int main (int argc, char **argv)
{
        git_repository *repo = NULL;
        int error;
        git_libgit2_init();
        error = git_clone(&repo, URL, PATH, NULL);
        if (error == GIT_EEXISTS){
                printf("Path %s already exists.\n Opening %s...", PATH, PATH);
                git_repository_open(&repo, PATH);
        }

        return 0;
}
