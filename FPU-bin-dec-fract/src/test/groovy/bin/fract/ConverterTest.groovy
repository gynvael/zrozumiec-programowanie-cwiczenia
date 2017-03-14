package bin.fract

import spock.lang.Specification
import spock.lang.Unroll

@Unroll
class ConverterTest extends Specification {

    def "Should return decimal #result fraction for binary #value fraction"() {
        given:
        def mathPower = new MathPower()
        def converter = new Converter(mathPower.getPowersOfTwo())

        when:
        def decimalFraction = converter.convertToDecimal(value)

        then:
        decimalFraction.toString() == result

        where:
        value        || result
        "0.100101"   || "0.578125"
        "0.10000000" || "0.5"
        "0.01000000" || "0.25"
        "0.11000000" || "0.75"
        "0.10110110" || "0.7109375"
    }

    def "Should return binary #result fraction for decimal #value fraction"() {
        given:
        def mathPower = new MathPower()
        def converter = new Converter(mathPower.getPowersOfTwo())

        when:
        def binaryFraction = converter.convertToBinary(value)

        then:
        binaryFraction == result

        where:
        value     || result
        0.578125  || "0.100101"
        0.5       || "0.1"
        0.25      || "0.01"
        0.75      || "0.11"
        0.7109375 || "0.1011011"
        0.125     || "0.001"
        0.3       || "0.010011001100110011001100110011001100110011001100110011"
        2.17      || "10.0010101110000101000111101011100001010001111010111"
        12.835    || "1100.11010101110000101000111101011100001010001111011"
        134.0     || "10000110.0"
        23.0      || "10111.0"
        137.9972  || "10001001.111111110100100001111111110010111001001000111"
    }
}
