# Smart Parking Allocation & Zone Management System - Design Document

## 1. System Architecture Overview

The Smart Parking System is an in-memory resource allocation system designed to manage parking requests across multiple zones in a city. The system follows a modular architecture with clear separation of concerns across multiple components.

### 1.1 Core Components

```
┌─────────────────────────────────────────────────────┐
│           ParkingSystem (Main Coordinator)          │
├─────────────────────────────────────────────────────┤
│                                                      │
│  ┌──────────────┐  ┌──────────────┐  ┌───────────┐  │
│  │ AllocationEngine  │  RollbackManager │ Analytics │  │
│  └──────────────┘  └──────────────┘  └───────────┘  │
│                                                      │
│  ┌──────────────────────────────────────────────┐   │
│  │  Zone[] → ParkingArea[] → ParkingSlot[]      │   │
│  └──────────────────────────────────────────────┘   │
│                                                      │
│  ┌──────────────────────────────────────────────┐   │
│  │  Vehicle[] | ParkingRequest[]                │   │
│  └──────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
```

---

## 2. Zone and Slot Representation

### 2.1 Data Structures

#### **Zone Representation**
- **Location:** `Zone.h / Zone.cpp`
- **Purpose:** Logical geographic division of the city into parking sectors
- **Key Attributes:**
  - `zoneID` (int): Unique identifier
  - `zoneName` (char[50]): Human-readable name (fixed-size array, no STL)
  - `numSlots` (int): Current count of parking slots
  - `capacity` (int): Maximum array capacity

#### **ParkingArea Representation**
- **Location:** `ParkingArea.h / ParkingArea.cpp`
- **Purpose:** Subdivision of zones into specific parking lots
- **Key Attributes:**
  - `areaID` (int): Unique identifier
  - `zoneID` (int): Associated zone
  - `slots` (ParkingSlot*): Dynamic array of slots
  - `numSlots`, `capacity`: Tracking array growth

#### **ParkingSlot Representation**
- **Location:** `ParkingSlot.h / ParkingSlot.cpp`
- **Purpose:** Individual parking position
- **Key Attributes:**
  - `slotID` (int): Unique identifier within area
  - `zoneID` (int): Associated zone
  - `isAvailable` (bool): Occupancy status

### 2.2 Hierarchical Structure

```
City
  ├─ Zone 1 (North Zone)
  │   ├─ ParkingArea 1.1
  │   │   ├─ Slot 101
  │   │   ├─ Slot 102
  │   │   └─ Slot 103
  │   └─ ParkingArea 1.2
  │       ├─ Slot 104
  │       └─ Slot 105
  │
  ├─ Zone 2 (South Zone)
  │   └─ ParkingArea 2.1
  │       ├─ Slot 201
  │       └─ Slot 202
  │
  └─ Zone 3 (East Zone)
      └─ ... and so on
```

### 2.3 Data Structure Design Rationale

- **Arrays over STL containers:** Constraint requirement; provides explicit control over memory
- **Fixed-size arrays:** Used where capacity is known (e.g., zone names)
- **Dynamic arrays:** Used where capacity grows (slots, areas, zones)
- **No graph containers:** Zone connections managed implicitly through area lookups

---

## 3. Allocation Strategy

### 3.1 Allocation Algorithm

The system implements a **Two-Phase Allocation Strategy**:

#### **Phase 1: Same-Zone Preference (Primary)**
```
For each ParkingArea in requested zone:
    Find first available slot
    If found:
        Mark slot as occupied
        Set penalty = false
        Return slot ID
```

#### **Phase 2: Cross-Zone Fallback (Secondary)**
```
For each ParkingArea NOT in requested zone:
    Find first available slot
    If found:
        Mark slot as occupied
        Set penalty = true
        Return slot ID

If no slot found anywhere:
    Return -1 (failure)
```

### 3.2 Location: `AllocationEngine.cpp`

**Key Method:** `int allocateSlot(int vehicleID, int preferredZone, int numberOfAreas)`

**Steps:**
1. Iterate through all areas filtering by `preferredZone`
2. Use `findFirstAvailableSlot()` to locate slot
3. If found in preferred zone: occupy and return (no penalty)
4. If not found, iterate through areas in OTHER zones
5. If found in cross-zone: occupy, mark penalty, and return
6. Return -1 if all zones exhausted

### 3.3 Cross-Zone Penalty

- **Tracking:** Boolean flag `penaltyApplied` in AllocationEngine
- **Use Case:** Indicates when a vehicle is parked outside preferred zone
- **Query Method:** `bool hasPenalty() const`
- **Note:** Current implementation tracks penalty but doesn't apply cost/fee. Future enhancement: add penalty fee field.

