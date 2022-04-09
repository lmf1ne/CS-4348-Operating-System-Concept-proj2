import java.util.*;
import java.util.concurrent.Semaphore;

public class BankSimulation {
    public static void main(String[] args) throws Exception {
        // initialization
        Semaphore t, m, s, cust_ready, teller_ready, manager_ready, mutax, count;
        t = new Semaphore(3, true);
        m = new Semaphore(1, true);
        s = new Semaphore(2, true);
        cust_ready = new Semaphore(0, true);
        teller_ready = new Semaphore(0, true);
        manager_ready = new Semaphore(0, true);
        mutax = new Semaphore(1, true);
        count = new Semaphore(-99, true);

        Semaphore[] t_producers = new Semaphore[100];
        for (int i = 0; i < 100; i++) {
            t_producers[i] = new Semaphore(0, true);
        }

        List<int[]> queue = new LinkedList<int[]>();

        Rand rand = new Rand();

        /* start of bank */
        Teller[] teller_threads = new Teller[3];
        Manager manager_thread = new Manager(teller_ready, m, manager_ready);
        Customer[] customer_threads = new Customer[100];
        String[] tasks = new String[100];
        int[] wait_time_arr = new int[100];
        manager_thread.start();

        for (int i = 0; i < 3; i++) {
            teller_threads[i] = new Teller(i, t, s, cust_ready, teller_ready, manager_ready, mutax, t_producers, count,
                    queue);
            teller_threads[i].start();
        }
        for (int i = 0; i < 100; i++) {
            int n = rand.getRandInt(0, 1);
            int wait_time = rand.getRandInt(0, 5000);
            wait_time_arr[i] = wait_time;
            String task;
            if (n == 0) {
                task = "deposit";
            } else {
                task = "withdrawal transcation";
            }
            tasks[i] = task;
            System.out.printf("Customer %d wants to perform a %s.%n", i, task);
        }
        for (int i = 0; i < 100; i++) {
            customer_threads[i] = new Customer(tasks[i], i, wait_time_arr[i], t, cust_ready, t_producers, mutax, queue);
            customer_threads[i].start();
        }

        /* waits for all customers finished */
        count.acquire();
        for (int i = 0; i < 3; i++) {
            System.out.printf("Teller %d is leaving for the day.%n", i);
        }
        System.out.println("The bank closes for the day.");
        System.exit(0);
        /* end of bank */
    }
}
