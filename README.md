Smart Parking Allocation & Zone Management System

This project is an in-memory Smart Parking Allocation System implemented in C++ as part of a Data Structures / Systems course project.

The system models a real-world parking environment using object-oriented design principles. It manages vehicles, parking requests, zones, parking areas, and parking slots while enforcing a controlled request lifecycle.

Key Features

In-memory implementation (no files or databases)

Object-oriented modular design

Parking request lifecycle using a state machine

Zone-based parking slot allocation with cross-zone fallback

Stack-based rollback mechanism to undo recent allocations

Clean separation between data, logic, and controller layers

No use of STL containers (arrays and pointers used instead)

System Components

ParkingSlot & Vehicle: Core entity classes

ParkingRequest: Implements request lifecycle using a state machine

Zone & ParkingArea: Represent physical parking structure

AllocationEngine: Handles parking slot allocation logic

RollbackManager: Supports undoing recent allocations using stack logic

ParkingSystem: Central controller coordinating all components

main.cpp: Demonstrates system usage (no business logic)
