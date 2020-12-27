/* ! \file game.сpp
*   \brief Файл содержит реализацию класса Game
*   \author Рыбаков Сергей
*   \date 24.12.2020
*/

#include <QLabel>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include "powerups.h"
#include "game.h"
#include "player.h"
#include "enemies.h"
#include "config.h"


/**
 * @brief Начинает работу программы с меню
 */
Game::Game(QWidget*)
{
    QLabel* heading = new QLabel("<h1>ARCADE FIRE</h1>");
    heading->setAlignment(Qt::AlignCenter);

    QLabel* pick_char = new QLabel("<h2>Pick your character:</h2>");
    pick_char->setAlignment(Qt::AlignCenter);

    //изображение персонажей
    QLabel* firstHero_label = new QLabel;
    QPixmap* firstHero_image = new QPixmap(":/Images/Goku_select.png");
    firstHero_label->setPixmap(*firstHero_image);
    QLabel* secondHero_label = new QLabel;
    QPixmap* secondHero_image = new QPixmap(":/Images/Hercule_select.png");
    secondHero_label->setPixmap(*secondHero_image);
    QLabel* thirdHero_label = new QLabel;
    QPixmap* thirdHero_image = new QPixmap(":/Images/Saiyaman_select.png");
    thirdHero_label->setPixmap(*thirdHero_image);
    QHBoxLayout* char_images = new QHBoxLayout;
    char_images->addWidget(firstHero_label);
    char_images->addWidget(secondHero_label);
    char_images->addWidget(thirdHero_label);
    char_images->setAlignment(Qt::AlignCenter);

    //кнопки выбора персонажей
    QRadioButton* firstHero_button = new QRadioButton("Goku");
    QRadioButton* secondHero_button = new QRadioButton("Hercule");
    QRadioButton* thirdHero_button = new QRadioButton("Saiyaman");
    QHBoxLayout* char_buttons = new QHBoxLayout;
    char_buttons->addWidget(firstHero_button);
    char_buttons->addWidget(secondHero_button);
    char_buttons->addWidget(thirdHero_button);
    char_buttons->setAlignment(Qt::AlignCenter);

    QButtonGroup* char_select = new QButtonGroup;
    char_select->addButton(firstHero_button);
    char_select->addButton(secondHero_button);
    char_select->addButton(thirdHero_button);

    QLabel* choose_difficulty = new QLabel("<h2>Choose Difficulty:</h2>");
    choose_difficulty->setAlignment(Qt::AlignCenter);

    //кнопки выбора сложности
    QRadioButton* easy = new QRadioButton("Easy");
    QRadioButton* medium = new QRadioButton("Medium");
    QRadioButton* hard = new QRadioButton("Hard");
    QVBoxLayout* difficulties = new QVBoxLayout;
    difficulties->addWidget(easy);
    difficulties->addWidget(medium);
    difficulties->addWidget(hard);
    difficulties->setAlignment(Qt::AlignCenter);

    QButtonGroup* difficulty_select = new QButtonGroup;
    difficulty_select->addButton(easy);
    difficulty_select->addButton(medium);
    difficulty_select->addButton(hard);

    //кнопка инструкций
    QPushButton* instructions = new QPushButton("INSTRUCTIONS");
    QApplication::connect(instructions, SIGNAL(clicked()), this, SLOT(show_instructions()));

    //START
    QPushButton* battle = new QPushButton("START");

    //вертикально располагаем элементы
    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addWidget(heading);
    vlay->addWidget(pick_char);
    vlay->addLayout(char_images);
    vlay->addLayout(char_buttons);
    vlay->addWidget(choose_difficulty);
    vlay->addLayout(difficulties);
    vlay->addWidget(instructions);
    vlay->addWidget(battle);

    //устанавливаем их в окно меню
    this->setLayout(vlay);

    QObject::connect(firstHero_button, SIGNAL(clicked()), this, SLOT(firstHero_set()));
    QObject::connect(secondHero_button, SIGNAL(clicked()), this, SLOT(secondHero_set()));
    QObject::connect(thirdHero_button, SIGNAL(clicked()), this, SLOT(thirdHero_set()));
    character = 1; //по умолчанию

    QObject::connect(easy, SIGNAL(clicked()), this, SLOT(easy_set()));
    QObject::connect(medium, SIGNAL(clicked()), this, SLOT(medium_set()));
    QObject::connect(hard, SIGNAL(clicked()), this, SLOT(hard_set()));
    difficulty = 1;
    wave_count = 0;
    enemy_count = 4;

    //разместил таймеры усилений тут чтобы при перезапуске не было нескольких таймеров
    speed_out = 0;
    burst_out = 0;
    check_powerups_timer = new QTimer;
    connect(check_powerups_timer, SIGNAL(timeout()), this, SLOT(check_powerups()));
    powerup_speed_timer = new QTimer;
    connect (powerup_speed_timer, SIGNAL(timeout()), this, SLOT(spawn_speed_powerup()));
    powerup_burst_timer = new QTimer;
    connect (powerup_burst_timer, SIGNAL(timeout()), this, SLOT(spawn_burst_powerup()));

    QApplication::connect(battle, SIGNAL(clicked()), this, SLOT(hide()));
    QApplication::connect(battle, SIGNAL(clicked()), this, SLOT(start_battle()));
}

