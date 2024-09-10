package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.List;
import java.util.stream.Collectors;

public class LLCorners implements Algorithm {
    private final Rubik_Impl rubik;

    public LLCorners(Rubik_Impl rubik) {
        this.rubik = rubik;
    }

    @Override
    public void exec() {
        // Search last layer
        final Colors upColor = rubik.faces.get(Faces.UP);
        final var LLCorners = rubik.facePieces.get(upColor).stream()
                .filter(Piece::isCorner)
                .collect(Collectors.toList());

        // while not all corners in right place
        while (!LLCorners.stream().allMatch(Piece::isRightPlace)) {
            final var CornerRightPlace = LLCorners.stream()
                    .filter(Piece::isRightPlace)
                    .findAny();
            if (CornerRightPlace.isPresent()) {
                Piece piece = CornerRightPlace.get();
                while (!(piece.containsValue(rubik.faces.get(Faces.FRONT)) &&
                        piece.containsValue(rubik.faces.get(Faces.RIGHT)))) {
                    rubik.rotate("U");
                }
            }
            rubik.permute("URU'L'UR'U'L");
        }
    }
}