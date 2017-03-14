package bin.fract;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class MathPower {

    public List<Double> getPowersOfTwo() {
        List<Double> powers = new ArrayList<>();

        IntStream.rangeClosed(-10, -1).forEach(i -> powers.add(Math.pow(2, i)));

        return powers.stream().sorted(Collections.reverseOrder()).collect(Collectors.toList());
    }
}
