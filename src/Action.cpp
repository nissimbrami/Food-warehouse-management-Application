#include "../include/Action.h"
#include <iostream>
using namespace std;
extern WareHouse *backup;

BaseAction::BaseAction() : errorMsg(), status() {}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error:" + errorMsg;
}

string BaseAction::statusToString(ActionStatus status) const
{
    if (status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else
    {
        return "ERROR";
    }
}
string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

////////////////////////////////////////////////////////////////////////////////////////

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

SimulateStep::SimulateStep(const SimulateStep &other) : BaseAction(), numOfSteps(other.numOfSteps) {} // New Copy Constructor

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        // changed implementation to avoid coping the vector
        for (Order *order : wareHouse.getPendingOrders()) // STEP 1 - hand the orders for the next operation
        {
            if (order->getStatus() == OrderStatus::PENDING)
            {
                Volunteer *collectorVol = wareHouse.getAveilableCollectorVolunteer(order);
                if (collectorVol != nullptr)
                {
                    allocateOrderToVolunteer(wareHouse, collectorVol, order, collectorVol->getType());
                }
            }
            else if (order->getStatus() == OrderStatus::COLLECTING)
            {
                Volunteer *driverVol = wareHouse.getAveilableDriverVolunteer(order);
                if (driverVol != nullptr)
                {
                    allocateOrderToVolunteer(wareHouse, driverVol, order, driverVol->getType());
                }
            }
        }
        // we get a copy of the vector in order to not mess up the loop

        vector<Order*> ordersToMove;

        for (Order *order : wareHouse.getPendingOrders())
        {
            if ((order->getStatus() == OrderStatus::COLLECTING && order->getCollectorId() != NO_VOLUNTEER) || (order->getStatus() == OrderStatus::DELIVERING && order->getDriverId() != NO_VOLUNTEER))
            {
                ordersToMove.push_back(order);
                //wareHouse.removePendingOrder(order);
                //wareHouse.addInProcessOrder(order);
            }
        }

        for (Order *order : ordersToMove)
        {
            wareHouse.removePendingOrder(order);
            wareHouse.addInProcessOrder(order);
        }

        // no need to copy volunteer vector, use pointer
        for (Volunteer *volunteer : wareHouse.getVolunteers()) // STEP 2 - make all the volunteers step
        {
            volunteer->step();
        }
        // no need to copy volunteer vector, use pointer
        for (Volunteer *volunteer : wareHouse.getVolunteers()) // STEP 3 - move from inProcess to pending/completed
        {
            moveOrders(wareHouse, *volunteer, wareHouse.getOrder(volunteer->getCompletedOrderId()));
        }

        removeLimitedVolunteers(wareHouse);
    }
    complete();
    wareHouse.addAction(this);
}

void SimulateStep::allocateOrderToVolunteer(WareHouse &wareHouse, Volunteer *volunteer, Order *order, VolunteerType volunteerType)
{
    if (volunteer->canTakeOrder(*order))
    {
        if ((wareHouse.getVolunteer(order->getCollectorId()).getActiveOrderId() == NO_ORDER) &&
            (volunteerType == VolunteerType::COLLECTORVOLUNTEER ||
             volunteerType == VolunteerType::LIMITEDCOLLECTORVOLUNTEER))
        {
            dynamic_cast<CollectorVolunteer *>(volunteer)->acceptOrder(*order);
            order->setStatus(OrderStatus::COLLECTING);
            order->setCollectorId(volunteer->getId());
        }
        else if (wareHouse.getVolunteer(order->getDriverId()).getActiveOrderId() == NO_ORDER &&
                 ((volunteerType == VolunteerType::DRIVERVOLUNTEER ||
                   volunteerType == VolunteerType::LIMITEDDRIVERVOLUNTEER)))
        {
            dynamic_cast<DriverVolunteer *>(volunteer)->acceptOrder(*order);
            order->setStatus(OrderStatus::DELIVERING);
            order->setDriverId(volunteer->getId());
        }
    }
}

void SimulateStep::moveOrders(WareHouse &wareHouse, Volunteer &volunteer, Order &order)
{

    if (volunteer.getActiveOrderId() == NO_ORDER && volunteer.getCompletedOrderId() == order.getId())
    {
        if (wareHouse.getVolunteer(order.getCollectorId()).getActiveOrderId() == NO_ORDER && order.getStatus() == OrderStatus::COLLECTING)
        {
            wareHouse.addPendingOrder(&order);
            wareHouse.removeInProcessOrder(&order);
        }
        else if (wareHouse.getVolunteer(order.getDriverId()).getActiveOrderId() == NO_ORDER && order.getStatus() == OrderStatus::DELIVERING)
        {
            order.setStatus(OrderStatus::COMPLETED);
            wareHouse.addCompletedOrder(&order);
            wareHouse.removeInProcessOrder(&order);
        }
    }
}

