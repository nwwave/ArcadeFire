/* ! \file enemies.cpp
*   \brief Файл содержит реализацию классов XEnemy и YEnemy
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#include <QTimer>
#include <QGraphicsScene>
#include <cmath>

#include "player.h"
#include "game.h"
#include "beam.h"
#include "config.h"
#include "enemies.h"
#include "enemy_beam.h"

/**
 * @brief создает врага движущегося по X
 * @param player: используется для доступа к координатам игрока для установки цели
 * @param y_coor: устанавливает координату Y для появления (сверху или снизу)
 * @param param_game: используется, чтобы сообщить игре, когда враг умирает (для экрана победы)
 */
XEnemy::XEnemy(Player* player, int y_coor, Game* param_game){
    target = player;
    game = param_game;

    //постоянно собирает координаты игрока
    QTimer* coor_timer = new QTimer;
    connect(coor_timer, SIGNAL(timeout()), this, SLOT(set_coor()));
    coor_timer->start(40);

    //я попытался сделать движение противника менее предсказуемым
    //каждые 2-5 секунд, есть 50% шанс изменения направления
    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(randomize()));
    timer->start((rand() % 3001) + 2000);

    //устанавливает произвольное положение вдоль края платформы
    setPos((rand() % 557) + 100, y_coor);

    //рандомно определяет расположение противника (1 right, 0 left)
    direction = rand() % 2;

    //для определения скорости врага (3-5 движений пикселей каждые 40 мс)
    speed = (rand() % 3) + 3;

    //устанавливаем изображение
    setPixmap(QPixmap(":/Images/enemy.png"));

    //связывает таймер с функцией перемещения чтобы враг двигался
    QTimer* moving_timer = new QTimer;
    connect(moving_timer, SIGNAL(timeout()), this, SLOT(move_and_shoot()));
    moving_timer->start(40);
}

/**
 * @brief устанавливает координаты игрока в координаты цели (по таймеру)
 */
void XEnemy::set_coor(){
    target_x = target->x() + 10;
    target_y = target->y();
}

/**
 * @brief шанс изменения направления (установлен на таймер)
 */
void XEnemy::randomize()
{
    //Шанс 50% на изменение направления на противоположное
    if (rand() % 2 == 0)
        direction = (direction + 1) % 2;
}

/**
 * @brief решает, двигаться ли и стрелять вниз или вверх в зависимости от координаты y
 */
void XEnemy::move_and_shoot()
{
    //стреляет в зависимости от направления
    if (y() == -10) move_and_shoot_down();
    else move_and_shoot_up();
}

/**
 * @brief перемещается влево и вправо и создает луч вниз, когда координата x
 * совпадает с координатой игрока
 */
void XEnemy::move_and_shoot_down()
{
    //если во врага попал снаряд он уничтожается
    //список содержит все элементы, с которыми сталкивается луч
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (typeid(*(colliding_items[i])) == typeid(Beam)){ //если тип совпадает
            --game->enemy_count;
            scene()->removeItem(colliding_items[i]); //удаляем луч
            scene()->removeItem(this); //удаляем врага
            //memory management
            delete colliding_items[i];
            delete this;
            //важно вернуться, чтобы удаленный луч не использовал код ниже
            return;
        }
    }

    //если враг ровняется с игроком (плюс минус 8 единиц) то он начинает стрелять
    if (abs(x()-target_x) <= 8){
        EnemyBeam* beam = new EnemyBeam(UP_DIRECTION, game);
        beam->setPos(x() + 10, y());
        scene()->addItem(beam);
    }

    //двигается вправо пока может
    if (direction == ENEMY_RIGHT){
        setPos(x() + speed, y());
        if (x() > 615)
            direction = ENEMY_LEFT;
    }
    //двигается влево пока может
    if (direction == ENEMY_LEFT){
        setPos(x() - speed, y());
        if (x() < 100)
            direction = ENEMY_RIGHT;
    }
}

/**
 * @brief moves left and right and creates an up beam when x-coordinate matches with player
 */
