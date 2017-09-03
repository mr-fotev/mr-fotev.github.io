### Сравнение на сортиращи алгоритми за неотрицателни числа
Тествано е на Intel i5-4210U, компилирано с Microsoft Visual C++ Compiler

N ~ 1 000 000 - 1 200 000<br>
Range = 1 000 000<br>
Attempts = 100<br>

Резултати в ms:

| | Avg | Min| Max |
| --- | --- | --- | --- |
| *Quick Sort* | 108.37 | 97 | 136 |
| *Merge Sort* | 151.87 | 136 | 170 |
| *Heap Sort* | 177.71 | 150 | 227 |
| *Counting Sort* | 70.2 | 60 | 102 |
| *Comb Sort* | 154.89 | 137 | 175 |
| *Shell Sort* | 183.82 | 164 | 214 |
| *STD Sort* | 128.05 | 111 | 153 |
