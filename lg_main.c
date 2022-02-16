#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define URL   "https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git"
#define PATH  "."


static void print_time(const git_time *intime, const char *prefix)
{
        char sign, out[32];
        struct tm *intm;
        int offset, hours, minutes;
        time_t t;

        offset = intime->offset;
        if (offset < 0) {
                sign = '-';
                offset = -offset;
        } else {
                sign = '+';
        }

        hours   = offset / 60;
        minutes = offset % 60;

        t = (time_t)intime->time + (intime->offset * 60);

        intm = gmtime(&t);
        strftime(out, sizeof(out), "%a %b %e %T %Y", intm);

        printf("%s%s %c%02d%02d\n", prefix, out, sign, hours, minutes);
}

int main (int argc, char **argv)
{
        git_repository *repo = NULL;
        int error;
        git_oid oid;
        const git_signature *sig;
        git_commit *commit = NULL, *prev_commit = NULL;
        char commit_hash[] = "3e865655c25eb8ec0d7f43ccf3aa6366e66f2e51k";
        git_revwalk *walk;
        unsigned int delta;

        git_libgit2_init();
        error = git_clone(&repo, URL, PATH, NULL);

        if (error == GIT_EEXISTS){
                printf("Path \"%s\" already exists.\nOpening \"%s\"...",
                       PATH, PATH);
                if (git_repository_open(&repo, PATH))
                        printf("error\n");
                else
                        printf("done\n");
        } else if (error != 0){
                fprintf(stderr,
                        "failed to lookup commit: %s\n",
                        git_error_last()->message);
        }
        printf("-----\n");

        git_oid_fromstr(&oid, commit_hash);
        git_revwalk_new(&walk, repo);
        git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
        git_revwalk_push(walk, &oid);
        error = git_revwalk_next(&oid, walk);
        while (error == 0){
                git_commit_lookup(&commit, repo, &oid);
                sig = git_commit_author(commit);
                print_time(&sig->when, "|> ");
                printf("|> %s commited\n\n", sig->name);
                git_commit_free(commit);
                prev_commit = commit;
                error = git_revwalk_next(&oid, walk);
                if (error){
                        git_commit_free(commit);
                        break;
                } else {
                        git_commit_lookup(&commit, repo, &oid);
                        delta = git_commit_author(commit)->when.time -
                                git_commit_author(prev_commit)->when.time;
                        sleep(delta);
                }

        }
        git_revwalk_free(walk);

        return 0;
}