void XEnemy::move_and_shoot_up()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (typeid(*(colliding_items[i])) == typeid(Beam)){
            --game->enemy_count;
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;
            return;
        }
    }

    if (abs(x()-target_x) <= 8){
        EnemyBeam* beam = new EnemyBeam(DOWN_DIRECTION, game);
        beam->setPos(x() + 10, y() + 20);
        scene()->addItem(beam);
    }

    if (direction == ENEMY_RIGHT){
        setPos(x() + speed, y());
        if (x() > 615)
            direction = ENEMY_LEFT;
    }

    if (direction == ENEMY_LEFT){
        setPos(x() - speed, y());
        if (x() < 100)
            direction = ENEMY_RIGHT;
    }
}

/**
 * @brief создает врага движущегося по Y
 * @param player: используется для доступа к координатам игрока для установки цели
 * @param x_coor: sets устанавливает координату X для появления (справа или слева)
 * @param param_game: используется, чтобы сообщить игре, когда враг умирает (для экрана победы)
 */
YEnemy::YEnemy(Player* player, int x_coor, Game* param_game){
    target = player;
    game = param_game;

    //постоянно собирает координаты игрока
    QTimer* coor_timer = new QTimer;
    connect(coor_timer, SIGNAL(timeout()), this, SLOT(set_coor()));
    coor_timer->start(40);

    //рандомизация движения
    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(randomize()));
    timer->start((rand() % 3001) + 2000);

    setPos(x_coor, (rand() % 390) + 150);

    direction = rand() % 2;

    //для определения скорости врага (3-5 движений пикселей каждые 40 мс)
    speed = (rand() % 3) + 3;

    setPixmap(QPixmap(":/Images/enemy.png"));

    //связываем таймер
    QTimer* moving_timer = new QTimer;
    connect(moving_timer, SIGNAL(timeout()), this, SLOT(move_and_shoot()));
    moving_timer->start(40);
}

/**
 * @brief устанавливает координаты игрока в координаты цели (по таймеру)
 */
void YEnemy::set_coor(){
    target_x = target->x();
    target_y = target->y();
}

/**
 * @brief шанс изменения направления (установлен на таймер)
 */
void YEnemy::randomize()
{
    if (rand() % 2 == 0)
        direction = (direction + 1) % 2;
}

/**
 * @brief решает, двигаться ли и стрелять влево или вправо в зависимости от координаты x
 */
void YEnemy::move_and_shoot()
{
    if (x() == 60) move_and_shoot_right();
    else move_and_shoot_left();
}

/**
 * @brief перемещается вверх и вниз и создает луч вправо,
 * когда координата y совпадает с координатой игрока
 */
void YEnemy::move_and_shoot_right()
{

    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
        if (typeid(*(colliding_items[i])) == typeid(Beam)){
            --game->enemy_count;
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;

            return;
        }

    if (abs(y()-target_y) <= 8){
        EnemyBeam* beam = new EnemyBeam(LEFT_DIRECTION, game);
        beam->setPos(x(), y() + 20);
        scene()->addItem(beam);
    }

    if (direction == ENEMY_UP){
        setPos(x(), y() - speed);
        if (y() < 40)
            direction = ENEMY_DOWN;
    }

    if (direction == ENEMY_DOWN){
        setPos(x(), y() + speed);
        if (y() > 530)
            direction = ENEMY_UP;
    }
}

/**
 * @brief перемещается вверх и вниз и стреляет влево,
 * когда координата y совпадает с координатой игрока
 */
void YEnemy::move_and_shoot_left()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
        if (typeid(*(colliding_items[i])) == typeid(Beam)){
            --game->enemy_count;
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;
            return;
        }

    if (abs(y()-target_y) <= 8){
        EnemyBeam* beam = new EnemyBeam(RIGHT_DIRECTION, game);
        beam->setPos(x(), y() + 20);
        scene()->addItem(beam);
    }

    if (direction == ENEMY_UP){
        setPos(x(), y() - speed);
        if (y() < 40)
            direction = ENEMY_DOWN;
    }
    if (direction == ENEMY_DOWN){
        setPos(x(), y() + speed);
        if (y() > 530)
            direction = ENEMY_UP;
    }
}
