<h1>RapLang by Ruzal</h1>
<h2>Frontend</h2>
<p>RapLang - Тьюринг-полный язык, позволяющий программировать на нем даже современным русским рэперам.</p>
<h3>Синтаксис</h3>
<p>Синтаксис RapLang похож на синтаксис языка C, поэтому описание будет происходить отталкиваясь от него</p>


<h4>Переменные</h4>
<p>Объявление переменной выполняется с помощью команды:</p>

| RapLang                      | C                      |
|------------------------------|------------------------|
| ```Bless name To expr grr``` | ```int name = expr;``` |

<p>Объявление переменной без инициализации невозможно в RapLang</p>
<p>После любого объявления, необходимо явно обозначить конец команды с помощью 'grr' </p>

Пример:
```
Bless a To 6 grr
```
<h5>Правила составления названий переменных</h5>

```
name = { {'A'-'Z'} || {'a'-'z'} || {'0'-'9'} || '_' } +
```

<h4>Условные операторы, циклы</h2>
<p>В RapLang определены условные операторы, циклы:</p>

<p>Условные операторы:</p>

| RapLang                                                | C                                |
|--------------------------------------------------------|----------------------------------|
| ```LegitCheck pisyat condition dva Love body Sosa ``` | ```if ( condition ) { body } ``` |

<p>За исключением того, что в RapLang нужно заканчивать условный оператор с помощью 'grr'</p>

<p>Циклы:</p>

| RapLang                                             | C                                 |
|-----------------------------------------------------|-----------------------------------|
| ```Sesh pisyat condition dva Love body Sosa grr ``` | ```while (condition) { body } ``` |

<h4>Функции</h4>

| RapLang                                                                | C                                                             |
|------------------------------------------------------------------------|---------------------------------------------------------------|
| ```VVS name pisyat arg1 $ arg2 $ arg3 $ ... dva Love body Sosa grr ``` | ```void name( int arg1, int arg2, int arg3, ...) { body } ``` |

<h4>Остальные операторы</h4>

| Оператор        | Описание                                                        | Пример                        |
|-----------------|-----------------------------------------------------------------|-------------------------------|
| FuckTwelve      | Эквивалент return из языка C                                    | FuckTwelve 1 grr              |
| *, /, ^, +, -   | Стандартные арифметические операции                             | -                             |
| >, <, \|, &, is | Стандартные логические операции Замечание: 'is' - это '==' из C | -                             |
| ShoutOut        | Выводит число в консоль                                         | ShoutOut 4 grr                |
| PullUp          | Ввод числа с консоли                                            | Bless a To 0 grr PullUp a grr |
| PawPaw          | Вывести ascii код символа в консоль                             | PawPaw 10 grr                 |


<h2>Что делает Frontend?</h2>
<p>По итогу, код на языке RapLang преобразуется в дерево, попутно Frontend создает для каждой функции свою локальную таблицу</p>
<p>RapLang работает на методе <a href="https://habr.com/ru/articles/489534/">Рекурсивного спуска</a>.</p>

<h3>Правила построения дерева</h3>
<h4>Обьединение команд</h4>
<p>Прежде всего, из синтаксиса мы помним, что все операторы, функции разделяются с помощью <b>grr</b>. Все дерево строится на узлах из этих окончательных символов:</p>

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/fba188e2-e375-4c58-bdbb-d655b6e925fb)


<h4>Условные операторы, циклы, определение переменных</h4>

Условные операторы:

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/f7b9fc3c-5474-44e8-bbb8-7cc262382946)

Циклы:

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/13895e4e-3a16-41a3-9c5d-463b34a5c82a)

Определение переменных:

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/4038066e-519a-46e9-97a7-63b49a3fe414)


<h4>Функции</h4>
Ниже представлено построенное дерево для объявления функции:

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/0d7d943e-e323-4512-814d-c5ab6cf0de60)


Вот так выглядит вызов функции:

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/eaf0688a-0155-4054-b062-e323fb1bfbeb)


<h4>Стандартные операции</h4>
Построение арифметических стандартных операций очевидно. Это построение дерева для *, /, ^, +, -, >, <, \|, &, is

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/1c44f6e5-1b59-422d-9d97-52b35db2d6c8)

<h4>Команды с одним аргументом</h4>
Это такие команды, как ShoutOut, FuckTwelve, PullUp, PawPaw

