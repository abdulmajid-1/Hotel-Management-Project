# Hotel Management System (Data Structures)

## Data Model

![data model of hotel management system](/images/hotel-data-model.jpg)

**Root** of our data model is Hotel Node. This is like a NULL node, this doesn't do anything, this is just the <ins>_starting point_</ins> of our application.

Our Tree just has **1 level**. On this level, there can be infinite nodes, thus there can be infinite Floors.

Each Floor contains a LinkedList of **Room Nodes**. There can be infinite rooms. Was this a good choice of using LinkedList instead of Array? In my opinion this was not a good choice, a simple array was better choice, because we are adding elements to the end of array.

## Folder Structure

```
.
├── data
│   ├── bookings.txt
│   ├── floors.txt
│   ├── rooms.txt
│   └── users.txt
├── images
│   └── hotel-data-model.jpg
├── lines.sh
├── main.cpp
├── models
│   ├── booking.hpp
│   ├── hotel.hpp
│   ├── persistor.cpp
│   ├── persistor.hpp
│   ├── user.cpp
│   └── user.hpp
├── README.md
├── run.sh
└── utilities
    ├── date.hpp
    ├── utils.cpp
    └── utils.hpp
```

- **main.cpp** is the starting point of application.
- **Models** contains the all the functionalities related to data handling, like _booking_ data, _users_ data and _hotel_ data.
- **Utilities** contains all the normal utils and and Date related Utils.
- **lines.sh** will show how many lines are in our project.
- **Data** folder has all the data that is handled through File Handling.
