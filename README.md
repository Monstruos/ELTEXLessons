# ELTEXLessons
Задания с летней школы ЭЛТЕКС (2018)

•	Занятие 1
1. Написать простейшую программу "Hello World" и вывести все этапы её компиляции в отдельные файлы.
2. Написать калькулятор с простейшими операциями и логарифмом. Скомпилировать обычно и раздельно. Замерить время перекомпиляции при     изменении логарифма с десятичного на натуральный.
3. Протестировать gdb. Написать программу с вводом и выводом сообщения в консоль и поменять его командой set.
4. Замерить время раздельной и обычной компиляции тестовой программы утилитой time.
•	Занятие 2
Изучить работу valgrind при выделении памяти без последующей инициализации, обращении за пределы памяти и использовании                 неинициализированной переменной.
•	Занятие 10.
Проверить распределение байтов в структуре с добиванием и без него.
•	Занятие 11.
Сделать программу со вложенностью функций и посмотреть её stacktrace с помощью gdb.
•	Занятие 14-15.
Создать и скомпоновать статическую библиотеку с математическими операциями. Сделать программу проверки библиотеки с меню.
Создать динамическую библиотеку, сделать плагиновый калькулятор, подключающий нужные библиотеки во время выполнения.
•	Занятие 16.
Написать простейшую программу "Hello World", работающую с файлами.
Вывести файл наоборот с помощью функции lseek.
Написать файловый редактор с помощью ncurses.
•	Занятие 17.
Сделать файловый менеджер с помощью ncurses.
Переопределить malloc() и free() с помощью brk() и sbrk().
•	Занятие 18.
С помощью функций fork() и wait() создать дерево процессов.
Добавить в файловый менеджер запуск бинарных файлов.
•	Занятие 19.
Написать многопоточную программу, выводящую корректные номера потоков.
Симулятор "Магазин". Есть массив из "магазинов", в которых изначально ~1000 единиц товара. Магазины посещают 2 вида посетителей-потоков: "погрузчик" и "покупатель". Погрузчик добавляет каждому магазину ~500 единиц товара и отправляется спать на 2 секунды. Покупатель забирает весь товар из магазина и засыпает на 1 секунду, пока не удовлетворит свои потребности целиком (~10000 единиц). Магазины для посещения выбираются случайным образом. Если магазин занят, то посетитель идет в следующий. Когда покупатель удовлетворяет потребности, он умирает. Когда все клиенты мертвы - погрузчик тоже умирает. Все этапы жизни посетителей магазина должны быть выведены на экран.
Написать реинтерабельную функцию errno с различными кодами ошибок.
Добавить в файловый менеджер возможность копирования файлов со статус-баром.
•	Занятие 20.
Написать командный интерпретатор с поддержкой конвейера (оператор |).
•	Занятие 21.
    Разработать чат на очередях сообщений (msgq).

•	Занятие 22.
Создать сегмент разделяемой памяти и передать через него сообщение из одного процесса в другой, использовать блокировку с помощью семафора.
Разработать чат на разделяемой памяти.
•	Занятие 23.
Написать обработчик сигнала SIGUSR1, который выводит какое-то сообщение. Ожидать его в бесконечном цикле.
С помощью функций pause() и sigprocmask() сделать ожидание более эффективным.
•	Занятие 24.
    Написать клиенты и echo-серверы, модифицирующие принятую строку, использующие UDP- и TCP-сокеты.
