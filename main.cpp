#include <cstdio>
#include <iostream>
#include <QApplication>
#include <QPushButton>
#include "Board.h"
#include "Game.h"


int main(int argc, char *argv[]) {
    QApplication app (argc, argv);
    game::Game game;
    game.show();
    return app.exec();
}
