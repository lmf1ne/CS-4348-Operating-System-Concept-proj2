import java.util.Random;

public class Rand {
   private Random r;

   public Rand() {
      this.r = new Random();
      this.r.setSeed(System.currentTimeMillis() / 1000);
   }

   public int getRandInt(int start, int end) {
      return start + r.nextInt(end - start + 1);
   }
}
