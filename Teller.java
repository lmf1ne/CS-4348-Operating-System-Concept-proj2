import java.util.List;
import java.util.concurrent.Semaphore;

public class Teller extends Thread {
   private int id;
   private Semaphore teller;
   private Semaphore safe;
   private Semaphore customer_ready;
   private Semaphore teller_ready;
   private Semaphore manager_ready;
   private Semaphore[] teller_producers;
   private Semaphore mutax;
   private Semaphore count;
   private List<int[]> queue;
   private Rand rand;

   public Teller(int id, Semaphore teller, Semaphore safe, Semaphore customer_ready, Semaphore teller_ready,
         Semaphore manager_ready, Semaphore mutax, Semaphore[] teller_producers, Semaphore count, List<int[]> queue) {
      this.id = id;
      this.teller = teller;
      this.safe = safe;
      this.customer_ready = customer_ready;
      this.teller_ready = teller_ready;
      this.manager_ready = manager_ready;
      this.teller_producers = teller_producers;
      this.mutax = mutax;
      this.count = count;
      this.queue = queue;
      this.rand = new Rand();
   }

   private void startTeller() throws InterruptedException {
      System.out.printf("Teller %d is ready to serve.%n", this.id);
      System.out.printf("Teller %d is waiting for a customer.%n", this.id);

      this.customer_ready.acquire();

      this.mutax.acquire();
      int[] data = queue.remove(0);
      int taskNum = data[0];
      String task = taskNum == 0 ? "deposit" : "withdrawal transcation";
      int custNum = data[1];
      this.mutax.release();

      System.out.printf("Customer %d goes to Teller %d.%n", custNum, this.id);
      System.out.printf("Customer %d introduces itself to Teller %d.%n", custNum, this.id);

      System.out.printf("Teller %d is serving Customer %d.%n", this.id, custNum);
      System.out.printf("Customer %d asks for %s.%n", this.id, task);

      if (taskNum == 1) {
         System.out.printf("Teller %d is going to the manager.%n", this.id);
         System.out.printf("Teller %d is getting the manager's permission.%n", this.id);
         this.teller_ready.release();
         this.manager_ready.acquire();
         System.out.printf("Teller %d got the manager's permission.%n", this.id);

         System.out.printf("Teller %d is going to the safe.%n", this.id);
         this.safe.acquire();
         // sleep
         Thread.sleep(this.rand.getRandInt(10, 50));
         this.safe.release();
         System.out.printf("Teller %d is leaving the safe.%n", this.id);
      } else {
         System.out.printf("Teller %d is going to the safe.%n", this.id);
         this.safe.acquire();
         // sleep
         this.safe.release();
         System.out.printf("Teller %d is leaving the safe.%n", this.id);
      }
      System.out.printf("Teller %d finishes Customer %d's %s.%n", this.id, custNum, task);
      this.teller_producers[custNum].release();
      this.teller.release();
      System.out.printf("Customer %d thanks Teller %d and leaves.%n", custNum, this.id);
      this.count.release(); // count the number of customers have been served
   }

   @Override
   public void run() {
      while (true) {
         try {
            startTeller();
         } catch (InterruptedException e) {
            e.printStackTrace();
         }
      }
   }
}
