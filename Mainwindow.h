#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QUrl>
#include <QString>
#include <QDir>
#include <QMouseEvent>
#include <QIcon>
#include "PlayList.h"
#include "ui_Mainwindow.h"
namespace Ui
{
	class MainWindow;
}
class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent=nullptr);
	~MainWindow();
	void init();
	bool LoadMedia(const QString& sourse);
	void cutMedia(int currentIndex);
	void LoadMusicTime(qint64 duration, qint64 position);
	void PlayPauseByBtnState();
	void VideoTipShow();
public slots:
	void on_PreMusicBtn_released();
	void on_NextMusicBtn_released();
	void on_PlayAndPauseMusicBtn_released();
	void on_VolumeChangeBtn_released();
	void on_durationChanged();
	void on_positionChanged(qint64 position);
	void on_sliderMoved(int position);
	void on_volumesliderMoved(int value);
	void on_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void on_currentIndexChanged(int changeIndex);
	void on_CloseBtnReleased();
	void on_HideBtnReleased();
protected:
	void mousePressEvent(QMouseEvent* event)override;
	bool eventFilter(QObject* obj, QEvent* ev)override;
private:
	Ui::MainWindow* ui;
	QMediaPlayer* mediaPlayer;
	QVideoWidget* videoWidget;
	QAudioOutput* audioOutput;
	PlayList* playList;
};


#endif //! MAINWINDOW_H