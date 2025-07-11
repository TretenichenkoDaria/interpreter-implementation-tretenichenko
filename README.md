# interpreter-implementation-tretenichenko

http://github.com/TretenichenkoDaria/interpreter-implementation-tretenichenko/

* main.cpp — головний файл, що запускає інтерпретатор, читає рядки з консолі, передає їх на парсинг та обчислює значення
* parser.cpp — розбирає текстовий вираз на дерево виразів (abstract syntax tree), підтримує числа, оператори, дужки, змінні та функції
* ast.hpp — описує класи для вузлів дерева виразів: число, змінна, функція, бінарна операція, присвоєння
* ast.cpp — реалізує метод evaluate для кожного класу, обчислює результат виразу

підтримувані можливості:

* базові арифметичні операції: +, -, \*, /
* дужки для визначення пріоритету
* змінні, які можна оголосити через var
* вбудовані функції: min, max, abs, pow
* обробка помилок: ділення на нуль, невизначені змінні, неправильна кількість аргументів

приклад роботи програми:

> var x = 7
7
> x + 1
8
> min(3+2, 4)
4
> abs(-3 + min(1,2)) * 2
4

для автоматичного тестування був написаний простий скрипт на python, який запускає інтерпретатор один раз, передає кілька виразів і перевіряє, чи відповідають результати очікуваним. це дозволяє швидко переконатись у правильності роботи програми.
