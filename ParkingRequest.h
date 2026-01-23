#ifndef PARKING_REQUEST_H
#define PARKING_REQUEST_H

class ParkingRequest {
public:
    // Enum for parking request states
    enum State {
        REQUESTED,  // Initial state when a parking request is made
        ALLOCATED,  // A parking slot has been assigned to the request
        OCCUPIED,   // The vehicle is currently parked in the allocated slot
        RELEASED,   // The parking slot has been vacated and is available again
        CANCELLED   // The request has been cancelled before completion
    };

private:
    int requestID;
    int vehicleID;
    int requestedZone;
    int requestTime;
    State currentState;

public:
    // Constructor
    ParkingRequest();

    // Destructor
    ~ParkingRequest();

    // Functions to change state
    void allocate();    // Change state to ALLOCATED
    void occupy();      // Change state to OCCUPIED
    void release();     // Change state to RELEASED
    void cancel();      // Change state to CANCELLED

    // Getter for current state
    State getState() const;

    // Setters for attributes
    void setRequestID(int id);
    void setVehicleID(int id);
    void setRequestedZone(int zone);
    void setRequestTime(int time);

    // Getters for attributes
    int getRequestID() const;
    int getVehicleID() const;
    int getRequestedZone() const;
    int getRequestTime() const;
};

#endif // PARKING_REQUEST_H