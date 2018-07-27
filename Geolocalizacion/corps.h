#ifndef CORPS_H
#define CORPS_H

#include "marker.h"
#include "bus.h"

struct Corps { //this save the object to show information in status and source widget
   Marker *marker;
   Bus *bus;
};
#endif // CORPS_H
