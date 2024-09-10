package org.rubik.api;

public interface Rubik {
    String MOVES = "UDRLFB'2";

    void print();

    // moves = "RANDOM" for random rotation
    void rotate(String moves);

    // moves = "RANDOM" for random shuffle
    void shuffle(String moves);

    void solve();

    boolean isSolved();

}