void SimulateStep::removeLimitedVolunteers(WareHouse &wareHouse)
{
    vector<Volunteer* > volunteersToDelete;

    for (auto *volunteer : wareHouse.getVolunteers()) // STEP 3 - move from inProcess to pending/completed
    {
        if (!(volunteer->hasOrdersLeft()) && !volunteer->isBusy()) // deletes the limited volunteers
        {
            if (volunteer->getType() == VolunteerType::LIMITEDCOLLECTORVOLUNTEER)
            {
                if ((dynamic_cast<LimitedCollectorVolunteer *>(volunteer))->getTimeLeft() == 0)
                {
                    volunteersToDelete.push_back(volunteer);
                }
            }
            else if (volunteer->getType() == VolunteerType::LIMITEDDRIVERVOLUNTEER)
            {
                if ((dynamic_cast<LimitedDriverVolunteer *>(volunteer))->getDistanceLeft() == 0)
                {
                    volunteersToDelete.push_back(volunteer);
                }
            }
        }
    }

    for (auto *volunteer : volunteersToDelete)
    {
        wareHouse.removeVolunteer(volunteer);
    }


}



std::string SimulateStep::toString() const
{
    return "simulateStep " + std::to_string(numOfSteps);
}
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

AddOrder::AddOrder(int id) : customerId(id) {}
void AddOrder::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId); // creating refernce to the customer
    if ((customer.getId() == -1) || !(customer.canMakeOrder()))
    {
        error(" Cannot place this order \n");
    }
    else
    {
        int currentOrderId = wareHouse.getOrderCounter();
        Order *order = new Order(currentOrderId, customerId, customer.getCustomerDistance()); // creating new pointer to new order
        order->setStatus(OrderStatus::PENDING);
        customer.addOrder(currentOrderId);
        wareHouse.addOrder(order);
        //delete order;
        complete();
    }
    wareHouse.addAction(this);
};

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId);
}
AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName), customerType((customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian)), distance(distance), maxOrders(maxOrders) {}
AddCustomer::AddCustomer(const AddCustomer &other) : BaseAction(), customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders) {}
void AddCustomer::act(WareHouse &wareHouse)
{
    if (customerType == CustomerType::Soldier)
    {
        SoldierCustomer *customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
    else if (customerType == CustomerType::Civilian)
    {
        CivilianCustomer *customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
    }
    complete();
    wareHouse.addAction(this);
}
AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}
string AddCustomer::toString() const
{
    string type;
    if (customerType == CustomerType::Soldier)
    {
        type = "Soldier";
    }
    else
    {
        type = "Civilian";
    }
    return "customer " + customerName + " " + type + " " + std::to_string(distance) + " " + std::to_string(maxOrders);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){};
