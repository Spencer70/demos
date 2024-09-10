package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.ArrayList;
import java.util.List;

public class BeginnersAlgorithm implements Algorithm {
    private final List<Algorithm> stages;

    public BeginnersAlgorithm(Rubik_Impl rubik) {

        stages = new ArrayList<>();

        stages.add(new TopCross(rubik));
        stages.add(new TopCorners(rubik));

        stages.add(new MiddleFlip(rubik));

        stages.add(new MiddleEdges(rubik));

        stages.add(new LLCross(rubik));
        stages.add(new LLSolvedCross(rubik));
        stages.add(new LLCorners(rubik));
        stages.add(new LLSolvedCorners(rubik));

        stages.add(new MiddleFlip(rubik));
     }

    @Override
    public void exec() {
        stages.forEach(Algorithm::exec);
    }
}
