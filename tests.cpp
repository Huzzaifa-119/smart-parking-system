#include <iostream>
#include <cassert>
#include "ParkingSystem.h"

class TestTracker {
private:
    static int testsRun;
    static int testsPassed;
public:
    static int getTestsRun() { return testsRun; }
    static int getTestsPassed() { return testsPassed; }
    static void incrementRun() { testsRun++; }
    static void incrementPassed() { testsPassed++; }
};

int TestTracker::testsRun = 0;
int TestTracker::testsPassed = 0;

void test_assert(bool condition, const char* testName) {
    TestTracker::incrementRun();
    if (condition) {
        TestTracker::incrementPassed();
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

void test_1_basic_slot_allocation() {
    std::cout << "\n=== TEST 1: Basic Slot Allocation ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    ParkingSlot slot1, slot2, slot3;
    slot1.setSlotID(101);
    slot1.setZoneID(1);
    slot1.setIsAvailable(true);
    area1.addSlot(slot1);

    slot2.setSlotID(102);
    slot2.setZoneID(1);
    slot2.setIsAvailable(true);
    area1.addSlot(slot2);

    slot3.setSlotID(103);
    slot3.setZoneID(1);
    slot3.setIsAvailable(true);
    area1.addSlot(slot3);

    system.addArea(area1);

    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setVehicleID(1);
    req1.setRequestedZone(1);
    req1.setRequestTime(100);
    system.addRequest(req1);

    int availableBefore = system.getAvailableSlots();
    int allocatedSlot = system.allocateForRequest(0);
    int availableAfter = system.getAvailableSlots();

    test_assert(allocatedSlot != -1, "Allocation successful");
    test_assert(availableAfter == availableBefore - 1, "Slot marked occupied");
}

void test_2_cross_zone_allocation() {
    std::cout << "\n=== TEST 2: Cross-Zone Allocation ===" << std::endl;

    ParkingSystem system(3, 5, 5, 3, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("North");
    system.addZone(zone1);

    Zone zone2;
    zone2.setZoneID(2);
    zone2.setZoneName("South");
    system.addZone(zone2);

    ParkingArea area1(1);
    area1.setAreaID(1);
    area1.setZoneID(1);

    ParkingSlot slot1;
    slot1.setSlotID(101);
    slot1.setZoneID(1);
    slot1.setIsAvailable(true);
    area1.addSlot(slot1);
    system.addArea(area1);

    ParkingArea area2(2);
    area2.setAreaID(2);
    area2.setZoneID(2);

    ParkingSlot slot2, slot3;
    slot2.setSlotID(201);
    slot2.setZoneID(2);
    slot2.setIsAvailable(true);
    area2.addSlot(slot2);

    slot3.setSlotID(202);
    slot3.setZoneID(2);
    slot3.setIsAvailable(true);
    area2.addSlot(slot3);
    system.addArea(area2);

    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setVehicleID(1);
    req1.setRequestedZone(1);
    req1.setRequestTime(100);
    system.addRequest(req1);

    int slot1_allocated = system.allocateForRequest(0);
    test_assert(slot1_allocated == 101, "Zone 1 preferred slot allocated");

    ParkingRequest req2;
    req2.setRequestID(2);
    req2.setVehicleID(1);
    req2.setRequestedZone(1);
    req2.setRequestTime(150);
    system.addRequest(req2);

    int slot2_allocated = system.allocateForRequest(1);
    test_assert(slot2_allocated != -1 && slot2_allocated != 101, "Falls back to zone 2");
}

void test_3_rollback() {
    std::cout << "\n=== TEST 3: Rollback Correctness ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 3; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setVehicleID(1);
    req1.setRequestedZone(1);
    req1.setRequestTime(100);
    system.addRequest(req1);

    int availableBefore = system.getAvailableSlots();
    int allocatedSlot = system.allocateForRequest(0);
    int availableAfterAlloc = system.getAvailableSlots();

    system.rollbackLast();
    int availableAfterRollback = system.getAvailableSlots();

    test_assert(allocatedSlot != -1, "Allocation successful");
    test_assert(availableAfterAlloc < availableBefore, "Slot occupied");
    test_assert(availableAfterRollback == availableBefore, "Slot restored after rollback");
}

void test_4_rollback_k() {
    std::cout << "\n=== TEST 4: Rollback Last K Operations ===" << std::endl;

    ParkingSystem system(2, 10, 10, 2, 20);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(5);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 5; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    for (int i = 0; i < 3; i++) {
        Vehicle veh;
        veh.setVehicleID(i + 1);
        veh.setPreferredZone(1);
        system.addVehicle(veh);

        ParkingRequest req;
        req.setRequestID(i + 1);
        req.setVehicleID(i + 1);
        req.setRequestedZone(1);
        req.setRequestTime(100 + i * 10);
        system.addRequest(req);
    }

    int availableBefore = system.getAvailableSlots();
    system.allocateForRequest(0);
    system.allocateForRequest(1);
    system.allocateForRequest(2);
    int availableAfter3 = system.getAvailableSlots();

    system.rollbackLastK(2);
    int availableAfterRollback = system.getAvailableSlots();

    test_assert(availableAfter3 == availableBefore - 3, "Allocated 3 slots");
    test_assert(availableAfterRollback == availableBefore - 1, "Rolled back 2 of 3");
}

void test_5_invalid_transitions() {
    std::cout << "\n=== TEST 5: Invalid State Transitions ===" << std::endl;

    ParkingRequest req;
    req.setRequestID(1);

    req.allocate();
    bool valid_allocate = (req.getState() == ParkingRequest::ALLOCATED);

    req.occupy();
    bool valid_occupy = (req.getState() == ParkingRequest::OCCUPIED);

    req.release();
    bool valid_release = (req.getState() == ParkingRequest::RELEASED);

    int stateBeforeInvalid = req.getState();
    req.allocate();
    bool invalid_prevented = (req.getState() == stateBeforeInvalid);

    test_assert(valid_allocate, "Valid REQUESTED->ALLOCATED");
    test_assert(valid_occupy, "Valid ALLOCATED->OCCUPIED");
    test_assert(valid_release, "Valid OCCUPIED->RELEASED");
    test_assert(invalid_prevented, "Invalid transition prevented");
}

void test_6_cancelled_immutable() {
    std::cout << "\n=== TEST 6: Cancelled State Immutability ===" << std::endl;

    ParkingRequest req;
    req.setRequestID(1);

    req.cancel();
    bool cancelled_from_requested = (req.getState() == ParkingRequest::CANCELLED);

    req.occupy();
    bool cannot_occupy_cancelled = (req.getState() == ParkingRequest::CANCELLED);

    req.release();
    bool cannot_release_cancelled = (req.getState() == ParkingRequest::CANCELLED);

    test_assert(cancelled_from_requested, "Can cancel from REQUESTED");
    test_assert(cannot_occupy_cancelled, "Cannot occupy CANCELLED");
    test_assert(cannot_release_cancelled, "Cannot release CANCELLED");
}

void test_7_cancelled_analytics() {
    std::cout << "\n=== TEST 7: Cancelled Requests Analytics ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 3; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    for (int i = 0; i < 3; i++) {
        Vehicle veh;
        veh.setVehicleID(i + 1);
        veh.setPreferredZone(1);
        system.addVehicle(veh);
    }

    for (int i = 0; i < 3; i++) {
        ParkingRequest req;
        req.setRequestID(i + 1);
        req.setVehicleID(i + 1);
        req.setRequestedZone(1);
        req.setRequestTime(100 + i * 10);
        system.addRequest(req);
    }

    system.allocateForRequest(0);
    system.allocateForRequest(1);
    system.rollbackLastK(2);

    int cancelledCount = system.getCancelledRequests();
    test_assert(cancelledCount == 2, "Cancelled requests tracked correctly");
}

void test_8_completed_state() {
    std::cout << "\n=== TEST 8: Completed Requests State ===" << std::endl;

    ParkingRequest req;
    req.setRequestID(1);

    req.allocate();
    req.occupy();
    req.release();

    bool completed = (req.getState() == ParkingRequest::RELEASED);
    test_assert(completed, "Request reaches RELEASED state");
}

void test_9_zone_utilization() {
    std::cout << "\n=== TEST 9: Zone Utilization Analytics ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(4);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 4; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    for (int i = 0; i < 2; i++) {
        Vehicle veh;
        veh.setVehicleID(i + 1);
        veh.setPreferredZone(1);
        system.addVehicle(veh);

        ParkingRequest req;
        req.setRequestID(i + 1);
        req.setVehicleID(i + 1);
        req.setRequestedZone(1);
        req.setRequestTime(100 + i * 10);
        system.addRequest(req);
    }

    system.allocateForRequest(0);
    system.allocateForRequest(1);

    double utilization = system.getZoneUtilization(1);
    bool util_correct = (utilization >= 49.0 && utilization <= 51.0);
    test_assert(util_correct, "Zone utilization calculated correctly (50%)");
}

void test_10_analytics_after_rollback() {
    std::cout << "\n=== TEST 10: Analytics After Rollback ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 3; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setVehicleID(1);
    req1.setRequestedZone(1);
    req1.setRequestTime(100);
    system.addRequest(req1);

    int totalSlots = system.getTotalSlots();
    int availableBefore = system.getAvailableSlots();
    double util_before = system.getZoneUtilization(1);

    system.allocateForRequest(0);
    system.rollbackLast();

    int availableAfter = system.getAvailableSlots();
    double util_after = system.getZoneUtilization(1);

    test_assert(totalSlots == 3, "Total slots consistent");
    test_assert(availableAfter == availableBefore, "Available slots restored");
    test_assert(util_after == util_before, "Utilization restored");
}

void test_11_complex_scenario() {
    std::cout << "\n=== TEST 11: Complex Rollback Scenario ===" << std::endl;

    ParkingSystem system(2, 10, 10, 2, 20);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(5);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 5; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    for (int i = 0; i < 5; i++) {
        Vehicle veh;
        veh.setVehicleID(i + 1);
        veh.setPreferredZone(1);
        system.addVehicle(veh);
    }

    for (int i = 0; i < 5; i++) {
        ParkingRequest req;
        req.setRequestID(i + 1);
        req.setVehicleID(i + 1);
        req.setRequestedZone(1);
        req.setRequestTime(100 + i * 10);
        system.addRequest(req);
    }

    int availableStart = system.getAvailableSlots();

    for (int i = 0; i < 5; i++) {
        system.allocateForRequest(i);
    }
    int availableAfter5 = system.getAvailableSlots();

    system.rollbackLastK(3);
    int availableAfterRollback = system.getAvailableSlots();

    test_assert(availableAfter5 == availableStart - 5, "Allocated all 5");
    test_assert(availableAfterRollback == availableStart - 2, "Rolled back 3 of 5");
}

void test_12_invalid_request() {
    std::cout << "\n=== TEST 12: Invalid Request Handling ===" << std::endl;

    ParkingSystem system(2, 5, 5, 2, 10);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    ParkingArea area1(3);
    area1.setAreaID(1);
    area1.setZoneID(1);

    for (int i = 0; i < 3; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    Vehicle veh1;
    veh1.setVehicleID(1);
    veh1.setPreferredZone(1);
    system.addVehicle(veh1);

    int result = system.allocateForRequest(999);
    test_assert(result == -1, "Invalid request index returns -1");
}

void test_13_zone_adjacency() {
    std::cout << "\n=== TEST 13: Zone Adjacency Structure ===" << std::endl;

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("North");

    Zone zone2;
    zone2.setZoneID(2);
    zone2.setZoneName("South");

    Zone zone3;
    zone3.setZoneID(3);
    zone3.setZoneName("East");

    zone1.addAdjacentZone(2);
    zone1.addAdjacentZone(3);
    zone2.addAdjacentZone(1);

    bool test13_z1_adj_z2 = zone1.isAdjacentZone(2);
    bool test13_z1_adj_z3 = zone1.isAdjacentZone(3);
    bool test13_z1_count = (zone1.getNumAdjacentZones() == 2);
    bool test13_z3_not_adj = !zone3.isAdjacentZone(1);

    test_assert(test13_z1_adj_z2, "Zone 1 adjacent to Zone 2");
    test_assert(test13_z1_adj_z3, "Zone 1 adjacent to Zone 3");
    test_assert(test13_z1_count, "No duplicate adjacencies (count = 2)");
    test_assert(test13_z3_not_adj, "Zone 3 not adjacent to Zone 1");
}

void test_14_peak_usage_zones() {
    std::cout << "\n=== TEST 14: Peak Usage Zones Analytics ===" << std::endl;

    ParkingSystem system(3, 10, 10, 3, 20);

    Zone zone1;
    zone1.setZoneID(1);
    zone1.setZoneName("Zone A");
    system.addZone(zone1);

    Zone zone2;
    zone2.setZoneID(2);
    zone2.setZoneName("Zone B");
    system.addZone(zone2);

    ParkingArea area1(2);
    area1.setAreaID(1);
    area1.setZoneID(1);
    for (int i = 0; i < 2; i++) {
        ParkingSlot slot;
        slot.setSlotID(101 + i);
        slot.setZoneID(1);
        slot.setIsAvailable(true);
        area1.addSlot(slot);
    }
    system.addArea(area1);

    ParkingArea area2(4);
    area2.setAreaID(2);
    area2.setZoneID(2);
    for (int i = 0; i < 4; i++) {
        ParkingSlot slot;
        slot.setSlotID(201 + i);
        slot.setZoneID(2);
        slot.setIsAvailable(true);
        area2.addSlot(slot);
    }
    system.addArea(area2);

    for (int i = 0; i < 4; i++) {
        Vehicle veh;
        veh.setVehicleID(i + 1);
        veh.setPreferredZone((i < 2) ? 1 : 2);
        system.addVehicle(veh);

        ParkingRequest req;
        req.setRequestID(i + 1);
        req.setVehicleID(i + 1);
        req.setRequestedZone((i < 2) ? 1 : 2);
        req.setRequestTime(100 + i * 10);
        system.addRequest(req);
    }

    system.allocateForRequest(0);
    system.allocateForRequest(1);
    system.allocateForRequest(2);
    system.allocateForRequest(3);

    int peakZone = system.getPeakUsageZone();
    bool test14_peak = (peakZone == 1);

    test_assert(test14_peak, "Peak usage zone correctly identified (Zone 1 at 100%)");
}

void test_15_parking_duration() {
    std::cout << "\n=== TEST 15: Parking Duration Tracking ===" << std::endl;

    ParkingRequest req1;
    req1.setRequestID(1);
    req1.setRequestTime(100);
    req1.allocate();
    req1.occupy();
    req1.setExitTime(105);
    req1.release();

    int duration = req1.getExitTime() - req1.getRequestTime();
    bool test15_duration = (duration == 5);
    bool test15_released = (req1.getState() == ParkingRequest::RELEASED);

    test_assert(test15_duration, "Parking duration tracked (5 units)");
    test_assert(test15_released, "Request in RELEASED state");
}

int main() {
    std::cout << "\n";
    std::cout << "============================================================" << std::endl;
    std::cout << "SMART PARKING SYSTEM - COMPREHENSIVE TEST SUITE" << std::endl;
    std::cout << "(15 Test Cases - DSA Semester Project)" << std::endl;
    std::cout << "============================================================" << std::endl;

    test_1_basic_slot_allocation();
    test_2_cross_zone_allocation();
    test_3_rollback();
    test_4_rollback_k();
    test_5_invalid_transitions();
    test_6_cancelled_immutable();
    test_7_cancelled_analytics();
    test_8_completed_state();
    test_9_zone_utilization();
    test_10_analytics_after_rollback();
    test_11_complex_scenario();
    test_12_invalid_request();
    test_13_zone_adjacency();
    test_14_peak_usage_zones();
    test_15_parking_duration();

    std::cout << "\n";
    std::cout << "============================================================" << std::endl;
    std::cout << "TEST RESULTS SUMMARY" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "Total Tests: " << TestTracker::getTestsRun() << std::endl;
    std::cout << "Tests Passed: " << TestTracker::getTestsPassed() << std::endl;
    std::cout << "Tests Failed: " << (TestTracker::getTestsRun() - TestTracker::getTestsPassed()) << std::endl;
    std::cout << "Pass Rate: " << (100.0 * TestTracker::getTestsPassed() / TestTracker::getTestsRun()) << "%" << std::endl;
    std::cout << "\nAll 15 test cases cover:" << std::endl;
    std::cout << "  - Slot allocation correctness" << std::endl;
    std::cout << "  - Cross-zone allocation handling" << std::endl;
    std::cout << "  - Cancellation and rollback correctness" << std::endl;
    std::cout << "  - Invalid state transition detection" << std::endl;
    std::cout << "  - Analytics correctness after rollback" << std::endl;
    std::cout << "  - Zone adjacency relationships" << std::endl;
    std::cout << "  - Peak usage zones analytics" << std::endl;
    std::cout << "  - Parking duration tracking" << std::endl;
    std::cout << "  - Edge cases and error handling" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
