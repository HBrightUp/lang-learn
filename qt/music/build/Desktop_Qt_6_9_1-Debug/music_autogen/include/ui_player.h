/********************************************************************************
** Form generated from reading UI file 'player.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_H
#define UI_PLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Player
{
public:
    QWidget *widget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_directory;
    QPushButton *btn_theme;
    QPushButton *btn_playmode;
    QPushButton *btn_prev;
    QPushButton *btn_play;
    QPushButton *btn_next;
    QPushButton *btn_volume;
    QListWidget *list_music;
    QSlider *music_slider;
    QLabel *lab_pos;
    QLabel *lab_total;
    QLabel *lab_bk;

    void setupUi(QWidget *Player)
    {
        if (Player->objectName().isEmpty())
            Player->setObjectName("Player");
        Player->resize(360, 640);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Player->sizePolicy().hasHeightForWidth());
        Player->setSizePolicy(sizePolicy);
        Player->setMinimumSize(QSize(360, 640));
        Player->setMaximumSize(QSize(360, 640));
        Player->setBaseSize(QSize(360, 640));
        Player->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        QIcon icon;
        if (QIcon::hasThemeIcon(QIcon::ThemeIcon::FormatTextBold)) {
            icon = QIcon::fromTheme(QIcon::ThemeIcon::FormatTextBold);
        } else {
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Disabled, QIcon::State::Off);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Disabled, QIcon::State::On);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Active, QIcon::State::Off);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Active, QIcon::State::On);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Selected, QIcon::State::Off);
            icon.addFile(QString::fromUtf8(":/app-icon3.png"), QSize(), QIcon::Mode::Selected, QIcon::State::On);
        }
        Player->setWindowIcon(icon);
        Player->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        Player->setAutoFillBackground(true);
        Player->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(Player);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 560, 360, 80));
        widget->setMinimumSize(QSize(0, 80));
        widget->setMaximumSize(QSize(16777215, 80));
        widget->setBaseSize(QSize(0, 80));
        widget->setStyleSheet(QString::fromUtf8("*{\n"
"	border:none;\n"
"}\n"
"\n"
""));
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 41, 351, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_directory = new QPushButton(layoutWidget);
        btn_directory->setObjectName("btn_directory");
        btn_directory->setStyleSheet(QString::fromUtf8(""));
        btn_directory->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(btn_directory);

        btn_theme = new QPushButton(layoutWidget);
        btn_theme->setObjectName("btn_theme");
        btn_theme->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	image: url(:/app-icon.png);\n"
"}\n"
""));

        horizontalLayout->addWidget(btn_theme);

        btn_playmode = new QPushButton(layoutWidget);
        btn_playmode->setObjectName("btn_playmode");
        btn_playmode->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(btn_playmode);

        btn_prev = new QPushButton(layoutWidget);
        btn_prev->setObjectName("btn_prev");
        btn_prev->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/front-64.png)"));

        horizontalLayout->addWidget(btn_prev);

        btn_play = new QPushButton(layoutWidget);
        btn_play->setObjectName("btn_play");
        btn_play->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(btn_play);

        btn_next = new QPushButton(layoutWidget);
        btn_next->setObjectName("btn_next");
        btn_next->setStyleSheet(QString::fromUtf8("image:url(:/back-64.png)"));

        horizontalLayout->addWidget(btn_next);

        btn_volume = new QPushButton(layoutWidget);
        btn_volume->setObjectName("btn_volume");
        btn_volume->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(btn_volume);

        list_music = new QListWidget(Player);
        list_music->setObjectName("list_music");
        list_music->setGeometry(QRect(0, 0, 360, 540));
        sizePolicy.setHeightForWidth(list_music->sizePolicy().hasHeightForWidth());
        list_music->setSizePolicy(sizePolicy);
        list_music->setMinimumSize(QSize(360, 540));
        list_music->setMaximumSize(QSize(360, 540));
        list_music->setBaseSize(QSize(360, 540));
        list_music->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        list_music->setStyleSheet(QString::fromUtf8(""));
        list_music->setFrameShape(QFrame::Shape::NoFrame);
        list_music->setFrameShadow(QFrame::Shadow::Plain);
        music_slider = new QSlider(Player);
        music_slider->setObjectName("music_slider");
        music_slider->setGeometry(QRect(2, 580, 354, 16));
        music_slider->setOrientation(Qt::Orientation::Horizontal);
        lab_pos = new QLabel(Player);
        lab_pos->setObjectName("lab_pos");
        lab_pos->setGeometry(QRect(2, 560, 40, 21));
        sizePolicy.setHeightForWidth(lab_pos->sizePolicy().hasHeightForWidth());
        lab_pos->setSizePolicy(sizePolicy);
        lab_pos->setMinimumSize(QSize(40, 21));
        lab_pos->setMaximumSize(QSize(40, 21));
        QFont font;
        font.setFamilies({QString::fromUtf8("Noto Sans Kayah Li")});
        font.setBold(true);
        lab_pos->setFont(font);
        lab_pos->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        lab_pos->setStyleSheet(QString::fromUtf8("*{\n"
"	color: #b4446c;\n"
"}"));
        lab_total = new QLabel(Player);
        lab_total->setObjectName("lab_total");
        lab_total->setGeometry(QRect(318, 560, 40, 21));
        sizePolicy.setHeightForWidth(lab_total->sizePolicy().hasHeightForWidth());
        lab_total->setSizePolicy(sizePolicy);
        lab_total->setMinimumSize(QSize(40, 21));
        lab_total->setMaximumSize(QSize(40, 21));
        lab_total->setSizeIncrement(QSize(40, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Noto Sans Chakma")});
        font1.setBold(true);
        lab_total->setFont(font1);
        lab_total->setStyleSheet(QString::fromUtf8("*{\n"
"	color: #b4446c;\n"
"}"));
        lab_bk = new QLabel(Player);
        lab_bk->setObjectName("lab_bk");
        lab_bk->setGeometry(QRect(0, 0, 360, 640));
        sizePolicy.setHeightForWidth(lab_bk->sizePolicy().hasHeightForWidth());
        lab_bk->setSizePolicy(sizePolicy);
        lab_bk->setMinimumSize(QSize(360, 640));
        lab_bk->setMaximumSize(QSize(360, 640));
        lab_bk->setSizeIncrement(QSize(360, 640));
        lab_bk->raise();
        widget->raise();
        list_music->raise();
        music_slider->raise();
        lab_pos->raise();
        lab_total->raise();

        retranslateUi(Player);

        list_music->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(Player);
    } // setupUi

    void retranslateUi(QWidget *Player)
    {
        Player->setWindowTitle(QCoreApplication::translate("Player", "Music Player", nullptr));
#if QT_CONFIG(tooltip)
        btn_directory->setToolTip(QCoreApplication::translate("Player", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        btn_directory->setWhatsThis(QCoreApplication::translate("Player", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        btn_directory->setText(QString());
        btn_theme->setText(QString());
        btn_playmode->setText(QString());
        btn_prev->setText(QString());
        btn_play->setText(QString());
        btn_next->setText(QString());
        btn_volume->setText(QString());
        lab_pos->setText(QCoreApplication::translate("Player", "00:00", nullptr));
        lab_total->setText(QCoreApplication::translate("Player", "00:00", nullptr));
        lab_bk->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Player: public Ui_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_H
