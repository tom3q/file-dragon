/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	osoperations.cpp
*	OSOperations class implementation.
*/

#include "osoperations.h"

#if defined(__linux__)

#include <cstdio>
#include <cstring>
#include <sys/statvfs.h>

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
	struct statvfs fiData;
	char buffer[1024];

	strcpy(buffer, disk.toStdString().c_str());
	statvfs(buffer, &fiData);
	return (qint64) (fiData.f_blocks-fiData.f_bfree)*fiData.f_bsize;
}

#elif defined(_WIN32)

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
