#include "Zone.h"
#include <cstring>  // For strcpy

Zone::Zone() {
    zoneID = 0;
    zoneName[0] = '\0';  // Initialize as empty string
    capacity = 10;  // Default capacity
    numSlots = 0;
    parkingSlots = new ParkingSlot[capacity];
}

Zone::Zone(int initialCapacity) {
    zoneID = 0;
    zoneName[0] = '\0';  // Initialize as empty string
    capacity = initialCapacity;
    numSlots = 0;
    parkingSlots = new ParkingSlot[capacity];
}

Zone::~Zone() {
    delete[] parkingSlots;
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