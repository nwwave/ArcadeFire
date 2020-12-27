/* ! \file beam.h
*   \brief Файл содержит описание класса Beam - снаряда от игрока, который выпускается
*    при нажатии пробела
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#ifndef BEAM
#define BEAM

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

/**
 * @brief снаряд от игрока, который выпускается при нажатии пробела
 */
class Beam : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Beam(int value);
    int direction;
public slots:
    void move();
};

#endif // BEAM

