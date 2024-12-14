#include "../include/Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(), numOfOrders(0){};

Customer::Customer(const Customer &other) : id(other.id), name(other.getName()), locationDistance(other.getCustomerDistance()), maxOrders(other.getMaxOrders()), ordersId(), numOfOrders(other.numOfOrders)
{
    //changed from auto to int, no need to check for type
    for (int orderId : other.ordersId)
    {
        ordersId.push_back(orderId);
    }
}

Customer::~Customer() {}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

// Returns maxOrders
int Customer::getMaxOrders() const
{
    return maxOrders;
}

// Returns num of orders the customer has made so far
int Customer::getNumOrders() const
{
    return numOfOrders;
}

// Returns true if the customer didn't reach max orders
bool Customer::canMakeOrder() const
{
    return (numOfOrders < maxOrders);
}

const vector<int> &Customer::getOrdersIds() const
{
    return ordersId;
}

int Customer::addOrder(int orderId)
{
    if (canMakeOrder())
    {
        ordersId.push_back(orderId);
        numOfOrders++;
        return orderId;
    }
    else
    {
        return -1;
    }
}

// vector<int> &Customer::getOrders() // ADDED
// {
//     return ordersId;
// }

int Customer::numOfOrdersLeft() // ADDED
{
    return (maxOrders - numOfOrders);
}

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer::SoldierCustomer(const SoldierCustomer &other)
    : Customer(other) {}

SoldierCustomer::~SoldierCustomer() {}

SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer::CivilianCustomer(const CivilianCustomer &other)
    : Customer(other) {}

CivilianCustomer::~CivilianCustomer() {}

CivilianCustomer *CivilianCustomer::clone() const
{
    return new CivilianCustomer(*this);
}
