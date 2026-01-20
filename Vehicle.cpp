#include "Vehicle.h"

Vehicle::Vehicle() {
    vehicleID = 0;
    preferredZone = 0;
}

Vehicle::~Vehicle() {
    // Destructor - no special cleanup needed
}

int Vehicle::getVehicleID() const {
    return vehicleID;
}

void Vehicle::setVehicleID(int id) {
    vehicleID = id;
}

int Vehicle::getPreferredZone() const {
    return preferredZone;
}

void Vehicle::setPreferredZone(int zone) {
    preferredZone = zone;
}