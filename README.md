<h1>RapLang by Ruzal</h1>
<h2>Frontend</h2>
<p>RapLang - Тьюринг-полный язык, позволяющий программировать на нем даже современным русским рэперам</p>

<h3>Синтаксис</h3>
<p>Синтаксис RapLang похож на синтаксис языка C, поэтому описание будет происходить в сравнении с ним</p>
<p>Прежде всего важно отметить, что RapLang работает на методе <a href="https://habr.com/ru/articles/489534/">Рекурсивного спуска</a>.</p>

<h4>Переменные</h4>
<p>Объявление переменной выполняется с помощью команды:</p>

```
Bless name To expr grr
```

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

```
LegitCheck pisyat condition dva Love
        body
Sosa grr
```

Что эквивалентно:

```c
if (condition) {
        body
}
```

<p>За исключением того, что в RapLang нужно заканчивать условный оператор с помощью 'grr'</p>
<p>Циклы:</p>

```
Sesh pisyat condition dva Love
        body
Sosa grr
```

Что эквивалентно:

```c
while (condition) {
        body
}
```

<h4>Функции</h4>

RapLang:

```
VVS main pisyat arg1 $ arg2 $ arg3 dva Love
        body
Sosa grr         
```

C:

```c
void main( int arg1, int arg2, int arg3) {
        body
}
```

<h4>Остальные операторы</h4>

| Оператор        | Описание                                                        | Пример                        |
|-----------------|-----------------------------------------------------------------|-------------------------------|
| FuckTwelve      | Эквивалент return из языка C                                    | FuckTwelve 1 grr              |
| *, /, ^, +, -   | Стандартные операции                                            | -                             |
| >, <, \|, &, is | Стандартные логические операции Замечание: 'is' - это '==' из C | -                             |
| ShoutOut        | Выводит число в консоль                                         | ShoutOut 4 grr                |
| PullUp          | Ввод числа с консоли                                            | Bless a To 0 grr PullUp a grr |
| PawPaw          | Вывести ascii код символа в консоль                             | PawPaw 10 grr                 |


<h2>Что делает Frontend?</h2>
<p>По итогу, код на языке RapLang преобразуется в дерево, попутно Frontend создает для каждой функции свою локальную таблицу</p>

<h3>Правила построения дерева</h3>
<h4>Обьединение команд</h4>
<p>Прежде всего, из синтаксиса мы помним, что все операторы, функции разделяются с помощью <b>grr</b>. Все дерево строится на узлах из этих окончательных символов:</p>

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/a2895803-bbd3-4810-935e-9b70109ed4ca)

<h4>Условные операторы, циклы, определение переменных</h4>

Условные операторы:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/15fa6564-9e56-4d3f-bca1-047dd7474f2c)

Циклы:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/09a766a0-2fa4-4bfa-924d-eaef54d9f2b9)


Определение переменных:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/e3159dac-aa06-4331-8dc6-94aec500d195)

<h4>Функции</h4>
Ниже представлено построенное дерево для объявления функции:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/2fd3a246-a3c0-4fe0-a07d-0258da1aad5d)

Вот так выглядит вызов функции:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/b7344b54-ac99-49e3-9452-40328fc89886)

<h4>Стандартные операции</h4>
Построение стандартных операций очевидно. Это построение дерева для *, /, ^, +, -, >, <, \|, &, is

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/a34e2636-240e-4fe9-a35a-f2b4e706e56d)

<h4>Команды с одним аргументом</h4>
Это такие команды, как ShoutOut, FuckTwelve, PullUp, PawPaw

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/36b71363-7954-494f-9920-ea12fbd2dffe)

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

Тогда, получаем дерево:

![image](https://github.com/SvetoCopy/RapLang/assets/65361271/f4f1fee9-e3e6-48b4-b4c0-79542c43da18)

<h2>Backend</h2>
<p>После получения дерева и таблиц имен, нужно преобразовать код из языка RapLang в <a href="https://github.com/SvetoCopy/CPU">ASM</a></p>
<p>Подробно процесс преобразования описан в LangBackend/def_operator.h. Таким образом, мы можем запустить нашу программу в <a href="https://github.com/SvetoCopy/CPU">ASM</a></p>
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

<h2>ReverseFrontend</h2>
<p>Связка Frontend + Backend достаточна для компиляции кода на языке RapLang. ReverseFrontend же выполняет совсем иную функцию.</p>
<p>Пусть у нас есть построенное дерево и таблицы имен для Backend. ReverseFrontend переводит из этого дерева в код на
языке RapLang. Принцип его работы очень схож с работой Backend, программа также обрабатывает каждый узел и создает код.</p>
<p>Давайте возьмем наш код для чисел Фиббоначи и пропустим его через Frontend, затем полученное дерево преобразуем обратно в код через ReverseFrontend. Затем сравним результат с исходным кодом.</p>

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
	LegitCheck pisyat  is  dva Love 
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
<p>Как видим, минимальный различия</p>




