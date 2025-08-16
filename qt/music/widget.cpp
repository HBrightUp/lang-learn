#include "widget.h"
#include "./ui_widget.h"
#include<QDebug>
#include<QFileDialog>
#include<QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        ui->lab_total->setText(QString("%1:%2").arg(duration / 1000 / 60, 2, 10, QChar('0')).arg(duration / 1000 % 60));
        ui->music_slider->setRange(0, duration);
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        ui->lab_pos->setText(QString("%1:%2").arg(pos / 1000 / 60, 2, 10, QChar('0')).arg(pos / 1000 % 60));
        ui->music_slider->setValue(pos);
    });

    connect(ui->music_slider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    audioOutput->setVolume(0.5);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_directory_clicked()
{
    qInfo() << "hello, bright" ;

    auto path = QFileDialog::getExistingDirectory(this, "Select directory of music:", "/home/hml/Downloads");
    qInfo()<< path;

    QDir dir(path);
    auto music_list = dir.entryList(QStringList() << "*.mp3" <<"*.wav");
    qInfo() << music_list;

    ui->list_music->addItems(music_list);
    ui->list_music->setCurrentRow(0);

    for(auto& file : music_list) {
        play_list.append(QUrl::fromLocalFile(path + "/"+ file));
    }

}


void Widget::on_btn_play_clicked()
{
    if (play_list.empty()) {
        return ;
    }

    switch (mediaPlayer->playbackState()) {
    case QMediaPlayer::PlaybackState::StoppedState:
    {
        current_play_index = ui->list_music->currentRow();
        mediaPlayer->setSource(play_list[current_play_index]);
        mediaPlayer->play();
        break;
    }
    case QMediaPlayer::PlaybackState::PlayingState:
        mediaPlayer->pause();
        break;
    case QMediaPlayer::PlaybackState::PausedState:
        mediaPlayer->play();
        break;
    default:
        break;
    }




}


void Widget::on_btn_next_clicked()
{
    current_play_index = (current_play_index + 1) % play_list.size();
    ui->list_music->setCurrentRow(current_play_index);
    mediaPlayer->setSource(play_list[current_play_index]);
    mediaPlayer->play();
}


void Widget::on_btn_prev_clicked()
{
    current_play_index = (current_play_index - 1) % play_list.size();
    ui->list_music->setCurrentRow(current_play_index);
    mediaPlayer->setSource(play_list[current_play_index]);
    mediaPlayer->play();
}


void Widget::on_list_music_doubleClicked(const QModelIndex &index)
{
    current_play_index = index.row();
    mediaPlayer->setSource(play_list[current_play_index]);
    mediaPlayer->play();
}

