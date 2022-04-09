import java.util.concurrent.Semaphore;


public class Manager extends Thread {
   private Semaphore teller_ready;
   private Semaphore manager;
   private Semaphore manager_ready;
   private Rand rand;

   public Manager(Semaphore teller_ready, Semaphore manager, Semaphore manager_ready) {
      this.manager = manager;
      this.teller_ready = teller_ready;
      this.manager_ready = manager_ready;
      this.rand = new Rand();
   }

   private void startManager() throws InterruptedException {
      this.teller_ready.acquire();
      this.manager.acquire();
      //sleep
      Thread.sleep(this.rand.getRandInt(5, 30));
      this.manager.release();
      this.manager_ready.release();
   }

   @Override
   public void run() {
      while(true) {
         try {
            startManager();
         } catch (InterruptedException e) {
            e.printStackTrace();
         }
      }
   }
}
