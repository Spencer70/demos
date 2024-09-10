package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;

public class TopCorners extends L1L2Stage {
    TopCorners(Rubik_Impl rubik) {
        super(rubik, "D");
    }

    public Piece findPiece(Colors side, Colors prev) {
        return fromFacets(rubik.faces.get(Faces.UP), side, prev);
    }

    public Colors preparePiece(Piece piece, Colors sideColor) {
        // send top piece to down
        if (piece.containsValue(rubik.faces.get(Faces.UP))) {
            // piece in top layer
            rubik.permute((piece.containsValue(rubik.faces.get(Faces.RIGHT)) ? "R'D'R" : "LDL'"));
        }
        return sideColor;
    }

    public void solvePiece(Piece piece) {
        // Bring piece to up face
        if (piece.get(rubik.faces.get(Faces.UP)) == rubik.faces.get(Faces.DOWN)) {
            rubik.permute("R'D2RDR'D'R");
        } else if (piece.get(rubik.faces.get(Faces.UP)) == rubik.faces.get(Faces.FRONT)) {
            rubik.permute("FDF'");
        } else {
            rubik.permute("R'D'R");
        }
    }
}
