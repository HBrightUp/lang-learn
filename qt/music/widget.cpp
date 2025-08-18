#include "widget.h"
#include "./ui_widget.h"
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QIcon>
#include<QSize>
#include<QFontDatabase>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    current_play_index_ = 0;
    current_theme_ = 0;

    // set background for application.
    ui->lab_bk->setPixmap(QPixmap(":/bk1.png"));


    //  load font for QListWidget
    int fontId = QFontDatabase::addApplicationFont(":/jianglan.ttf");
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()){
            QString familyName = fontFamilies.first();
            QFont font(familyName);
            font.setPointSize(14);
            font.setWeight(QFont::Weight::Light);
            ui->list_music->setFont(font);

        }
    }

    // load media and audio
    //audio_(new QAudioOutput(this));
    audio_.reset(new QAudioOutput(this));

    player_.reset(new QMediaPlayer(this));
    //player_ = new QMediaPlayer(this);
    player_.get()->setAudioOutput(audio_.get());
    set_mute(false);

    // listen the progress of player and set total length of slider .
    connect(player_.get(), &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        ui->lab_total->setText(QString("%1:%2").arg(duration / 1000 / 60, 2, 10, QChar('0')).arg(duration / 1000 % 60, 2, 10, QChar('0')));
        ui->music_slider->setRange(0, duration);
    });

    // Set progressplayer_list_ bar of slider in real time
    connect(player_.get(), &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        ui->lab_pos->setText(QString("%1:%2").arg(pos / 1000 / 60, 2, 10, QChar('0')).arg(pos / 1000 % 60, 2, 10, QChar('0')));
        if (!is_silder_pressed_) {
            ui->music_slider->setValue(pos);
        }
    });

    //  Monitor the playing status of the music, and if one song is finished, play the next one.
    connect(player_.get(), &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
       if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
            play_next();
       }
    });

    connect(player_.get(), &QMediaPlayer::playbackStateChanged,[=](QMediaPlayer::PlaybackState newState){
        if (newState == QMediaPlayer::PlaybackState::StoppedState || newState == QMediaPlayer::PlaybackState::PausedState) {
            ui->btn_play->setIcon(QIcon(":/pause-96.png"));
            ui->btn_play->setIconSize(ui->btn_play->size());
        } else if(newState == QMediaPlayer::PlaybackState::PlayingState) {
            ui->btn_play->setIcon(QIcon(":/play-96.png"));
            ui->btn_play->setIconSize(ui->btn_play->size());
        } else {

        }
    });


    //  Listen for users dragging the slider to play music.
    //connect(ui->music_slider, &QSlider::sliderMoved, player_, &QMediaPlayer::setPosition);
    connect(ui->music_slider, &QSlider::sliderPressed, this, [=]() {
        is_silder_pressed_ = true;
    });
    connect(ui->music_slider, &QSlider::sliderReleased, this, [=]() {
        int pos = ui->music_slider->value();
        is_silder_pressed_ = false;
        player_->setPosition(pos);
    });


    // set volume(0.00~1.00)  valueChanged
    audio_->setVolume(0.5);

    // set default director of music.
    update_player_list(QDir::homePath() + "/Music");

    QString strStyle = "QListWidget{font-size:18px;   "
                        "color: darkBlue; background:#00000000;"
                        "padding-left:0px;"
                        "border: none solid none}"
                        "QListWidget::item{height:30px; }"
                        /*列表项扫过时文本、背景变化*/
                        "QListWidget::item:hover{color: darkGreen; background: #FFC0CB;}"
                        /**列表项选中*/
                        "QListWidget::item::selected{ color: white; background: #b4446c;}";

    ui->list_music->setStyleSheet(strStyle);
}

bool Widget::is_playable() {
    return !playlist_.empty();
}

Widget::~Widget()
{
    player_->stop();
    playlist_.clear();
    delete ui;
}

void Widget::on_btn_directory_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this, "Select directory of music:", "/home/hml/Music");
    update_player_list(path);
}
void Widget::update_player_list(const QString& path){

    if(!playlist_.empty()) {
        playlist_.clear();
        ui->list_music->clear();
    }

    QDir dir(path);
    QStringList music_list = dir.entryList(QStringList() << "*.mp3" <<"*.wav");
    for(auto& file : music_list) {
        playlist_.append(QUrl::fromLocalFile(path + "/"+ file));
    }

    for(auto& list : music_list) {
        ui->list_music->addItem(list.left(list.size() - 4));
    }

    ui->list_music->setCurrentRow(0);
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
    if (!is_playable()) {
            return ;
        }

    play_next();
}
void Widget::play_next() {

    current_play_index_ = (current_play_index_ + 1) % playlist_.size();
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
    ui->list_music->setCurrentRow(current_play_index_);

    player_->setSource(playlist_[current_play_index_]);
    player_->play();
}


void Widget::on_btn_volume_clicked()
{
    set_mute(!audio_->isMuted());
    // if (is_muted) {
    //     audio_->setMuted(false);
    //     ui->btn_volume->setIcon(QIcon(":/volume.png"));
    //     ui->btn_volume->setIconSize(ui->btn_volume->size());
    // } else {
    //     audio_->setMuted(true);
    //     ui->btn_volume->setIcon(QIcon(":/volume-mute.png"));
    //     ui->btn_volume->setIconSize(ui->btn_volume->size());
    // }
}

void Widget::set_mute(bool mute) {
    if (mute) {
        audio_->setMuted(true);
        ui->btn_volume->setIcon(QIcon(":/volume-mute.png"));
        ui->btn_volume->setIconSize(ui->btn_volume->size());
    } else {
        audio_->setMuted(false);
        ui->btn_volume->setIcon(QIcon(":/volume.png"));
        ui->btn_volume->setIconSize(ui->btn_volume->size());
    }
}

void Widget::on_btn_theme_clicked()
{
    switch(current_theme_) {
        case 0: {
            QPixmap pixmap(":/bk2.jpg");
            QPixmap scaledPixmap = pixmap.scaled(360, 640, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            ui->lab_bk->setPixmap(scaledPixmap);
            current_theme_ = 1;
            break;
        }
        case 1: {
            QPixmap pixmap(":/bk3.jpg");
            QPixmap scaledPixmap = pixmap.scaled(ui->lab_bk->size(), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
            ui->lab_bk->setPixmap(scaledPixmap);
            current_theme_ = 2;
            break;
        }
        case 2: {
            QPixmap pixmap(":/bk4.jpg");
            QPixmap scaledPixmap = pixmap.scaled(ui->lab_bk->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            ui->lab_bk->setPixmap(scaledPixmap);
            current_theme_ = 3;
            break;
        }
        case 3: {
            QPixmap pixmap(":/bk5.jpg");
            QPixmap scaledPixmap = pixmap.scaled(ui->lab_bk->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            ui->lab_bk->setPixmap(scaledPixmap);
            current_theme_ = 4;
            break;
        }
        case 4: {
            QPixmap pixmap(":/bk1.png");
            QPixmap scaledPixmap = pixmap.scaled(ui->lab_bk->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            ui->lab_bk->setPixmap(scaledPixmap);
            current_theme_ = 0;
            break;
        }
    }

    ui->lab_bk->resize(360, 640);
}

