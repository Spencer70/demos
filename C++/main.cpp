//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <tuple>
#include <string>

#include "Rubik.hpp"

using namespace std;
 
tuple<int, bool, int, string, string, string> 
parse(int argc, char *argv[])
{
    int err = 0;
    bool show = false;
    int loop = 1;
    string strRotations = "RANDOM";
    string strMoves = "RANDOM";
    string algorithm = "Test";

    int c;
    while ((c = getopt (argc, argv, "sr:m:a:l:")) != -1)
        switch (c)
        {
        case 'r':
            strRotations = optarg;
            break;
        case 'm':
            strMoves = optarg;
            break;
        case 'a':
            algorithm = optarg;
            break;
        case 's':
            show = true;
            break;
       case 'l':
            loop = atoi(optarg);
            break;
        default:
            printf("Unknown option\n");
            err = -1;
        }
    if (optind < argc)
    {
        printf("Unknown params\n");
        err = -2;
    }
 
    return make_tuple(err, show, loop, strRotations, strMoves, algorithm);
}

// main function -
// where the execution of program begins
int main(int argc, char *argv[])
{
    int err;
    bool show;
    int loop;
    string strRotations, strMoves, algorithm;

    tie(err, show, loop, strRotations, strMoves, algorithm) = parse(argc, argv);
    if (err != 0)
    {
        printf("main -l <loop_count>-r <rotations> -m <moves> -a <algorithm_name> -s\n");
        exit(err);
    }

    printf("Rubik solver!");
    // Create rubik
    printf("\nInitial state\n");

    unique_ptr<Rubik> rbk = Rubik_Factory::create_Rubik(algorithm, show);

    if (!rbk)
    {
        printf("Unkown algorithm: %s\n", algorithm.c_str());
        printf("Available algorithms:\n");
        for (auto a : Rubik_Factory::available_Algorithms())
            printf("\t%s\n", a.c_str());

        exit(-3);
    }

    rbk->print();
    if (!rbk->isSolved())
        exit(-4);
    
    for(int i = 0; i < loop; i++)
    {
        // Rotate cube
        rbk->rotate(strRotations);
        printf("\nRotated state\n");
        rbk->print();
        if (!rbk->isSolved())
            exit(-4);
        rbk->shuffle(strMoves);
        printf("\nShuffled state\n");
        rbk->print();
        rbk->solve();
        printf("\nSolved state\n");
        rbk->print();
        if (rbk->isSolved())
            printf("\nRubik solved!\n");
    }
 
	return 0;
}
