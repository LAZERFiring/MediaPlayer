#include "Mainwindow.h"
#include <dwmapi.h>

MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
	,ui(new Ui::MainWindow)
	,mediaPlayer(new QMediaPlayer(this))
	,audioOutput(new QAudioOutput(this))
	,videoWidget(new QVideoWidget)
	,playList(new PlayList(this))
{
	ui->setupUi(this);
	init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::init()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowTitle("MediaPlayer");
	setFixedSize(QSize(1015, 606));
	installEventFilter(this);
	ui->VolumeSlider->hide();
	ui->VolumeSlider->setRange(0, 100);
	videoWidget->setParent(ui->MediaArea);
	videoWidget->resize(ui->MediaArea->width(),ui->MediaArea->height()-25);
	videoWidget->hide();
	mediaPlayer->setAudioOutput(audioOutput);
	mediaPlayer->setVideoOutput(videoWidget);
	audioOutput->setVolume(0.15);
	ui->VolumeSlider->setValue(0.15 * 100);
	ui->CloseBtn->setText("");
	ui->CloseBtn->setIcon(QIcon("./Assests/Icon/redclose.png"));
	ui->HideBtn->setIcon(QIcon("./Assests/Icon/hide.png"));
	if (LoadMedia("./Assests/music"))
	{
		printf("music load successful!\n");
	}
	ui->PlayListWidget->setCurrentRow(0);
	QObject::connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
	QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
	QObject::connect(ui->timeSlider, &QSlider::sliderMoved, this, &MainWindow::on_sliderMoved);
	QObject::connect(ui->VolumeSlider, &QSlider::sliderMoved, this, &MainWindow::on_volumesliderMoved);
	QObject::connect(ui->PlayListWidget, &QListWidget::currentItemChanged, this, &MainWindow::on_currentItemChanged);
	QObject::connect(playList, &PlayList::currentIndexChange, this, &MainWindow::on_currentIndexChanged);
	QObject::connect(ui->CloseBtn, &QPushButton::clicked, this, &MainWindow::on_CloseBtnReleased);
	QObject::connect(ui->HideBtn, &QPushButton::clicked, this, &MainWindow::on_HideBtnReleased);
}

bool MainWindow::LoadMedia(const QString& sourse)
{
	QDir dir(sourse);
	if (dir.exists())
	{
		auto list = dir.entryList(QStringList() << "*.mp3" << "*.mp4" << "*.flac" << "*.webm");
		qInfo() << "Music Lists:" << list;
		ui->PlayListWidget->addItems(list);
		for (auto& SoureName : list)
		{
			this->playList->addUrl(QDir(sourse +"/"+ SoureName));
		}
		cutMedia(this->playList->currentIndex);
		return true;
	}
	return false;
}

void MainWindow::cutMedia(int currentIndex)
{
	QUrl sourse = playList->MusicplayList->at(currentIndex).path();
	if (sourse.fileName().contains("mp4",Qt::CaseInsensitive)||sourse.fileName().contains("webm", Qt::CaseInsensitive))
	{
		videoWidget->show();
	}
	else
	{
		videoWidget->hide();
		VideoTipShow();
	}
	mediaPlayer->setSource(sourse);
	PlayPauseByBtnState();
}

void MainWindow::LoadMusicTime(qint64 duration, qint64 position)
{
	ui->timeLabel->setText(QString("%1:%2/%3:%4").
		arg(position / 1000 / 60, 2, 10, QChar('0')).
		arg(position / 1000 % 60, 2, 10, QChar('0')).
		arg(duration / 1000 / 60, 2, 10, QChar('0')).
		arg(duration / 1000 % 60, 2, 10, QChar('0')));
}

void MainWindow::PlayPauseByBtnState()
{
	if (ui->PlayAndPauseMusicBtn->text() == "Play")
		mediaPlayer->pause();
	else
		mediaPlayer->play();
}

void MainWindow::VideoTipShow()
{
	ui->VideoTip->setStyleSheet("QLabel {"
                            "    font: bold italic;"
                            "    font-size: 40px; /* À©´óÁ½±¶ */"
                            "    color: rgba(255, 255, 255, 128);"
                            "    background-color: rgba(0, 0, 0, 0); /* Í¸Ã÷±³¾° */"
                            "    border: none; /* È¥µô±ß¿ò */"
                            "    border-radius: 5px;"
                            "    padding: 20px;"
                            "    text-align: center;"
		"}");
	ui->VideoTip->setText("NO VIDEO INPUT");
	ui->VideoTip->setFixedSize(450, 101);
}

void MainWindow::on_PreMusicBtn_released()
{
	playList->preMusic();
	cutMedia(playList->currentIndex);
}

void MainWindow::on_NextMusicBtn_released()
{
	playList->nextMusic();
	cutMedia(playList->currentIndex);
}

void MainWindow::on_PlayAndPauseMusicBtn_released()
{
	if (!mediaPlayer->isPlaying())
	{
		ui->PlayAndPauseMusicBtn->setText(QString("Pause"));
		mediaPlayer->play();
	}
	else
	{
		mediaPlayer->pause();
		ui->PlayAndPauseMusicBtn->setText(QString("Play"));
	}
}

void MainWindow::on_VolumeChangeBtn_released()
{
	ui->VolumeSlider->show();
	ui->VolumeSlider->raise();
}

void MainWindow::on_durationChanged()
{
	LoadMusicTime(mediaPlayer->duration(), mediaPlayer->position());
	ui->timeSlider->setRange(0, mediaPlayer->duration());
}

void MainWindow::on_positionChanged(qint64 position)
{
	LoadMusicTime(mediaPlayer->duration(), mediaPlayer->position());
	ui->timeSlider->setValue(mediaPlayer->position());
}

void MainWindow::on_sliderMoved(int position)
{
	mediaPlayer->setPosition(position);
}

void MainWindow::on_volumesliderMoved(int value)
{
	audioOutput->setVolume(((float)value) / 100);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	if (!ui->VolumeSlider->rect().contains(event->pos()))
	{
		ui->VolumeSlider->hide();
	}
}

void MainWindow::on_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	for (size_t i = 0; i < playList->MusicplayList->size(); i++)
	{
		if (playList->MusicplayList->at(i).dirName() == current->text())
		{
			playList->currentIndex = i;
		}
	}
	emit playList->currentIndexChange(playList->currentIndex);
}

void MainWindow::on_currentIndexChanged(int changeIndex)
{
	cutMedia(changeIndex);
	PlayPauseByBtnState();
}

void MainWindow::on_CloseBtnReleased()
{
	this->close();
}

void MainWindow::on_HideBtnReleased()
{
	
}

bool MainWindow::eventFilter(QObject* obj, QEvent* ev)
{
	if (this->focusWidget() == static_cast<QWidget*>(ui->VolumeChangeBtn)|| 
		this->focusWidget() == static_cast<QWidget*>(ui->VolumeSlider))
	{
		ui->VolumeSlider->show();
	}
	else
	{
		ui->VolumeSlider->hide();
	}
	return QWidget::eventFilter(obj,ev);
}
