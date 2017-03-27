package bin.fract;

import java.util.ArrayList;
import java.util.List;

import com.google.common.primitives.Chars;

public class Converter {

    private List<Double> powersOfTwo;

    public Converter(List<Double> powersOfTwo) {
        this.powersOfTwo = powersOfTwo;
    }

    public Double convertToDecimal(String binaryFraction) {
        List<Integer> splitBinaryFraction = splitBinaryFraction(binaryFraction);
        Double result = 0d;

        for (int i = 0; i < splitBinaryFraction.size(); i++) {
            result += splitBinaryFraction.get(i) * powersOfTwo.get(i);
        }

        return result;
    }

    public String convertToBinary(double decimalFraction) {
        StringBuilder result = new StringBuilder();
        int integerPart = (int) Math.floor(decimalFraction);
        result = getNextIntegerBit(result, integerPart);
        result = result.append(".");
        result = getNextFractionalBit(result, decimalFraction - integerPart);

        // I'm too lazy to convert result for better representation of repeating decimals. So sorry :).
        return result.toString();
    }

    private List<Integer> splitBinaryFraction(String binaryFraction) {
        int i = binaryFraction.indexOf(".");
        String numbersAfterPoint = binaryFraction.substring(i + 1);
        List<Integer> splitBinaryFraction = new ArrayList<>();

        Chars.asList(numbersAfterPoint.toCharArray()).forEach(c -> splitBinaryFraction.add(new Integer(c.toString())));
        return splitBinaryFraction;
    }

    private StringBuilder getNextFractionalBit(StringBuilder result, double fractionalPart) {
        if (fractionalPart == 0) {
            return result.append(String.valueOf((int)fractionalPart));
        }

        double multiplicationResult = fractionalPart * 2;
        double floor = Math.floor(multiplicationResult);
        double rest = multiplicationResult - floor;
        result = result.append(String.valueOf((int) floor));
        if (multiplicationResult != 1) {
            return getNextFractionalBit(result, rest);
        }

        return result;
    }

    private StringBuilder getNextIntegerBit(StringBuilder result, int integerPart) {
        int modulo = integerPart % 2;
        int division = integerPart / 2;
        result = result.append(String.valueOf(modulo));
        if (division != 0) {
            return getNextIntegerBit(result, division);
        }

        return result.reverse();
    }
}
