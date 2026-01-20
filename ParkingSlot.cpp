#include "ParkingSlot.h"

ParkingSlot::ParkingSlot() {
    slotID = 0;
    zoneID = 0;
    isAvailable = true;
}

ParkingSlot::~ParkingSlot() {
    // Destructor - no special cleanup needed
}

int ParkingSlot::getSlotID() const {
    return slotID;
}

void ParkingSlot::setSlotID(int id) {
    slotID = id;
}

int ParkingSlot::getZoneID() const {
    return zoneID;
}

void ParkingSlot::setZoneID(int id) {
    zoneID = id;
}

bool ParkingSlot::getIsAvailable() const {
    return isAvailable;
}

void ParkingSlot::setIsAvailable(bool available) {
    isAvailable = available;
}