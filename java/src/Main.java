import org.rubik.api.Rubik;
import org.rubik.api.RubikFactory;

public class Main {
    public static void main(String[] args) {

        System.out.println("Rubik solver!");

        // Create rubik
        System.out.println("\nInitial state\n");
        Rubik rubik = RubikFactory.create();
        rubik.print();
        if (!rubik.isSolved()) throw new AssertionError();

        for(int i = 0; i < 1000; i++) {

            // Rotate cube
            System.out.println("\nRotated state\n");
            String strRotations = (args.length > 0 ? args[0].toUpperCase() : "RANDOM");
            rubik.rotate(strRotations);
            rubik.print();
            if (!rubik.isSolved()) throw new AssertionError();

            // Shuffle
            System.out.println("\nShuffled state\n");
            String strMoves = (args.length > 1 ? args[1].toUpperCase() : "RANDOM");
            rubik.shuffle(strMoves);
            rubik.print();

            // Solve
            System.out.println("\nSolved state\n");
            rubik.solve();
            rubik.print();

            if (rubik.isSolved()) {
                System.out.println("\nRubik solved!\n");
            }

        }
    }
}