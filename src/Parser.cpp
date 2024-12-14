#include "../include/Parser.h"
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include <fstream>
#include <sstream>

void Parser::parseConfigurationFile(const std::string& configFilePath, WareHouse& wareHouse) {
    std::ifstream file(configFilePath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "customer"){
                std::string name, customerType;
                int locationDistance, maxOrders;
                iss >> name >> customerType >> locationDistance >> maxOrders;

                //Customer* customer;
                if (customerType == "soldier") {
                    //SoldierCustomer* customer = new SoldierCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders);
                    wareHouse.addCustomer(new SoldierCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders));
                    //delete customer;
                    
                }
                else {
                    //CivilianCustomer* customer = new CivilianCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders);
                    wareHouse.addCustomer(new CivilianCustomer(wareHouse.getCustomerCounter(), name, locationDistance, maxOrders));
                    //delete customer;
                }
                //delete customer;
            }

            else if (key == "volunteer"){
                std::string name, volunteerType;
                iss >> name >> volunteerType;
                //Volunteer* volunteer;
                if (volunteerType == "collector") {
                    int cooldown;
                    iss >> cooldown;
                    //CollectorVolunteer* volunteer = ;
                    wareHouse.addVolunteer(new CollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown));
                    //delete volunteer;
                    
                }
                else if (volunteerType == "limited_collector") {
                    int cooldown, maxOrders;
                    iss >> cooldown >> maxOrders;
                    //LimitedCollectorVolunteer* volunteer = ;
                    wareHouse.addVolunteer(new LimitedCollectorVolunteer(wareHouse.getVolunteerCounter(), name, cooldown, maxOrders));
                    //delete volunteer;
                    
                }
                else if (volunteerType == "driver"){
                    int maxDistance, distancePerStep;
                    iss >> maxDistance >> distancePerStep;
                    //DriverVolunteer* volunteer = new DriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep);
                    wareHouse.addVolunteer(new DriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep));
                    //delete volunteer;
                    
                }
                else {
                    int maxDistance, distancePerStep, maxOrders;
                    iss >> maxDistance >> distancePerStep >> maxOrders;
                    //LimitedDriverVolunteer* volunteer = new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep, maxOrders);
                    wareHouse.addVolunteer(new LimitedDriverVolunteer(wareHouse.getVolunteerCounter(), name, maxDistance, distancePerStep, maxOrders));
                    //delete volunteer;
                    
                }
                
            }
        }
    }
}