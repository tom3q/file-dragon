#include <QFileInfo>
#include <QDir>
#include "filemanager.h"

using namespace std;

FileManager::FileManager()
{
	start();
	QObject::moveToThread(this);
}

FileManager::~FileManager()
{
	this->quit();
	this->wait();
}

void FileManager::queueFiles(const QStringList &files)
{
	QStringList::const_iterator i;

	for (i = files.constBegin(); i != files.constEnd(); ++i)
		queuedFiles_.insert(*i);
}

const QStringList FileManager::getQueuedFiles() const
{
	QStringList files;
	std::set<QString>::const_iterator i; 

	files.reserve(queuedFiles_.size());

	for (i = queuedFiles_.begin(); i != queuedFiles_.end(); ++i)
		files.push_back(*i);

	return files;
}

void FileManager::clear()
{
	queuedFiles_.clear();
}

void FileManager::flush()
{
	shouldCancel_ = false;

	if (queuedFiles_.empty()) {
		emit done();
		return;
	}

	emit progressUpdated(0);

	std::set<QString>::const_iterator i;
	int count = queuedFiles_.size(), cur = 0;

	for (i = queuedFiles_.begin(); i != queuedFiles_.end(); ++i)
	{
		QFileInfo info(*i);
		QDir dir = info.dir();

		const QString path = info.absoluteFilePath();
		emit nowRemoving(path);
		dir.remove(path);

		emit progressUpdated((100 * ++cur) / count);

		if (shouldCancel_)
			break;
	}

	queuedFiles_.erase(queuedFiles_.begin(), i);

	emit progressUpdated(100);
	emit done();
}
