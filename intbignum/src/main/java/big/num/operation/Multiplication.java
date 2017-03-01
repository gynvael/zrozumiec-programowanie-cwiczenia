package big.num.operation;

import com.google.common.collect.Lists;

import java.util.ArrayList;
import java.util.List;

public class Multiplication extends CommonOperation {

    public static List<List<Integer>> multiply(List<Integer> multiplier, List<Integer> multiplicand) {
        // // Reversing list order - multiplication from end
        List<Integer> reversedMultiplier = Lists.reverse(multiplier);
        List<Integer> reversedMultiplicand = Lists.reverse(multiplicand);

        return makeMultiplication(reversedMultiplier, reversedMultiplicand);
    }

    private static List<List<Integer>> makeMultiplication(List<Integer> reversedMultiplier, List<Integer> reversedMultiplicand) {
        // list of results which will be added together
        List<List<Integer>> partialResults = new ArrayList<>();

        // Multiply each digit from multiplier with each digit from multiplicand
        for (Integer multiplier : reversedMultiplier) {
            List<Integer> partialResult = calculatePartialResult(reversedMultiplicand, multiplier, partialResults.size());
            partialResults.add(Lists.reverse(partialResult));
        }

        return partialResults;
    }

    private static List<Integer> calculatePartialResult(List<Integer> reversedMultiplicand, Integer multiplier, int numberOfZeros) {
        List<Integer> partialResult = new ArrayList<>();
        // aligning list size, next one partial result is completed by zeros - easier loop iteration when both lists have same size
        fillWithZeros(partialResult, numberOfZeros);
        int valueToCarry = 0;
        for (Integer multiplicand : reversedMultiplicand) {
            int multiplication = multiplicand * multiplier + valueToCarry;
            partialResult.add(getOperationResult(multiplication));
            valueToCarry = getValueToCarry(multiplication);
        }

        if (valueToCarry != 0) {
            partialResult.add(valueToCarry);
        }
        return partialResult;
    }
}
