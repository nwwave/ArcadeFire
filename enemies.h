/* ! \file enemies.h
*   \brief Файл содержит описание классов XEnemy и YEnemy - врагов в игре
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#ifndef ENEMY
#define ENEMY

#include <QObject>
#include <QGraphicsPixmapItem>
#include "player.h"
#include "game.h"
#include "config.h"

/**
 * @brief Враг, который двигается координате x (сверху и снизу)
 */
class XEnemy : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    XEnemy(Player* player, int y_coor, Game* param_game);
    void move_and_shoot_down();
    void move_and_shoot_up();
public slots:
    void set_coor();
    void randomize();
    void move_and_shoot();
private:
    bool direction;
    int speed;
    Player* target;
    int target_x;
    int target_y;
    Game* game;
};

/**
 * @brief Враг, который двигается координате y (справа и слева)
 */
class YEnemy : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    YEnemy(Player* player, int x_coor, Game* param_game);
    void move_and_shoot_right();
    void move_and_shoot_left();
public slots:
    void set_coor();
    void randomize();
    void move_and_shoot();
private:
    bool direction;
    int speed;
    Player* target;
    int target_x;
    int target_y;
    Game* game;
};

#endif // ENEMY

