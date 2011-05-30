#ifndef FileFrame_H
#define FileFrame_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

#include "filenode.h"

class FileFrame : public QFrame
{
public:
	static const int NAME = 1;
	static const int SIZE = 2;
	static const int MOD_DATE = 4;

	FileFrame(QWidget *parent);
	~FileFrame();

	void updateInfo(FileNode *file);
	void setFlags(int flags);

private:
	QLabel *lblName_;
	QLabel *lblSize_;
	QLabel *lblModDate_;
	QGridLayout *layout_;
};

#endif // FileFrame_H
