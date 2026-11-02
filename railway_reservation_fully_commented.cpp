#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std; 

// ============================================================
// LIBRARIES USED IN THIS PROJECT
// ------------------------------------------------------------
// <iostream> -> Used for console input and output: cin, cout.
// <iomanip>  -> Used for formatted output such as setprecision()
//               when displaying fare values.
// <fstream>  -> Used for file handling: saving and loading
//               users and ticket information.
// <string>   -> Used for storing names, stations, usernames,
//               passwords, PNRs and other text data.
// ============================================================
// Maximum number of registered users, tickets, and trains
// ================================================================
// PROJECT LIMITS / CONSTANTS
// These constants define the maximum number of records stored in memory.
// ================================================================
const int MAX_USERS = 50;
const int MAX_TICKETS = 100;
const int TOTAL_TRAINS = 6;
// Converts an integer into a string without using to_string()
// ================================================================
// UTILITY MODULE
// Converts numbers to strings so PNR values can be created without
// using the C++ to_string() function.
// ================================================================
string numberToString(int number)
{
    if (number == 0)
    {
        return "0";
	}
    string result = "";
    while (number > 0)
	{
        int digit = number % 10;
        result = char('0' + digit) + result;
        number = number / 10;
    }
    return result;
};
// Reads one field from a | separated text-file record
// Reads a particular field from a text-file line separated by |
string getField(string line, int number){
    int start = 0;
    for (int i = 0; i < number; i++)
	{
        int pos = line.find('|', start);
        if (pos == -1)
            return "";
        start = pos + 1;
    }
    int end = line.find('|', start);
    if (end == -1)
        return line.substr(start);
    return line.substr(start, end - start);
}
// Converts numeric text into an integer
// Converts saved numeric text back into an integer while loading files.
int stringToInt(string value){
    int number = 0;
    for (int i = 0; i < (int)value.length(); i++)
        if (value[i] >= '0' && value[i] <= '9')
            number = number * 10 + value[i] - '0';
    return number;
}
// Converts decimal text into a double value
// Converts saved decimal text back into a double for ticket fares.
double stringToDouble(string value){
    double number = 0;
    double place = 0.1;
    bool decimal = false;
    for (int i = 0; i < (int)value.length(); i++){
        if (value[i] == '.')
            decimal = true; else if (value[i] >= '0' && value[i] <= '9'){
            if (!decimal)
                number = number * 10 + value[i] - '0'; else{
                number += (value[i] - '0') * place;
                place = place / 10;
            }
        }
    }
    return number;
}
// ============================================================
// OOP CONCEPTS USED IN THIS PROJECT
// ------------------------------------------------------------
// 1. CLASS & OBJECT:
//    Person, User, Admin, Train, Passenger and Ticket are classes.
//    Objects are created from these classes to store real data.
//
// 2. INHERITANCE:
//    User and Admin inherit from Person.
//    Common details such as name, mobile and email are reused.
//
// 3. ENCAPSULATION:
//    Data members are kept private/protected and accessed through
//    public member functions such as getters and setters.
//
// 4. ABSTRACTION:
//    Complex internal operations such as fare calculation,
//    PNR generation and file handling are performed through
//    simple functions, so the user only sees the required menu.
//
// 5. MODULARITY:
//    Each class/function handles a specific part of the system,
//    making the project easier to understand and maintain.
// ============================================================

