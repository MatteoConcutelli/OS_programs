you have to fill out the 3 files.

To execute you have 2 possibilities:
1. either start Main
2. or start the producer first and then the consumer

--------------

How it works: 
1. the main creates functions, producer (writes) and the consumer (reads) , these communicate with each other using a shared buffer. Here is the access/change problem on the variable. It may happen that the producer writes but the consumer was taken back before it printed the old variable. 
2. first starts the producer, which creates the shared memory and loads some strings onto it. Then it starts the consumer which reads into the memory the strings put by the producer, and finally deallocates it.
