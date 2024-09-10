package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;
import org.rubik.impl.algorithms.Algorithm;

import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

abstract class L1L2Stage implements Algorithm {
    protected final Rubik_Impl rubik;

    protected String workingFace;
    L1L2Stage(Rubik_Impl rubik, String workingFace) {
        this.rubik = rubik;
        this.workingFace = workingFace;
    }

    protected Piece fromFacets(Colors... facets) {
        // Search whole cube
        return rubik.cube.stream().filter(piece -> piece.match(facets)).findAny().orElse(null);
    }


    public void exec() {
        final List<Colors> sideColors = Arrays.stream(Faces.UP.getSides())
                .map(rubik.faces::get)
                .collect(Collectors.toList());
        Colors prevSideColor = sideColors.get(sideColors.size() - 1);
        for (var sideColor : sideColors) {
            Piece piece = findPiece(sideColor, prevSideColor);
            // check piece in right position; do nothing
            if (piece.isSolved()) {
                prevSideColor = sideColor;
                continue;
            }

            // Rotate cube sides to bring piece to front
            while (!piece.containsValue(rubik.faces.get(Faces.FRONT)) ||
                    piece.isCorner() && !piece.containsValue(rubik.faces.get(Faces.RIGHT))) {
                rubik.rotate("U");
            }

            final Colors frontColor = preparePiece(piece, sideColor);

            // Rotate cube sides to bring sideColor face to front
            while (rubik.faces.get(Faces.FRONT) != frontColor) {
                rubik.rotate("U");
            }

            // Bring piece to front
            while (!piece.containsValue(frontColor) ||
                    piece.isCorner() && !piece.containsValue(prevSideColor)) {
                rubik.permute(workingFace);
            }

            solvePiece(piece);

            prevSideColor = sideColor;
        }

    }

    public abstract Piece findPiece(Colors side, Colors prev);

    public abstract Colors preparePiece(Piece piece, Colors sideColor);

    public abstract void solvePiece(Piece piece);

}
