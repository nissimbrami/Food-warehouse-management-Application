#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

Volunteer::Volunteer(const Volunteer &other)
    : completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId), id(other.id), name(other.name) {}

Volunteer::~Volunteer() {}

int Volunteer::getId() const
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
    return completedOrderId;
}

bool Volunteer::isBusy() const
{
    return getActiveOrderId() != NO_ORDER;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown), type(VolunteerType::COLLECTORVOLUNTEER) {}

CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other)
    : Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft), type(other.type) {}

CollectorVolunteer::~CollectorVolunteer() {}

CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()
{
    decreaseCoolDown();
}

int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
    if (isBusy())
    {
        timeLeft--;
        if (timeLeft == 0)
        {
            this->completedOrderId = this->activeOrderId;
            this->activeOrderId = NO_ORDER;
            return false;
        }

        return true;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (!isBusy() && order.getStatus() == OrderStatus::PENDING);
}

void CollectorVolunteer::acceptOrder(const Order &order)
{

    activeOrderId = order.getId();
    setTimeLeft();
}
void CollectorVolunteer::setTimeLeft()
{
    timeLeft = getCoolDown();
}
string CollectorVolunteer::toString() const
{
    return "volunteerID: " + std::to_string(getId()) + "\n" +
           "isBusy: " + (isBusy() ? "true" : "false") + "\n" +
           "OrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n" +
           "Timeleft: " + (isBusy() ? std::to_string(getTimeLeft()) : "None") + "\n" +
           "ordersLeft: No Limit";
}

const VolunteerType CollectorVolunteer::getType() const
{
    return type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders), type(VolunteerType::LIMITEDCOLLECTORVOLUNTEER) {}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other)
    : CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft), type(other.type) {}

LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{

    return (ordersLeft > 0);
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return (hasOrdersLeft() & CollectorVolunteer::canTakeOrder(order));
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        this->ordersLeft--;
        CollectorVolunteer::acceptOrder(order);
    }
}

int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
    return "volunteerID: " + std::to_string(Volunteer::getId()) + "\n" +
           "isBusy: " + (isBusy() ? "true" : "false") + "\n" +
           "OrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n" +
           "Timeleft: " + (isBusy() ? std::to_string(getTimeLeft()) : "None") + "\n" +
           "ordersLeft: " + std::to_string(LimitedCollectorVolunteer::getNumOrdersLeft());
}

const VolunteerType LimitedCollectorVolunteer::getType() const
{
    return type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(maxDistance), type(VolunteerType::DRIVERVOLUNTEER) {}

DriverVolunteer::DriverVolunteer(const DriverVolunteer &other)
    : Volunteer(other), maxDistance(other.maxDistance), distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft), type(other.type) {}

DriverVolunteer::~DriverVolunteer() {}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    if (isBusy())
    {
        if (getDistancePerStep() > getDistanceLeft())
        {
            this->completedOrderId = getActiveOrderId();
            this->distanceLeft = 0;
            this->activeOrderId = NO_ORDER;
            return true;
        }
        else
        {
            this->distanceLeft -= this->distancePerStep;
            return false;
        }
    }
    return false;
}
bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    return ((!isBusy()) && (order.getDistance() <= getMaxDistance()) && (order.getStatus() == OrderStatus::COLLECTING));
}

void DriverVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        this->distanceLeft = order.getDistance();
        this->activeOrderId = order.getId();
    }
}

void DriverVolunteer::step()
{
    decreaseDistanceLeft();
}

string DriverVolunteer::toString() const
{
    return "volunteerID: " + std::to_string(Volunteer::getId()) + "\n" +
           "isBusy: " + (isBusy() ? "true" : "false") + "\n" +
           "OrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n" +
           "DistanceLeft : " + (isBusy() ? std::to_string(getDistanceLeft()) : "None") + "\n" +
           "ordersLeft: No limit";
}

const VolunteerType DriverVolunteer::getType() const
{
    return type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders), type(VolunteerType::LIMITEDDRIVERVOLUNTEER) {}

LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other)
    : DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft), type(other.type) {}

LimitedDriverVolunteer::~LimitedDriverVolunteer() {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    return (getNumOrdersLeft() > 0);
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    return (DriverVolunteer::canTakeOrder(order) & hasOrdersLeft());
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    if (canTakeOrder(order))
    {
        this->ordersLeft--;
        DriverVolunteer::acceptOrder(order);
    }
}

string LimitedDriverVolunteer::toString() const

{
    return "volunteerID: " + std::to_string(Volunteer::getId()) + "\n" +
           "isBusy: " + (isBusy() ? "true" : "false") + "\n" +
           "OrderID: " + (isBusy() ? std::to_string(getActiveOrderId()) : "None") + "\n" +
           "DistanceLeft : " + (isBusy() ? std::to_string(getDistanceLeft()) : "None") + "\n" +
           "ordersLeft: " + std::to_string(getNumOrdersLeft());
}

const VolunteerType LimitedDriverVolunteer::getType() const
{
    return type;
}