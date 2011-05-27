/**
  * This class contains static methods which act differently
    depending on the operating system.
  */

#include "osoperations.h"

OSOperations::OSOperations()
{
}

/**
  * Returns a list of mounted drives/partitions.
  * @return QStringList containing paths..
  */

#if defined(__linux__)

#include <cstdio>
#include <cstring>
#include <cassert>

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

qint64 OSOperations::getUsedSpace(const QString &disk)
{
	assert(0); // unimplemented
}

#elif defined(_WINDOWS)

#include <QDir>
#include <QFile>
#include <Windows.h>

using namespace std;

QStringList OSOperations::diskList()
{
	QFileInfoList flist = QDir::drives();
	QFileInfoList::const_iterator i;
	QStringList list;

	for (i = flist.constBegin(); i != flist.end(); ++i)
		list.append(i->absolutePath());

	return list;
}

qint64 OSOperations::getUsedSpace(const QString &disk)
{
	ULARGE_INTEGER total, free;
	GetDiskFreeSpaceExW(disk.toStdWString().c_str(), &free, &total, 0);
	return total.QuadPart - free.QuadPart;
}

#else
#error UNSUPPORTED PLATFORM
#endif
