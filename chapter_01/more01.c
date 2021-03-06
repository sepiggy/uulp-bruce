/*
 * page 17
 * more01.c - version 0.1 of more
 * read and print 24 lines then pause for a few special commands
 */

#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 24
#define LINELEN 512


void do_more(FILE *);

int see_more(void);

int main(int argc, char **argv) {
    FILE *fp;
    if (argc == 1)
        /* get data from stdin */
        do_more(stdin);
    else
        /* get data from ordinary file */
        while (--argc) {
            if ((fp = fopen(*++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
    return 0;
}

/*
 * read PAGELEN lines, then call see_more() for further instructions
 */
void do_more(FILE *fp) {
    char line[LINELEN];
    int num_of_lines = 0;
    int reply;

    while (fgets(line, LINELEN, fp)) {      /* more input */
        if (num_of_lines == PAGELEN) {      /* full screen? */
            reply = see_more();            /* y: ask user */
            if (reply == 0) {               /* n: done */
                break;
            }
            num_of_lines -= reply;          /* reset count */
        }
        if (fputs(line, stdout) == EOF) {   /* show line */
            exit(1);                        /* or die */
        }
        num_of_lines++;                     /* count it */
    }
}

/*
 * print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
 */
int see_more() {
    int c;
    printf("\033[7m more? \033[m");         /* reverse on a vt100*/
    while ((c = getchar()) != EOF) {        /* get response */
        if (c == 'q')                       /* q -> N */
            return 0;
        if (c == ' ')                       /* ' ' -> next page */
            return PAGELEN;                 /* how many to show */
        if (c == '\n')                      /* Enter key -> 1 line */
            return 1;
    }
    return 0;
}
