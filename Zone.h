#ifndef ZONE_H
#define ZONE_H

#include "ParkingSlot.h"

class Zone {
private:
    int zoneID;
    char zoneName[50];  // Fixed-size character array for zone name (no STL strings)
    ParkingSlot* parkingSlots;  // Array of ParkingSlot pointers
    int numSlots;      // Current number of slots in the zone
    int capacity;      // Maximum capacity of the parking slots array
    int* adjacentZones;  // Array of adjacent zone IDs
    int numAdjacentZones;  // Number of adjacent zones
    int adjacencyCapacity;  // Capacity of adjacentZones array

public:
    // Default constructor
    Zone();

    // Constructor with initial capacity
    Zone(int initialCapacity);

    // Destructor
    ~Zone();

    // Function to add a parking slot to the zone
    void addSlot(const ParkingSlot& slot);

    // Function to check if the zone has any available parking slots
    bool isAvailable() const;

    // Function to check if a specific slot is available by slotID
    bool isSlotAvailable(int slotID) const;

    // Getter for zoneID
    int getZoneID() const;

    // Setter for zoneID
    void setZoneID(int id);

    // Getter for zoneName
    const char* getZoneName() const;

    // Setter for zoneName
    void setZoneName(const char* name);

    // Function to add an adjacent zone (for zone relationships)
    void addAdjacentZone(int adjacentZoneID);

    // Function to check if a zone is adjacent
    bool isAdjacentZone(int zoneID) const;

    // Getter for adjacent zones
    int* getAdjacentZones() const;

    // Getter for number of adjacent zones
    int getNumAdjacentZones() const;
};

#endif // ZONE_H