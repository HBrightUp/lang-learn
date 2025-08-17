#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_directory_clicked();
    void on_btn_play_clicked();
    void on_btn_next_clicked();
    void on_btn_prev_clicked();
    void on_list_music_doubleClicked(const QModelIndex &index);
    void on_btn_volume_clicked();
    void on_btn_theme_clicked();
    void update_player_list(const QString& path);
    void play_next();
    bool is_playable();

private:
    Ui::Widget *ui;
    QAudioOutput* audio_;
    QMediaPlayer* player_;
    QList<QUrl> playlist_;

    int current_play_index_;
    quint8 current_theme_;
};
#endif // WIDGET_H
