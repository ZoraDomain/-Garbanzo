#include "signalmapper.h"
#include "coordinate.h"

SignalMapper::SignalMapper(QObject *parent) : QObject(parent) {
}

SignalMapper::~SignalMapper() {
}

void SignalMapper::setMapping(QObject *sender, int index) {
    indexHash.insert(sender, index);
}

QObject *SignalMapper::mapping(int index) const {
    return indexHash.key(index);
}

void SignalMapper::removeMapping(QObject *sender) {
    indexHash.remove(sender);
}

void SignalMapper::map() { map(sender());}

void SignalMapper::map(QObject *sender) {
    if (indexHash.contains(sender)) {
        emit mapped(indexHash.value(sender));
    }
}
