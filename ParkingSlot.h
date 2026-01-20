#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

class ParkingSlot {
private:
    int slotID;
    int zoneID;
    bool isAvailable;

public:
    // Constructor
    ParkingSlot();

    // Destructor
    ~ParkingSlot();

    // Getter for slotID
    int getSlotID() const;

    // Setter for slotID
    void setSlotID(int id);

    // Getter for zoneID
    int getZoneID() const;

    // Setter for zoneID
    void setZoneID(int id);

    // Getter for isAvailable
    bool getIsAvailable() const;

    // Setter for isAvailable
    void setIsAvailable(bool available);
};

#endif // PARKING_SLOT_H