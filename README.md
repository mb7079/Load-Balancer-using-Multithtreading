# Load-Balancer-using-Multithtreading
    Load balancing an application workload among threads is critical to performance. However, achieving perfect load 
    balance is non-trivial, and it depends on the parallelism within the application, workload, the number of threads,
    load balancing policy, and the threading implementation.   
	
    We present a multithreaded model for load-balancing on servers, where the incoming client requests are 
    redirected onto a server chosen from multiple available server instances .Multithreading is used as a 
    means of conversing between the Load balancer and the servers to obtain load and also to transfer the TCP socket 
    from the client. Our preliminary analysis indicates that multithreading can be used as a mechanism to mask 
    overheads required for the dynamic balancing of server / processor workloads. Also, multithreading can simplify 
    the implementation of dynamic load-balancing algorithms. Unfortunately, multithreading does not always simplify 
    program complexity, often makes code reusability not an easy task, and increases software complexity.
      

 **#EXECUTION**
    Our code uses 3 server instancer. You can change the number just by adding another port in the load balancer file 
    and with minor changer in the code.
    
    1.  Run the server files first using "-lpthread" keywork to initialize threads.
    2.  Give port numbers as 6001, 6002, 6003. ( You have to change them in the load balancer file if you want to ).
    2.  Run the Load balancer file with "-lpthread" keyword.
    3.  Now, run the client files one by one.
    4.  Observe on which server, the clients are getting assigned.
    
    
 You will also see the loads on each server in the Load balancer running terminal.