// Base class containing common personal details
// ================================================================
// MODULE 1 : PERSON
// Purpose : stores common personal information such as name, mobile,
// and email. User and Admin reuse this class through inheritance.
// OOP concept : base class.
// ================================================================
// ============================================================
// MODULE 1: PERSON - BASE CLASS
// ============================================================
// OOP CONCEPT: INHERITANCE
// Person contains details common to both User and Admin.
// User and Admin inherit these members instead of writing them again.
// This avoids code duplication and demonstrates reusability.
// ============================================================
class Person{
protected:
    string name;
    string mobile;
    string email;
public:
    Person(){
        name = "";
        mobile = "";
        email = "";
    }
    void setPerson(string n, string m, string e){
        name = n;
        mobile = m;
        email = e;
    }
    void displayPerson(){
        cout << "Name       : " << name << endl;
        cout << "Mobile     : " << mobile << endl;
        cout << "Email      : " << email << endl;
    }
};
// User class inherits common details from Person
// ================================================================
// MODULE 2 : USER
// Purpose : handles user login information and user profile details.
// OOP concept : inheritance - User inherits name, mobile and email
// from Person.
// ================================================================
// ============================================================
// MODULE 2: USER
// ============================================================
// INHERITANCE USED HERE:
// User : public Person means User gets name, mobile and email
// from the Person base class.
// User additionally contains username and password for login.
// ============================================================
class User : public Person{
private:
    string username;
    string password;
public:
    User(){
        username = "";
        password = "";
    }
    void setLogin(string u, string p){
        username = u;
        password = p;
    }
    string getUsername(){
        return username;
    }
    string getPassword(){
        return password;
    }
    void saveData(ofstream &file){
        file << username << "|" << password << "|" << name << "|"
             << mobile << "|" << email << endl;
    }
    void loadData(string line){
        username = getField(line, 0);
        password = getField(line, 1);
        name = getField(line, 2);
        mobile = getField(line, 3);
        email = getField(line, 4);
    }
    // Displays the logged-in user profile
    void displayProfile(){
        cout << "\n========================================" << endl;
        cout << "              USER PROFILE              " << endl;
        cout << "========================================" << endl;
        displayPerson();
        cout << "Username   : " << username << endl;
        cout << "========================================" << endl;
    }
};
// Admin class inherits common details from Person
// ================================================================
// MODULE 3 : ADMIN
// Purpose : provides administrator login and access to admin operations.
// OOP concept : inheritance - Admin inherits common Person details.
// ================================================================
// ============================================================
// MODULE 3: ADMIN
// ============================================================
// INHERITANCE USED HERE:
// Admin also inherits common personal details from Person.
// Admin has its own username and password for administrator login.
// ============================================================
class Admin : public Person{
private:
    string adminUsername;
    string adminPassword;
public:
    Admin(){
        adminUsername = "admin";
        adminPassword = "admin123";
    }
    bool login(string u, string p){
        if (u == adminUsername && p == adminPassword)
            return true;
        return false;
    }
};
// Stores train details, four halts, timings, and class-wise seats
// ================================================================
// MODULE 4 : TRAIN
// Purpose : stores train ID, train name, four halts, timings and
// class-wise seat availability.
// Seat categories : First AC, Second AC, Third AC and Sleeper.
// ================================================================
// ============================================================
// MODULE 4: TRAIN
// ============================================================
// PURPOSE:
// Stores train ID, train name, 4 halts, timings and class-wise seats.
// Each Train object represents one train in our railway system.
// ============================================================
class Train{
private:
    int trainId;
    string trainName;
    string stops[4];
    string departureTime;
    string arrivalTime;
    int firstAC;
    int secondAC;
    int thirdAC;
    int sleeper;
public:
    Train(){
        trainId = 0;
        trainName = "";
        departureTime = "";
        arrivalTime = "";
        firstAC = 10;
        secondAC = 20;
        thirdAC = 30;
        sleeper = 40;
    }
    void setTrain(int id, string n, string s1, string s2, string s3, string s4,
                  string dep, string arr){
        trainId = id;
        trainName = n;
        stops[0] = s1;
        stops[1] = s2;
        stops[2] = s3;
        stops[3] = s4;
        departureTime = dep;
        arrivalTime = arr;
        firstAC = 10;
        secondAC = 20;
        thirdAC = 30;
        sleeper = 40;
    }
    int getId(){
        return trainId;
    }
    string getStop(int index){
        if (index >= 0 && index < 4)
            return stops[index];
        return "";
    }
    int getSeatCount(int type){
        if (type == 1)
            return firstAC; else if (type == 2)
            return secondAC; else if (type == 3)
            return thirdAC; else if (type == 4)
            return sleeper;
        return 0;
    }
    void reduceSeat(int type){
        if (type == 1 && firstAC > 0)
            firstAC--; else if (type == 2 && secondAC > 0)
            secondAC--; else if (type == 3 && thirdAC > 0)
            thirdAC--; else if (type == 4 && sleeper > 0)
            sleeper--;
    }
    void addSeat(int type){
        if (type == 1 && firstAC < 10)
            firstAC++; else if (type == 2 && secondAC < 20)
            secondAC++; else if (type == 3 && thirdAC < 30)
            thirdAC++; else if (type == 4 && sleeper < 40)
            sleeper++;
    }
    // Allows admin to update train information
    void updateTrain(){
        cout << "\nEnter New Train Name : ";
        cin >> trainName;
        cout << "Enter Halt 1 : ";
        cin >> stops[0];
        cout << "Enter Halt 2 : ";
        cin >> stops[1];
        cout << "Enter Halt 3 : ";
        cin >> stops[2];
        cout << "Enter Halt 4 : ";
        cin >> stops[3];
        cout << "Enter Departure Time : ";
        cin >> departureTime;
        cout << "Enter Arrival Time   : ";
        cin >> arrivalTime;
        cout << "\nTrain details updated successfully.\n";
    }
    // Displays complete train information and seat availability
    void displayTrain(){
        cout << "\n--------------------------------------------" << endl;
        cout << "Train ID       : " << trainId << endl;
        cout << "Train Name     : " << trainName << endl;
        cout << "Route          : " << stops[0] << " -> "
             << stops[1] << " -> " << stops[2] << " -> " << stops[3] << endl;
        cout << "Departure Time : " << departureTime << endl;
        cout << "Arrival Time   : " << arrivalTime << endl;
        cout << "1AC Seats      : " << firstAC << " / 10" << endl;
        cout << "2AC Seats      : " << secondAC << " / 20" << endl;
        cout << "3AC Seats      : " << thirdAC << " / 30" << endl;
        cout << "Sleeper Seats  : " << sleeper << " / 40" << endl;
        cout << "Total Available: "
             << firstAC + secondAC + thirdAC + sleeper << " / 100" << endl;
        cout << "--------------------------------------------" << endl;
    }
};
// Stores details of each passenger travelling on a ticket
// ================================================================
// MODULE 5 : PASSENGER
// Purpose : stores passenger name, age, gender, berth preference
// and the seat assigned to the passenger.
// ================================================================
// ============================================================
// MODULE 5: PASSENGER
// ============================================================
// PURPOSE:
// Stores the details of a passenger travelling in a ticket.
// A Passenger object contains name, age, gender, berth and seat.
// ============================================================
class Passenger{
private:
    int passengerId;
    string passengerName;
    int age;
    string gender;
    int seatNo;
    string berthPreference;
public:
    Passenger()
	{
        passengerId = 0;
        passengerName = "";
        age = 0;
        gender = "";
        seatNo = 0;
        berthPreference = "";
    }
    // Reads passenger information from the user
    void inputPassenger(int id){
        passengerId = id;
        cout << "\nEnter Passenger Name : ";
        cin >> passengerName;
        cout << "Enter Age            : ";
        cin >> age;
        cout << "Enter Gender         : ";
        cin >> gender;
        cout << "Enter Berth Preference (Lower/Middle/Upper): ";
        cin >> berthPreference;
    }
    void setSeatNo(int seat){
        seatNo = seat;
    }
    void saveData(ofstream &file)
	{
        file << passengerId << "|" << passengerName << "|" << age << "|"
             << gender << "|" << seatNo << "|" << berthPreference;
    }
    void loadData(string line){
        passengerId = stringToInt(getField(line, 0));
        passengerName = getField(line, 1);
        age = stringToInt(getField(line, 2));
        gender = getField(line, 3);
        seatNo = stringToInt(getField(line, 4));
        berthPreference = getField(line, 5);
    }
    // Displays passenger information
    void displayPassenger(){
        cout << "Passenger ID       : " << passengerId << endl;
        cout << "Passenger Name     : " << passengerName << endl;
        cout << "Age                : " << age << endl;
        cout << "Gender             : " << gender << endl;
        cout << "Seat Number        : " << seatNo << endl;
        cout << "Berth Preference   : " << berthPreference << endl;
    }
};
// Stores PNR, route, class, seat, fare, status, and passenger details
// ================================================================
// MODULE 6 : TICKET / RESERVATION
// Purpose : stores booking details such as PNR, route, train, class,
// seat, fare, status and passenger information.
// ================================================================
// ============================================================
// MODULE 6: TICKET / RESERVATION
// ============================================================
// PURPOSE:
// Stores complete reservation information.
// It connects the user, train, route, class, seat, fare, PNR
// and passenger details into one ticket object.
// ============================================================
class Ticket{
private:
    int ticketId;
    string pnr;
    int trainId;
    string username;
    string source;
    string destination;
    string classType;
    int classNumber;
    int seatNo;
    double fare;
    string status;
    Passenger passenger;
public:
    Ticket(){
        ticketId = 0;
        pnr = "";
        trainId = 0;
        username = "";
        source = "";
        destination = "";
        classType = "";
        classNumber = 0;
        seatNo = 0;
        fare = 0;
        status = "";
    }
    void createTicket(int id, string p, int tid, string user, string src, string dest, string type, int typeNo,
                      int seat, double f, Passenger pass){
        ticketId = id;
        pnr = p;
        trainId = tid;
        username = user;
        source = src;
        destination = dest;
        classType = type;
        classNumber = typeNo;
        seatNo = seat;
        fare = f;
        status = "CONFIRMED";
        passenger = pass;
    }
    string getPNR(){
        return pnr;
    }
    int getTrainId(){
        return trainId;
    }
    string getUsername(){
        return username;
    }
    int getClassNumber(){
        return classNumber;
    }
    int getSeatNo(){
        return seatNo;
    }
    string getStatus(){
        return status;
    }
    void setStatus(string s){
        status = s;
    }
    void saveData(ofstream &file){
        file << ticketId << "|" << pnr << "|" << trainId << "|" << username
             << "|" << source << "|" << destination << "|" << classType << "|"
             << classNumber << "|" << seatNo << "|" << fixed << setprecision(2)
             << fare << "|" << status << "|";
        passenger.saveData(file);
        file << endl;
    }
    void loadData(string line){
        ticketId = stringToInt(getField(line, 0));
        pnr = getField(line, 1);
        trainId = stringToInt(getField(line, 2));
        username = getField(line, 3);
        source = getField(line, 4);
        destination = getField(line, 5);
        classType = getField(line, 6);
        classNumber = stringToInt(getField(line, 7));
        seatNo = stringToInt(getField(line, 8));
        fare = stringToDouble(getField(line, 9));
        status = getField(line, 10);
        string passengerData = getField(line, 11);
        string rest = line;
        int start = 0;
        for (int i = 0; i < 11; i++){
            int pos = rest.find('|', start);
            if (pos == -1)
                return;
            start = pos + 1;
        }
        passengerData = rest.substr(start);
        passenger.loadData(passengerData);
    }
    // Displays complete ticket details
    void displayTicket(){
        cout << "\n============================================" << endl;
        cout << "             RAILWAY TICKET                " << endl;
        cout << "============================================" << endl;
        cout << "Ticket ID       : " << ticketId << endl;
        cout << "PNR             : " << pnr << endl;
        cout << "Username        : " << username << endl;
        cout << "Train ID        : " << trainId << endl;
        cout << "From            : " << source << endl;
        cout << "To              : " << destination << endl;
        cout << "Class           : " << classType << endl;
        cout << "Seat Number     : " << seatNo << endl;
        cout << "Fare            : Rs." << fixed << setprecision(2) << fare << endl;
        cout << "Status          : " << status << endl;
        cout << "--------------------------------------------" << endl;
        passenger.displayPassenger();
        cout << "============================================" << endl;
    }
};
// ============================================================
// PROJECT MODULE MAP
// ------------------------------------------------------------
// Person       -> Common personal information
// User         -> Registration, login and profile
// Admin        -> Administrator login and management
// Train        -> Trains, halts, timings and seat availability
// Passenger    -> Passenger information
// Ticket       -> PNR, route, class, seat, fare and status
// RailwaySystem-> Main controller connecting all modules
// File Handling-> Permanent storage using users.txt/bookings.txt
// ============================================================