![example1](https://github.com/SvetoCopy/RapLang/assets/65361271/e8350f4b-a558-4059-b827-6e56e0bbe998)

<h4>Пример</h4>
<p>Посмотрим как это выглядит для небольшой программы, которая считает числа Фиббоначи:</p>

```c
VVS Fib pisyat n dva Love
        ShoutOut n grr
        LegitCheck pisyat n is 1 dva Love
		FuckTwelve 1 grr
	Sosa grr
	
	Bless a To n * Fib pisyat n - 1 dva grr 
	ShoutOut n grr
	
	FuckTwelve a grr
Sosa grr

VVS main pisyat dva Love
	ShoutOut Fib pisyat 3 dva grr
	FuckTwelve 0 grr
Sosa grr
```

Тогда, получаем дерево, построенное с помощью GraphViz:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/f4f1fee9-e3e6-48b4-b4c0-79542c43da18)

<h2>Backend</h2>
<p>После получения дерева и таблиц имен, нужно транслировать код из языка RapLang в <a href="https://github.com/SvetoCopy/CPU">ASM</a></p>
<p>Подробно трансляция описана в LangBackend/def_operator.h. Таким образом, мы можем запустить нашу программу в <a href="https://github.com/SvetoCopy/CPU">ASM</a>. Это такой проект, который эмулирует работу реального процессора</p>
<p>Вот так выглядит <a href="https://github.com/SvetoCopy/CPU">ASM</a> код для нашего примера с числами Фиббоначи, сгенерированного с помощью  Backend:</p>

```asm
CALL main
HLT
Factorial:

; the next operator has gone from 2 line
PUSH [0]
OUT

; the next operator has gone from 3 line
PUSH 1.000000
PUSH [0]
JE cond_1
PUSH 0
JMP end_cond2
cond_1:
PUSH 1
end_cond2:
PUSH 0
JE end_if_3

; the next operator has gone from 4 line
PUSH 1.000000
POP RAX
RET
end_if_3:

; the next operator has gone from 7 line
PUSH [0]
PUSH [0] ; saving 0 arg
PUSH [1] ; saving 1 arg
PUSH [0]
PUSH 1.000000
SUB

POP [0]
CALL Factorial
POP [1]; getting old 1 arg
POP [0]; getting old 0 arg
PUSH RAX
MUL
POP [1]

; the next operator has gone from 8 line
PUSH [0]
OUT

; the next operator has gone from 10 line
PUSH [1]
POP RAX
RET
main:

; the next operator has gone from 14 line
PUSH 3.000000

POP [0]
CALL Factorial
PUSH RAX
OUT

; the next operator has gone from 15 line
PUSH 0.000000
POP RAX
RET
```
<p>Комментарии вставляются автоматически для отображения строки, которую мы транслируем</p>


<h2>ReverseFrontend</h2>
<p>Связка Frontend + Backend достаточна для компиляции кода на языке RapLang. ReverseFrontend же выполняет совсем иную функцию.</p>
<p>Пусть у нас есть построенное дерево и таблицы имен для Backend. ReverseFrontend переводит из этого дерева в код на
языке RapLang. Принцип его работы очень схож с работой Backend, программа также обрабатывает каждый узел и создает код.</p>
<p>Давайте возьмем наш код для чисел Фиббоначи и пропустим его через Frontend, затем полученное дерево транслируем обратно в код через ReverseFrontend. Затем сравним результат с исходным кодом.</p>

<p>Исходный код:</p>

```
VVS Factorial pisyat n dva Love
	ShoutOut n grr
	LegitCheck pisyat n is 1 dva Love
		FuckTwelve 1 grr
	Sosa grr
	
	Bless a To n * Factorial pisyat n - 1 dva grr 
	ShoutOut n grr
	
	FuckTwelve a grr
Sosa grr

VVS main pisyat dva Love
	ShoutOut Factorial pisyat 3 dva grr
	FuckTwelve 0 grr
Sosa grr
```

<p>Полученный код:</p>

```
VVS Factorial pisyat n dva  Love
	ShoutOut n grr
	LegitCheck pisyat n is 1.000000 dva Love 
		FuckTwelve 1.000000 grr
	Sosa  grr
	Bless a To  pisyat n * Factorial pisyat  pisyat n - 1.000000 dva  dva  dva  grr
	ShoutOut n grr
	FuckTwelve a grr
Sosa  grr
VVS main pisyat  dva  Love
	ShoutOut Factorial pisyat 3.000000 dva  grr
	FuckTwelve 0.000000 grr
Sosa  grr
```
<p>Как видим, минимальные различия</p>




