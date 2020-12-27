/* ! \file enemy_beam.cpp
*   \brief Файл содержит реализацию класса EnemyBeam
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/


#include <QTimer>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include <QApplication>
#include <QList>
#include <QGraphicsPixmapItem>
#include "enemy_beam.h"
#include "player.h"
#include "game.h"
#include "config.h"


/**
 * @brief создает вражеский луч в определенном направлении
 * @param направление
 * @param param_game: используется для доступа к счетчику врагов и игроку (участнику игры)
 */
EnemyBeam::EnemyBeam(int direction, Game* param_game)
{
    game = param_game;
    dir = direction;

    //создает луч
    if (direction == UP_DIRECTION || direction == DOWN_DIRECTION)
        setPixmap(QPixmap(":/Images/enemy_up_down.png"));
    else
        setPixmap(QPixmap(":/Images/enemy_left_right.png"));


    QTimer* timer = new QTimer;


    //луч идет сверху и стреляем вниз
    if (direction == UP_DIRECTION){
        connect(timer, SIGNAL(timeout()), this, SLOT(move_down()));
        timer->start(40);
    }
    //луч идет снизу стреляем вверх
    else if (direction == DOWN_DIRECTION){
        connect(timer, SIGNAL(timeout()), this, SLOT(move_up()));
        timer->start(40);
    }

    //луч идет слева стреляем вправо
    else if (direction == LEFT_DIRECTION){
        connect(timer, SIGNAL(timeout()), this, SLOT(move_right()));
        timer->start(40);
    }

    else {
        connect(timer, SIGNAL(timeout()), this, SLOT(move_left()));
        timer->start(40);
    }


//    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
//    timer->start(40);
}

/*
void EnemyBeam::move()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (typeid(*(colliding_items[i])) == typeid(Player) && game->enemy_count != 9000){
            QGraphicsPixmapItem* tombstone = new QGraphicsPixmapItem(QPixmap(":/Images/tombstone.png"));
            tombstone->setPos(colliding_items[i]->x(), colliding_items[i]->y());
            game->player_dead = 1;
            scene()->removeItem(game->player->shoot_ready);
            scene()->removeItem(game->player->shield_ready);
            scene()->addItem(tombstone);
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);

            delete this;

            //меню проигрыша
            game->game_over();

            return;
        }
    }

    if (dir == UP_DIRECTION){
        setPos(x(), y() + 4);
        if (y() > 660){
            scene()->removeItem(this);
            delete this;
        }
    }
    else if (dir == DOWN_DIRECTION){
        setPos(x(), y() - 4);
        if (y() < -50){
            scene()->removeItem(this);
            delete this;
        }
    }
    else if (dir == LEFT_DIRECTION){
        setPos(x() + 4, y());
        if (x() > 670){
            scene()->removeItem(this);
            delete this;
        }
    }
    else {
        setPos(x() - 4, y());
        if (x() < 0){
            scene()->removeItem(this);
            delete this;
        }
    }

}
*/

/**
 * @brief перемещает луч и проверяет коллизию с персонажем
 */
void EnemyBeam::move_down(){
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if ((typeid(*(colliding_items[i])) == typeid(Player)) && game->enemy_count != 9000){
            QGraphicsPixmapItem* tombstone = new QGraphicsPixmapItem(QPixmap(":/Images/tombstone.png"));
            tombstone->setPos(colliding_items[i]->x(), colliding_items[i]->y());
            game->player_dead = 1;
            scene()->removeItem(game->player->shoot_ready);
            scene()->removeItem(game->player->shield_ready);
            scene()->addItem(tombstone);
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            //
            delete this;

            //меню проигрыша
            game->game_over();

            return;
        }
    }

    setPos(x(), y() + 4);
    if (y() > 660){
        scene()->removeItem(this);
        delete this;
    }
}

/**
 * @brief перемещает луч и проверяет коллизию с персонажем
 */
void EnemyBeam::move_up(){
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if ((typeid(*(colliding_items[i])) == typeid(Player)) && game->enemy_count != 9000){
            QGraphicsPixmapItem* tombstone = new QGraphicsPixmapItem(QPixmap(":/Images/tombstone.png"));
            tombstone->setPos(colliding_items[i]->x(), colliding_items[i]->y());
            game->player_dead = 1;
            scene()->removeItem(game->player->shoot_ready);
            scene()->removeItem(game->player->shield_ready);
            scene()->addItem(tombstone);
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            //
            delete this;

            //меню проигрыша
            game->game_over();

            return;
        }
    }

    setPos(x(), y() - 4);
    if (y() < -50){
        scene()->removeItem(this);
        delete this;
    }
}

/**
 * @brief перемещает луч в определенном направлении
 */
void EnemyBeam::move_right()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if ((typeid(*(colliding_items[i])) == typeid(Player)) && game->enemy_count != 9000){
            QGraphicsPixmapItem* tombstone = new QGraphicsPixmapItem(QPixmap(":/Images/tombstone.png"));
            tombstone->setPos(colliding_items[i]->x(), colliding_items[i]->y());
            game->player_dead = 1;
            scene()->removeItem(game->player->shoot_ready);
            scene()->removeItem(game->player->shield_ready);
            scene()->addItem(tombstone);
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            //
            delete this;

            //меню проигрыша
            game->game_over();

            return;
        }
    }

    setPos(x() + 4, y());
    if (x() > 670){
        scene()->removeItem(this);
        delete this;
    }
}

/**
 * @brief перемещает луч в определенном направлении
 */
void EnemyBeam::move_left()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if ((typeid(*(colliding_items[i])) == typeid(Player)) && game->enemy_count != 9000){
            QGraphicsPixmapItem* tombstone = new QGraphicsPixmapItem(QPixmap(":/Images/tombstone.png"));
            tombstone->setPos(colliding_items[i]->x(), colliding_items[i]->y());
            game->player_dead = 1;
            scene()->removeItem(game->player->shoot_ready);
            scene()->removeItem(game->player->shield_ready);
            scene()->addItem(tombstone);
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            //
            delete this;

            //меню проигрыша
            game->game_over();

            return;
        }
    }

    setPos(x() - 4, y());
    if (x() < 0){
        scene()->removeItem(this);
        delete this;
    }
}
