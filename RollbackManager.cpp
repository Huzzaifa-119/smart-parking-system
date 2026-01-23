#include "RollbackManager.h"

RollbackManager::RollbackManager(int maxOperations) {
    capacity = maxOperations;
    stack = new AllocationRecord[capacity];
    top = -1;
}

RollbackManager::~RollbackManager() {
    delete[] stack;
}

void RollbackManager::recordAllocation(int areaIndex, int slotID, int requestID) {
    if (top < capacity - 1) {
        top++;
        stack[top].areaIndex = areaIndex;
        stack[top].slotID = slotID;
        stack[top].requestID = requestID;
    }
    // If stack is full, do nothing (could add overflow handling)
}

void RollbackManager::undoLast(ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests) {
    if (top >= 0) {
        // Step 1: Pop the last allocation record from the stack
        AllocationRecord rec = stack[top];
        top--;

        // Step 2: Restore slot availability by freeing the slot
        areas[rec.areaIndex].freeSlot(rec.slotID);

        // Step 3: Restore request state by cancelling the associated request
        for (int i = 0; i < numRequests; i++) {
            if (requests[i].getRequestID() == rec.requestID) {
                requests[i].cancel();
                break;
            }
        }
    }
}

void RollbackManager::undoLastK(int k, ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests) {
    // Undo the last k allocations by calling undoLast k times
    for (int i = 0; i < k && !isEmpty(); i++) {
        undoLast(areas, numAreas, requests, numRequests);
    }
}

bool RollbackManager::isEmpty() const {
    return top == -1;
}

int RollbackManager::size() const {
    return top + 1;
}