# finite_automation

## Инструкция: 
- скомпилировать - ./compile.sh
- запустить (после компиляции) - ./run.sh
- запустить тесты (после компиляции) - ./test.sh

Вводить данные нужно в input.txt.

## Формат данных:
первая строка - последовательность команд, разделенных |

следующие строки - ввод автомата/регулярного выражения.

Команды представляют собой текстовые строки. 
Первая команда обязательно должна быть на ввод автомата/регулярного выражения.
Больше команды ввода не должны встречаться.
После последней команды | не нужен.

## Что умееть программа и соответствующая команда:
- считывать конечный автомат - automaton
- считывать регулярное выражение - regular expression
- считывать регулярное выражение в обратной польской нотации - reverse polish notation
- преобразовывать регулярное выражение в конечный автомат - to automaton
- удалить пустые слова из конечного автомата - delete empty words
- преобразовать КА без пустых слов в детерминированный конечный автомат - to dfa
- преобразовать ДКА в полный ДКА - to complete dfa
- преобразовать ПДКА в минимальный ПДКА - to minimal
- преобразовать минимальный ПДКА в регулярное выражени - to regular expression

Как видно из команд, пока что предыдущая команда детерминирует следующую. 
Это значит, что нельзя написать, например, automaton | to complete dfa, т.к. перед to complete dfa должна идти команда to dfa.

## Формат ввода/вывода регулярного выражения
Допустимые символы: строчные латинские буквы, символ `ε` для пустого слова и символы `()+*`

Звезда Клини относится ко всему слову, то есть `abba*` означает `(abba)*`. 
Если нужно применить звезду Клини не ко всему слову, используйте скобки: `abb(a)*`.

## Формат ввода/вывода конечного автомата
Первая строка - два числа: количество вершин n и количество рёбер m.

Далее в m строках перечисляются рёбра: первое число - откуда исходит ребро, второе число - куда идет ребро, далее строка - слово на ребре.
Слово должно состоять из строчных латинский букв.

В следующей строке вводится номер стартовой вершины.

Далее число k - количество завершающих вершин. На следующей строке k чисел - номера завершающих вершин.

Вершины в вводе должны быть от 0 до n - 1 включительно.

# Еще о вводе

Пример ввода автомата и вообще ввода можно посмотреть в input.txt.

Если ввод некорректный, то Undefined Behaviour (в лучшем случае упадет на одном из assert).

Вот пример ещё пример корректного ввода:

regular expression | to automaton | delete empty words | to dfa | to complete dfa | to minimal | to regular expression

`aba+caba  *a+ε +b(a)*(a+b*)` 
