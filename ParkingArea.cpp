#include "ParkingArea.h"

ParkingArea::ParkingArea() {
    areaID = 0;
    zoneID = 0;
    capacity = 10;  // Default capacity
    numSlots = 0;
    slots = new ParkingSlot[capacity];
}

ParkingArea::ParkingArea(int initialCapacity) {
    areaID = 0;
    zoneID = 0;
    capacity = initialCapacity;
    numSlots = 0;
    slots = nullptr;
    slots = new ParkingSlot[capacity];
}

ParkingArea::ParkingArea(const ParkingArea& other) {
    areaID = other.areaID;
    zoneID = other.zoneID;
    capacity = other.capacity;
    numSlots = other.numSlots;
    slots = new ParkingSlot[capacity];
    for (int i = 0; i < numSlots; i++) {
        slots[i] = other.slots[i];
    }
}

ParkingArea& ParkingArea::operator=(const ParkingArea& other) {
    if (this != &other) {
        if (slots != nullptr) {
            delete[] slots;
        }
        areaID = other.areaID;
        zoneID = other.zoneID;
        capacity = other.capacity;
        numSlots = other.numSlots;
        slots = new ParkingSlot[capacity];
        for (int i = 0; i < numSlots; i++) {
            slots[i] = other.slots[i];
        }
    }
    return *this;
}

ParkingArea::~ParkingArea() {
    if (slots != nullptr) {
        delete[] slots;
        slots = nullptr;
    }
}

void ParkingArea::addSlot(const ParkingSlot& slot) {
    if (numSlots < capacity) {
        slots[numSlots] = slot;
        numSlots++;
    }
    // If capacity is full, do nothing
}

int ParkingArea::getAreaID() const {
    return areaID;
}

void ParkingArea::setAreaID(int id) {
    areaID = id;
}

int ParkingArea::getZoneID() const {
    return zoneID;
}

void ParkingArea::setZoneID(int id) {
    zoneID = id;
}

// Find the first available slot by returning its slotID, or -1 if none available
int ParkingArea::findFirstAvailableSlot() const {
    for (int i = 0; i < numSlots; i++) {
        if (slots[i].getIsAvailable()) {
            return slots[i].getSlotID();
        }
    }
    return -1;  // No available slot found
}

// Mark a specific slot as occupied (not available)
void ParkingArea::occupySlot(int slotID) {
    for (int i = 0; i < numSlots; i++) {
        if (slots[i].getSlotID() == slotID) {
            slots[i].setIsAvailable(false);
            return;
        }
    }
    // If slotID not found, do nothing
}

// Mark a specific slot as free (available)
void ParkingArea::freeSlot(int slotID) {
    for (int i = 0; i < numSlots; i++) {
        if (slots[i].getSlotID() == slotID) {
            slots[i].setIsAvailable(true);
            return;
        }
    }
    // If slotID not found, do nothing
}

int ParkingArea::getNumSlots() const {
    return numSlots;
}

int ParkingArea::getAvailableSlots() const {
    int count = 0;
    for (int i = 0; i < numSlots; i++) {
        if (slots[i].getIsAvailable()) {
            count++;
        }
    }
    return count;
}