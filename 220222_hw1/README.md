# Описание задачи

**Двухсторонняя передача данных через неименованные каналы**

Написать 2 программы.

[Первая](source/myPipeS.c) создаёт процесс второй программы. И через каналы сначала отправляет ей данные, затем получает ответ.

[Вторая программа](source/myPipeR.c) сначала принимает данные через канал, затем отправляет свои данные обратно в первую и закрывается.

# Содержимое папки

1. [Makefile](Makefile) - Мейк, компилирующий программу.
2. [source](source) - папка с исходниками и хедерами.