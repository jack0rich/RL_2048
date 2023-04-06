//
// Created by Jack Rich on 2023/3/24.
//

#ifndef RL_2048_GAME_H
#define RL_2048_GAME_H

#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <map>
#include "Board.h"


class QPushButton;
typedef std::map<int, std::string> color_map;
namespace game {
    class Game : public QWidget {
        Q_OBJECT
    public:
        explicit Game(QWidget *parent = 0);
        void keyPressEvent(QKeyEvent *event) override;
    private:
        int score = 0;
        QPushButton *m_button;
        QGridLayout *gameAreaLayout;
        QLabel *scoreLabel;
        QGridLayout *layout;
        QFont *font;
        board::Board *core;
        color_map board_colors{
                {1, "#EEE4DA"},{2, "#EDE0C8"},{3, "#F2B179"},{4, "#F59563"},
                {5, "#F67C5F"},{6, "#F65E3B"},{7, "#EDCF72"},{8, "#EDCC61"},
                {9, "#EDC850"},{10, "#EDC53F"},{11, "#EDC22E"},{12, "#3C3A32"},
                {13, "#3C3A32"},{14, "#3C3A32"},{15, "#3C3A32"},{16, "#3C3A32"},
                {17, "#3C3A32"},{18, "#3C3A32"},{19, "#3C3A32"},{20, "#3C3A32"},
        };

        void resetGame();
        void clearBoard();
        void loadBoard();
        void isGameOver();
        int right();
        int left();
        int up();
        int down();



    signals:
    public slots:
        void slotButtonClicked(bool checked);
        void updateScore();
    };

} // game

#endif //RL_2048_GAME_H
