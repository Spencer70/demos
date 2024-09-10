package org.rubik.impl.algorithms.test;

import org.rubik.impl.Move;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

public class TestAlgorithm implements Algorithm {
    private final Rubik_Impl rubik;

    public TestAlgorithm(Rubik_Impl rubik) {
        this.rubik = rubik;
    }

    @Override
    public void exec() {
        List<Move> moves = new ArrayList<>(rubik.moves.size());
        ListIterator<Move> it = rubik.moves.listIterator(rubik.moves.size());
        while (it.hasPrevious()) {
            Move move = it.previous();
            moves.add(new Move(move.face, !move.clockWise, move.step));
        }
        rubik.permute(moves);
    }

}
