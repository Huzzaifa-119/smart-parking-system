#ifndef ALLOCATION_ENGINE_H
#define ALLOCATION_ENGINE_H

#include "ParkingArea.h"

class AllocationEngine {
private:
    ParkingArea* areas;     // Array of ParkingArea pointers
    bool penaltyApplied;    // Flag indicating if cross-zone allocation penalty was applied
    int lastAllocatedArea;  // Index of the last allocated area

public:
    // Constructor: Initializes the engine with an array of parking areas
    AllocationEngine(ParkingArea* parkingAreas);

    // Destructor
    ~AllocationEngine();

    // Main responsibility: Allocate a parking slot for a vehicle
    // Prefers allocation in the same zone as preferredZone
    // Falls back to cross-zone allocation with penalty if same-zone not available
    // Returns the allocated slotID or -1 if no slot available
    int allocateSlot(int vehicleID, int preferredZone, int numberOfAreas);

    // Check if the last allocation incurred a cross-zone penalty
    bool hasPenalty() const;

    // Reset the penalty flag (utility function)
    void resetPenalty();

    // Get the index of the last allocated area
    int getLastAllocatedArea() const;
};

#endif // ALLOCATION_ENGINE_H