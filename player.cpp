/* ! \file player.cpp
*   \brief Файл содержит реализацию класса Player
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/


#include <QKeyEvent>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QList>
#include "powerups.h"
#include "player.h"
#include "beam.h"
#include "stdlib.h"
#include "enemies.h"
#include "enemy_beam.h"

/**
 * @brief создает игрока с индикаторами перезарядки выстрела и щита
 * @param value: номер выбранного персонажа
 * @param shoot_indicator: доступ к объекту
 * @param shield_indicator: доступ к объекту
 */
Player::Player(int value, QGraphicsPixmapItem* shoot_indicator, QGraphicsPixmapItem* shield_indicator)
{
    character = value;
    shoot_cooldown = true;
    shield_cooldown = true;
    left = false;
    right = false;
    up = false;
    down = false;
    space = false;

    shoot_ready = shoot_indicator;
    shield_ready = shield_indicator;

    speed = 6;
    burst_fire = 0;

    if (character == SECOND_CHARACTER){
        setPixmap(QPixmap(":/Images/hercule_stand.png"));
        this->setPos(360, 270);
    }

    else if (character == 3){
        setPixmap(QPixmap(":/Images/saiyaman_stand.png"));
        this->setPos(360, 270);
   }

    else {
        setPixmap(QPixmap(":/Images/goku_stand.png"));
        this->setPos(340, 270);
    }

    //создает таймер, который обновляет местоположение индикаторов для отслеживания
    QTimer* indicator_follow = new QTimer;
    connect(indicator_follow, SIGNAL(timeout()), this, SLOT(indicator_follow()));
    indicator_follow->start(10);

    //проверяет нажатие клавиш
    QTimer* smooth_timer = new QTimer;
    connect(smooth_timer, SIGNAL(timeout()), this, SLOT(check_keys()));
    smooth_timer->start(45);

    //2 секунды
    shoot_timer = new QTimer;
    connect(shoot_timer, SIGNAL(timeout()), this, SLOT(shoot_cooled_down()));

    //10 секунд
    shield_timer = new QTimer;
    connect(shield_timer, SIGNAL(timeout()), this, SLOT(shield_cooled_down()));
}

void Player::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Left){
        left = true;
    }
    else if (event->key() == Qt::Key_Right){
        right = true;
    }
    else if (event->key() == Qt::Key_Up){
        up = true;
    }
    else if (event->key() == Qt::Key_Down){
        down = true;
    }
    else if (event->key() == Qt::Key_Space){
        space = true;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left){
        left = false;
    }
    else if (event->key() == Qt::Key_Right){
        right = false;
    }
    else if (event->key() == Qt::Key_Up){
        up = false;
    }
    else if (event->key() == Qt::Key_Down){
        down = false;
    }
    else if (event->key() == Qt::Key_Space){
        space = false;
    }
}


void Player::shoot_cooled_down()
{
    shoot_timer->stop();
    shoot_cooldown = true;
}


void Player::shield_cooled_down()
{
    shield_timer->stop();
    shield_cooldown = true;
}

/**
 * @brief проверяет условия чтобы двигаться и стрелять
 * also checks for collisions with powerups
 */
void Player::check_keys()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
        if (typeid(*(colliding_items[i])) == typeid(SpeedUp)){
            speed = 12;
        }
        else if (typeid(*(colliding_items[i])) == typeid(Burst)){
            burst_fire = true;
            shoot_cooldown = true;
        }

    if (character == FIRST_CHARACTER){
    if (left && up){
        if (x() > 111 && y() > 70)
            setPos(x() - speed, y() - speed);
    }
    else if (right && up){
        if (x() < 592 && y() > 70)
            setPos(x() + speed, y() - speed);
    }
    else if (left && down){
        if (x() > 111 && y() < 510)
            setPos(x() - speed, y() + speed);
    }
    else if (right & down){
        if (x() < 592 && y() < 510)
            setPos(x() + speed, y() + speed);
    }
    else if (left){
        if (x() > 111)
            setPos(x() - speed, y());
    }
    else if (right){
        if (x() < 592)
            setPos(x() + speed, y());
    }
    else if (up){
        if (y() > 70)
            setPos(x(), y() - speed);
    }
    else if (down){
        if (y() < 510)
            setPos(x(), y() + speed);
    }
    }

    else if (character == SECOND_CHARACTER || character == THIRD_CHARACTER){
        if (left && up){
            if (x() > 120 && y() > 70)
                setPos(x() - speed, y() - speed);
        }
        else if (right && up){
            if (x() < 606 && y() > 70)
                setPos(x() + speed, y() - speed);
        }
        else if (left && down){
            if (x() > 120 && y() < 520)
                setPos(x() - speed, y() + speed);
        }
        else if (right & down){
            if (x() < 606 && y() < 520)
                setPos(x() + speed, y() + speed);
        }
        else if (left){
            if (x() > 120)
                setPos(x() - speed, y());
        }
        else if (right){
            if (x() < 606)
                setPos(x() + speed, y());
        }
        else if (up){
            if (y() > 70)
                setPos(x(), y() - speed);
        }
        else if (down){
            if (y() < 520)
                setPos(x(), y() + speed);
        }
    }


    if (space && shield_cooldown && !up && !down && !left && !right){
        Shield* shield = new Shield(this);
        scene()->addItem(shield);
        shield_cooldown = false;
        shield_ready->setPos(0, -100);
        shield_timer->start(10000);
    }
    else if (up && space)
        shoot(1);
    else if (down && space)
        shoot(2);
    else if (left && space)
        shoot(3);
    else if (right && space)
        shoot(4);
}

