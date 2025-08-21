#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include<QLabel>;
#include<QMediaPlayer>

namespace Ui {
class Video;
}

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();


private:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void seek(int seconds);
    QString formatTime(qint64 timeMs);

private:
    Ui::Video *ui;

    QSlider *progressSlider;
    QLabel *totalTimeLabel;
    QLabel *currentTimeLabel;
    QMediaPlayer *player;
};

#endif // VIDEO_H
