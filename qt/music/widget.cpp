#include "widget.h"
#include "./ui_widget.h"
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QIcon>
#include<QSize>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    current_play_index_ = 0;

    ui->lab_bk->setPixmap(QPixmap(":/bk1.png"));

    audio_ = new QAudioOutput(this);
    player_ = new QMediaPlayer(this);
    player_->setAudioOutput(audio_);

    connect(player_, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        ui->lab_total->setText(QString("%1:%2").arg(duration / 1000 / 60, 2, 10, QChar('0')).arg(duration / 1000 % 60));
        ui->music_slider->setRange(0, duration);
    });

    connect(player_, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        ui->lab_pos->setText(QString("%1:%2").arg(pos / 1000 / 60, 2, 10, QChar('0')).arg(pos / 1000 % 60));
        ui->music_slider->setValue(pos);
    });

    connect(player_, &QMediaPlayer::mediaStatusChanged,[=](QMediaPlayer::MediaStatus status){
       if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
            qInfo() << "end of media";
            play_next();
       }
    });

    // connect(player_, &QMediaPlayer::playbackStateChanged,[=](QMediaPlayer::PlaybackState newState){
    //     qInfo() << newState;
    //     if (newState == QMediaPlayer::PlaybackState::StoppedState || newState == QMediaPlayer::PlaybackState::PausedState) {
    //         ui->btn_play->setIcon(QIcon(":/assets/pause-48.png"));
    //     } else if(newState == QMediaPlayer::PlaybackState::PlayingState) {
    //         ui->btn_play->setIcon(QIcon(":/assets/play-48.png"));
    //     } else {

    //     }
    // });

    connect(ui->music_slider, &QSlider::sliderMoved, player_, &QMediaPlayer::setPosition);

    audio_->setVolume(0.5);
    update_player_list(QDir::homePath() + "/music");

}

bool Widget::is_playable() {

    return !playlist_.empty();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_directory_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this, "Select directory of music:", "/home/hml/Downloads");
    update_player_list(path);
}
void Widget::update_player_list(const QString& path){
    QDir dir(path);
    auto music_list = dir.entryList(QStringList() << "*.mp3" <<"*.wav");
    qInfo() << music_list;

    ui->list_music->addItems(music_list);
    ui->list_music->setCurrentRow(0);

    for(auto& file : music_list) {
        playlist_.append(QUrl::fromLocalFile(path + "/"+ file));
    }
}

void Widget::on_btn_play_clicked()
{

    if (!is_playable()) {
        return ;
    }

    switch (player_->playbackState()) {
        case QMediaPlayer::PlaybackState::StoppedState:
        {
            current_play_index_ = ui->list_music->currentRow();
            qInfo() << current_play_index_;
            player_->setSource(playlist_[current_play_index_]);
            player_->play();
            break;
        }
        case QMediaPlayer::PlaybackState::PlayingState:
            player_->pause();
            break;
        case QMediaPlayer::PlaybackState::PausedState:
            player_->play();
            break;
        default:
            break;
    }




}


void Widget::on_btn_next_clicked()
{
    qInfo() << is_playable() ;
    if (!is_playable()) {
            return ;
        }

    play_next();
}
void Widget::play_next() {
    qInfo() << current_play_index_ ;

    qInfo() << playlist_.size() ;

    current_play_index_ = (current_play_index_ + 1) % playlist_.size();
    qInfo() << current_play_index_ ;
    qInfo() << "1111";
    ui->list_music->setCurrentRow(current_play_index_);
    player_->setSource(playlist_[current_play_index_]);
    player_->play();
}


void Widget::on_btn_prev_clicked()
{
    if (!is_playable()) {
            return ;
    }

    if (current_play_index_ == 0) {
        current_play_index_ = playlist_.size() - 1;
    } else {
        current_play_index_ = (current_play_index_ - 1) % playlist_.size();
    }

    qInfo() << current_play_index_;
    ui->list_music->setCurrentRow(current_play_index_);
    player_->setSource(playlist_[current_play_index_]);
    player_->play();
}


void Widget::on_list_music_doubleClicked(const QModelIndex &index)
{
    if (!is_playable()) {
            return ;
    }

    current_play_index_ = index.row();
    player_->setSource(playlist_[current_play_index_]);
    player_->play();
}


void Widget::on_btn_volume_clicked()
{
    bool is_muted = audio_->isMuted();

    qInfo() << is_muted;
    if (is_muted) {
        audio_->setMuted(false);
    } else {
        audio_->setMuted(true);
    }
}

