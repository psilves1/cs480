  # Random Number Generator Covert Channel
  
  ## Build
  
  ```
  mkdir build
  cd build
  cmake ..
  make
  ```
  
  You will find the cc_drng executable in the '/build/channel/drng/' directory.
  
  ## Run
  
  For example:

./cc_drng thread 1 0.003 0 0.03 0.03
which launches spy and trojan as two threads 
  or

./cc_drng process 1 0.003 0 0.03 0.03
Which launches spy and trojan as two processes

