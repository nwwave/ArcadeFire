/* ! \file powerups.h
*   \brief Файл содержит описание классов SpeedUp Burst - усилений
*   который спавнится при совпадении координат противника и героя
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#ifndef POWERUPS
#define POWERUPS

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include <QTimer>
#include "game.h"

/**
 * @brief Усиление повышающее скорость игрока
 */
class SpeedUp : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    SpeedUp(Game* game_arg);
    Game* game;
    bool has_died;
public slots:
    void begone();
    void check_player();
};

/**
 * @brief Усиление, которое устраняет перезарядку выстрела игрока
 */
class Burst : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Burst(Game* game_arg);
    Game* game;
    bool has_died;
public slots:
    void begone();
    void check_player();

};

#endif // POWERUPS

