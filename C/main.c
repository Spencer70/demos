//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "Rubik.h"


int parse(int argc, char *argv[], char **strRotations, char **strMoves, char **algorithm, bool *show, int *loop)
{
    *strRotations = *strMoves = "RANDOM";
    *algorithm = NULL;
    *show = false;
    *loop = 1;

    int c;
    while ((c = getopt (argc, argv, "sr:m:a:l:")) != -1)
        switch (c)
        {
        case 'r':
            *strRotations = optarg;
            break;
        case 'm':
            *strMoves = optarg;
            break;
        case 'a':
            *algorithm = optarg;
            break;
        case 's':
            *show = true;
            break;
       case 'l':
            *loop = atoi(optarg);
            break;
        default:
            return -1;
        }
    if (optind < argc)
    {
        printf("Unknown params\n");
        return -2;
    }
    return 0;
}

// main function -
// where the execution of program begins
int main(int argc, char *argv[])
{
    char *strRotations, *strMoves, *algorithm;
    bool show;
    int loop;

    int err = parse(argc, argv, &strRotations, &strMoves, &algorithm, &show, &loop);
    if (err != 0)
    {
        printf("main -l <loop_count>-r <rotations> -m <moves> -a <algorithm_name> -s\n");
        exit(err);
    }

    printf("Rubik solver!");
    // Create rubik
    printf("\nInitial state\n");

    HRBK hrbk = Rubik_create(algorithm, show);
    if (!hrbk)
    {
        printf("Unkown algorithm: %s", algorithm);
        exit(-3);
    }

    Rubik_print(hrbk);
    if (!Rubik_isSolved(hrbk))
        exit(-4);
    
    for(int i = 0; i < loop; i++)
    {
        // Rotate cube
        printf("\nRotated state\n");
        Rubik_rotate(hrbk, strRotations);
        Rubik_print(hrbk);
        if (!Rubik_isSolved(hrbk))
            exit(-4);
        printf("\nShuffled state\n");
        Rubik_shuffle(hrbk, strMoves);
        Rubik_print(hrbk);
        printf("\nSolved state\n");
        Rubik_solve(hrbk);
        Rubik_print(hrbk);
        if (Rubik_isSolved(hrbk))
            printf("\nRubik solved!\n");
    }

    Rubik_destroy(hrbk);
 
	return 0;
}