// Main class that controls users, trains, tickets, menus, and files
// ================================================================
// MODULE 7 : RAILWAY SYSTEM CONTROLLER
// Purpose : connects all modules and controls the complete console
// application including registration, login, search, booking,
// PNR checking, cancellation, admin operations and file handling.
// ================================================================
// ============================================================
// MODULE 7: RAILWAY SYSTEM
// ============================================================
// PURPOSE:
// This is the main controller of the project.
// It manages users, trains, tickets, menus and file handling.
// All major modules are connected through this class.
// ============================================================
class RailwaySystem{
private:
    Train trains[TOTAL_TRAINS];
    User users[MAX_USERS];
    Ticket tickets[MAX_TICKETS];
    int userCount;
    int ticketCount;
    int passengerCounter;
    int pnrCounter;
    Admin admin;
public:
    // Constructor initializes the system and loads saved data
    RailwaySystem(){
        userCount = 0;
        ticketCount = 0;
        passengerCounter = 1;
        pnrCounter = 100001;
        initializeTrains();
        loadUsers();
        loadTickets();
    }
    // Creates the six default trains
    void initializeTrains(){
        trains[0].setTrain(101, "Godavari Express", "Rajahmundry", "Vijayawada", "Guntur", "Chennai",
                           "06:00", "16:00"); trains[1].setTrain(102, "Krishna Express", "Visakhapatnam", "Rajahmundry", "Vijayawada", "Hyderabad",
                           "05:30", "17:30"); trains[2].setTrain(103, "Charminar Express", "Chennai", "Nellore", "Vijayawada", "Hyderabad",
                           "07:00", "19:00"); trains[3].setTrain(104, "Coromandel Express", "Bhubaneswar", "Visakhapatnam", "Vijayawada", "Chennai",
                           "08:00", "20:00"); trains[4].setTrain(105, "East Coast Express", "Howrah", "Bhubaneswar", "Visakhapatnam", "Vijayawada",
                           "09:00", "21:00"); trains[5].setTrain(106, "Deccan Express", "Hyderabad", "Guntur", "Vijayawada", "Rajahmundry",
                           "10:00", "18:00");
    }
    // Converts class number into a readable class name.
    // Example: 1 = First AC, 2 = Second AC, 3 = Third AC, 4 = Sleeper.
    string className(int type){
        if (type == 1)
            return "First AC"; else if (type == 2)
            return "Second AC"; else if (type == 3)
            return "Third AC"; else if (type == 4)
            return "Sleeper";
        return "Unknown";
    }
    // Searches the train array using the train ID.
    int findTrain(int trainId){
        for (int i = 0; i < TOTAL_TRAINS; i++){
            if (trains[i].getId() == trainId)
                return i;
        }
        return -1;
    }
    // Finds the position of a station in a train's 4-stop route.
    // This helps us verify source -> destination order.
    int stopPosition(int trainIndex, string stop){
        for (int i = 0; i < 4; i++){
            if (trains[trainIndex].getStop(i) == stop)
                return i;
        }
        return -1;
    }
    // Displays numbered halts so the user can select source
    // and destination easily during ticket booking.
    void displayStops(int index){
        cout << "\nAvailable Stops:" << endl;
        for (int i = 0; i < 4; i++){
            cout << i + 1 << ". " << trains[index].getStop(i) << endl;
        }
    }
    // Shows available seats separately for each ticket class.
    void displayClasses(int index){
        cout << "\n========== SEAT AVAILABILITY ==========" << endl;
        cout << "1. First AC  : " << trains[index].getSeatCount(1) << " / 10" << endl;
        cout << "2. Second AC : " << trains[index].getSeatCount(2) << " / 20" << endl;
        cout << "3. Third AC  : " << trains[index].getSeatCount(3) << " / 30" << endl;
        cout << "4. Sleeper    : " << trains[index].getSeatCount(4) << " / 40" << endl;
        cout << "========================================" << endl;
    }
    // Displays all available trains
    void viewAllTrains(){
        cout << "\n============================================" << endl;
        cout << "              AVAILABLE TRAINS             " << endl;
        cout << "============================================" << endl;
        for (int i = 0; i < TOTAL_TRAINS; i++){
            trains[i].displayTrain();
        }
    }
    // Searches trains using source and destination station names
void searchTrain(){
    string source;
    string destination;
    bool found = false;
    cout << "\n========================================" << endl;
    cout << "              SEARCH TRAIN              " << endl;
    cout << "========================================" << endl;
    cout << "\nEnter Source Station      : ";
    cin >> source;
    cout << "Enter Destination Station : ";
    cin >> destination;
    cout << "\n========== SEARCH RESULTS ==========" << endl;
    for (int i = 0; i < TOTAL_TRAINS; i++){
        int sourceIndex = stopPosition(i, source);
        int destinationIndex = stopPosition(i, destination);
        if (sourceIndex != -1 && destinationIndex != -1 &&
            sourceIndex < destinationIndex){
            cout << "\nTrain ID   : " << trains[i].getId() << endl;
            cout << "Route      : ";
            for (int j = sourceIndex; j <= destinationIndex; j++){
                cout << trains[i].getStop(j);
                if (j < destinationIndex)
                    cout << " -> ";
            }
            cout << endl;
            cout << "Departure  : " << endl;
            displayClasses(i);
            found = true;
        }
    }
    if (!found){
        cout << "\nNo train found for this route." << endl;
    }
    cout << "====================================" << endl;
}
    // Calculates fare based on class and number of route segments
    // Calculates fare according to:
    // 1. Selected travel class
    // 2. Number of route segments travelled
    double calculateFare(int type, int sourceIndex, int destinationIndex){
        int segments = destinationIndex - sourceIndex;
        double baseFare = 0;
        if (type == 1)
            baseFare = 500; else if (type == 2)
            baseFare = 350; else if (type == 3)
            baseFare = 250; else if (type == 4)
            baseFare = 150;
        return baseFare + (segments - 1) * 75;
    }
    // Generates a unique PNR number
    // Generates a unique PNR for every booked ticket.
    // PNR is used later to check ticket status.
    string generatePNR(){
        string pnr = "PNR" + numberToString(pnrCounter);
        pnrCounter++;
        return pnr;
    }
    // Generates the next seat number for the selected class
    // Generates a seat number based on train and class.
    // Different classes use different seat number ranges.
    int generateSeatNumber(int trainIndex, int type){
        int totalCapacity = 0;
        int usedSeats = 0;
        for (int i = 0; i < ticketCount; i++){
            if (tickets[i].getTrainId() == trains[trainIndex].getId() &&
                tickets[i].getClassNumber() == type &&
                tickets[i].getStatus() == "CONFIRMED"){
                usedSeats++;
            }
        }
        if (type == 1){
            totalCapacity = 10;
            return usedSeats + 1;
        } else if (type == 2){
            totalCapacity = 20;
            return 10 + usedSeats + 1;
        } else if (type == 3){
            totalCapacity = 30;
            return 30 + usedSeats + 1;
        } else{
            totalCapacity = 40;
            return 60 + usedSeats + 1;
        }
        return totalCapacity;
    }
    // Registers a new user and saves the details
    // USER REGISTRATION MODULE
    // Takes personal details and login details from a new user.
    // After registration, details are saved into users.txt.
    void registerUser(){
        if (userCount >= MAX_USERS){
            cout << "User storage is full.\n";
            return;
        }
        string name;
        string mobile;
        string email;
        string username;
        string password;
        cout << "\n========================================" << endl;
        cout << "            USER REGISTRATION           " << endl;
        cout << "========================================" << endl;
        cout << "Enter Name     : ";
        cin >> name;
        cout << "Enter Mobile   : ";
        cin >> mobile;
        cout << "Enter Email    : ";
        cin >> email;
        cout << "Enter Username : ";
        cin >> username;
        for (int i = 0; i < userCount; i++){
            if (users[i].getUsername() == username){
                cout << "Username already exists.\n";
                return;
            }
        }
        cout << "Enter Password : ";
        cin >> password;
        users[userCount].setPerson(name, mobile, email);
        users[userCount].setLogin(username, password);
        userCount++;
        saveUsers();
        cout << "\nRegistration completed successfully.\n";
    }
    // Checks user credentials and returns the user index
    // USER LOGIN MODULE
    // Compares entered username and password with saved user data.
    // Returns the matching user index when login is successful.
    int userLogin(){
        string username;
        string password;
        cout << "\n========================================" << endl;
        cout << "                USER LOGIN               " << endl;
        cout << "========================================" << endl;
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        cin >> password;
        for (int i = 0; i < userCount; i++)
		{ 
			if (users[i].getUsername() == username && users[i].getPassword() == password)
			{
                cout << "Login successful.\n";
                return i;
            }
        }
        cout << "Invalid username or password.\n";
        return -1;
    }
    // Books a ticket for the logged-in user
    // TICKET BOOKING MODULE
    // Flow:
    // 1. Select train
    // 2. Select source and destination
    // 3. Select class
    // 4. Enter passenger details
    // 5. Generate seat, fare and PNR
    // 6. Save the ticket
    void bookTicket(int userIndex){
        if (ticketCount >= MAX_TICKETS){
            cout << "Ticket storage is full.\n";
            return;
        }
        int trainId;
        int type;
        cout << "\n========================================" << endl;
        cout << "              BOOK TICKET                " << endl;
        cout << "========================================" << endl;
        viewAllTrains();
        cout << "\nEnter Train ID : ";
        cin >> trainId;
        int trainIndex = findTrain(trainId);
        if (trainIndex == -1){
            cout << "Invalid Train ID.\n";
            return;
        }
        displayStops(trainIndex);
        int sourceChoice;
        int destinationChoice;
        cout << "Enter Source Stop Number      : ";
        cin >> sourceChoice;
        cout << "Enter Destination Stop Number : ";
        cin >> destinationChoice;
        if (sourceChoice < 1 || sourceChoice > 4 ||
            destinationChoice < 1 || destinationChoice > 4){
            cout << "Invalid stop number. Please enter 1 to 4.\n";
            return;
        }
        if (sourceChoice >= destinationChoice){
            cout << "Source must be before destination.\n";
            return;
        }
        int sourceIndex = sourceChoice - 1;
        int destinationIndex = destinationChoice - 1;
        string source = trains[trainIndex].getStop(sourceIndex);
        string destination = trains[trainIndex].getStop(destinationIndex);
        displayClasses(trainIndex);
        cout << "Choose Class : ";
        cin >> type;
        if (type < 1 || type > 4){
            cout << "Invalid class selection.\n";
            return;
        }
        if (trains[trainIndex].getSeatCount(type) <= 0){
            cout << "No seats available in this class.\n";
            cout << "Please select another class.\n";
            return;
        }
        Passenger passenger;
        passenger.inputPassenger(passengerCounter);
        int seatNumber = generateSeatNumber(trainIndex, type);
        passenger.setSeatNo(seatNumber);
        double fare = calculateFare(type, sourceIndex, destinationIndex);
        string pnr = generatePNR();
        tickets[ticketCount].createTicket(
            ticketCount + 1, pnr, trainId, users[userIndex].getUsername(),
            source, destination, className(type), type, seatNumber, fare, passenger
        );
        trains[trainIndex].reduceSeat(type);
        ticketCount++;
        passengerCounter++;
        saveTickets();
        cout << "\nTicket booked successfully.\n";
        cout << "PNR : " << pnr << endl;
        cout << "Seat Number : " << seatNumber << endl;
        cout << "Fare : Rs." << fixed << setprecision(2) << fare << endl;
    }
    // Searches and displays a ticket using its PNR
    // PNR STATUS MODULE
    // Searches the saved tickets using the PNR number.
    void checkPNR(){
        string pnr;
        cout << "\n========== CHECK PNR STATUS ==========" << endl;
        cout << "Enter PNR : ";
        cin >> pnr;
        for (int i = 0; i < ticketCount; i++){
            if (tickets[i].getPNR() == pnr){
                tickets[i].displayTicket();
                return;
            }
        }
        cout << "PNR not found.\n";
    }
    // Displays every ticket belonging to the logged-in user
    // MY TICKETS MODULE
    // Displays all tickets belonging to the currently logged-in user.
    void viewMyTickets(int userIndex){
        bool found = false;
        cout << "\n========== MY TICKETS ==========" << endl;
        for (int i = 0; i < ticketCount; i++){
            if (tickets[i].getUsername() == users[userIndex].getUsername()){
                tickets[i].displayTicket();
                found = true;
            }
        }
        if (!found)
            cout << "No tickets found for this user.\n";
    }
    // Cancels the selected user ticket and restores the seat
    // TICKET CANCELLATION MODULE
    // Verifies ownership, changes ticket status to CANCELLED
    // and adds the seat back to the train availability.
    void cancelTicket(int userIndex){
        string pnr;
        cout << "\n========== CANCEL TICKET ==========" << endl;
        cout << "Enter PNR : ";
        cin >> pnr;
        for (int i = 0; i < ticketCount; i++){
            if (tickets[i].getPNR() == pnr){
                if (tickets[i].getUsername() != users[userIndex].getUsername()){
                    cout << "You can cancel only your own ticket.\n";
                    return;
                }
                if (tickets[i].getStatus() == "CANCELLED"){
                    cout << "This ticket is already cancelled.\n";
                    return;
                }
                int trainIndex = findTrain(tickets[i].getTrainId());
                if (trainIndex != -1){
                    trains[trainIndex].addSeat(tickets[i].getClassNumber());
                }
                tickets[i].setStatus("CANCELLED");
                saveTickets();
                cout << "Ticket cancelled successfully.\n";
                return;
            }
        }
        cout << "PNR not found.\n";
    }
    // Displays the current user profile
    void profile(int userIndex){
        users[userIndex].displayProfile();
    }
    // Admin menu for train and ticket management
    // ADMIN MODULE
    // Provides administrative operations such as viewing trains,
    // updating train details, viewing all tickets and reports.
    void adminMenu(){
        string username;
        string password;
        cout << "\n========================================" << endl;
        cout << "               ADMIN LOGIN               " << endl;
        cout << "========================================" << endl;
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        cin >> password;
        if (!admin.login(username, password)){
            cout << "Invalid admin login.\n";
            return;
        }
        cout << "Admin login successful.\n";
        int choice;
        do{
            cout << "\n========================================" << endl;
            cout << "                ADMIN MENU               " << endl;
            cout << "========================================" << endl;
            cout << "1. View All Trains" << endl;
            cout << "2. Update Train" << endl;
            cout << "3. View All Tickets" << endl;
            cout << "4. System Report" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter Choice : ";
            cin >> choice;
            switch (choice){
                case 1:
                    viewAllTrains();
                    break;
                case 2:
                    updateTrain();
                    break;
                case 3:
                    viewAllTickets();
                    break;
                case 4:
                    systemReport();
                    break;
                case 5:
                    cout << "Admin logged out successfully.\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 5);
    }
    void updateTrain(){
        int trainId;
        cout << "\n========== UPDATE TRAIN ==========" << endl;
        cout << "Enter Train ID : ";
        cin >> trainId;
        int index = findTrain(trainId);
        if (index == -1){
            cout << "Train not found.\n";
            return;
        }
        trains[index].updateTrain();
    }
    // Displays all tickets booked by all users
    // ADMIN TICKET MODULE
    // Displays every ticket stored in the system,
    // irrespective of which user booked it.
    void viewAllTickets(){
        cout << "\n========== ALL TICKETS ==========" << endl;
        if (ticketCount == 0){
            cout << "No tickets available.\n";
            return;
        }
        for (int i = 0; i < ticketCount; i++){
            tickets[i].displayTicket();
        }
    }
    // Displays basic system statistics
    // REPORT MODULE
    // Shows basic system statistics such as users,
    // total tickets, confirmed tickets and cancelled tickets.
    void systemReport(){
        int confirmed = 0;
        int cancelled = 0;
        for (int i = 0; i < ticketCount; i++){
            if (tickets[i].getStatus() == "CONFIRMED")
                confirmed++; else if (tickets[i].getStatus() == "CANCELLED")
                cancelled++;
        }
        cout << "\n========================================" << endl;
        cout << "             SYSTEM REPORT              " << endl;
        cout << "========================================" << endl;
        cout << "Registered Users  : " << userCount << endl;
        cout << "Total Tickets     : " << ticketCount << endl;
        cout << "Confirmed Tickets : " << confirmed << endl;
        cout << "Cancelled Tickets : " << cancelled << endl;
        cout << "========================================" << endl;
    }
    // Saves all registered users into users.txt
    // FILE HANDLING - WRITE USERS
    // ofstream writes registered user details permanently into users.txt.
    // This allows user information to remain after the program closes.
    void saveUsers(){
        ofstream file("users.txt");
        if (!file)
            return;
        for (int i = 0; i < userCount; i++)
            users[i].saveData(file);
        file.close();
    }
    // Loads previously saved users from users.txt
    // FILE HANDLING - READ USERS
    // ifstream reads previously saved users from users.txt
    // when the program starts.
    void loadUsers(){
        ifstream file("users.txt");
        if (!file)
            return;
        string line;
        while (getline(file, line) && userCount < MAX_USERS){
            if (line == "")
                continue;
            if (line.find('|') == string::npos){
                int space = line.find(' ');
                if (space != -1){ users[userCount].setLogin( line.substr(0, space),
                        line.substr(space + 1));
                    userCount++;
                }
            } else{
                users[userCount].loadData(line);
                userCount++;
            }
        }
        file.close();
    }
    // Saves every ticket into bookings.txt
    // FILE HANDLING - WRITE TICKETS
    // Saves every booked ticket into bookings.txt.
    void saveTickets(){
        ofstream file("bookings.txt");
        if (!file)
            return;
        for (int i = 0; i < ticketCount; i++)
            tickets[i].saveData(file);
        file.close();
    }
    // Loads saved tickets and restores confirmed seat counts
    // FILE HANDLING - READ TICKETS
    // Loads old tickets when the program starts and restores
    // the corresponding confirmed seat counts.
    void loadTickets(){
        ifstream file("bookings.txt");
        if (!file)
            return;
        string line;
        while (getline(file, line) && ticketCount < MAX_TICKETS){
            if (line == "" || line.find('|') == string::npos)
                continue;
            tickets[ticketCount].loadData(line);
            int trainIndex = findTrain(tickets[ticketCount].getTrainId());
            if (tickets[ticketCount].getStatus() == "CONFIRMED" &&
                trainIndex != -1){ trains[trainIndex].reduceSeat(
                    tickets[ticketCount].getClassNumber());
            }
            int pnrNumber = 0;
            string pnr = tickets[ticketCount].getPNR();
            if (pnr.length() > 3)
                pnrNumber = stringToInt(pnr.substr(3));
            if (pnrNumber >= pnrCounter)
                pnrCounter = pnrNumber + 1;
            ticketCount++;
        }
        file.close();
    }
    // Menu shown after a user logs in
    // USER MENU MODULE
    // Contains all operations available after successful login.
    void userMenu(int userIndex){
        int choice;
        do{
            cout << "\n============================================" << endl;
            cout << "                 USER MENU                 " << endl;
            cout << "============================================" << endl;
            cout << "1. View All Trains" << endl;
            cout << "2. Search Train" << endl;
            cout << "3. Book Ticket" << endl;
            cout << "4. Check PNR Status" << endl;
            cout << "5. View My Tickets" << endl;
            cout << "6. Cancel Ticket" << endl;
            cout << "7. View Profile" << endl;
            cout << "8. Logout" << endl;
            cout << "Enter Choice : ";
            cin >> choice;
            switch (choice){
                case 1:
                    viewAllTrains();
                    break;
                case 2:
                    searchTrain();
                    break;
                case 3:
                    bookTicket(userIndex);
                    break;
                case 4:
                    checkPNR();
                    break;
                case 5:
                    viewMyTickets(userIndex);
                    break;
                case 6:
                    cancelTicket(userIndex);
                    break;
                case 7:
                    profile(userIndex);
                    break;
                case 8:
                    cout << "Logged out successfully.\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 8);
    }
    // Main menu of the railway reservation system
    // MAIN MENU MODULE
    // Entry point for the console interface:
    // Register, Login, Admin Login and Exit.
    void start(){
        int choice;
        do{
            cout << "\n\n============================================" << endl;
            cout << "        RAILWAY RESERVATION SYSTEM         " << endl;
            cout << "============================================" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Admin Login" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter Choice : ";
            cin >> choice;
            switch (choice){
                case 1:
                    registerUser();
                    break;
                case 2:{
                    int userIndex = userLogin();
                    if (userIndex != -1)
                        userMenu(userIndex);
                    break;
                }
                case 3:
                    adminMenu();
                    break;
                case 4:
                    cout << "\nThank you for using Railway Reservation System.\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 4);
    }
};
// ================================================================
// PROGRAM START
// main() creates the RailwaySystem object and starts the main menu.
// ================================================================
// ============================================================
// PROGRAM EXECUTION
// ============================================================
// main() creates one RailwaySystem object.
// The constructor prepares the system and the start() function
// displays the main menu.
// ============================================================
// Program execution starts here
int main(){
    RailwaySystem system;
    system.start();
    return 0;
}
