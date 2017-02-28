package big.num.operation;

import java.util.List;
import java.util.stream.IntStream;

public class CommonOperation {

    static void fillWithZeros(List<Integer> reverseCurrentNumber, int numberOfZeros) {
        IntStream.range(0, numberOfZeros).forEach(i -> reverseCurrentNumber.add(0));
    }

    static int getOperationResult(int operationValue) {
        return operationValue % 10;
    }

    static int getValueToCarry(int operationValue) {
        return operationValue / 10;
    }
}

