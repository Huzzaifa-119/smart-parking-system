#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "ParkingArea.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

class ParkingSystem {
private:
    Zone* zones;                    // Array of zones
    int numZones;                   // Current number of zones
    int maxZones;                   // Maximum zones capacity

    Vehicle* vehicles;              // Array of vehicles
    int numVehicles;                // Current number of vehicles
    int maxVehicles;                // Maximum vehicles capacity

    ParkingRequest* requests;       // Array of parking requests
    int numRequests;                // Current number of requests
    int maxRequests;                // Maximum requests capacity

    ParkingArea* areas;             // Array of parking areas
    int numAreas;                   // Current number of areas
    int maxAreas;                   // Maximum areas capacity

    AllocationEngine* allocEngine;  // Engine for slot allocation
    RollbackManager* rollbackMgr;   // Manager for rollback operations

public:
    // Constructor: Initializes the system with maximum capacities
    ParkingSystem(int maxZ, int maxV, int maxR, int maxA, int maxOps);

    // Destructor
    ~ParkingSystem();

    // Manage zones: Add a zone to the system
    void addZone(const Zone& zone);

    // Manage vehicles: Add a vehicle to the system
    void addVehicle(const Vehicle& vehicle);

    // Manage requests: Add a parking request to the system
    void addRequest(const ParkingRequest& request);

    // Manage areas: Add a parking area to the system
    void addArea(const ParkingArea& area);

    // Coordinate allocation: Allocate a slot for a specific request
    int allocateForRequest(int requestIndex);

    // Coordinate rollback: Undo the last allocation
    void rollbackLast();

    // Coordinate rollback: Undo the last k allocations
    void rollbackLastK(int k);

    // Analytics: Get total number of parking slots across all areas
    int getTotalSlots() const;

    // Analytics: Get number of available parking slots
    int getAvailableSlots() const;

    // Analytics: Get number of active requests
    int getActiveRequests() const;

    // Analytics: Get utilization percentage for a specific zone
    // Formula: (occupied slots / total slots) * 100
    double getZoneUtilization(int zoneID) const;

    // Analytics: Get average parking duration (placeholder - duration not currently tracked)
    double getAverageParkingDuration() const;

    // Analytics: Get number of cancelled requests
    int getCancelledRequests() const;

    // Analytics: Get number of completed requests (those that reached RELEASED state)
    int getCompletedRequests() const;
};

#endif // PARKING_SYSTEM_H