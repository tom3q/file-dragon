#ifndef OSOPERATIONS_H
#define OSOPERATIONS_H

#include <QStringList>

class OSOperations
{
public:
	OSOperations();

	static QStringList diskList();
	static qint64 getUsedSpace(const QString &);
};

#endif // OSOPERATIONS_H
