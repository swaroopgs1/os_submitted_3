#! /bin/bash
  # Run command from parameters in background
  $* &
  # Save pid of last background command
  pid=$!
  # Install probe, running on the pid specified
  sudo rmmod k_probe 
  echo "$pid"
  sudo insmod k_probe.ko pid=$pid 
  # wait for the background job to complete
  wait $pid
  sudo ../apps/userprg
  # Remove the module
  sudo rmmod k_probe 
  # Look at the last 50 lines of the console print
  dmesg | tail -50
  # dmesg
