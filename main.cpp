#include "game.h"

#include <QApplication>
#include <player.h>
#include "game.h"
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(NULL));

    Game* game = new Game;
    game->show();

    return a.exec();
}
