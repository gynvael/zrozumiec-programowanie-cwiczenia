package integer.negabinary;

public class Converter {

    private static final double BASE = -2.0;

    public String toNegabinary(int number) {
        StringBuilder result = new StringBuilder();
        return convert(result, number).reverse().toString();
    }

    private StringBuilder convert(StringBuilder result, int number) {
        int ceilOfDivision = (int) Math.ceil(number / BASE);
        int reminder = (int) (number % BASE);
        result = result.append(Math.abs(reminder));

        if (ceilOfDivision != 0) {
            return convert(result, ceilOfDivision);
        }

        return result;
    }
}
