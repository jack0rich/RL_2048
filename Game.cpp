//
// Created by Jack Rich on 2023/3/24.
//

#include "Game.h"
#include <iostream>
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPainterPath>
#include <QFontDatabase>
#include <cmath>
#include <QMessageBox>


namespace game {
    Game::Game(QWidget *parent) :
            QWidget(parent) {

        this->core = new board::Board();

        QString path = "/Users/jackrich/CLionProjects/RL_2048/Content/SmileySans-Oblique.ttf";
        int fontId= QFontDatabase::addApplicationFont(path);
        std::cout << fontId;
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        font = new QFont(fontFamily, 32);

        // 创建一个网格布局
        layout = new QGridLayout(this);

        // 创建一个标题标签
        auto *title = new QLabel(tr("<h1>2048</h1>"), this);
        layout->addWidget(title, 0, 0, 1, 4);

        // 创建一个分数标签
        this->scoreLabel = new QLabel(tr("Score: 0"), this);
        layout->addWidget(scoreLabel, 1, 0);

        // 创建一个重新开始按钮
        this->m_button = new QPushButton(tr("RESTART"), this);
        m_button->setFont(*font);
        QPalette palette = m_button->palette();
        palette.setColor(QPalette::Button, QColor(0, 0, 255)); // 设置按钮背景色
        palette.setColor(QPalette::ButtonText, QColor(255, 255, 255)); // 设置按钮文本颜色
        palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, QColor(0, 0, 128)); // 设置禁用状态下的背景色
        palette.setColor(QPalette::ColorGroup::Active,  QPalette::ColorRole::Button, QColor(0, 0, 200)); // 设置激活状态下的背景色
        palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::Button, QColor(0, 0, 64)); // 设置正常状态下的背景色
        m_button->setPalette(palette);
        layout->addWidget(this->m_button, 1, 3);

        // 创建一个游戏区域，使用网格布局
        this->gameAreaLayout = new QGridLayout();
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                QLabel *label = new QLabel(this);
                label->setFrameShape(QFrame::Panel);
                label->setFrameShadow(QFrame::Raised);
                label->setFixedSize(100, 100);
                gameAreaLayout->addWidget(label, row, col);
            }
        }

        this->resetGame();
//        this->core->display();

        layout->addLayout(gameAreaLayout, 2, 0, 1, 4);

        // 设置窗口的布局
        setLayout(layout);

        m_button->setCheckable(true);

        connect(m_button, SIGNAL (clicked(bool)), this, SLOT (slotButtonClicked(bool)));
    }

    void Game::slotButtonClicked(bool checked) {
        if (m_button->isChecked()) {
            this->resetGame();
            m_button->setChecked(false);
        }
    }

    void Game::updateScore() {
        this->scoreLabel->setText(tr("Score: %1").arg(this->score));
    }

    void Game::keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
            this->clearBoard();
            score += core->up();
            this->loadBoard();
            this->isGameOver();

        } else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
            this->clearBoard();
            score += core->down();
            this->loadBoard();
            this->isGameOver();

        } else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
            this->clearBoard();
            score += core->left();
            this->loadBoard();
            this->isGameOver();

        } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
            this->clearBoard();
            score += core->right();
            this->loadBoard();
            this->isGameOver();

        } else {
            QWidget::keyPressEvent(event);
        }
    }

    void Game::resetGame() {
        delete this->core;
        this->score = 0;
        this->updateScore();
        core = new board::Board();
        this->clearBoard();
        this->loadBoard();
    }

    void Game::loadBoard() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                int num_pow = core->blocks_array[i][j];
                if (num_pow != 0) {
                    QLayoutItem *item = gameAreaLayout->itemAtPosition(i, j);
                    if (item != nullptr) {
                        QLabel *label = qobject_cast<QLabel *>(item->widget());
                        if (label != nullptr) {
                            label->setText(tr("%1").arg(pow(2, num_pow)));
                            label->setFont(*font);
                            label->setAlignment(Qt::AlignCenter);
                            label->setStyleSheet(tr("background-color: %1;").arg(board_colors[num_pow].c_str()));
                            label->setSizePolicy(QSizePolicy::Expanding,
                                                 QSizePolicy::Expanding); // 设置大小策略
                            label->setMinimumSize(100, 100);// 设置文本大小
                        }
                    }
                }
            }
        }
        this->updateScore();
    }

    void Game::clearBoard() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                QLayoutItem *item = gameAreaLayout->itemAtPosition(i, j);
                if (item != nullptr) {
                    QLabel *label = qobject_cast<QLabel *>(item->widget());
                    if (label != nullptr) {
                        label->setText("");
                        label->setFont(*font);
                        label->setStyleSheet("background-color: white;");
                    }
                }
            }
        }
    }

    void Game::isGameOver(){
        if(this->core->is_end()){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Game Over");
            msgBox.setText(tr("Game Over! Your final score is: %1").arg(this->score));
            msgBox.setStandardButtons(QMessageBox::Reset | QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Reset);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Reset) {
                this->resetGame();
            } else {
                qApp->exit();
            }
        }
    }

} // game
