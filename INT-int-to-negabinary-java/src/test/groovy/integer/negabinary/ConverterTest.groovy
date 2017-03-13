package integer.negabinary

import spock.lang.Specification
import spock.lang.Unroll

@Unroll
class ConverterTest extends Specification {

    def "Should return #negabinary value of number #number"() {
        given:
        def converter = new Converter()

        when:
        def convertedValue = converter.toNegabinary(number)

        then:
        convertedValue == negabinary

        where:
        number || negabinary
        1      || "1"
        -1     || "11"
        2      || "110"
        -2     || "10"
        3      || "111"
        -3     || "1101"
        4      || "100"
        -4     || "1100"
        5      || "101"
        6      || "11010"
        7      || "11011"
        8      || "11000"
        9      || "11001"
        10     || "11110"
        11     || "11111"
        12     || "11100"
        13     || "11101"
        14     || "10010"
        15     || "10011"
        16     || "10000"
        17     || "10001"
        18     || "10110"
        19     || "10111"
        20     || "10100"
    }
}