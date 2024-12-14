#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"
#include "../include/Parser.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
using namespace std;

Order errorOrder(-1, -1, -1);
SoldierCustomer errorCustomer(-1, "", -1, -1);
DriverVolunteer errorVolunteer(-1, "", -1, -1);

WareHouse::WareHouse(const string &configFilePath)
    : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    Parser::parseConfigurationFile(configFilePath, *this);
}

WareHouse::WareHouse(const WareHouse &other)
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    for (auto &customer : other.customers)
    {
        if (customer)
        {
            this->customers.push_back(customer->clone());
        }
    }

    for (auto &volunteer : other.volunteers)
    {
        if (volunteer)
        {
            this->volunteers.push_back(volunteer->clone());
        }
    }

    for (auto &order : other.pendingOrders)
    {
        if (order)
        {
            this->pendingOrders.push_back(order->clone());
        }
    }

    for (auto &order : other.inProcessOrders)
    {
        if (order)
        {
            this->inProcessOrders.push_back(order->clone());
        }
    }

    for (auto &order : other.completedOrders)
    {
        if (order)
        {
            this->completedOrders.push_back(order->clone());
        }
    }

    for (auto &action : other.actionsLog)
    {
        if (action)
        {
            this->actionsLog.push_back(action->clone());
        }
    }
}

