# Food Warehouse Management System

A C++ implementation of a warehouse management system that handles food package orders, volunteers, and customers during wartime. The system manages order processing, volunteer assignments, and delivery tracking using object-oriented design principles.

## Project Overview

### Features
- Customer and order management
- Volunteer task assignment and tracking
- Real-time order status monitoring
- Multi-threaded volunteer processing
- Backup and restore functionality
- Comprehensive action logging

## Getting Started

### Prerequisites
- GCC/G++ compiler with C++11 support
- Make build system
- Valgrind (for memory leak testing)
- UNIX-based system (for testing)

### Building the Project
```bash
# Clone the repository
git clone https://github.com/nissimbrami/warehouse-management.git
cd warehouse-management

# Build the project
make
```

### Running Tests
```bash
# Run with example input
./bin/warehouse ExampleInput.txt

# Test for memory leaks
valgrind --leak-check=full --show-reachable=yes ./bin/warehouse ExampleInput.txt
```

## Project Structure

```
├── src/             # Source files (.cpp)
├── include/         # Header files (.h)
├── bin/            # Binary output directory
└── makefile        # Build configuration
```

## Core Components

### Classes
1. **Warehouse**
   - Manages orders, volunteers, and customers
   - Handles order queues: pending, in-process, completed
   - Maintains unique IDs for customers and volunteers

2. **Customer**
   - Types: Soldier, Civilian
   - Properties: ID, distance, max orders limit

3. **Volunteer**
   - Abstract base class for volunteer types
   - Types:
     - Collector (Regular/Limited)
     - Driver (Regular/Limited)
   - Properties: coolDown, maxDistance, maxOrders

4. **Order**
   - States: PENDING, COLLECTING, DELIVERING, COMPLETED
   - Tracks associated volunteers and customer

5. **Action**
   - Base class for all system actions
   - Supports logging and error handling

### Key Features

#### Order Processing Flow
1. Customer places order (PENDING)
2. Collector processes order (COLLECTING)
3. Driver delivers order (DELIVERING)
4. Delivery completion (COMPLETED)

#### Command Interface
```bash
# Add customer
customer <name> <type> <distance> <max_orders>

# Add order
order <customer_id>

# Process steps
step <number_of_steps>

# View status
orderStatus <order_id>
customerStatus <customer_id>
volunteerStatus <volunteer_id>
```

## Implementation Details

### Memory Management
- Implements Rule of 5 for classes with resources
- Proper memory cleanup on program termination
- Valgrind-verified memory leak prevention

### Compilation Flags
```bash
-g -Wall -Weffc++ -std=c++11 -Iinclude
```

### Error Handling
- Comprehensive error checking for all operations
- Detailed error messages for invalid operations
- Status tracking for all actions

## Program Flow

1. **Initialization**
   - Load configuration from file
   - Initialize warehouse
   - Start volunteer processing

2. **Operation**
   - Accept user commands
   - Process orders through stages
   - Update status and perform logging

3. **Termination**
   - Cleanup resources
   - Print final status
   - Exit gracefully

## Author

Nissim Brami

## License

This project is licensed under the MIT License - see the LICENSE file for details.
