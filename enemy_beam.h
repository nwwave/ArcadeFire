/* ! \file enemy_beam.h
*   \brief Файл содержит описание класса EnemyBeam - снаряда противников
*   который спавнится при совпадении координат противника и героя
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#ifndef ENEMY_BEAM
#define ENEMY_BEAM

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include "game.h"

/**
 * @brief снаряда противников который спавнится при совпадении координат
 * противника и героя
 */
class EnemyBeam : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    EnemyBeam(int direction, Game* param_game);
    int dir;
public slots:
    void move_down();
    void move_up();
    void move_right();
    void move_left();
    //void move();
private:
    Game* game;
};

#endif // ENEMY_BEAM

