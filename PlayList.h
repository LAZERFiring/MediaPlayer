#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QObject>
#include <QDir>
#include <QList>
class PlayList : public QObject
{
	Q_OBJECT
public:
	PlayList(QObject* parent = nullptr);
	void addUrl(QDir SoursePath);
	void addUrl(QList<QDir> dirList);
	void flushIndex();
signals:
	void currentIndexChange(int changeIndex);
public slots:
	void preMusic();
	void nextMusic();
public:
	QList<QDir>* MusicplayList;
	int currentIndex;
	int maxIndex;
};
#endif // !PLAYLIST_H