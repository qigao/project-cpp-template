# Readme

## Что сделано

Реализация вычисления формул случайных величин с бросками кубов.<br/>

Использована обратная польская нотация: https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%80%D0%B0%D1%82%D0%BD%D0%B0%D1%8F_%D0%BF%D0%BE%D0%BB%D1%8C%D1%81%D0%BA%D0%B0%D1%8F_%D0%B7%D0%B0%D0%BF%D0%B8%D1%81%D1%8C<br/>

Использован генератор парсеров ragel: http://www.colm.net/open-source/ragel/

Для сборки рагельских файлов использован модуль: https://github.com/gsauthof/cmake-ragel

## Как это работает

![схема автоматов парсера](https://github.com/bulgvkov/diceParserRagel/blob/main/schemeOfParser.png)

## Benchmark

Формула: 2 + 2<br/>
![Teст 1: 2 + 2](https://github.com/bulgvkov/diceParserRagel/blob/main/tests/test1.png)

Формула: (3d6! + 5) * 2 + (1d6)d3M1<br/>
![Тест 2: (3d6! + 5)*2 + (1d6)d3M1](https://github.com/bulgvkov/diceParserRagel/blob/main/tests/test2.png)

Формула: (20d100!)d((2 + 2 * 2)d30M3) - (d20d30m15d((10 + 4d8! % 9) / 3 + 1))<br/>
![Teст 3: (20d100!)d((2 + 2 * 2)d30M3) - (d20d30m15d((10 + 4d8! % 9) / 3 + 1))](https://github.com/bulgvkov/diceParserRagel/blob/main/tests/test3.png)
