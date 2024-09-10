package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;

public class TopCross extends L1L2Stage {
    TopCross (Rubik_Impl rubik) {
        super(rubik, "D");
    }

    public Piece findPiece(Colors side, Colors prev) {
        return fromFacets(rubik.faces.get(Faces.UP), side);
    }

    public Colors preparePiece(Piece piece, Colors sideColor) {
        // send top/middle piece to down
        if (piece.containsValue(rubik.faces.get(Faces.UP))) {
            // piece in top layer
            rubik.permute("F2");
        } else if (!piece.containsValue(rubik.faces.get(Faces.DOWN))) {
            // piece in middle layer
            rubik.permute((piece.containsValue(rubik.faces.get(Faces.RIGHT)) ? "FDF'" : "F'DF"));
        }
        return sideColor;
    }

    public void solvePiece(Piece piece) {
        // Bring piece to up face
        rubik.permute((piece.get(rubik.faces.get(Faces.UP)) == rubik.faces.get(Faces.DOWN) ? "F2" : "DRF'R'"));
    }
}
