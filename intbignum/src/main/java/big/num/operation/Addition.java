package big.num.operation;

import com.google.common.collect.Lists;

import java.util.ArrayList;
import java.util.List;

public class Addition extends CommonOperation {

    public static List<Integer> add(List<Integer> firstQuantity, List<Integer> secondQuantity) {
        // Reversing list order - addition from end
        List<Integer> reversedFirstQuantity = Lists.reverse(firstQuantity);
        List<Integer> reversedSecondQuantity = Lists.reverse(secondQuantity);

        // aligning list size, shorter is completed by zeros - easier loop iteration when both lists have same size
        alignListsToSameSize(reversedFirstQuantity, reversedSecondQuantity);

        // addition
        List<Integer> reversedResultList = makeAddition(reversedFirstQuantity, reversedSecondQuantity);

        // reversing result list
        return Lists.reverse(reversedResultList);
    }

    private static void alignListsToSameSize(List<Integer> reverseNumberToAddition, List<Integer> reverseCurrentNumber) {
        int currentNumberSize = reverseCurrentNumber.size();
        int numberToAdditionSize = reverseNumberToAddition.size();

        if (currentNumberSize > numberToAdditionSize) {
            int numberOfZeros = currentNumberSize - numberToAdditionSize;
            fillWithZeros(reverseNumberToAddition, numberOfZeros);
        } else {
            int numberOfZeros = numberToAdditionSize - currentNumberSize;
            fillWithZeros(reverseCurrentNumber, numberOfZeros);
        }
    }

    private static List<Integer> makeAddition(List<Integer> reverseNumberToAddition, List<Integer> reverseCurrentNumber) {
        List<Integer> reversedResultList = new ArrayList<>();
        int valueToCarry = 0;

        for (int i = 0; i < reverseCurrentNumber.size(); i++) {
            int result = reverseCurrentNumber.get(i) + reverseNumberToAddition.get(i) + valueToCarry;

            reversedResultList.add(getOperationResult(result));
            valueToCarry = getValueToCarry(result);
        }

        if (valueToCarry != 0) {
            reversedResultList.add(valueToCarry);
        }

        return reversedResultList;
    }
}
