#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

void readFile(const string& fileName, unordered_map<string, vector<int>>& taskMap, int& numberOfTasks) 
{
    ifstream inFile(fileName);
    if (!inFile) exit(0);

    string line;

    while (getline(inFile, line)) 
    {
        istringstream lineStream(line);
        string keyComma, valueString;

        // Get the key/TaskName
        getline(lineStream, keyComma, ',');

        // Clean String
        string key = keyComma;
        key.erase(key.find_last_not_of(" \n\r\t") + 1);

        // Read values
        while (getline(lineStream, valueString, ',')) 
        {
            valueString.erase(0, valueString.find_first_not_of(" "));
            int value = stoi(valueString);
            taskMap[key].push_back(value);
        }
        numberOfTasks++;
    }
    inFile.close();
}

int main(int argc, char* argv[]) 
{
    if (argc < 2)
    {
        cerr << "FAIL";
        return 1;
    }
    unordered_map<string, vector<int>> taskMap;
    int numberOfTasks = 0;
    readFile(argv[1], taskMap, numberOfTasks);

    int totalBT = 0;
    // Calculate the Total Burst Time
    for (const auto& pair : taskMap) 
    {
        totalBT += pair.second[2]; 
    }
    cout << "Number of Tasks: " << numberOfTasks << endl;
    cout << "Total Burst Time: " << totalBT << endl;

    unordered_map<string, vector<string>> finalOutput;
    unordered_map<string, int> remainingTime;

    // Initialize remaining time for each task
    for (const auto& pair : taskMap) 
    {
        remainingTime[pair.first] = pair.second[2];
        // This is for Start, Preempted, Resumed, End
        finalOutput[pair.first].resize(4); 
    }

    string currentTask = "";
    int currentTime = 0;

    while (currentTime < totalBT) 
    {
        string taskWithHighestPriority;
        int highestPriority = numeric_limits<int>::min();
        int earliestArrival = numeric_limits<int>::max();

        // Check which tasks have arrived and what the highest priority is checking with arrival time if needed
        for (const auto& task : taskMap) 
        {
            if (task.second[0] <= currentTime && remainingTime[task.first] > 0) 
            { // Arrived and remaining time
                if (task.second[1] > highestPriority || (task.second[1] == highestPriority && task.second[0] < earliestArrival)) 
                {
                    highestPriority = task.second[1];
                    earliestArrival = task.second[0];
                    taskWithHighestPriority = task.first;
                }
            }
        }

        // Actually recording what happens as the burst time increases
        // Check to see if there is something to run
        if (!taskWithHighestPriority.empty()) 
        {
            // If the current task is preempted
            if (currentTask != taskWithHighestPriority) 
            {
                if (!currentTask.empty()) 
                {
                    finalOutput[currentTask][1] = "Preempted at: " + to_string(currentTime);
                }
                if (finalOutput[taskWithHighestPriority][0].empty()) 
                {
                    finalOutput[taskWithHighestPriority][0] = to_string(currentTime); // Start time
                } else 
                {
                    finalOutput[taskWithHighestPriority][2] = "Resumed at: " + to_string(currentTime);
                }
            }

            // Executing the task by taking away a burst time second
            remainingTime[taskWithHighestPriority]--;
            // Update current task
            currentTask = taskWithHighestPriority; 

            // If this task finishes
            if (remainingTime[taskWithHighestPriority] == 0) 
            {
                finalOutput[taskWithHighestPriority][3] = "End: " + to_string(currentTime + 1);
                // Clear current task if finished
                currentTask.clear(); 
            }
        } 
        else 
        {
            // No task to run
            currentTask.clear();
        }
        // Increment time
        currentTime++; 
    }

    // Print final output
    for (const auto& pair : finalOutput) 
    {
        cout << "Task: " << pair.first << ", Start: " << pair.second[0];
        if (!pair.second[1].empty()) 
        {
            cout << ", " << pair.second[1];
        }
        if (!pair.second[2].empty()) 
        {
            cout << ", " << pair.second[2];
        }
        if (!pair.second[3].empty()) 
        {
            cout << ", " << pair.second[3];
        }
        cout << endl;
    }

    return 0;
}