/**
 * @brief позволяет индикаторам следовать за персонажем
 */
void Player::indicator_follow()
{
    if (character == FIRST_CHARACTER){
        if (shoot_cooldown == true)
            shoot_ready->setPos(x() - 12, y() + 29);
        if (shield_cooldown == true)
            shield_ready->setPos(x() + 55, y() + 26);
    }
    else if (character == SECOND_CHARACTER){
        if (shoot_cooldown == true)
            shoot_ready->setPos(x() - 25, y() + 29);
        if (shield_cooldown == true)
            shield_ready->setPos(x() + 37, y() + 26);
    }
    else if (character == THIRD_CHARACTER){
        if (shoot_cooldown == true)
            shoot_ready->setPos(x() - 25, y() + 29);
        if (shield_cooldown == true)
            shield_ready->setPos(x() + 37, y() + 26);
    }
}

/**
 * @brief создает снаряд
 * @param value: показывает направление
 */
void Player::shoot(int value)
{
    //создает луч и центрирует на игроке
    if (shoot_cooldown == true){
        Beam* beam = new Beam(value);
        if (character == FIRST_CHARACTER){
            if (value == 1)
                beam->setPos(x() + 23, y() - 50);
            else if (value == 2)
                beam->setPos(x() + 23, y() + 75);
            else if (value == 3)
                beam->setPos(x() - 40, y() + 30);
            else if (value == 4)
                beam->setPos(x() + 53, y() + 30);
        }
        else if (character == SECOND_CHARACTER || character == THIRD_CHARACTER){
            if (value == 1)
                beam->setPos(x() + 6, y() - 58);
            else if (value == 2)
                beam->setPos(x() + 6, y() + 83);
            else if (value == 3)
                beam->setPos(x() - 60, y() + 30);
            else if (value == 4)
                beam->setPos(x() + 40, y() + 30);
        }

        scene()->addItem(beam);
        if (!burst_fire){
        shoot_cooldown = false;
        shoot_ready->setPos(0, -200);
        shoot_timer->start(2000);
        }
    }
}

/**
 * @brief создает щит
 */
Shield::Shield(Player* player)
{
    character = player;

    if (character->character == SECOND_CHARACTER || character->character == THIRD_CHARACTER)
        setPos(character->x() - 46, character->y() - 25);
    else
        setPos(character->x() - 34, character->y() - 25);

    setPixmap(QPixmap(":Images/shield_sprite.png"));
    QTimer* follow_timer = new QTimer;
    connect(follow_timer, SIGNAL(timeout()), this, SLOT(follow()));
    follow_timer->start(1);

    QTimer* shield_usage_timer = new QTimer;
    connect(shield_usage_timer, SIGNAL(timeout()), this, SLOT(stop()));
    shield_usage_timer->start(4500);
}

void Shield::follow()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
        if (typeid(*(colliding_items[i])) == typeid(EnemyBeam)){
            scene()->removeItem(colliding_items[i]);
            delete colliding_items[i];
        }

    if (character->character == SECOND_CHARACTER || character->character == THIRD_CHARACTER)
        setPos(character->x() - 47, character->y() - 25);
    else
        setPos(character->x() - 34, character->y() - 25);
}


void Shield::stop()
{
    scene()->removeItem(this);
    delete this;
}
