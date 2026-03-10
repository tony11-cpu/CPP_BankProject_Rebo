C++ Banking System
My first ever C++ project — a full console-based banking management system built from scratch.
What It Does
The system has two sides:
Bank Management (Staff Login)

Add, delete, update, and search clients
Deposit and withdraw money
View all client balances and total balance
Manage system users and their permissions
Role-based access control (each user only sees what they're allowed to)

ATM (Client Login)

Quick withdraw (preset amounts: 20, 50, 100...)
Normal withdraw (any amount, must be multiple of 5)
Deposit money
Check balance
View past transactions history

How to Run

Open the project in Visual Studio
Press F5 to build and run
On startup choose between ATM mode or Bank Management mode

How Data is Stored
All data is saved in plain .txt files using a custom #//#  separator format:

Clients.txt — stores all client records
Users.txt — stores all system users and their permissions
OldTransactions.txt — stores transaction history with dates

What I Learned Building This

File handling with fstream (read, write, append)
Structs to model real-world data (clients, users, dates)
Enums for menus and permissions
Bitwise permissions system (each permission is a power of 2)
Vectors for in-memory data management
String parsing and splitting manually
Console UI with formatted tables using setw and iomanip

Built With

C++
Visual Studio
No external libraries — everything from scratch
