package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.List;
import java.util.stream.Collectors;


public class LLCross implements Algorithm {
    private final Rubik_Impl rubik;

    public LLCross(Rubik_Impl rubik) {
        this.rubik = rubik;
    }

     @Override
    public void exec() {
        // Search last layer
        final Colors upColor = rubik.faces.get(Faces.UP);
        final var LLEdges = rubik.facePieces.get(upColor).stream()
                 .filter(Piece::isEdge)
                 .collect(Collectors.toList());

        // Repeat until upcolor cross solved.
        List<Piece> crossPieces;
        // Find cross pieces with solved upcolor
        while ((crossPieces = LLEdges.stream()
                .filter(piece -> piece.get(upColor) == upColor)
                .collect(Collectors.toList())).size() < 4) {
            if (crossPieces.size() == 2) {
                // Order found pieces
                Piece leftPiece;
                while ((leftPiece = findSemiCross(crossPieces)) == null) {
                    rubik.rotate("U");
                }
                // Match semi-cross to left side color
                while (leftPiece.get(rubik.faces.get(Faces.LEFT)) != rubik.faces.get(Faces.LEFT)) {
                    rubik.permute("U'");
                    rubik.rotate("U");
                }
            }
            // Try to solve 4 cross pieces to upcolor
            rubik.permute("FRUR'U'F'");
        }
    }

    Piece findSemiCross(List<Piece> crossPieces) {
        Piece leftPiece = null;

        if (crossPieces.stream()
                .anyMatch(piece -> piece.containsValue(rubik.faces.get(Faces.BACK)) ||
                                    piece.containsValue(rubik.faces.get(Faces.RIGHT)))) {
            leftPiece = crossPieces.stream()
                    .filter(piece -> piece.containsValue(rubik.faces.get(Faces.LEFT)))
                    .findAny()
                    .orElse(null);
        }
        return leftPiece;
    }
}