### 3.4 Time Complexity

| Operation | Complexity | Reasoning |
|-----------|-----------|-----------|
| Allocate Slot | O(n·m) | n = number of areas, m = slots per area |
| Find Available | O(m) | Linear scan of slots in area |
| Occupy/Free Slot | O(m) | Linear search by slot ID |

### 3.5 Optimization Opportunities

1. **Index by Zone ID:** Cache area indices by zone for O(1) lookup
2. **Available Slot Queue:** Maintain queue of free slots per area
3. **Bitmap:** Use bit array for slot availability (reduces memory)

---

## 4. Request Lifecycle State Machine

### 4.1 State Diagram

```
                    ┌────────────────────┐
                    │    REQUESTED       │
                    │  (Initial State)   │
                    └────────┬───────────┘
                             │
                ┌────────────┼────────────┐
                │            │            │
         allocate()    cancel() [valid]   │
                │            │            │
                ▼            ▼            │
          ┌─────────────┐  ┌──────────┐  │
          │  ALLOCATED  │  │CANCELLED◄───┘
          └──────┬──────┘  └──────────┘
                 │
         occupy() [valid transition]
                 │
                 ▼
          ┌──────────────┐
          │   OCCUPIED   │
          └──────┬───────┘
                 │
         release() [valid transition]
                 │
                 ▼
          ┌──────────────┐
          │  RELEASED    │ (Final State)
          │ (Completed)  │
          └──────────────┘
```

### 4.2 State Definitions

| State | Meaning | Valid Transitions |
|-------|---------|-------------------|
| **REQUESTED** | Request created, waiting for allocation | → ALLOCATED, → CANCELLED |
| **ALLOCATED** | Slot assigned to vehicle | → OCCUPIED, → CANCELLED |
| **OCCUPIED** | Vehicle actively parked | → RELEASED |
| **RELEASED** | Vehicle departed, slot now free | (Final - no transitions) |
| **CANCELLED** | Request cancelled before completion | (Final - no transitions) |

### 4.3 Implementation: `ParkingRequest.cpp`

**State Transition Methods:**
```cpp
void allocate()   // REQUESTED → ALLOCATED (validates current state)
void occupy()     // ALLOCATED → OCCUPIED (validates current state)
void release()    // OCCUPIED → RELEASED (validates current state)
void cancel()     // REQUESTED/ALLOCATED → CANCELLED (validates current state)
```

**Validation:**
```cpp
// Example: allocate() implementation
if (currentState == REQUESTED) {
    currentState = ALLOCATED;
} else {
    std::cout << "Error: Can only allocate from REQUESTED state" << std::endl;
}
```

### 4.4 Invalid Transition Prevention

- All state change methods validate preconditions
- Errors logged to `std::cout` when invalid transitions attempted
- State remains unchanged if transition is invalid
- No exceptions thrown (follows C++ error handling pattern)

### 4.5 Time Complexity

| Operation | Complexity |
|-----------|-----------|
| State Transition | O(1) |
| Query State | O(1) |
| Validate Transition | O(1) |

---

## 5. Rollback Design

### 5.1 Rollback Mechanism Overview

The rollback system enables **undo functionality** for parking allocations, restoring the system to previous states.

### 5.2 Data Structure: Allocation Stack

**Location:** `RollbackManager.h / RollbackManager.cpp`

**Stack-based Record:**
```cpp
struct AllocationRecord {
    int areaIndex;      // Which area the slot belongs to
    int slotID;         // The freed slot ID
    int requestID;      // Associated request ID
};
```

**Why Stack?**
- **LIFO (Last In, First Out)** property matches allocation order
- Most recent allocation is most likely to be rolled back first
- Efficient undo/redo patterns
- O(1) push/pop operations

### 5.3 Rollback Operations

#### **5.3.1 Record Allocation**
```cpp
void recordAllocation(int areaIndex, int slotID, int requestID)
```
- Called by `ParkingSystem::allocateForRequest()` after successful allocation
- Pushes allocation record onto stack
- Time Complexity: O(1)

**Example Flow:**
```
1. Vehicle requests parking
2. AllocationEngine allocates slot 105 in area 1
3. RollbackManager records: {areaIndex: 0, slotID: 105, requestID: req1}
4. Stack: [record1] ← top
```

#### **5.3.2 Undo Last Allocation**
```cpp
void undoLast(ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests)
```

