#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle {
private:
    int vehicleID;
    int preferredZone;

public:
    // Constructor
    Vehicle();

    // Destructor
    ~Vehicle();

    // Getter for vehicleID
    int getVehicleID() const;

    // Setter for vehicleID
    void setVehicleID(int id);

    // Getter for preferredZone
    int getPreferredZone() const;

    // Setter for preferredZone
    void setPreferredZone(int zone);
};

#endif // VEHICLE_H