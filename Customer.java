import java.util.*;
import java.util.concurrent.Semaphore;

public class Customer extends Thread {
   private int id;
   private int wait_time;
   private Semaphore teller;
   private Semaphore cust_ready;
   private Semaphore[] teller_producers;
   private Semaphore mutax;
   private List<int[]> queue;
   private String task;

   public Customer(String task, int id, int wait_time, Semaphore teller, Semaphore cust_ready, Semaphore[] teller_producers,
         Semaphore mutax, List<int[]> queue) {
      this.id = id;
      this.wait_time = wait_time;
      this.teller = teller;
      this.cust_ready = cust_ready;
      this.teller_producers = teller_producers;
      this.mutax = mutax;
      this.queue = queue;
      this.task = task;
   }

   private void startCustomer() throws InterruptedException {
      Thread.sleep(this.wait_time);

      System.out.printf("Customer %d is going to the bank.%n", this.id);
      System.out.printf("Customer %d is getting in line.%n", this.id);
      System.out.printf("Customer %d is selecting a teller.%n", this.id);

      this.teller.acquire();

      // System.out.printf("Customer %d goes to Teller %d.%n", this.id, telNum);
      // System.out.printf("Customer %d introduces itself to Teller %d.%n", this.id,
      // telNum);

      this.mutax.acquire();
      int taskNum = this.task.equals("deposit") ? 0 : 1;
      this.queue.add(new int[] { taskNum, this.id });
      this.mutax.release();

      this.cust_ready.release();
      this.teller_producers[this.id].acquire();

      // System.out.printf("Customer %d thanks Teller %d and leaves.%n", this.id,
      // telNum);
   }

   @Override
   public void run() {
      try {
         startCustomer();
      } catch (InterruptedException e) {
         e.printStackTrace();
      }
   }
}
