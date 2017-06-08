# Recast [![Build Status](https://travis-ci.org/bender-wardrobe/Recast.svg?branch=master)](https://travis-ci.org/bender-wardrobe/Recast)
Основная идея проекта - не ограничивать игрока в создании заклинаний. Игрок может создавать заклинания, опираясь на физические законы окружающего мира.

## Основные концепции проекта:
- Проект - игра в жанре RPG
- Двумерный игровой мир в представлении пользователя
- Трёхмерный игровой мир с фиксированной глубиной со стороны логики игры
- Основной “фишкой” игры является возможность построения - произвольных заклинаний
- Заклинания строятся в отдельном трёхмерном интерфейсе, управление глубиной на колёсико мыши или wasd+shift+space
- Заклинания представляют из себя трёхмерные графы, состоящие из узлов и связывающих их рёбер
- Заклинания завязаны на законах физики и связаны с температурой объектов и кинетикой
- Создание заклинаний базируется на схемотехнике
- Приоритет в разработке отдаётся созданию движка создания заклинаний
- Прототип схож с игрой Terraria
- Разрушаемый мир
- Деление мира на тайлы

## Примеры заклинаний:
- Огненный шар
- Отталкивание

## Архитектура проекта:
### Диаграмма общей архитектуры
![Архитектура проекта](https://lionzxy.github.io/Recast/custom-docs/simple_architecture.svg)
### Диаграмма потока авторизации
![Архитектура проекта](https://lionzxy.github.io/Recast/custom-docs/procces_pocketlife.svg)
## Команда "Шкаф Бендера":
- Олег Морозенков
- Василий Дмитриев
- Михаил Волынов
- Юрий Голубев
- Куликов Никита
