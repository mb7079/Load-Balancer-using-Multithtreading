# Load-Balancer-using-Multithtreading
    Load balancing an application workload among threads is critical to performance. However, achieving perfect load balance is non-trivial, and it depends on the parallelism within the application, workload, the number of threads, load balancing policy, and the threading implementation.           

    We present a multithreaded model for load-balancing on servers, where the incoming client requests are redirected onto a server chosen from multiple available server instances . Multithreading is used as a means of conversing between the Load balancer and the servers to obtain load and also to transfer the TCP socket from the client. Our preliminary analysis indicates that multithreading can be used as a mechanism to mask overheads required for the dynamic balancing of server / processor workloads. Also, multithreading can simplify the implementation of dynamic load-balancing algorithms. Unfortunately, multithreading does not always simplify program complexity, often makes code reusability not an easy task, and increases software complexity.
      

