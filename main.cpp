#include <iostream>
#include "ParkingSystem.h"

int main() {
    std::cout << "Smart Parking System Test" << std::endl;

    // Create the parking system with maximum capacities
    ParkingSystem system(5, 5, 5, 5, 5);

    // Create and add a zone
    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("North Zone");
    system.addZone(zone1);

    // Create and add a parking area
    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    // Add parking slots to the area
    ParkingSlot slot1;
    slot1.setSlotID(101);
    slot1.setZoneID(1);
    slot1.setIsAvailable(true);
    area1.addSlot(slot1);

    ParkingSlot slot2;
    slot2.setSlotID(102);
    slot2.setZoneID(1);
    slot2.setIsAvailable(true);
    area1.addSlot(slot2);

    ParkingSlot slot3;
    slot3.setSlotID(103);
    slot3.setZoneID(1);
    slot3.setIsAvailable(true);
    area1.addSlot(slot3);

    system.addArea(area1);

    // Create and add a vehicle
    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    // Create and add a parking request
    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setVehicleID(1);
    req1.setRequestedZone(1);
    req1.setRequestTime(100);
    system.addRequest(req1);

    // Print initial system state
    std::cout << "Initial state:" << std::endl;
    std::cout << "Total slots: " << system.getTotalSlots() << std::endl;
    std::cout << "Available slots: " << system.getAvailableSlots() << std::endl;
    std::cout << "Active requests: " << system.getActiveRequests() << std::endl;
    std::cout << "Zone 1 utilization: " << system.getZoneUtilization(1) << "%" << std::endl;
    std::cout << "Average parking duration: " << system.getAverageParkingDuration() << " hours" << std::endl;
    std::cout << "Cancelled requests: " << system.getCancelledRequests() << std::endl;
    std::cout << "Completed requests: " << system.getCompletedRequests() << std::endl;

    // Demonstrate allocation
    std::cout << "\nAllocating slot for request 0..." << std::endl;
    int allocatedSlot = system.allocateForRequest(0);
    std::cout << "Allocated slot ID: " << allocatedSlot << std::endl;
    std::cout << "Available slots after allocation: " << system.getAvailableSlots() << std::endl;
    std::cout << "Zone 1 utilization after allocation: " << system.getZoneUtilization(1) << "%" << std::endl;

    // Demonstrate rollback
    std::cout << "\nRolling back last allocation..." << std::endl;
    system.rollbackLast();
    std::cout << "Available slots after rollback: " << system.getAvailableSlots() << std::endl;
    std::cout << "Active requests after rollback: " << system.getActiveRequests() << std::endl;
    std::cout << "Cancelled requests after rollback: " << system.getCancelledRequests() << std::endl;
    std::cout << "Zone 1 utilization after rollback: " << system.getZoneUtilization(1) << "%" << std::endl;

    std::cout << "\nTest completed." << std::endl;
    return 0;
}