void PrintOrderStatus::act(WareHouse &wareHouse)
{
    Order order = wareHouse.getOrder(orderId);
    if (order.getId() == -1)
    {
        // wareHouse.removeOrder(wareHouse.getPendingOrdersPointer(), &order);
        // wareHouse.removeOrder(wareHouse.getInProcessOrdersPointer(), &order);
        // wareHouse.removeOrder(wareHouse.getCompletedOrdersPointer(), &order);
        error(" Order doesnt exist\n");
    }
    else
    {
        std::cout << "OrderId: " << orderId << std::endl;
        std::cout << "OrderStatus: " << order.toString() << std::endl;
        std::cout << "CustomerID: " << order.getCustomerId() << std::endl;
        if (order.getCollectorId() == NO_VOLUNTEER)
        {
            std::cout << "Collector: None" << std::endl;
        }
        else
        {
            std::cout << "Collector: " << order.getCollectorId() << std::endl;
        }
        if (order.getDriverId() == NO_VOLUNTEER)
        {
            std::cout << "Driver: None" << std::endl;
        }
        else
        {
            std::cout << "Driver: " << order.getDriverId() << std::endl;
        }
        complete();
    }
    wareHouse.addAction(this);
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){};
void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1)
    {
        // dont remove customers
        // wareHouse.removeCustomer(wareHouse.getCustomers(), &customer);
        error(" Customer doesnt exist \n");
    }
    else
    {
        std::cout << "CustomerID: " << customer.getId() << std::endl;
        for (int order : customer.getOrdersIds())
        {
            std::cout << "OrderID: " << order << std::endl;
            std::cout << "OrderStatus: " << wareHouse.getOrder(order).toString() << std::endl;
        }
        std::cout << "numOrdersLeft: " << customer.numOfOrdersLeft() << std::endl;
        complete();
    }
    wareHouse.addAction(this);
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id){};
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    Volunteer &vol = wareHouse.getVolunteer(volunteerId);
    if (vol.getId() == -1)
    {
        error(" Volunteer doesnt exist\n");
    }
    else
    {
        std::cout << "VolunteerID: " << vol.getId() << std::endl;
        std::cout << "isBusy: " << (vol.isBusy() == 1 ? "True" : "False") << std::endl;
        if (vol.isBusy())
        {
            std::cout << "OrderID: " << vol.getActiveOrderId() << std::endl;
        }
        else
        {
            std::cout << "OrderID: None" << std::endl;
        }
        if (auto *driver = dynamic_cast<DriverVolunteer *>(&vol))
        {
            std::cout << "DistanceLeft: " << driver->getDistanceLeft() << std::endl;
        }
        else if (auto *collector = dynamic_cast<CollectorVolunteer *>(&vol))
        {
            std::cout << "TimeLeft: " << collector->getTimeLeft() << std::endl;
        }
        else
        {
            std::cout << "TimeLeft: None" << std::endl;
        }
        if (auto *limitedDriver = dynamic_cast<LimitedDriverVolunteer *>(&vol))
        {
            std::cout << "OrdersLeft: " << limitedDriver->getNumOrdersLeft() << std::endl;
        }
        else if (auto *limitedCollector = dynamic_cast<LimitedCollectorVolunteer *>(&vol))
        {
            std::cout << "OrdersLeft: " << limitedCollector->getNumOrdersLeft() << std::endl;
        }
        else
        {
            std::cout << "OrdersLeft: No Limit" << std::endl;
        }
        complete();
    }
    wareHouse.addAction(this);
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PrintActionsLog::PrintActionsLog() : BaseAction(){};
void PrintActionsLog::act(WareHouse &wareHouse)
{

    //std::cout << "---------------------------------------------------------------------\n";
    for (auto action : wareHouse.getActionsLog())
    {
        std::cout << action->toString() << " " << BaseAction::statusToString(action->getStatus()) << std::endl;
    }
    //std::cout << "---------------------------------------------------------------------\n";
    complete();
    wareHouse.addAction(this);
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const
{
    return "log";
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Close::Close() : BaseAction(){};
void Close::act(WareHouse &wareHouse)
{
    for (Order *order : wareHouse.getPendingOrders())
    {
        std::cout << "OrderID: " << order->getId() << ", "
                  << "CustomerID: " << order->getCustomerId() << ", "
                  << "OrderStatus: " << order->toString() << std::endl;
    }
    for (Order *order : wareHouse.getInProcessOrders())
    {
        std::cout << "OrderID: " << order->getId() << ", "
                  << "CustomerID: " << order->getCustomerId() << ", "
                  << "OrderStatus: " << order->toString() << std::endl;
    }
    for (Order *order : wareHouse.getCompletedOrders())
    {
        std::cout << "OrderID: " << order->getId() << ", "
                  << "CustomerID: " << order->getCustomerId() << ", "
                  << "OrderStatus: " << order->toString() << std::endl;
    }
    complete();
    wareHouse.addAction(this);
    wareHouse.close();
}
Close *Close::clone() const
{
    return new Close(*this);
}
string Close::toString() const
{
    return "close";
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BackupWareHouse::BackupWareHouse() : BaseAction(){};
void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup == NULL)
    {
        backup = new WareHouse(wareHouse);
    }
    else
    {
        delete backup;
        backup = new WareHouse(wareHouse);
    }
    complete();
    wareHouse.addAction(this);
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const
{
    return "backup";
}

///////////////////////////////////////////////////////////////////////////////////////////////////

RestoreWareHouse::RestoreWareHouse(){};
void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup != NULL)
    {
        wareHouse = *backup;
        complete();
        
    }
    else
    {
        
        //std::cout << "No backup available";
        error(" No backup available\n");

    }
    wareHouse.addAction(this);
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const
{
    return "restore";
}
