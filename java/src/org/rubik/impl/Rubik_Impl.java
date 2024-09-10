package org.rubik.impl;

import org.rubik.api.Rubik;
import org.rubik.api.RubikFactory;
import org.rubik.impl.algorithms.Algorithm;
import org.rubik.impl.algorithms.beginners.BeginnersAlgorithm;
import org.rubik.impl.algorithms.test.TestAlgorithm;

import java.util.*;
import java.util.stream.Collectors;

public class Rubik_Impl implements Rubik {
    public final List<Piece> cube;
    public final Map<Faces, Colors> faces;
    public final Map<Colors, List<Piece>> facePieces;
    public List<Move> moves;
    private final Random seed = new Random();

    private final int NUM_ROTATIONS = 5;
    private final int NUM_SHUFFLE = 10;

    private boolean showMoves = true;


    public Rubik_Impl(RubikFactory factory) {
        if (factory == null) {
            throw new IllegalArgumentException("Illegal Rubik creation. Call RubikFactory.create method instead");
        }
        // cube list of pieces: 8 corners + 12 edges
        cube = new ArrayList<>(20);
        // Face index
        facePieces = new EnumMap<>(Colors.class);
        for (Colors color : Colors.values()) {
            facePieces.put(color, new ArrayList<>(8));
        }

        // Create cube faces
        faces = new EnumMap<>(Faces.class);
        for (Faces face : Faces.values()) {
            // Initial colors as ordered in enum declarations
            // WHITE, YELLOW, BLUE, GREEN, RED, ORANGE
            // UP, DOWN, RIGHT, LEFT, FRONT, BACK;
            faces.put(face, Colors.values()[face.ordinal()]);
        }

        // Create pieces
        Colors[] sideFaces = new Colors[4];
        for (int i = 0; i < Faces.DOWN.getSides().length; i++) {
            sideFaces[i] = faces.get(Faces.DOWN.getSides()[i]);
        }
        Colors[] upDown = new Colors[]{faces.get(Faces.UP), faces.get(Faces.DOWN)};
        Piece edge, corner;
        for (int i = 0; i < sideFaces.length; i++) {
            int j = (i + 1) % sideFaces.length;
            // Middle edges
            edge = new Piece(sideFaces[i], sideFaces[j]);
            cube.add(edge);
            facePieces.get(sideFaces[i]).add(edge);
            facePieces.get(sideFaces[j]).add(edge);
            for (int k = 0; k < upDown.length; k++) {
                // up, down edges
                edge = new Piece(sideFaces[i], upDown[k]);
                cube.add(edge);
                facePieces.get(sideFaces[i]).add(edge);
                facePieces.get(upDown[k]).add(edge);
                // corners
                corner = new Piece(sideFaces[i], sideFaces[j], upDown[k]);
                cube.add(corner);
                facePieces.get(sideFaces[i]).add(corner);
                facePieces.get(sideFaces[j]).add(corner);
                facePieces.get(upDown[k]).add(corner);
            }
        }
    }

    public void print() {
        // print cube faces unfolded:
        // UP
        List<String> layout = draw(Faces.UP);
        layout.forEach(row -> System.out.printf("%" + (row.length() + 6) + "s%n", row));

         // LEFT - FRONT - RIGHT - BACK. Sides of DOWN
        var sides = Faces.DOWN.getSides();
        Map<Faces, List<String>> layouts = new HashMap<>(sides.length);
        for (var side : sides) {
            layouts.put(side, draw(side));
        }

        for (int i = 0; i < layouts.get(sides[0]).size(); i++) {
            for (var side : sides) {
                System.out.printf("%s ", layouts.get(side).get(i));
            }
            System.out.println();
        }
        // DOWN
        layout = draw(Faces.DOWN);
        layout.forEach(row -> System.out.printf("%" + (row.length() + 6) + "s%n", row));
    }

    private String randomMoves(int numMoves) {
        String strMoves = "";
        for (int i = 0; i < numMoves; i++) {
            strMoves += Rubik.MOVES.charAt(seed.nextInt(Rubik.MOVES.length()));
        }
        return strMoves;
    }

    public void rotate(String strMoves) {
        if (strMoves.equals("RANDOM")) {
            strMoves = randomMoves(NUM_ROTATIONS);
        }
        List<Move> rotations = parse(strMoves);

        // Debug helper
        if (showMoves) {
            System.out.println(draw(rotations));
        }

        rotate(rotations);
    }

    private void rotate(Move move) {
        var sideFaces = move.face.getSides();

        int step = (move.clockWise ? move.step : -move.step);

        // Assign previous face color to current face
        final Map<Faces, Colors> prevFaces = new EnumMap<>(faces);
        final int N = sideFaces.length;
        for (int i = 0; i < N; i++) {
            int prevFace = (N + i - step) % N;
            Colors prevColor = prevFaces.get(sideFaces[prevFace]);
            faces.put(sideFaces[i], prevColor);
        }

        if (showMoves) {
            System.out.println(move);
            print();
        }
    }

