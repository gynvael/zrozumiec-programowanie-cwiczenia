package big.num

import spock.lang.Specification
import spock.lang.Unroll

@Unroll
class BigNumTest extends Specification {

    def "Should return String value of number #num"() {
        given:
        def bigNum = new BigNum(num)

        when:
        def stringNum = bigNum.toString()

        then:
        stringNum == string

        where:
        num                     || string
        21                      || "21"
        "234"                   || "234"
        9223372036854775807L    || "9223372036854775807"
        2147483647              || "2147483647"
        "922337203685477580789" || "922337203685477580789"
    }

    def "Should add two numbers: #firstNum and #secondNum"() {
        given:
        def firstBigNum = new BigNum(firstNum)
        def secondBigNum = new BigNum(secondNum)

        when:
        firstBigNum.add(secondBigNum);

        then:
        firstBigNum.toString() == result

        where:
        firstNum                || secondNum                                       || result
        "1230"                  || "123"                                           || "1353"
        "123"                   || "1222"                                          || "1345"
        9223372036854775807L    || 1                                               || "9223372036854775808"
        2L                      || 2147483647                                      || "2147483649"
        "922337203685477580783" || 6                                               || "922337203685477580789"
        "922337203685477580789" || 2                                               || "922337203685477580791"
        "99"                    || 11                                              || "110"
        "99"                    || 111                                             || "210"
        "922337203685477580789" || "922337203685477580789"                         || "1844674407370955161578"
        "922337203685477580789" || "100000000000000000000000000000000000000000000" || "100000000000000000000000922337203685477580789"
        "922337203685477599789" || "100000000000000000000000000000000000000000400" || "100000000000000000000000922337203685477600189"
    }

    def "Should add three numbers"() {
        given:
        def firstBigNum = new BigNum("1230000998776654433")
        def secondBigNum = new BigNum(2147483647)
        def thirdBigNum = new BigNum(9223372036854775807L)

        when:
        firstBigNum.add(secondBigNum).add(thirdBigNum);

        then:
        firstBigNum.toString() == "10453373037778913887"
    }

    def "Should mutliply numbers: #firstNum and #secondNum"() {
        given:
        def firstBigNum = new BigNum(firstNum)
        def secondBigNum = new BigNum(secondNum)

        when:
        firstBigNum.multiply(secondBigNum)

        then:
        firstBigNum.toString() == result

        where:
        firstNum                || secondNum                                       || result
        "11"                    || "12"                                            || "132"
        6                       || 8                                               || "48"
        17                      || 213L                                            || "3621"
        "922337203685477580789" || "100000000000000000000000000000000000000000000" || "92233720368547758078900000000000000000000000000000000000000000000"
        "922337203685477599789" || "100000000000000000000000000000000000000000400" || "92233720368547759978900000000000000000000368934881474191039915600"
        "123"                   || "1222"                                          || "150306"
        9223372036854775807L    || 1                                               || "9223372036854775807"
        2L                      || 2147483647                                      || "4294967294"
        "922337203685477580783" || 6                                               || "5534023222112865484698"
        "99"                    || 111                                             || "10989"
        "922337203685477580789" || "922337203685477580789"                         || "850705917302346158638145100098340021862521"
    }

}