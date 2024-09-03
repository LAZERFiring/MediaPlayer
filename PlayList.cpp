#include "PlayList.h"

PlayList::PlayList(QObject* parent)
	:QObject(parent)
	,MusicplayList(new QList<QDir>)
	,currentIndex(0)
	,maxIndex(0)
{

}

void PlayList::addUrl(QDir SoursePath)
{
	this->MusicplayList->append(SoursePath);
	flushIndex();
}

void PlayList::addUrl(QList<QDir> dirList)
{
	MusicplayList->append(dirList);
	flushIndex();
}

void PlayList::flushIndex()
{
	currentIndex = 0;
	maxIndex = this->MusicplayList->size()-1;
}

void PlayList::preMusic()
{
	if (--currentIndex < 0)
	{
		currentIndex = maxIndex;
	}
}

void PlayList::nextMusic()
{
	if (++currentIndex > maxIndex)
	{
		currentIndex = 0;
	}
}