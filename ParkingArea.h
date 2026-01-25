#ifndef PARKING_AREA_H
#define PARKING_AREA_H

#include "ParkingSlot.h"

class ParkingArea {
private:
    int areaID;
    int zoneID;
    ParkingSlot* slots;  // Array of ParkingSlot objects
    int numSlots;        // Current number of slots
    int capacity;        // Maximum capacity

public:
    // Default constructor
    ParkingArea();

    // Constructor with initial capacity
    ParkingArea(int initialCapacity);

    // Copy constructor
    ParkingArea(const ParkingArea& other);

    // Copy assignment operator
    ParkingArea& operator=(const ParkingArea& other);

    // Destructor
    ~ParkingArea();

    // Function to add a parking slot
    void addSlot(const ParkingSlot& slot);

    // Getter for areaID
    int getAreaID() const;

    // Setter for areaID
    void setAreaID(int id);

    // Getter for zoneID
    int getZoneID() const;

    // Setter for zoneID
    void setZoneID(int id);

    // Function to find the first available slot ID (-1 if none)
    int findFirstAvailableSlot() const;

    // Function to mark a slot as occupied
    void occupySlot(int slotID);

    // Function to mark a slot as free
    void freeSlot(int slotID);

    // Getter for number of slots
    int getNumSlots() const;

    // Getter for number of available slots
    int getAvailableSlots() const;
};

#endif // PARKING_AREA_H