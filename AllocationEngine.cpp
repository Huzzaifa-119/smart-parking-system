#include "AllocationEngine.h"

AllocationEngine::AllocationEngine(ParkingArea* parkingAreas) {
    areas = parkingAreas;
    penaltyApplied = false;
    lastAllocatedArea = -1;
}

AllocationEngine::~AllocationEngine() {
    // Destructor - no dynamic memory to clean up
}

int AllocationEngine::allocateSlot(int vehicleID, int preferredZone, int numberOfAreas) {
    // Step 1: Attempt same-zone allocation first (preferred zone)
    // Loop through all parking areas to find those in the preferred zone
    for (int i = 0; i < numberOfAreas; i++) {
        if (areas[i].getZoneID() == preferredZone) {
            // Step 2: For each area in preferred zone, find the first available slot
            int slotID = areas[i].findFirstAvailableSlot();
            if (slotID != -1) {
                // Step 3: If available slot found, occupy it and mark no penalty
                areas[i].occupySlot(slotID);
                penaltyApplied = false;
                lastAllocatedArea = i;
                return slotID;
            }
        }
    }

    // Step 4: If no slot in preferred zone, attempt cross-zone allocation
    // Loop through all parking areas in other zones
    for (int i = 0; i < numberOfAreas; i++) {
        if (areas[i].getZoneID() != preferredZone) {
            // Step 5: Find first available slot in cross-zone area
            int slotID = areas[i].findFirstAvailableSlot();
            if (slotID != -1) {
                // Step 6: If available slot found, occupy it and mark penalty applied
                areas[i].occupySlot(slotID);
                penaltyApplied = true;
                lastAllocatedArea = i;
                return slotID;
            }
        }
    }

    // Step 7: If no slots available anywhere, return -1 (allocation failed)
    return -1;
}

bool AllocationEngine::hasPenalty() const {
    return penaltyApplied;
}

void AllocationEngine::resetPenalty() {
    penaltyApplied = false;
}

int AllocationEngine::getLastAllocatedArea() const {
    return lastAllocatedArea;
}