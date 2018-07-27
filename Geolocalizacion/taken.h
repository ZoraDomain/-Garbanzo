#ifndef TAKEN_H
#define TAKEN_H

struct Taken { ///this save the status of the object to planner
    int index;
    bool isReadable;
    bool isWasRead;
    bool isThereSomeone;
};

#endif // TAKEN_H
