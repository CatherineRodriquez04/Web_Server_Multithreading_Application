# Web Server Multithreading Application
### Summary:
This program simulates a web server handling jobs from multiple clients concurrently using POSIX threads (Pthreads). It sets up client and server threads to manage job generation, processing, and logging of thread activities.

### Code Information:
     Source code: serversim.cpp
      - written in c++
     Code execution: serversim
     Log file: service.log
      - contains the records of the timestamp, client/server thread ID, index of the job pool entry that accommodates this 
      job, and the job ID all for when a client thread or a server thread adds/removes a job to/from the job pool

### Functionality:
	- Client Threads: Generate job requests and add them to the job pool. Ensure thread safety using mutex locks and condition 
    variables. Log events related to job creation and addition to the job pool.

	- Server Threads: Remove jobs from the job pool for processing in FIFO order. Simulate job processing time by sleeping for 
    a specified duration. Ensure thread safety using mutex locks and condition variables. Log events related to job processing 
    and removal from the job pool.

	- Main Function: Parse command-line arguments to specify simulation parameters. Create client and server threads accordingly. 
    Wait for thread execution to complete, print statistics, and close the log file.

### Compile the program using:
	g++ -o serversim serversim.cpp -pthread

### Run the compiled program with command-line arguments and replace with appropriate values:
	Please Enter: ./serversim <num_clients> <num_servers> <job_pool_size> <total_jobs> <job_processing_time>

### Code Expected Output Examples:
    Input:
	    ./serversim 2 3 10 1000 5
    Output:
	    Number of jobs created by each client thread:
	    Client 0: 500
	    Client 1: 500
	    Number of jobs processed by each server thread:
	    Server 0: 333
	    Server 1: 333
	    Server 2: 333
	    Event log file: service.log

    Input:
	    ./serversim 5 2 20 5000 3
    Output: 
	    Number of jobs created by each client thread:
	    Client 0: 1000
	    Client 1: 1000
	    Client 2: 1000
	    Client 3: 1000
	    Client 4: 1000
	    Number of jobs processed by each server thread:
	    Server 0: 2500
	    Server 1: 2500	
	    Event log file: service.log


