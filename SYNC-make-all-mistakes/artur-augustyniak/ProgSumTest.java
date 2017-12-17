import java.util.concurrent.TimeUnit;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class ProgSumTest {

    private static final int N_TH = 9;
    private static final int CYCLES = 200;
    private static final int ROUNDS = 500;

    /**
     * czesto mozna sie spotkac z twierdzeniem ze volatile
     * pomoze, w Javie w przypadku int zapis do takiej zmiennej
     * powinien byc atomowy ale to juz implementation dep.
     * postinkrementacja to:
     * read
     * increment
     * assign
     */
    private volatile int value = 0;

    /**
     * synchronized bo o ile dobrze pamietam
     * w Javie synchronizacja jest tez bariera pamieci
     */
    public synchronized void reset() {
        this.value = 0;
    }

    /**
     * zwraca unikalna wartosc?
     */
    public synchronized int getNextSync() {
        return value++;
    }

    /**
     * zwraca unikalna wartosc?
     */
    public int getNext() {
        return value++;
    }


    private void runTest(int expectedRes, boolean useLock) {
        /**
         * w 'tescie' uzywam atomica zeby wyeliminowac
         * bledy pochodzace z samego testu
         */
        final AtomicInteger actual = new AtomicInteger(0);
        final ExecutorService exec = Executors.newFixedThreadPool(N_TH);

        for (int i = 0; i < N_TH; i++) {
            Runnable task = new Runnable() {
                @Override
                public void run() {
                    for (int j = 0; j < CYCLES; j++) {
                        /**
                         * Niestety overlading albo obecnosc w metodzie
                         * warunkowo wrzuconego bloku synchronized prowadzi do
                         * poprawnej(niejszej?) synchronizacji
                         * openjdk version "1.8.0_151"
                         * OpenJDK Runtime Environment (build 1.8.0_151-b12)
                         * OpenJDK 64-Bit Server VM (build 25.151-b12, mixed mode)
                         */
                        int n;
                        if (useLock) {
                            n = getNextSync();
                        } else {
                            n = getNext();
                        }
                        Thread.yield();
                        actual.addAndGet(n);
                    }
                }
            };
            exec.execute(task);
        }
        exec.shutdown();
        /**
         * czekam az wszystkie watki skonczą
         */
        try {
            exec.awaitTermination(Long.MAX_VALUE, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (this.getClass().desiredAssertionStatus()) {
            try {
                assert actual.get() == expectedRes;
                System.out.print("[+] OK!\t\t");
            } catch (AssertionError e) {
                System.out.print("[!] ERR!\t");
            }
        }
        System.out.format("%s:\texpected: %d - actual: %d%n",
                useLock ? "SYNC" : "UNSYNC",
                expectedRes,
                actual.get());

        if (useLock) {
            assert actual.get() == expectedRes;
        }

    }

    /**
     * kompilacja i uruchomienie:
     * javac ProgSumTest.java  && java -enableassertions ProgSumTest
     *
     * @param args
     */
    public static void main(String[] args) {
        ProgSumTest ups = new ProgSumTest();
        /**
         * Oczekujemy sumy skonczonego ciagu
         */
        int expected = ((N_TH * CYCLES) * (0 + (N_TH * CYCLES) - 1)) / 2;
        for (int k = 0; k < ROUNDS; k++) {
            ups.runTest(expected, false);
            ups.reset();
            ups.runTest(expected, true);
        }

    }

}