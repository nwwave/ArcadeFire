/* ! \file beam.cpp
*   \brief Файл содержит реализацию класса Beam
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/


#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "enemies.h"
#include "config.h"
#include "beam.h"

/**
 * @brief создает луч в определенном направлении
 * @param value: указывает направление
 */
Beam::Beam(int value){
    //создает луч
    direction = value;

    if (value == UP_DIRECTION || value == DOWN_DIRECTION)
        setPixmap(QPixmap(":/Images/up_down.png"));
    else if (value == LEFT_DIRECTION || value == RIGHT_DIRECTION)
        setPixmap(QPixmap(":/Images/left_right.png"));

    //связывает таймер с функцией перемещения для создания движущегося луча
    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(40);
}

/**
 * @brief перемещает луч в определенном направлении
 */
void Beam::move()
{
    if (direction == UP_DIRECTION){
        //перемещает снаряд вверх
        setPos(x(), y() - 6);
        //когда луч достигает краев экрана он удаляется
        if (y() < 0){
            scene()->removeItem(this);
            delete this;
        }
    }
    else if (direction == DOWN_DIRECTION){
        //перемещает снаряд вниз
        setPos(x(), y() + 6);
        //когда луч достигает краев экрана он удаляется
        if (y() > 660){
            scene()->removeItem(this);
            delete this;
        }
    }
    else if (direction == LEFT_DIRECTION){
        //перемещает снаряд влево
        setPos(x() - 6, y());
        //когда луч достигает краев экрана он удаляется
        if (x() < 0){
            scene()->removeItem(this);
            delete this;
        }
    }
    else if (direction == RIGHT_DIRECTION){
        //перемещает снаряд вправо
        setPos(x() + 6, y());
        //когда луч достигает краев экрана он удаляется
        if (x() > 670){
            scene()->removeItem(this);
            delete this;
        }
    }
}
