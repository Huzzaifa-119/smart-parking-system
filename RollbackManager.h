#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include "ParkingArea.h"
#include "ParkingRequest.h"

// Stack is used because allocation operations follow LIFO (Last In, First Out) order,
// allowing us to undo the most recent allocations first, which is essential for
// rollback functionality in parking systems.

class RollbackManager {
private:
    struct AllocationRecord {
        int areaIndex;
        int slotID;
        int requestID;
    };

    AllocationRecord* stack;  // Array-based stack for allocation records
    int top;                  // Index of the top element in the stack
    int capacity;             // Maximum capacity of the stack

public:
    // Constructor: Initializes the stack with given capacity
    RollbackManager(int maxOperations);

    // Destructor
    ~RollbackManager();

    // Record a parking allocation on the stack
    void recordAllocation(int areaIndex, int slotID, int requestID);

    // Undo the last allocation: free the slot and restore request state
    void undoLast(ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests);

    // Undo the last k allocations: free slots and restore request states
    void undoLastK(int k, ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests);

    // Check if stack is empty
    bool isEmpty() const;

    // Get current number of recorded allocations
    int size() const;
};

#endif // ROLLBACK_MANAGER_H