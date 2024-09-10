package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

public class MiddleFlip implements Algorithm {
    private final Rubik_Impl rubik;

    public MiddleFlip(Rubik_Impl rubik) {
        this.rubik = rubik;
    }


    @Override
    public void exec() {
        // Rotate cube upside down
        rubik.rotate("R2");
    }
}
