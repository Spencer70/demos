package org.rubik.impl;

import java.util.*;

// Pieces are invariant in colors
// Pieces faces are referenced to center pieces colors. Invariant for rotations
public class Piece extends EnumMap<Colors, Colors> {
    // Create ordered.
    // Every color in face of that color
    public Piece(Colors... facets) {
        super(Colors.class);
        for (Colors facet : facets) {
            put(facet, facet);
        }
    }

    public boolean isEdge() {
        return (size() == 2);
    }

    public boolean isCorner() {
        return (size() == 3);
    }

    public boolean match(Colors... facet) {
        return keySet().equals(new HashSet<>(List.of(facet)));
    }

    public boolean isSolved() {
        return entrySet().stream().allMatch(e -> e.getKey() == e.getValue());
    }

    public boolean isRightPlace() {
        return match(values().toArray(new Colors[0]));
    }

}