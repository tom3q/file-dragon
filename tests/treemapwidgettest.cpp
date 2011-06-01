#include <algorithm>
#include <QtTest/QtTest>
#include <QObject>
#include <QSet>
#include "../treemapwidget.h"
#include "../abstractrenderer.h"
#include "../filenode.h"
#include "../directorynode.h"
#include "../defaultrenderer.h"

using namespace std;

class TreemapWidgetTest : public QObject
{
	Q_OBJECT

	class TestRenderer : public DefaultRenderer
	{
	public:
		TestRenderer(QHash<FileNode *, QRectF> *n, TreemapWidget *w) : nodes(n), DefaultRenderer(w) {}

	private:
		virtual void renderCell(QPainter &painter, QRectF &rect, FileNode *file)
		{
			QVERIFY(nodes->find(file) != nodes->end());
			nodes->operator[](file) = rect;
			DefaultRenderer::renderCell(painter, rect, file);
		}

		QHash<FileNode *, QRectF> *nodes;
	};

	static const int NUM_FILES = 16;
	static const int NUM_SUBDIRS = 4;
	static const int MAX_SIZE = 2048;
	static const int MIN_SIZE = 1024;
	static const int MAX_DEPTH = 4;

	qint64 generateTree(DirectoryNode *tree, unsigned depth, int &count)
	{
		qint64 sum = 0;

		for (int i = 0; i < NUM_FILES; ++i) {
			FileNode *f = new FileNode();
			qint64 size = MIN_SIZE + i*((MAX_SIZE - MIN_SIZE)/(NUM_FILES-1));
			f->setName(tree->getName() + QString("/File%1").arg(i));
			f->setSize(size);
			f->setModified(QDateTime::currentDateTime());
			tree->addFile(f);
			nodes.insert(f, QRectF());
			sum += size;
		}

		count = NUM_FILES;

		if (depth > 1)
		{
			for (int i = 0; i < NUM_SUBDIRS; ++i) {
				DirectoryNode *d = tree->addDir(tree->getName() + QString("/Subdir%1").arg(i));
				int subCount = 0;
				qint64 size = generateTree(d, depth - 1, subCount);
				count += subCount;
				sum += size;
			}
		}

		tree->setSize(sum);
		return sum;
	}

	class SelectNodeFunc
	{
		TreemapWidget *widget;
	public:
		SelectNodeFunc(TreemapWidget *w) : widget(w) {}

		void operator()(FileNode *f, QRectF r)
		{
			QRect alignedRect = r.toAlignedRect();
			if (alignedRect.width() == 0 || alignedRect.height() == 0)
				return;
			QTest::mouseClick(widget, Qt::MouseButton::LeftButton, 0, alignedRect.center());
			QCOMPARE(widget->getSelectedCount(), 1);
			QCOMPARE(widget->getSelected(0), f);
			QCOMPARE(widget->getSelectedFiles().size(), (unsigned)1);
			QCOMPARE(*(widget->getSelectedFiles().begin()), f);
		}
	};

	class SelectAllFunc
	{
		TreemapWidget *widget;
	public:
		SelectAllFunc(TreemapWidget *w) : widget(w) {}

		void operator()(FileNode *f, QRectF r)
		{
			QRect alignedRect = r.toAlignedRect();
			if (alignedRect.width() == 0 || alignedRect.height() == 0)
				return;
			QTest::mouseClick(widget, Qt::MouseButton::LeftButton, Qt::KeyboardModifier::ControlModifier, r.toAlignedRect().center());
		}
	};


	QHash<FileNode *, QRectF> nodes;
	TreemapWidget *widget;
	TestRenderer *renderer;
	DirectoryNode *root;
	int expectedFileCount;

private slots:
	void initTestCase()
	{
		// Prepare the widget
		widget = new TreemapWidget();
		QVERIFY(widget != 0);
		widget->resize(800, 600);
		widget->show();
		// Prepare the renderer
		renderer = new TestRenderer(&nodes, widget);
		QVERIFY(renderer != 0);
		widget->setRenderer(renderer);
		// Prepare sample directory tree
		root = widget->getFileTree().getRoot();
		QVERIFY(root != 0);
		root->setName("root");
		qint64 size = generateTree(root, MAX_DEPTH, expectedFileCount);
		root->setSize(size);
		//dumpTree(root);
	}

	void renderingTest()
	{
		widget->fileTreeUpdated();
		widget->repaint();
	}

	void selectionTest()
	{
		QHash<FileNode *, QRectF>::const_iterator it;

		// Select each single node and check the selection
		SelectNodeFunc f1 = SelectNodeFunc(widget);
		for (it = nodes.constBegin(); it != nodes.constEnd(); ++it)
			f1(it.key(), it.value());
		widget->removeSelection();

		// Select all nodes
		SelectAllFunc f2 = SelectAllFunc(widget);
		for (it = nodes.constBegin(); it != nodes.constEnd(); ++it)
			f2(it.key(), it.value());
		// Check the selection
		QHash<FileNode *, QRectF> hash(nodes);
		for (int i = 0; i < widget->getSelectedCount(); ++i)
		{
			QHash<FileNode *, QRectF>::iterator it;
			it = hash.find(widget->getSelected(i));
			QVERIFY(it != hash.end());
			hash.erase(it);
		}
		for (it = hash.constBegin(); it != hash.constEnd(); ++it)
		{
			QRect alignedRect = it.value().toAlignedRect();
			QVERIFY(alignedRect.width() == 0 || alignedRect.height() == 0);
		}
	}

	void cleanupTestCase()
	{
		delete widget;
	}
};

QTEST_MAIN(TreemapWidgetTest)
#include "treemapwidgettest.moc"
