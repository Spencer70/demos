package org.rubik.impl.algorithms.beginners;

import org.rubik.impl.Colors;
import org.rubik.impl.Faces;
import org.rubik.impl.Piece;
import org.rubik.impl.Rubik_Impl;

import java.util.ArrayList;
import java.util.List;

public class MiddleEdges extends L1L2Stage {
    MiddleEdges(Rubik_Impl rubik) {
        super(rubik, "U");
    }

    public Piece findPiece(Colors side, Colors prev) {
        return fromFacets(side, prev);
    }

    public Colors preparePiece(Piece piece, Colors sideColor) {
        // send middle piece to up
        if (!piece.containsValue(rubik.faces.get(Faces.UP))) {
            swapEdges(piece, piece.containsValue(rubik.faces.get(Faces.RIGHT)));
        }
        List<Colors> keys = new ArrayList<>(piece.keySet());
        return (piece.get(keys.get(0)) != rubik.faces.get(Faces.UP) ? keys.get(0) : keys.get(1));
    }

    public void solvePiece(Piece piece) {
        swapEdges(piece, piece.get(rubik.faces.get(Faces.RIGHT)) == rubik.faces.get(Faces.UP));
    }

    private void swapEdges(Piece piece, boolean toRight) {
        if (toRight) {
            rubik.permute("URU'R'U'F'UF");
        } else {
            rubik.permute("U'L'ULUFU'F'");
        }
    }
}