**Steps:**
1. Pop allocation record from stack
2. Free the slot (set `isAvailable = true`)
3. Find associated request and cancel it
4. Time Complexity: O(n) - n = numRequests (linear search for request)

**Example:**
```
Before rollback:
  Slot 105: occupied (unavailable)
  Request 1: state = ALLOCATED
  Stack: [record1] ← top

After rollback:
  Slot 105: available
  Request 1: state = CANCELLED
  Stack: [] (empty)
```

#### **5.3.3 Undo Last K Allocations**
```cpp
void undoLastK(int k, ParkingArea* areas, int numAreas, ParkingRequest* requests, int numRequests)
```

- Calls `undoLast()` k times in succession
- Stops if stack becomes empty
- Time Complexity: O(k·n)

**Use Case:** Batch cancel multiple recent requests

### 5.4 System Integration

**In ParkingSystem:**
```cpp
void rollbackLast()           // Wrapper: calls undoLastK(1, ...)
void rollbackLastK(int k)     // Wrapper: calls undoLastK(k, ...)
```

### 5.5 Limitations & Future Enhancements

| Limitation | Impact | Solution |
|-----------|--------|----------|
| Stack capacity fixed | Cannot exceed maxOperations | Dynamic resizing array |
| No forward (redo) | Cannot redo after rollback | Implement redo stack |
| Linear request search | O(n) lookup | Hash map or request index array |
| No timestamp tracking | Cannot roll back to specific time | Add timestamps to records |

---

## 6. Time and Space Complexity Analysis

### 6.1 Time Complexity by Operation

| Operation | Best Case | Average Case | Worst Case | Notes |
|-----------|-----------|--------------|-----------|-------|
| Add Zone | O(1) | O(1) | O(1) | Array append |
| Add Vehicle | O(1) | O(1) | O(1) | Array append |
| Add Request | O(1) | O(1) | O(1) | Array append |
| Add Area | O(1) | O(1) | O(1) | Array append |
| **Allocate Slot** | O(m) | O(n·m) | O(n·m) | n=areas, m=slots/area |
| Find Available Slot | O(1) | O(m/2) | O(m) | Linear scan |
| Occupy/Free Slot | O(1) | O(m/2) | O(m) | By ID lookup |
| **Rollback Last** | O(1) | O(n) | O(n) | Request lookup |
| **Rollback Last K** | O(k) | O(k·n) | O(k·n) | k rollbacks |
| Get Total Slots | O(a) | O(a) | O(a) | a=num areas |
| Get Available Slots | O(a·m) | O(a·m) | O(a·m) | All slots scan |
| Get Active Requests | O(r) | O(r) | O(r) | r=num requests |
| Get Zone Utilization | O(a·m) | O(a·m) | O(a·m) | Filter + scan |
| Get Cancelled Requests | O(r) | O(r) | O(r) | All requests scan |
| Get Completed Requests | O(r) | O(r) | O(r) | All requests scan |

### 6.2 Space Complexity by Component

| Component | Space Used | Formula | Notes |
|-----------|-----------|---------|-------|
| Zones | O(z) | z = max zones | Array of zone objects |
| Vehicles | O(v) | v = max vehicles | Array of vehicle objects |
| Requests | O(r) | r = max requests | Array of request objects |
| Areas | O(a) | a = max areas | Array of area objects |
| Slots | O(a·m) | m = slots per area | Nested arrays in areas |
| Rollback Stack | O(k) | k = max operations | Allocation record array |
| **Total** | **O(z + v + r + a·m + k)** | | Linear in capacities |

### 6.3 Detailed Component Analysis

#### **Zone Management**
```
Space: O(z)
Where z = maxZones

Structure:
  Zone[maxZones]
    ├─ zoneID (int)
    ├─ zoneName (char[50])
    └─ Each Zone contains ParkingArea[]
```

#### **Slot Management**
```
Space: O(a·m)
Where a = number of areas, m = slots per area

Structure:
  ParkingArea[a]
    └─ ParkingSlot[m] per area
      ├─ slotID (int)
      ├─ zoneID (int)
      └─ isAvailable (bool)

Total slots = a × m
```

#### **Request Lifecycle Tracking**
```
Space: O(r)
Where r = maxRequests

Structure:
  ParkingRequest[r]
    ├─ requestID (int)
    ├─ vehicleID (int)
    ├─ requestedZone (int)
    ├─ requestTime (int)
    └─ currentState (enum)
```

