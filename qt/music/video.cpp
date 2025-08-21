#include<QMediaPlayer>
#include<QVideoWidget>
#include<QPushButton>
#include<QSlider>
#include<QLabel>
#include<QVBoxLayout>
#include "video.h"
#include "ui_video.h"



Video::Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Video)
{
    ui->setupUi(this);
    this->setWindowTitle("Movie Theater");


    player = new QMediaPlayer(this);
    QVideoWidget *videoWidget = new QVideoWidget(this);
    QPushButton *playButton = new QPushButton("Play", this);
    QPushButton *pauseButton = new QPushButton("Pause", this);
    QPushButton *stopButton = new QPushButton("Stop", this);
    QSlider *volumeSlider = new QSlider(Qt::Horizontal, this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    currentTimeLabel = new QLabel("00:00", this);
    totalTimeLabel = new QLabel("00:00", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    layout->addWidget(playButton);
    layout->addWidget(pauseButton);
    layout->addWidget(stopButton);
    layout->addWidget(volumeSlider);
    layout->addWidget(progressSlider);
    layout->addWidget(currentTimeLabel);
    layout->addWidget(totalTimeLabel);

    setLayout(layout);

    player->setVideoOutput(videoWidget);

    connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
    connect(stopButton, &QPushButton::clicked, player, &QMediaPlayer::stop);

    connect(player, &QMediaPlayer::durationChanged, this, &Video::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Video::positionChanged);

    connect(progressSlider, &QSlider::sliderMoved, this, &Video::seek);

    connect(volumeSlider, &QSlider::valueChanged, player, &QMediaPlayer::durationChanged);

}

Video::~Video()
{
    delete ui;
}

void Video::durationChanged(qint64 duration) {
    progressSlider->setRange(0, duration);
    totalTimeLabel->setText(formatTime(duration));
}

void Video::positionChanged(qint64 position) {
    progressSlider->setValue(position);
    currentTimeLabel->setText(formatTime(position));
}

void Video::seek(int seconds) {
    player->setPosition(seconds);
}

QString Video::formatTime(qint64 timeMs) {
    qint64 seconds = timeMs / 1000;
    qint64 minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