/**
 * @brief обнуляет игровые параметры и выводит меню
 */
void Game::game_over()
{
    check_powerups_timer->stop();
    powerup_burst_timer->stop();
    powerup_speed_timer->stop();
    speed_out = 0;
    burst_out = 0;

    wave_count = 0;
    if (difficulty == 1)
        enemy_count = 4;
    else if (difficulty == 2)
        enemy_count = 6;
    else if (difficulty == 3)
        enemy_count = 8;

    QWidget* game_over_menu = new QWidget;

    QLabel* you_lose = new QLabel("<h1> YOU LOSE </h1>");
    you_lose->setAlignment(Qt::AlignCenter);
    QPushButton* restart = new QPushButton("TRY AGAIN");
    QPushButton* change_options = new QPushButton("CHANGE OPTIONS");
    QPushButton* quit = new QPushButton("QUIT");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(you_lose);
    layout->addWidget(restart);
    layout->addWidget(change_options);
    layout->addWidget(quit);

    game_over_menu->setLayout(layout);
    game_over_menu->show();

    connect(restart, SIGNAL(clicked()), view, SLOT(close()));
    connect(restart, SIGNAL(clicked()), game_over_menu, SLOT(close()));
    connect(restart, SIGNAL(clicked()), this, SLOT(start_battle()));

    connect(change_options, SIGNAL(clicked()), view, SLOT(close()));
    connect(change_options, SIGNAL(clicked()), game_over_menu, SLOT(close()));
    connect(change_options, SIGNAL(clicked()), this, SLOT(show()));

    connect(quit, SIGNAL(clicked()), view, SLOT(close()));
    connect(quit, SIGNAL(clicked()), game_over_menu, SLOT(close()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
}

/**
 * @brief открыть инструкцию
 */
void Game::show_instructions()
{
    QWidget* instruction_window = new QWidget;
    QLabel* instructions1 = new QLabel("<h3>8</h3>");
    QLabel* instructions2 = new QLabel("*** СТРЕЛКИ ДЛЯ ДВИЖЕНИЯ ***");
    instructions2->setAlignment(Qt::AlignCenter);
    QLabel* instructions3 = new QLabel("*** ПРОБЕЛ - СТРЕЛЯТЬ (только во время движения) ***");
    instructions3->setAlignment(Qt::AlignCenter);
    QLabel* instructions4 = new QLabel("*** ПРОБЕЛ - АКТИВИРОВАТЬ ЩИТ (не двигаясь) ***");
    instructions4->setAlignment(Qt::AlignCenter);
    QLabel* instructions5 = new QLabel("***СИНЯЯ СФЕРА - ВЫСТРЕЛ ГОТОВ; ОРАНДЕВАЯ СФЕРА - ЩИТ ГОТОВ ***");
    instructions5->setAlignment(Qt::AlignCenter);
    QLabel* instructions6 = new QLabel("*** НА КАРТЕ ПОЯВЛЯЮТСЯ УСИЛЕНИЯ ***");
    instructions6->setAlignment(Qt::AlignCenter);
    QPushButton* close = new QPushButton("DONE");

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(instructions1);
    vlayout->addWidget(instructions2);
    vlayout->addWidget(instructions3);
    vlayout->addWidget(instructions4);
    vlayout->addWidget(instructions5);
    vlayout->addWidget(instructions6);
    vlayout->addWidget(close);
    instruction_window->setLayout(vlayout);
    instruction_window->show();

    QApplication::connect(close, SIGNAL(clicked()), instruction_window, SLOT(close()));
}

/**
 * @brief проверяет количество врагов и номер волны
 */
void Game::new_wave_or_win()
{
    if (wave_count == 1 && enemy_count == 0){
        player->left = false;
        player->right = false;
        player->up = false;
        player->down = false;

        QLabel* winner_label = new QLabel;
        QPixmap* face;
        switch(character)
        {
        case FIRST_CHARACTER:
            face = new QPixmap(":/Images/goku_face.png");
            break;
        case SECOND_CHARACTER:
            face = new QPixmap(":/Images/hercule_face.png");
            break;
        case THIRD_CHARACTER:
            face = new QPixmap(":/Images/saiyaman_face.png");
            break;
        default:
            face = new QPixmap(":/Images/goku_face.png");
            break;
        }

        winner_label->setPixmap(*face);
        QLabel* you_win = new QLabel("<h1>YOU WIN</h1>");
        you_win->setAlignment(Qt::AlignCenter);
        QPushButton* close = new QPushButton("SEE YOU LATER");

        QVBoxLayout* vlayout = new QVBoxLayout;
        vlayout->addWidget(winner_label);
        vlayout->addWidget(you_win);
        vlayout->addWidget(close);

        QWidget* win_window = new QWidget;
        win_window->setLayout(vlayout);
        win_window->show();

        connect(close, SIGNAL(clicked()), view, SLOT(close()));
        connect(close, SIGNAL(clicked()), win_window, SLOT(close()));

        enemy_count = 9000; // от повторного открытия окна
    }
    if (enemy_count == 0 && wave_count == 0){
        ++wave_count;
        //спавн врагов
        XEnemy* top_enemy1 = new XEnemy(player, -10, this);
        XEnemy* top_enemy2 = new XEnemy(player, -10, this);
        scene->addItem(top_enemy1);
        scene->addItem(top_enemy2);
        XEnemy* bottom_enemy1 = new XEnemy(player, 590, this);
        XEnemy* bottom_enemy2 = new XEnemy(player, 590, this);
        scene->addItem(bottom_enemy1);
        scene->addItem(bottom_enemy2);

        if (difficulty >= 1){
            for (int i = 0; i < wave_count; ++i){
            XEnemy* another_top_enemy = new XEnemy(player, -10, this);
            scene->addItem(another_top_enemy);
            XEnemy* another_bottom_enemy = new XEnemy(player, 590, this);
            scene->addItem(another_bottom_enemy);
            }
            enemy_count = 6;
        }
        if (difficulty >= 2){
            YEnemy* left_enemy1 = new YEnemy(player, 60, this);
            YEnemy*left_enemy2 = new YEnemy(player, 60, this);
            scene->addItem(left_enemy1);
            scene->addItem(left_enemy2);
            YEnemy* another_left_enemy = new YEnemy(player, 60, this);
            scene->addItem(another_left_enemy);
            enemy_count = 9;
        }
        if (difficulty == 3){
            YEnemy* right_enemy1 = new YEnemy(player, 655, this);
            YEnemy* right_enemy2 = new YEnemy(player, 655, this);
            scene->addItem(right_enemy1);
            scene->addItem(right_enemy2);
            YEnemy* another_right_enemy = new YEnemy(player, 655, this);
            scene->addItem(another_right_enemy);
            enemy_count = 12;
        }
    }
}

/**
 * @brief выбран первый герой
 */
void Game::firstHero_set()
{
    character = FIRST_CHARACTER;
}

/**
 * @brief выбран второй герой
 */
void Game::secondHero_set()
{
    character = SECOND_CHARACTER ;
}

/**
 * @brief выбран третий герой
 */
void Game::thirdHero_set()
{
    character = THIRD_CHARACTER;
}

/**
 * @brief легкий уровень сложности
 */
void Game::easy_set()
{
    difficulty = 1;
    enemy_count = 4;
}

/**
 * @brief средний легкий уровень сложности
 */
void Game::medium_set()
{
    difficulty = 2;
    enemy_count = 6;
}

/**
 * @brief тяжелый легкий уровень сложности
 */
void Game::hard_set()
{
    difficulty = 3;
    enemy_count = 8;
}

/**
 * @brief создает игровое поле
 */
void Game::start_battle()
{
    view = new QGraphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    //графика для индикаторов
    QGraphicsPixmapItem* shoot_indicator = new QGraphicsPixmapItem(QPixmap (":/Images/shoot_indicator.png"));
    QGraphicsPixmapItem* shield_indicator = new QGraphicsPixmapItem(QPixmap (":/Images/shield_indicator.png"));
    scene->addItem(shoot_indicator);
    scene->addItem(shield_indicator);
    player = new Player(character, shoot_indicator, shield_indicator);
    scene->addItem(player);
    player_dead = 0;
    //фокус для ввода с клавиатуры
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setFixedSize(WND_WIDTH, WND_HEIGHT);
    scene->setSceneRect(56, 0, WND_WIDTH, WND_HEIGHT); // костыль

    //background
    scene->setBackgroundBrush(QBrush(QImage(":/Images/background.png")));

    //спавн врагов
    XEnemy* top_enemy1 = new XEnemy(player, -10, this);
    XEnemy* top_enemy2 = new XEnemy(player, -10, this);
    XEnemy* bottom_enemy1 = new XEnemy(player, 590, this);
    XEnemy* bottom_enemy2 = new XEnemy(player, 590, this);
    scene->addItem(top_enemy1);
    scene->addItem(top_enemy2);
    scene->addItem(bottom_enemy1);
    scene->addItem(bottom_enemy2);
    if (difficulty == 2){
        YEnemy* left_enemy1 = new YEnemy(player, 60, this);
        YEnemy* left_enemy2 = new YEnemy(player, 60, this);
        scene->addItem(left_enemy1);
        scene->addItem(left_enemy2);
    }
    if (difficulty == 3){
        YEnemy* left_enemy1 = new YEnemy(player, 60, this);
        YEnemy* left_enemy2 = new YEnemy(player, 60, this);
        scene->addItem(left_enemy1);
        scene->addItem(left_enemy2);
        YEnemy* right_enemy1 = new YEnemy(player, 655, this);
        YEnemy* right_enemy2 = new YEnemy(player, 655, this);
        scene->addItem(right_enemy1);
        scene->addItem(right_enemy2);
     }

    QTimer* check_enemynwave = new QTimer;
    connect(check_enemynwave, SIGNAL(timeout()), this, SLOT(new_wave_or_win()));
    check_enemynwave->start(1000);

    powerup_speed_timer->start(20000);
    powerup_burst_timer->start(20000);
    check_powerups_timer->start(10);


    view->show();
}

void Game::check_powerups()
{
    if (speed_out == 1){
        powerup_speed_timer->stop();
    }
    if (burst_out == 1){
        powerup_burst_timer->stop();
    }
}

/**
 * @brief спавн усилений
 */
void Game::spawn_speed_powerup()
{
    powerup_speed_timer->stop();
    SpeedUp* speedup = new SpeedUp(this);
    scene->addItem(speedup);
}

/**
 * @brief спавн усилений
 */
void Game::spawn_burst_powerup()
{
    powerup_burst_timer->stop();
    Burst* burst = new Burst(this);
    scene->addItem(burst);
}