    private void rotate(List<Move> moves) {
        moves.forEach(this::rotate);
    }

    public boolean isSolved() {
        return cube.stream().allMatch(Piece::isSolved);
    }

    public void shuffle(String strMoves) {
        // Generate random moves
        if (strMoves.equals("RANDOM")) {
            strMoves = randomMoves(NUM_SHUFFLE + seed.nextInt(NUM_SHUFFLE));
        }

        //  Simplest algorithm: remember moves
        moves = parse(strMoves);
        // Debug helper
        if (showMoves) {
            System.out.println(draw(moves));
        }

        permute(moves);
    }

    public void solve() {
        showMoves = false;

        boolean simplest = false;
        if (simplest) {
            //  Test algorithm: undo moves
            Algorithm testAlgorithm = new TestAlgorithm(this);
            testAlgorithm.exec();
            return;
        }

        //  Beginners algorithm
        Algorithm beginnersAlgorithm = new BeginnersAlgorithm(this);
        beginnersAlgorithm.exec();
    }

    private List<String> draw(Faces face) {
        var faceColor = faces.get(face);
        var sideColors = Arrays.stream(face.getSides())
                .map(faces::get)
                .collect(Collectors.toList());

        String[][] layout = new String[3][3];
        // Set center piece
        layout[1][1] = faceColor.toString();
        facePieces.get(faceColor).forEach(piece -> {
            int row = 1, col = 1;
            Colors pieceColor = faceColor;
            for (var entry : piece.entrySet()) {
                if (entry.getValue() == faceColor) {
                    pieceColor = entry.getKey();
                } else if (entry.getValue() == sideColors.get(Faces.Sides.TOP.ordinal())) {
                    row = 0;
                } else if (entry.getValue() == sideColors.get(Faces.Sides.BOTTOM.ordinal())) {
                    row = 2;
                }
                if (entry.getValue() == sideColors.get(Faces.Sides.LEFT.ordinal())) {
                    col = 0;
                } else if (entry.getValue() == sideColors.get(Faces.Sides.RIGHT.ordinal())) {
                    col = 2;
                }
            }
            layout[row][col] = pieceColor.toString();
        });

        // Draw rows as strings of colors
        List<String> rows = new ArrayList<>(3);
        for (String[] row : layout) {
            rows.add(String.join(" ", row));
        }
        return rows;
    }

    public void permute(String strMoves) {
        permute(parse(strMoves));
    }

    public void permute(Move move) {
        // pre-process move
        var faceColor = faces.get(move.face);
        var sideColors = Arrays.stream(move.face.getSides())
                .map(faces::get)
                .collect(Collectors.toList());
        int step = (move.clockWise ? move.step : -move.step);

        // Remove pieces of this rotating face from side faces
        for (var sideColor : sideColors) {
            var Pieces = facePieces.get(sideColor);
            Pieces.removeIf(piece -> piece.containsValue(faceColor));
        }

        // Rotate pieces && add to new side faces
        var Pieces = facePieces.get(faceColor);
        Pieces.forEach(piece -> {
            final int N = sideColors.size();
            for (var entry : piece.entrySet()) {
                int i = sideColors.indexOf(entry.getValue());
                if (i != -1) {
                    int nextFace = (N + i + step) % N;
                    Colors newFace = sideColors.get(nextFace);
                    entry.setValue(newFace);
                    // Add rotated piece to new face
                    facePieces.get(newFace).add(piece);
                }
            }
        });

        if (showMoves) {
            System.out.println(move);
            print();
        }
    }

    public void permute(List<Move> moves) {
        moves.forEach(this::permute);
    }

    private String draw(List<Move> moves) {
        StringBuilder strMoves = new StringBuilder();
        moves.forEach(strMoves::append);

        return strMoves.toString();
    }

    private List<Move> parse(String strMoves) {
        List<Move> moves = new ArrayList<>(strMoves.length());

        Faces face = null;
        boolean clockWise = true;
        int step = 1;
        for (int i = 0; i < strMoves.length(); i++) {
            int m = Rubik.MOVES.indexOf(strMoves.charAt(i));
            if (m < 0) {
                continue;
            }
            if (m < 6) {
                if (face != null) {
                    moves.add(new Move(face, clockWise, step));
                }
                face = Faces.values()[m];
                clockWise = true;
                step = 1;
            } else if (m == 6) {
                clockWise = false;
            } else {
                step = 2;
            }
        }
        if (face != null) {
            moves.add(new Move(face, clockWise, step));
        }

        return moves;
    }

}
