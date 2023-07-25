#!/usr/bin/env python3

from rubik.RubikFactory import RubikFactory

import sys, getopt

def parse(argv):
    strRotations = 'RANDOM'
    strMoves = 'RANDOM'
    test = False
    show = False
    try:
        opts, args = getopt.getopt(argv, "r:m:ts")
    except getopt.GetoptError:
        print('main.py -r <rotations> -m <moves> -t -s')
        sys.exit(2)
    for opt, arg in opts:
        if opt == "-r":
            strRotations = arg.upper()
        elif opt == "-m":
            strMoves = arg.upper()
        elif opt == "-t":
            test = True
        elif opt == "-s":
            show = True
        else:
            print('main.py -r <rotations> -m <moves> -t -s')
            sys.exit()

    return strRotations, strMoves, test, show


if __name__ == '__main__':
    strRotations, strMoves, test, show = parse(sys.argv[1:])

    print("Rubik solver!")
    #  Create rubik
    print("\nInitial state\n")
    rubik = RubikFactory.create(test, show)
    rubik.print()
    if not rubik.isSolved():
        raise AssertionError()
    #  Rotate cube
    print("\nRotated state\n")
    rubik.rotate(strRotations)
    rubik.print()
    if not rubik.isSolved():
        raise AssertionError()
    print("\nShuffled state\n")
    rubik.shuffle(strMoves)
    rubik.print()
    print("\nSolved state\n")
    rubik.solve()
    rubik.print()
    if rubik.isSolved():
        print("\nRubik solved!\n")


