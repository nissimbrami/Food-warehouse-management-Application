#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse
{

public:
    WareHouse(const string &configFilePath);
    WareHouse(const WareHouse &other);
    WareHouse(WareHouse &&other) noexcept;
    WareHouse &operator=(const WareHouse &other);
    ~WareHouse();
    void start();
    void addOrder(Order *order);
    void addAction(BaseAction *action);
    Customer &getCustomer(int customerId) const;
    Volunteer &getVolunteer(int volunteerId) const;
    Order &getOrder(int orderId) const;
    const vector<BaseAction *> &getActions() const;
    void close();
    void open();

    /*
        added functions, used in action class
    */

    Volunteer *getAveilableCollectorVolunteer(Order *order);
    Volunteer *getAveilableDriverVolunteer(Order *order);

    int getOrderCounter();

    void addPendingOrder(Order *order);
    void addInProcessOrder(Order *order);
    void addCompletedOrder(Order *order);

    void addCustomer(Customer *customer);
    void addVolunteer(Volunteer *volunteer);
    vector<Order *> &getPendingOrders();
    vector<Order *> &getInProcessOrders();
    vector<Order *> &getCompletedOrders();
    vector<Volunteer *> &getVolunteers();
    vector<Customer *> &getCustomers();
    vector<BaseAction *> &getActionsLog();


    // changed function signature, no need to pass in vector
    void removeOrder(vector<Order *> *orders, Order *order);

    void removePendingOrder(Order *order);
    void removeInProcessOrder(Order *order);

    void removeVolunteer(Volunteer *volunteer);

    // no need for this function as we do not delete customers
    //  void removeCustomer(vector<Customer *> &customers, Customer *customer);

    int getCustomerCounter();
    int getVolunteerCounter();

private:
    bool isOpen;
    vector<BaseAction *> actionsLog;
    vector<Volunteer *> volunteers;
    vector<Order *> pendingOrders;
    vector<Order *> inProcessOrders;
    vector<Order *> completedOrders;
    vector<Customer *> customers;
    int customerCounter;  // For assigning unique customer IDs
    int volunteerCounter; // For assigning unique volunteer IDs
    int orderCounter;     // unique order Id
};