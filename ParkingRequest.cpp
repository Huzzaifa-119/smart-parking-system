#include "ParkingRequest.h"
#include <iostream>

ParkingRequest::ParkingRequest() {
    requestID = 0;
    vehicleID = 0;
    requestedZone = 0;
    requestTime = 0;
    exitTime = 0;  // Not set until RELEASED
    currentState = REQUESTED;
}

ParkingRequest::~ParkingRequest() {
    // Destructor - no special cleanup needed
}

void ParkingRequest::allocate() {
    // Transition from REQUESTED to ALLOCATED
    // Valid only if current state is REQUESTED
    if (currentState == REQUESTED) {
        currentState = ALLOCATED;
    } else {
        std::cout << "Error: Can only allocate from REQUESTED state" << std::endl;
    }
}

void ParkingRequest::occupy() {
    // Transition from ALLOCATED to OCCUPIED
    // Valid only if current state is ALLOCATED
    if (currentState == ALLOCATED) {
        currentState = OCCUPIED;
    } else {
        std::cout << "Error: Can only occupy from ALLOCATED state" << std::endl;
    }
}

void ParkingRequest::release() {
    // Transition from OCCUPIED to RELEASED
    // Valid only if current state is OCCUPIED
    if (currentState == OCCUPIED) {
        currentState = RELEASED;
    } else {
        std::cout << "Error: Can only release from OCCUPIED state" << std::endl;
    }
}

void ParkingRequest::cancel() {
    // Transition to CANCELLED from REQUESTED or ALLOCATED
    // Valid only if current state is REQUESTED or ALLOCATED
    if (currentState == REQUESTED || currentState == ALLOCATED) {
        currentState = CANCELLED;
    } else {
        std::cout << "Error: Can only cancel from REQUESTED or ALLOCATED state" << std::endl;
    }
}

ParkingRequest::State ParkingRequest::getState() const {
    return currentState;
}

void ParkingRequest::setRequestID(int id) {
    requestID = id;
}

void ParkingRequest::setVehicleID(int id) {
    vehicleID = id;
}

void ParkingRequest::setRequestedZone(int zone) {
    requestedZone = zone;
}

void ParkingRequest::setRequestTime(int time) {
    requestTime = time;
}

int ParkingRequest::getRequestID() const {
    return requestID;
}

int ParkingRequest::getVehicleID() const {
    return vehicleID;
}

int ParkingRequest::getRequestedZone() const {
    return requestedZone;
}

int ParkingRequest::getRequestTime() const {
    return requestTime;
}

int ParkingRequest::getExitTime() const {
    return exitTime;
}

void ParkingRequest::setExitTime(int time) {
    exitTime = time;
}