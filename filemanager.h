#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QThread>
#include <QString>
#include <set>
#include "filenode.h"

class FileManager : public QThread
{
	Q_OBJECT

public:
	FileManager();
	~FileManager();

	const QStringList getQueuedFiles() const;

	/**
	*	Cancels pending operation.
	*/
	inline void cancel()
	{
		shouldCancel_ = true;
	}

public slots:
	void queueFiles(const QStringList &);
	void clear();
	void flush();

signals:
	void nowRemoving(const QString &);
	void progressUpdated(int progress);
	void done();

private:
	std::set<QString> queuedFiles_;
	bool shouldCancel_;
};

#endif
