package integer.negabinary;

public class App {
    public static void main(String[] args) {
        Converter converter = new Converter();
        int number = -9;
        System.out.println(number + " converted to negabinary is: " + converter.toNegabinary(number));
    }
}
