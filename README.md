# os_assignment_3

To run follow these command:

1. go to apps/ folder, perform "make".
2. go to probe/ folder, perform "make".
3. To run as individual program:
  go to probe/ folder, Run,
  1. sudo insmod k_probe.ko
  2. ../apps/sim or any other application.
  
 To run the script:
 
 1. bash run_script.sh ../apps/sim
 2. bash run_script.sh sysbench cpu run --verbosity=0
 3. bash run_script.sh sysbench sysbench --test=memory --memory-total-size=1G run
 4. iperf3 -s 127.0.0.1 -f K 
    bash run_script.sh iperf3 -c 127.0.0.1 -f K
    
 ioctl() implementation is included in the run_script.
 
