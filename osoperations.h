/*
*	FILE DRAGON
*	A disk space management utility.
*
*	Developed by:
*	-> Tomasz Figa
*	-> Piotr T¹kiel
*
*	osoperations.h
*	OSOperations class prototype.
*/

#ifndef OSOPERATIONS_H
#define OSOPERATIONS_H

#include <QStringList>

/**
 * This class contains static methods which act differently
 * depending on the operating system.
 */
class OSOperations
{
	/* Private constructor to deny creation of instances */
	OSOperations();

public:
	/**
	 * Returns a list of mounted drives/partitions.
	 * @return QStringList containing paths..
	 */
	static QStringList diskList();

	/**
	 * Returns the total number of bytes used on the disk.
	 * @param disk Disk mount point.
	 * @return Total used space in bytes.
	 */
	static qint64 getUsedSpace(const QString &disk);
};

#endif // OSOPERATIONS_H
