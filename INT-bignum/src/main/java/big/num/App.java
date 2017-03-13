package big.num;

/*
The MIT License (MIT)
Copyright (c) 2017 Kinga Makowiecka

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation file
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

public class App {
    public static void main(String[] args) {
        BigNum firstBigNum = new BigNum("1022222222000000000000000232342354454564");
        BigNum secondBigNum = new BigNum("1022220098762000000000000000232342354454564");
        BigNum thirdBigNum = new BigNum("6660098762000000000000000232342154564009");


        System.out.println("FirstBigNum: " + firstBigNum.toString());
        System.out.println("SecondBigNum: " + secondBigNum.toString());
        System.out.println("ThirdBigNum: " + thirdBigNum.toString());

        System.out.println(firstBigNum + " + " + secondBigNum + " = " + firstBigNum.add(secondBigNum));
        System.out.println(firstBigNum + " * " + secondBigNum + " = " + firstBigNum.multiply(secondBigNum));
        System.out.println(firstBigNum + " * " + secondBigNum + " + " + thirdBigNum + " = " + firstBigNum.multiply(secondBigNum).add(thirdBigNum));
    }
}
