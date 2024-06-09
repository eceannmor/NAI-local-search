import java.math.BigInteger;
import java.util.ArrayList;

class KnapsackLocalSearch {

    static int weights[] = { 3, 1, 6, 10, 1, 4, 9, 1, 7, 2, 6, 1, 6, 2, 2, 4, 8, 1, 7, 3, 6, 2, 9, 5, 3, 3, 4, 7, 3, 5,
            30,
            50 };
    static int values[] = { 7, 4, 9, 18, 9, 15, 4, 2, 6, 13, 18, 12, 12, 16, 19, 19, 10, 16, 14, 3, 14, 4, 15, 7, 5, 10,
            10, 13, 19, 9, 8, 5 };
    static int size = 32;
    static int W_max = 75;

    final static int ITER_LIMIT = 200000;

    public static void main(String[] args) {
        BigInteger best = new BigInteger("0");
        for (int i = 0; i < ITER_LIMIT; i++) {
            BigInteger iterResult = iteration(i + 1);
            if (best.compareTo(iterResult) < 0) {
                best = iterResult;
                System.out.println("Iteration " + i);
                System.out.println(best.toString(2));
            }
        }
        System.out.println("\n\n\n\n\n\n" + "*".repeat(16) + "| FINAL RESULT |" + "*".repeat(16) + "\n");
        int sumVal = 0;
        for (int j = 0; j < 32; j++) {
            if (best.testBit(j)) {
                sumVal += values[j];
            }
        }
        System.out.println(sumVal);
        System.out.println(best.toString(2));
        System.out.println("\n" + "*".repeat(16 * 3) + "\n\n\n\n\n\n");
    }

    public static BigInteger iteration(int iterId) {

        BigInteger randomStartingPoint = new BigInteger("2").pow(size).subtract(new BigInteger("1"))
                .divide(new BigInteger(String.valueOf((int) ((Math.random() + 1) * 10 * iterId))));

        BigInteger currentBest = randomStartingPoint;
        int bestVal = 0;

        while (true) {
            boolean end = true;
            ArrayList<BigInteger> bigList = new ArrayList<>();
            for (int i = 0; i < 32; i++) {
                bigList.add(currentBest.flipBit(i));
            }
            for (BigInteger big : bigList) {
                int sumWeight = 0;
                int sumVal = 0;
                for (int j = 0; j < 32; j++) {
                    if (big.testBit(j)) {
                        sumWeight += weights[j];
                        sumVal += values[j];
                    }
                }
                if (sumWeight > W_max) {
                    continue;
                }
                if (sumVal >= bestVal) {
                    end = false;
                    currentBest = big;
                    bestVal = sumVal;
                }
            }
            if (end) {
                break;
            }
        }
        return currentBest;
    }
}
