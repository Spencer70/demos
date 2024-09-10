package org.rubik.impl;

public enum Faces {

    // WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
    UP, DOWN, RIGHT, LEFT, FRONT, BACK;

    // Side faces ordered clockwise as left, top, right, bottom
    public enum Sides {
        LEFT, TOP, RIGHT, BOTTOM
    }

    private Faces[] sides;
    private Faces opposite;

    // Define cube topology
    static {
        UP.sides = new Faces[]{ LEFT, BACK, RIGHT, FRONT };
        UP.opposite = DOWN;

        DOWN.sides = new Faces[]{ LEFT, FRONT, RIGHT, BACK };
        DOWN.opposite = UP;

        RIGHT.sides = new Faces[]{ FRONT, UP, BACK, DOWN };
        RIGHT.opposite = LEFT;

        LEFT.sides = new Faces[]{ BACK, UP, FRONT, DOWN };
        LEFT.opposite = RIGHT;

        FRONT.sides = new Faces[]{ LEFT, UP, RIGHT, DOWN };
        FRONT.opposite = BACK;

        BACK.sides = new Faces[]{ RIGHT, UP, LEFT, DOWN };
        BACK.opposite = FRONT;
    }

    public Faces[] getSides() { return sides;  }
    public Faces opposite() { return opposite; }
}
