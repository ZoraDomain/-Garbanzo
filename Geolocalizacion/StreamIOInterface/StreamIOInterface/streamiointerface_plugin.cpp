#include "streamiointerface_plugin.h"
#include "streamio.h"

#include <qqml.h>

void StreamIOInterfacePlugin::registerTypes(const char *uri)
{
    // @uri com.maker.StreamIO
    qmlRegisterType<StreamIO>(uri, 1, 0, "StreamIO");
}

