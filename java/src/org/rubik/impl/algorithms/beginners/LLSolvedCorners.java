package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.List;
import java.util.stream.Collectors;

public class LLSolvedCorners implements Algorithm {
    private final Rubik_Impl rubik;

    public LLSolvedCorners(Rubik_Impl rubik) {
        this.rubik = rubik;
    }

    @Override
    public void exec() {
        // Search last layer
        final Colors upColor = rubik.faces.get(Faces.UP);
        final var LLCornersNotSolved = rubik.facePieces.get(upColor).stream()
                .filter(piece -> piece.isCorner() && !piece.isSolved())
                .collect(Collectors.toList());

        if (!LLCornersNotSolved.isEmpty()) {
            // Order 1rst corner not solved
            Piece piece1rst = LLCornersNotSolved.get(0);
            while (!(piece1rst.containsValue(rubik.faces.get(Faces.FRONT)) &&
                    piece1rst.containsValue(rubik.faces.get(Faces.RIGHT)))) {
                rubik.rotate("U");
            }

            LLCornersNotSolved.forEach(piece -> {
                while (!(piece.containsValue(rubik.faces.get(Faces.FRONT)) &&
                        piece.containsValue(rubik.faces.get(Faces.RIGHT)))) {
                    rubik.permute("U");
                }
                // Solve piece
                while (piece.get(upColor) != upColor) {
                    rubik.permute("R'D'RD");
                }
            });
            // Permute LL to match sides
            while (!piece1rst.isSolved()) {
                rubik.permute("U");
            }
        }
    }
}