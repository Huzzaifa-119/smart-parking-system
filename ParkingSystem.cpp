#include "ParkingSystem.h"

ParkingSystem::ParkingSystem(int maxZ, int maxV, int maxR, int maxA, int maxOps) {
    maxZones = maxZ;
    zones = new Zone[maxZones];
    numZones = 0;

    maxVehicles = maxV;
    vehicles = new Vehicle[maxVehicles];
    numVehicles = 0;

    maxRequests = maxR;
    requests = new ParkingRequest[maxRequests];
    numRequests = 0;

    maxAreas = maxA;
    areas = new ParkingArea[maxAreas];
    numAreas = 0;

    allocEngine = new AllocationEngine(areas);
    rollbackMgr = new RollbackManager(maxOps);
}

ParkingSystem::~ParkingSystem() {
    delete[] zones;
    delete[] vehicles;
    delete[] requests;
    delete[] areas;
    delete allocEngine;
    delete rollbackMgr;
}

void ParkingSystem::addZone(const Zone& zone) {
    if (numZones < maxZones) {
        zones[numZones] = zone;
        numZones++;
    }
}

void ParkingSystem::addVehicle(const Vehicle& vehicle) {
    if (numVehicles < maxVehicles) {
        vehicles[numVehicles] = vehicle;
        numVehicles++;
    }
}

void ParkingSystem::addRequest(const ParkingRequest& request) {
    if (numRequests < maxRequests) {
        requests[numRequests] = request;
        numRequests++;
    }
}

void ParkingSystem::addArea(const ParkingArea& area) {
    if (numAreas < maxAreas) {
        areas[numAreas] = area;
        numAreas++;
    }
}

int ParkingSystem::allocateForRequest(int requestIndex) {
    // Validate request index
    if (requestIndex < 0 || requestIndex >= numRequests) {
        return -1;
    }

    ParkingRequest& req = requests[requestIndex];

    // Attempt to allocate a slot using the allocation engine
    int slotID = allocEngine->allocateSlot(req.vehicleID, req.requestedZone, numAreas);

    if (slotID != -1) {
        // Record the allocation for potential rollback
        int areaIndex = allocEngine->getLastAllocatedArea();
        rollbackMgr->recordAllocation(areaIndex, slotID, req.requestID);

        // Update request state to allocated
        req.allocate();

        return slotID;
    }

    return -1;  // Allocation failed
}

void ParkingSystem::rollbackLast() {
    // Rollback the last allocation
    rollbackMgr->undoLastK(1, areas, numAreas, requests, numRequests);
}

void ParkingSystem::rollbackLastK(int k) {
    // Rollback the last k allocations
    rollbackMgr->undoLastK(k, areas, numAreas, requests, numRequests);
}

int ParkingSystem::getTotalSlots() const {
    int total = 0;
    for (int i = 0; i < numAreas; i++) {
        total += areas[i].getNumSlots();
    }
    return total;
}

int ParkingSystem::getAvailableSlots() const {
    int available = 0;
    for (int i = 0; i < numAreas; i++) {
        available += areas[i].getAvailableSlots();
    }
    return available;
}

int ParkingSystem::getActiveRequests() const {
    int active = 0;
    for (int i = 0; i < numRequests; i++) {
        ParkingRequest::State state = requests[i].getState();
        if (state != ParkingRequest::CANCELLED && state != ParkingRequest::RELEASED) {
            active++;
        }
    }
    return active;
}

double ParkingSystem::getZoneUtilization(int zoneID) const {
    // Formula: (occupied slots / total slots) * 100
    // Occupied slots = total slots - available slots
    int totalSlots = 0;
    int availableSlots = 0;
    for (int i = 0; i < numAreas; i++) {
        if (areas[i].getZoneID() == zoneID) {
            totalSlots += areas[i].getNumSlots();
            availableSlots += areas[i].getAvailableSlots();
        }
    }
    if (totalSlots == 0) {
        return 0.0;
    }
    int occupiedSlots = totalSlots - availableSlots;
    return (static_cast<double>(occupiedSlots) / totalSlots) * 100.0;
}

double ParkingSystem::getAverageParkingDuration() const {
    // Placeholder: Average parking duration calculation
    // Currently not implemented as duration tracking is not in place
    // Formula would be: sum of (end_time - start_time) for completed requests / number of completed requests
    return 0.0;
}

int ParkingSystem::getCancelledRequests() const {
    int cancelled = 0;
    for (int i = 0; i < numRequests; i++) {
        if (requests[i].getState() == ParkingRequest::CANCELLED) {
            cancelled++;
        }
    }
    return cancelled;
}

int ParkingSystem::getCompletedRequests() const {
    int completed = 0;
    for (int i = 0; i < numRequests; i++) {
        if (requests[i].getState() == ParkingRequest::RELEASED) {
            completed++;
        }
    }
    return completed;
}