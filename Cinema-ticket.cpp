#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

class Entity {
public:
    virtual void display() const {
        cout << "Entity Info\n";
    }
};

class Person : public Entity {
private:
    string name;
    int age;

public:
    virtual void input() {
        cout << "Enter your name: ";
        getline(cin >> ws, name);
        cout << "Enter your age: ";
        while (!(cin >> age) || age <= 0) {
            cout << "Invalid age. Try again: ";
            clearInput();
        }
    }

    string getName() const {
        return name;
    }

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

class Customer : public Person {
private:
    string phone;

public:
    void input() override {
        Person::input();
        cout << "Enter your phone number: ";
        while (true) {
            cin >> phone;
            if (phone.length() >= 7 && phone.length() <= 15 && all_of(phone.begin(), phone.end(), ::isdigit)) {
                break;
            }
            cout << "Invalid phone number. Try again: ";
        }
    }

    void display() const {
        cout << "\n--- Customer Info ---\n";
        cout << "Name: " << getName() << "\n";
        cout << "Phone: " << phone << "\n";
    }
};

class Movie : public Entity {
private:
    string title;
    string genre;
    string time;
    int availableSeats;

public:
    void setData(string t, string g, string ti, int seats) {
        title = t;
        genre = g;
        time = ti;
        availableSeats = seats;
    }

    void display() const override {
        cout << "-----------------------------\n";
        cout << title << " - " << genre << " - " << time << " - Seats: " << availableSeats << endl;
        cout << "-----------------------------\n";
    }

    bool book(int quantity) {
        if (quantity <= availableSeats) {
            availableSeats -= quantity;
            return true;
        }
        return false;
    }

    string getTitle() const {
        return title;
    }

    int getAvailableSeats() const {
        return availableSeats;
    }

    bool isSoldOut() const {
        return availableSeats == 0;
    }
};

class Theater : public Entity {
private:
    string name;
    string location;
    int totalSeats;

public:
    void setInfo(string n, string l, int seats) {
        name = n;
        location = l;
        totalSeats = seats;
    }

    void display() const override {
        cout << "==============================\n";
        cout << "Welcome to " << name << " - " << location << endl;
        cout << "Total Seats: " << totalSeats << "\n";
        cout << "==============================\n\n";
    }
};

class Ticket {
private:
    string movieTitle;
    string customerName;
    int quantity;

public:
    void setDetails(string movie, string customer, int qty) {
        movieTitle = movie;
        customerName = customer;
        quantity = qty;
    }

    void setDetails(string movie, string customer) {
        movieTitle = movie;
        customerName = customer;
        quantity = 1;
    }

    void displayTicket() const {
        cout << toString();
    }

    string toString() const {
        string info = "\n-----------------------------\n";
        info += "Booking Successful!\n";
        info += "Customer: " + customerName + "\n";
        info += "Movie: " + movieTitle + "\n";
        info += "Tickets: " + to_string(quantity) + "\n";
        info += "-----------------------------\n";
        return info;
    }
};

class MovieSystem {
private:
    Movie* movies[4];
    Theater theater;

public:
    MovieSystem() {
        for (int i = 0; i < 4; i++)
            movies[i] = nullptr;
    }

    ~MovieSystem() {
        for (int i = 0; i < 4; i++) {
            delete movies[i];
        }
    }

    void initialize() {
        theater.setInfo("TAJ Cinema", "TAJ Mall", 200);
        movies[0] = new Movie(); movies[0]->setData("Omar & Salma", "Romantic", "5:00 PM", 50);
        movies[1] = new Movie(); movies[1]->setData("El Mamar", "War", "7:00 PM", 50);
        movies[2] = new Movie(); movies[2]->setData("Awlad Rizk", "Action", "9:00 PM", 50);
        movies[3] = new Movie(); movies[3]->setData("Catch Me If You Can", "Crime", "3:00 PM", 50);
    }

    void showMovies() const {
        cout << "Available Movies:\n";
        for (int i = 0; i < 4; i++) {
            if (movies[i] != nullptr) {
                cout << i + 1 << ". ";
                movies[i]->display();
            }
        }
    }

    void bookTickets() {
        theater.display();
        showMovies();

        Customer customer;
        customer.input();

        int choice, tickets;
        cout << "Choose movie number (1-4): ";
        while (!(cin >> choice) || choice < 1 || choice > 4 || movies[choice - 1] == nullptr) {
            cout << "Invalid choice. Try again: ";
            clearInput();
        }

        cout << "How many tickets? ";
        while (!(cin >> tickets) || tickets <= 0) {
            cout << "Invalid number. Try again: ";
            clearInput();
        }

        Movie* selected = movies[choice - 1];
        if (selected->book(tickets)) {
            Ticket ticket;
            if (tickets == 1)
                ticket.setDetails(selected->getTitle(), customer.getName());
            else
                ticket.setDetails(selected->getTitle(), customer.getName(), tickets);

            ticket.displayTicket();
            cout << "Remaining Seats: " << selected->getAvailableSeats() << endl;

            if (selected->isSoldOut()) {
                delete movies[choice - 1];
                movies[choice - 1] = nullptr;
                cout << "[Movie removed from system due to zero seats.]\n";
            }
        } else {
            cout << "Sorry, not enough seats.\n";
        }

        customer.display();
    }

    void run() {
        initialize();
        char again;
        do {
            bookTickets();
            cout << "\nDo you want to book again? (y/n): ";
            cin >> again;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (tolower(again) == 'y');

        cout << "\nThank you for using our system!\n";
    }

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

int main() {
    MovieSystem system;
    system.run();
    return 0;
}