#### **Rollback Stack**
```
Space: O(k)
Where k = maxOperations

Structure:
  AllocationRecord[k]
    ├─ areaIndex (int)
    ├─ slotID (int)
    └─ requestID (int)

Per record: 3 integers = 12 bytes (typical)
Total: 12k bytes
```

### 6.4 Overall System Space

**Example Configuration:**
```
ParkingSystem(
    maxZones = 5
    maxVehicles = 100
    maxRequests = 200
    maxAreas = 20
    maxOperations = 500
)
```

**Detailed Breakdown:**
```
Zones:      5 × (4 + 50 + 8 + 4 + 8) = 370 bytes
Vehicles:   100 × (4 + 4) = 800 bytes
Requests:   200 × (4 + 4 + 4 + 4 + 4) = 4,000 bytes
Areas:      20 × (4 + 4 + 8 + 4 + 4) = 480 bytes
Slots:      20 × 5 × (4 + 4 + 1) = 1,800 bytes (assuming 5 slots/area)
Stack:      500 × (4 + 4 + 4) = 6,000 bytes
─────────────────────────────────────
Total:      ≈ 13.5 KB
```

### 6.5 Scalability Analysis

| Scenario | Bottleneck | Impact |
|----------|-----------|--------|
| 1000 requests | Request lookup in rollback | O(1000) per rollback |
| 100 zones | Zone iteration in allocation | O(100) per allocation |
| 10,000 slots/area | Slot search within area | O(10,000) per allocation |
| 1000 rollback operations | Stack size limits | Fixed capacity = limit |

### 6.6 Performance Optimization Opportunities

1. **Allocation Speedup:**
   - Index areas by zone: O(n·m) → O(m)
   - Use bitmap for slots: Reduce memory, faster scanning

2. **Rollback Speedup:**
   - Hash map for request lookup: O(n) → O(1)
   - Result: `undoLast()` becomes O(1) instead of O(n)

3. **Analytics Speedup:**
   - Cache zone utilization
   - Maintain running counters for active/cancelled requests
   - Result: O(a·m) → O(1) queries

---

## 7. Error Handling & Edge Cases

### 7.1 State Transition Errors
- **Invalid Transition Attempt:** Logged to `std::cout`, state unchanged
- **Example:** Trying to occupy a request already released

### 7.2 Allocation Failures
- **No Available Slots:** Returns -1
- **Request Index Out of Bounds:** Returns -1
- **Area Index Invalid:** Silent failure (no return)

### 7.3 Rollback Edge Cases
- **Empty Stack:** `isEmpty()` prevents over-unrolling
- **Insufficient Records:** `undoLastK(k)` stops at stack empty
- **Request Not Found:** Silently continues (potential bug)

---

## 8. Key Design Decisions

| Decision | Rationale | Alternative |
|----------|-----------|-------------|
| Array-based storage | No STL containers allowed | Linked lists |
| Stack for rollback | LIFO matches undo semantics | Queue (would need reversal) |
| Fixed-size arrays | Known capacities at creation | Dynamic vectors |
| Linear search allocation | Simple, correct, matches spec | Hash tables (not allowed) |
| No adjacency graph | Zones managed implicitly | Explicit adjacency matrix |
| Enum for states | Type-safe state management | Integer constants |
| Penalty flag (boolean) | Simple tracking mechanism | Penalty fee (integer value) |

---

## 9. Constraints Satisfied

✅ **No STL Graph/Map containers** - Uses arrays only
✅ **No External APIs** - Pure in-memory system
✅ **No Global Variables** - All state encapsulated in classes
✅ **Multi-file Implementation** - 8 header/implementation files
✅ **Header-only Declarations** - `.h` files contain only declarations
✅ **C++ Object-Oriented Design** - Classes with encapsulation

---

## 10. Future Enhancements

1. **Zone Adjacency:** Implement adjacency list for multi-zone allocation strategies
2. **Peak Usage Analytics:** Track peak hours and zones
3. **Duration Tracking:** Record entry/exit times for accurate duration calculations
4. **Penalty Fee System:** Add monetary penalty for cross-zone allocation
5. **Concurrency:** Add mutex locks for thread-safe operations
6. **Persistent Storage:** Serialize/deserialize state to disk
7. **Redo Stack:** Enable redo operations after rollback
8. **Advanced Allocation:** Use heuristics (distance-based, cost-based)

---

## Document Metadata

- **Version:** 1.0
- **Last Updated:** January 24, 2026
- **Team:** 2 students (DSA Semester Project)
- **Language:** C++11 and above
- **Compiler:** Standard C++ compiler (g++, clang++, MSVC)
