#include "Zone.h"
#include <cstring>  // For strcpy

Zone::Zone() {
    zoneID = 0;
    zoneName[0] = '\0';  // Initialize as empty string
    capacity = 10;  // Default capacity
    numSlots = 0;
    parkingSlots = new ParkingSlot[capacity];
    adjacencyCapacity = 10;  // Default adjacency capacity
    numAdjacentZones = 0;
    adjacentZones = new int[adjacencyCapacity];
}

Zone::Zone(int initialCapacity) {
    zoneID = 0;
    zoneName[0] = '\0';  // Initialize as empty string
    capacity = initialCapacity;
    numSlots = 0;
    parkingSlots = new ParkingSlot[capacity];
    adjacencyCapacity = 10;  // Default adjacency capacity
    numAdjacentZones = 0;
    adjacentZones = new int[adjacencyCapacity];
}

Zone::~Zone() {
    delete[] parkingSlots;
    delete[] adjacentZones;
}

void Zone::addSlot(const ParkingSlot& slot) {
    if (numSlots < capacity) {
        parkingSlots[numSlots] = slot;
        numSlots++;
    }
    // If capacity is full, do nothing (could add error handling if needed)
}

bool Zone::isAvailable() const {
    for (int i = 0; i < numSlots; i++) {
        if (parkingSlots[i].getIsAvailable()) {
            return true;
        }
    }
    return false;
}

bool Zone::isSlotAvailable(int slotID) const {
    for (int i = 0; i < numSlots; i++) {
        if (parkingSlots[i].getSlotID() == slotID) {
            return parkingSlots[i].getIsAvailable();
        }
    }
    return false;  // Slot not found or not available
}

int Zone::getZoneID() const {
    return zoneID;
}

void Zone::setZoneID(int id) {
    zoneID = id;
}

const char* Zone::getZoneName() const {
    return zoneName;
}

void Zone::setZoneName(const char* name) {
    if (name != nullptr) {
        strcpy(zoneName, name);
    }
}

void Zone::addAdjacentZone(int adjacentZoneID) {
    // Check if already adjacent
    for (int i = 0; i < numAdjacentZones; i++) {
        if (adjacentZones[i] == adjacentZoneID) {
            return;  // Already adjacent, skip
        }
    }
    
    // Add if capacity allows
    if (numAdjacentZones < adjacencyCapacity) {
        adjacentZones[numAdjacentZones] = adjacentZoneID;
        numAdjacentZones++;
    }
}

bool Zone::isAdjacentZone(int zoneID) const {
    for (int i = 0; i < numAdjacentZones; i++) {
        if (adjacentZones[i] == zoneID) {
            return true;
        }
    }
    return false;
}

int* Zone::getAdjacentZones() const {
    return adjacentZones;
}

int Zone::getNumAdjacentZones() const {
    return numAdjacentZones;
}