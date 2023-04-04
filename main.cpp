#include <QApplication>
#include "Game.h"


int main(int argc, char *argv[]) {
    QApplication app (argc, argv);
    game::Game game;
    game.show();
    return app.exec();
}
