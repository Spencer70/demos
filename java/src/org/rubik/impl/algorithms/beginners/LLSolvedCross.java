package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.List;
import java.util.stream.Collectors;

public class LLSolvedCross implements Algorithm {
    private final Rubik_Impl rubik;

    public LLSolvedCross(Rubik_Impl rubik) {
        this.rubik = rubik;
    }

    @Override
    public void exec() {
        // Search last layer
        final Colors upColor = rubik.faces.get(Faces.UP);
        final var LLEdges = rubik.facePieces.get(upColor).stream()
                .filter(Piece::isEdge)
                .collect(Collectors.toList());

        // Repeat until cross is solved.
        while (true) {
            // there always are 2 solved cross pieces just moving upface
            List<Piece> crossSolvedPieces;
            while ((crossSolvedPieces = LLEdges.stream()
                    .filter(Piece::isSolved)
                    .collect(Collectors.toList())).size() < 2) {
                rubik.permute("U");
            }
            if (crossSolvedPieces.size() == 4)
                break;

            // Order solved pieces
            while (!findSemiCross(crossSolvedPieces)) {
                rubik.rotate("U");
            }
            // Try to solve 4 cross pieces
            rubik.permute("URUR'URU2R'");
        }
/*
        // Initially, solve 2 cross pieces
        while (LLEdges.stream()
                .filter(Piece::isSolved)
                .count() < 2) {
            rubik.permute("U");
        }

        // Repeat until cross is solved.
        List<Piece> crossSolvedPieces;
        while ((crossSolvedPieces = LLEdges.stream()
                .filter(Piece::isSolved)
                .collect(Collectors.toList())).size() < 4) {
            // Order solved pieces
            while (!findSemiCross(crossSolvedPieces)) {
                rubik.rotate("U");
            }
            // Try to solve 4 cross pieces
            rubik.permute("URUR'URU2R'");
        }
*/
    }

    boolean findSemiCross(List<Piece> crossPieces) {
        return (crossPieces.stream()
                .anyMatch(piece -> piece.containsValue(rubik.faces.get(Faces.BACK)) ||
                        piece.containsValue(rubik.faces.get(Faces.RIGHT))) &&
                crossPieces.stream()
                    .anyMatch(piece -> piece.containsValue(rubik.faces.get(Faces.LEFT))));
    }
}