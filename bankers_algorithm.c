#include <stdio.h>
#include <stdbool.h>

// Define the number of processes and resource types
#define N 5  // Number of processes
#define M 3  // Number of resource types

// State structure to store resource data
struct state {
    int resource[M];      // Total instances of each resource type
    int available[M];     // Currently available resources
    int claim[N][M];      // Maximum resources each process can request
    int alloc[N][M];      // Resources currently allocated to each process
};

// Function prototypes
bool safe(struct state *S);
void request_resources(struct state *S, int process, int request[M]);

int main() {
    // Initialize the system state
    struct state S = {
        .resource = {10, 5, 7},  // Total instances of each resource type
        .available = {3, 2, 2},  // Available instances of each resource type
        .claim = {               // Maximum claim of each process
            {7, 5, 3},
            {3, 2, 2},
            {9, 0, 2},
            {2, 2, 2},
            {4, 3, 3}
        },
        .alloc = {               // Currently allocated resources
            {0, 1, 0},
            {2, 0, 0},
            {3, 0, 2},
            {2, 1, 1},
            {0, 0, 2}
        }
    };

    int request[M];  // Array to store the request

    // Example: Process 1 making a resource request
    printf("Enter the resource request for process 1 (3 values): ");
    for (int i = 0; i < M; i++) {
        scanf("%d", &request[i]);
    }

    // Process the resource request for Process 1
    request_resources(&S, 1, request);

    return 0;
}

// Function to process a resource request
void request_resources(struct state *S, int process, int request[M]) {
    // Check if the request exceeds the process's maximum claim
    for (int i = 0; i < M; i++) {
        if (S->alloc[process][i] + request[i] > S->claim[process][i]) {
            printf("Error: Request exceeds maximum claim!\n");
            return;
        }
    }

    // Check if the requested resources are available
    for (int i = 0; i < M; i++) {
        if (request[i] > S->available[i]) {
            printf("Process %d must wait: Not enough resources available.\n", process);
            return;
        }
    }

    // Temporarily allocate resources to the process
    for (int i = 0; i < M; i++) {
        S->alloc[process][i] += request[i];
        S->available[i] -= request[i];
    }

    // Check if the new state is safe
    if (safe(S)) {
        printf("Request granted: System is in a safe state.\n");
    } else {
        // Revert the allocation if the state is unsafe
        for (int i = 0; i < M; i++) {
            S->alloc[process][i] -= request[i];
            S->available[i] += request[i];
        }
        printf("Request denied: System would enter an unsafe state.\n");
    }
}

// Function to check if the system is in a safe state
bool safe(struct state *S) {
    int currentavail[M];     // Temporary available resources
    bool finish[N] = {false}; // Track which processes can finish

    // Copy available resources to the temporary array
    for (int i = 0; i < M; i++) {
        currentavail[i] = S->available[i];
    }

    while (true) {
        bool found = false;

        // Find a process that can finish with the current resources
        for (int i = 0; i < N; i++) {
            if (!finish[i]) {  // If the process hasn't finished
                bool canProceed = true;

                // Check if the process's needs can be satisfied
                for (int j = 0; j < M; j++) {
                    if (S->claim[i][j] - S->alloc[i][j] > currentavail[j]) {
                        canProceed = false;
                        break;
                    }
                }

                // If the process can finish, simulate its execution
                if (canProceed) {
                    for (int j = 0; j < M; j++) {
                        currentavail[j] += S->alloc[i][j];  // Release resources
                    }
                    finish[i] = true;  // Mark the process as finished
                    found = true;
                }
            }
        }

        // If no suitable process is found, break the loop
        if (!found) break;
    }

    // Check if all processes finished successfully
    for (int i = 0; i < N; i++) {
        if (!finish[i]) return false;  // If any process is unfinished, return false
    }
    return true;  // All processes finished, system is in a safe state
}