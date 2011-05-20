/**
  * This class contains static methods which act differently
    depending on the operating system.
  */

#include <cstdio>
#include <string.h>
#include "osoperations.h"

OSOperations::OSOperations()
{
}

/**
  * Returns a list of mounted drives/partitions.
  * @return QStringList containing paths..
  */

#if BUILD_PLATFORM == LINUX_BUILD

QStringList OSOperations::diskList()
{
    QStringList list;
    FILE *fptr;
    char buffer[1024];
    char dev[1024];
    char path[1024];
    char system[1024];
    char flags[1024];
    int a, b;

    fptr = fopen("/proc/mounts", "r");
    if (fptr == NULL)
        return list;

    while (fgets(buffer, 1024, fptr))
    {
        sscanf(buffer, "%s %s %s %s %d %d",
               dev, path, system, flags, &a, &b);

        QString qDev(dev), qPath(path);
        qPath.replace("\\040", " ");

        if (qDev.startsWith("/dev") || qDev == "/")
        if (qPath != "/dev")
            list.append(qPath);
    }

    fclose(fptr);
    return list;
}

#elif BUILD_PLATFORM == WINDOWS_BUILD

QStringList OSOperations::diskList()
{
    return QStringList();
}

#else
#error UNSUPPORTED PLATFORM
#endif
