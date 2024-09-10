package org.rubik.impl;

public class Move {
    public final Faces face;
    public final boolean clockWise;
    public final int step;

    public Move(Faces face) {
        this(face, true, 1);
    }

    public Move(Faces face, boolean clockWise) {
        this(face, clockWise, 1);
    }
    public Move(Faces face, int step) {
        this(face, true, step);
    }

    public Move(Faces face, boolean clockWise, int step) {
        this.face = face;
        // Normalize to allowed step values: 1, 2
        this.step = (step > 1 ? 2 : 1);
        // step = 2 -> normalize to clockwise
        this.clockWise = (step == 2 || clockWise);
    }

    @Override
    public String toString() {
        String move = face.toString().substring(0, 1);
        if (!clockWise) {
            move += "'";
        } else if (step == 2) {
            move += step;
        }

        return move;
    }
}
