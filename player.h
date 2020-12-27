/* ! \file player.h
*   \brief Файл содержит описание класса Player - главного персонажа
*   и класса Shield - игрового умения
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#ifndef PLAYER
#define PLAYER

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>


/**
 * @brief Играбельный персонаж
 */
class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent *event);
    Player(int value, QGraphicsPixmapItem* shoot_indicator, QGraphicsPixmapItem* shield_indicator);
    int character;
    QTimer* shoot_timer;
    QTimer* shield_timer;
    QGraphicsPixmapItem* shoot_ready;
    QGraphicsPixmapItem* shield_ready;
    bool shoot_cooldown;
    bool shield_cooldown;
    bool left;
    bool right;
    bool up;
    bool down;
    bool space;
    void shoot(int value);
    int speed;
    bool burst_fire;
public slots:
    void shoot_cooled_down();
    void shield_cooled_down();
    void check_keys();
    void indicator_follow();
};

/**
 * @brief Щит, который защищает игрока
 */
class Shield : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Shield(Player* player);
    Player* character;
public slots:
    void follow();
    void stop();
};

#endif // PLAYER