WareHouse::WareHouse(WareHouse &&other) noexcept
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{
    for (auto &customer : other.customers)
    {
        if (customer)
        {
            this->customers.push_back(customer->clone());
        }
    }

    for (auto &volunteer : other.volunteers)
    {
        if (volunteer)
        {
            this->volunteers.push_back(volunteer->clone());
        }
    }

    for (auto &order : other.pendingOrders)
    {
        if (order)
        {
            this->pendingOrders.push_back(order->clone());
        }
    }

    for (auto &order : other.inProcessOrders)
    {
        if (order)
        {
            this->inProcessOrders.push_back(order->clone());
        }
    }

    for (auto &order : other.completedOrders)
    {
        if (order)
        {
            this->completedOrders.push_back(order->clone());
        }
    }

    for (auto &action : other.actionsLog)
    {
        if (action)
        {
            this->actionsLog.push_back(action->clone());
        }
    }

    //other.~WareHouse();
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {

        for (auto *customer : customers)
        {
            delete customer;
        }
        customers.clear();

        for (auto *volunteer : volunteers)
        {
            delete volunteer;
        }
        volunteers.clear();

        for (auto *order : pendingOrders)
        {
            delete order;
        }
        pendingOrders.clear();

        for (auto *order : inProcessOrders)
        {
            delete order;
        }
        inProcessOrders.clear();

        for (auto *order : completedOrders)
        {
            delete order;
        }
        completedOrders.clear();

        for (auto *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();

        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        for (auto &customer : other.customers)
        {
            if (customer)
            {
                this->customers.push_back(customer->clone());
            }
        }

        for (auto &volunteer : other.volunteers)
        {
            if (volunteer)
            {
                this->volunteers.push_back(volunteer->clone());
            }
        }

        for (auto &order : other.pendingOrders)
        {
            if (order)
            {
                this->pendingOrders.push_back(order->clone());
            }
        }

        for (auto &order : other.inProcessOrders)
        {
            if (order)
            {
                this->inProcessOrders.push_back(order->clone());
            }
        }

        for (auto &order : other.completedOrders)
        {
            if (order)
            {
                this->completedOrders.push_back(order->clone());
            }
        }

        for (auto &action : other.actionsLog)
        {
            if (action)
            {
                this->actionsLog.push_back(action->clone());
            }
        }
    }
    return *this;
}

WareHouse::~WareHouse()
{
    for (auto *customer : customers)
    {
        delete customer;
    }
    customers.clear();

    for (auto *volunteer : volunteers)
    {
        delete volunteer;
    }
    volunteers.clear();

    //pendingOrders.shrink_to_fit();

    for (auto *order : pendingOrders)
    {
        //std::cout << "order deleted: " << order->getId();
        delete order;
    }
    pendingOrders.clear();

    //inProcessOrders.shrink_to_fit();

    for (auto *order : inProcessOrders)
    {
        //std::cout << "order deleted: " << order->getId();
        delete order;
    }
    inProcessOrders.clear();

    //completedOrders.shrink_to_fit();

    for (auto *order : completedOrders)
    {
        //std::cout << "order deleted: " << order->getId();
        delete order;
    }
    completedOrders.clear();

    for (auto *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();
}

void WareHouse::start()
{
    open();
    cout << "Warehouse is open!" << endl;
    string input;
    while (isOpen)
    {
        cout << "Enter command: ";
        getline(cin, input);
        if (!input.empty())
        {
            std::istringstream iss(input);
            string action;
            iss >> action;

            if (action == "order")
            {
                int customerId;
                iss >> customerId;
                AddOrder *addOrder = new AddOrder(customerId);
                addOrder->act(*this);
            }
            else if (action == "orderStatus")
            {
                int orderId;
                iss >> orderId;
                PrintOrderStatus *printOrderStatus = new PrintOrderStatus(orderId);
                printOrderStatus->act(*this);
            }
            else if (action == "customerStatus")
            {
                int customerId;
                iss >> customerId;
                PrintCustomerStatus *printCustomerStatus = new PrintCustomerStatus(customerId);
                printCustomerStatus->act(*this);
            }
            else if (action == "volunteerStatus")
            {
                int volunteerId;
                iss >> volunteerId;
                PrintVolunteerStatus *printVolunteerStatus = new PrintVolunteerStatus(volunteerId);
                printVolunteerStatus->act(*this);
            }
            else if (action == "customer")
            {
                string name, customerType;
                int locationDistance, maxOrders;
                iss >> name >> customerType >> locationDistance >> maxOrders;
                AddCustomer *addCustomer = new AddCustomer(name, customerType, locationDistance, maxOrders);
                addCustomer->act(*this);
            }
            else if (action == "step")
            {
                int numOfSteps;
                iss >> numOfSteps;
                SimulateStep *simulateStep = new SimulateStep(numOfSteps);
                simulateStep->act(*this);
            }
            else if (action == "close")
            {
                Close *close = new Close();
                close->act(*this);
            }
            else if (action == "log")
            {
                PrintActionsLog *printActionsLog = new PrintActionsLog();
                printActionsLog->act(*this);
            }
            else if (action == "restore")
            {
                RestoreWareHouse *restoreWareHouse = new RestoreWareHouse();
                restoreWareHouse->act(*this);
            }
            else if (action == "backup")
            {
                BackupWareHouse *backupWareHouse = new BackupWareHouse();
                backupWareHouse->act(*this);
            }
        }
        else
        {
            cout << "Invalid input" << endl;
        }
    }
}

void WareHouse::addOrder(Order *order)
{
    // added check for error order
    if (order->getId() != -1)
    {
        pendingOrders.push_back(order);
        orderCounter++;
    }
}

int WareHouse::getOrderCounter()
{
    return orderCounter;
}

void WareHouse::addPendingOrder(Order *order)
{
    if (order->getId() != -1 && order->getCustomerId() != -1)
    {
        pendingOrders.push_back(order);
        // orderCounter++;
    }
}

void WareHouse::addInProcessOrder(Order *order)
{
    if (order->getId() != -1 && order->getCustomerId() != -1)
    {
        inProcessOrders.push_back(order);
        // orderCounter++;
    }
}

void WareHouse::addCompletedOrder(Order *order)
{
    if (order->getId() != -1 && order->getCustomerId() != -1)
    {
        completedOrders.push_back(order);
        // orderCounter++;
    }
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::addCustomer(Customer *customer)
{
    // added check for error customer
    if (customer->getId() != -1)
    {
        customers.push_back(customer);
        customerCounter++;
    }
}

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    // added check for error volunteer
    if (volunteer->getId() != -1)
    {
        volunteers.push_back(volunteer);
        volunteerCounter++;
    }
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (auto &customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    return errorCustomer;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    return errorVolunteer;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (auto &order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (auto &order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (auto &order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    return errorOrder;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}
vector<Order *> &WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}
vector<Order *> &WareHouse::getCompletedOrders()
{
    return completedOrders;
}
vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
}
vector<Customer *> &WareHouse::getCustomers()
{
    return customers;
}
vector<BaseAction *> &WareHouse::getActionsLog()
{
    return actionsLog;
}

// changed implementation to recive a pointer, no need to copy data
void WareHouse::removeOrder(vector<Order *> *orders, Order *order)
{
    auto it = std::find((*orders).begin(), (*orders).end(), order);
    if (it != (*orders).end())
    {
        (*orders).erase(it);
        // orderCounter--;
    }
}

void WareHouse::removePendingOrder(Order *order)
{
    auto it = std::find(pendingOrders.begin(), pendingOrders.end(), order);
    if (it != pendingOrders.end())
    {
        pendingOrders.erase(it);
        // orderCounter--;
    }
}

void WareHouse::removeInProcessOrder(Order *order)
{
    auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), order);
    if (it != inProcessOrders.end())
    {
        inProcessOrders.erase(it);
        // orderCounter--;
    }
}

void WareHouse::removeVolunteer(Volunteer *volunteer)
{
    auto it = std::find(volunteers.begin(), volunteers.end(), volunteer);
    if (it != volunteers.end())
    {
        if (!volunteer->isBusy())
        {
            //volunteerCounter--;
            volunteers.erase(it);
            delete volunteer;

        }
    }
}
int WareHouse::getCustomerCounter()
{
    return customerCounter;
}
int WareHouse::getVolunteerCounter()
{
    return volunteerCounter;
}

Volunteer *WareHouse::getAveilableCollectorVolunteer(Order *order)
{
    for (Volunteer *volunteer : volunteers)
    {
        if (volunteer->getType() == VolunteerType::COLLECTORVOLUNTEER)
        {
            dynamic_cast<CollectorVolunteer *>(volunteer);
            if (volunteer->canTakeOrder(*order))
            {
                return volunteer;
            }
        }
        else if (volunteer->getType() == VolunteerType::LIMITEDCOLLECTORVOLUNTEER)
        {
            dynamic_cast<LimitedCollectorVolunteer *>(volunteer);
            if (volunteer->canTakeOrder(*order))
            {
                return volunteer;
            }
        }
    }

    return nullptr;
}
Volunteer *WareHouse::getAveilableDriverVolunteer(Order *order)
{
    for (Volunteer *volunteer : volunteers)
    {
        if (volunteer->getType() == VolunteerType::DRIVERVOLUNTEER)
        {
            dynamic_cast<DriverVolunteer *>(volunteer);
            if (volunteer->canTakeOrder(*order))
            {
                return volunteer;
            }
        }
        else if (volunteer->getType() == VolunteerType::LIMITEDDRIVERVOLUNTEER)
        {
            dynamic_cast<LimitedDriverVolunteer *>(volunteer);
            if (volunteer->canTakeOrder(*order))
            {
                return volunteer;
            }
        }
    }

    return nullptr;
}
