import java.math.BigInteger;

class KnapsackBruteforce {
    public static void main(String[] args) {
        int weights[] = { 3, 1, 6, 10, 1, 4, 9, 1, 7, 2, 6, 1, 6, 2, 2, 4, 8, 1, 7, 3, 6, 2, 9, 5, 3, 3, 4, 7, 3, 5, 30,
                50 };
        int values[] = { 7, 4, 9, 18, 9, 15, 4, 2, 6, 13, 18, 12, 12, 16, 19, 19, 10, 16, 14, 3, 14, 4, 15, 7, 5, 10,
                10, 13, 19, 9, 8, 5 };
        int size = 32;
        BigInteger numPossible = new BigInteger("2").pow(size);
        int W_max = 75;

        BigInteger resultBits = new BigInteger("-1");
        int resultVal = 0;

        for (BigInteger i = new BigInteger("0"); i.compareTo(numPossible) < 0; i = i.add(new BigInteger("1"))) {
            int sumWeight = 0;
            int sumVal = 0;
            for (int j = 0; j < size - 1; j++) {
                if (i.testBit(j)) {
                    sumWeight += weights[j];
                    sumVal += values[j];
                }
            }
            if (sumWeight > W_max) {
                continue;
            }
            if (sumVal >= resultVal) {
                resultBits = i;
                resultVal = sumVal;
                System.out.println(resultVal);
                System.out.println(resultBits.toString(2));
            }
        }
        System.out.println("\n\n\n");
        System.out.println(resultVal);
        System.out.println(resultBits.toString(2));
    }
}