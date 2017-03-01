package big.num;

import big.num.operation.Addition;
import big.num.operation.Multiplication;
import com.google.common.primitives.Chars;

import java.util.ArrayList;
import java.util.List;

public class BigNum {

    private List<Integer> numberAsList = new ArrayList<>();

    public BigNum(Integer number) {
        numberAsList = parseNumberToList(number.toString());
    }

    public BigNum(Long number) {
        numberAsList = parseNumberToList(number.toString());
    }

    public BigNum(String number) {
        numberAsList = parseNumberToList(number);
    }

    public List<Integer> getNumberAsList() {
        return numberAsList;
    }

    public BigNum add(BigNum numberToAddition) {
        numberAsList = Addition.add(numberAsList, numberToAddition.getNumberAsList());
        return this;
    }


    public BigNum multiply(BigNum numberToMultiply) {
        List<List<Integer>> partialResults = Multiplication.multiply(numberAsList, numberToMultiply.getNumberAsList());

        List<Integer> result = new ArrayList<>();
        for (List<Integer> partialResult : partialResults) {
            result = Addition.add(result, partialResult);
        }

        numberAsList = result;
        return this;
    }

    public String toString() {
        StringBuilder builder = new StringBuilder();
        numberAsList.forEach(n -> builder.append(n));
        return builder.toString();
    }

    private List<Integer> parseNumberToList(String number) {
        ArrayList<Integer> numberAsList = new ArrayList<>();
        Chars.asList(number.toCharArray()).forEach(c -> numberAsList.add(new Integer(c.toString())));
        return numberAsList;
    }
}
