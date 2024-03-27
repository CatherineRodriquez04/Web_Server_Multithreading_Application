/*
 * Rodriquez, Catherine
 * crodr99@lsu.edu
 * PA-1 (Multithreading)
 * Feng Chen
 * cs4103-sp24
 * cs410356
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

//variables (global)
std::vector<int> jobPool;
std::mutex jobPoolMutex;
std::condition_variable jobPoolCV; 
int numJobsCreated = 0;
int numJobsProcessed = 0;
std::ofstream logFile; //used for service log

//function to service log events
void logEvent(const std::string& threadType, int threadID, int poolIndex, int jobID) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
    logFile << timestamp << " " << threadType << " " << threadID << " " << poolIndex << " " << jobID << std::endl;
}

//client thread function
void clientThread(int id, int totalJobs, int jobProcessingTime) {
    while (numJobsCreated < totalJobs) {
        std::unique_lock<std::mutex> lock(jobPoolMutex);
        jobPoolCV.wait(lock, []{ return jobPool.size() < 10; }); //waits until the job pool has space

        //add job to job pool
        jobPool.push_back(numJobsCreated);
        int jobID = numJobsCreated++;
        logEvent("Client", id, jobPool.size() - 1, jobID);

        lock.unlock();
        jobPoolCV.notify_all();
    }
}

//server thread function
void serverThread(int id, int totalJobs, int jobProcessingTime) {
    while (numJobsProcessed < totalJobs) {
        std::unique_lock<std::mutex> lock(jobPoolMutex);
        jobPoolCV.wait(lock, []{ return !jobPool.empty(); }); //waits until the job pool is not empty

        //get job from job pool
        int jobID = jobPool.front();
        jobPool.erase(jobPool.begin());
        numJobsProcessed++;
        logEvent("Server", id, 0, jobID);

        lock.unlock();
        jobPoolCV.notify_all();

        //simulate job processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(jobProcessingTime));
    }
}

int main(int argc, char *argv[]) {
    //emsures user enters the correct input
    if (argc != 6) {
        std::cerr << "Incorrect Entry Type.\nPlease Enter: " << argv[0] << " <num_clients> <num_servers> <job_pool_size> <total_jobs> <job_processing_time>" << std::endl;
        return 1;
    }

    //arugments for each input
    int numClients = std::stoi(argv[1]);
    int numServers = std::stoi(argv[2]);
    int jobPoolSize = std::stoi(argv[3]);
    int totalJobs = std::stoi(argv[4]);
    int jobProcessingTime = std::stoi(argv[5]);

    //open service log file
    logFile.open("service.log");

    //create client threads
    std::vector<std::thread> clientThreads;
    for (int i = 0; i < numClients; ++i) {
        clientThreads.emplace_back(clientThread, i, totalJobs, jobProcessingTime);
    }

    //create server threads
    std::vector<std::thread> serverThreads;
    for (int i = 0; i < numServers; ++i) {
        serverThreads.emplace_back(serverThread, i, totalJobs, jobProcessingTime);
    }

    //join client threads
    for (auto& thread : clientThreads) {
        thread.join();
    }

    //join server threads
    for (auto& thread : serverThreads) {
        thread.join();
    }

    //print statistics for each client
    std::cout << "Number of jobs created by each client thread:" << std::endl;
    for (int i = 0; i < numClients; ++i) {
        std::cout << "Client " << i << ": " << totalJobs / numClients << std::endl;
    }

    //print statistics for each server
    std::cout << "Number of jobs processed by each server thread:" << std::endl;
    for (int i = 0; i < numServers; ++i) {
        std::cout << "Server " << i << ": " << totalJobs / numServers << std::endl;
    }

    std::cout << "Event log file: service.log" << std::endl;

    //close service log file
    logFile.close();

    return 0;
}

