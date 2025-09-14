## 12.1. Những bước đầu tiên trong tối ưu mã: Code Profiling (Phân tích hiệu năng mã)

*“Vấn đề thực sự là các lập trình viên đã dành quá nhiều thời gian lo lắng về hiệu suất ở những nơi sai và vào những thời điểm sai; tối ưu hóa sớm là cội nguồn của mọi điều xấu (hoặc ít nhất là phần lớn) trong lập trình.”* — Don Knuth, *The Art of Computer Programming*

Một trong những nguy hiểm lớn nhất trong tối ưu mã là khái niệm **premature optimization** (tối ưu hóa sớm). Tối ưu hóa sớm xảy ra khi lập trình viên cố gắng tối ưu dựa trên “cảm giác” về nơi xảy ra sự kém hiệu quả, thay vì dựa trên dữ liệu. Bất cứ khi nào có thể, điều quan trọng là phải đo thời gian chạy của các phần khác nhau trong mã với các đầu vào khác nhau *trước khi* bắt đầu tối ưu, để xác định **hot spot** (điểm nóng) — những khu vực trong chương trình mà có nhiều lệnh được thực thi nhất.

Để tìm cách tối ưu [optExample.c](_attachments/optExample.c), hãy bắt đầu bằng cách xem kỹ hơn hàm `main`:

```c
int main(int argc, char **argv) {
    // error-handling and timing code omitted for brevity

    int limit = strtol(argv[1], NULL, 10);
    int length = limit;
    int *array = allocateArray(length); //allocates array of specified length

    genPrimeSequence(array, limit, &length); //generates sequence of primes

    return 0;
}
```

Hàm `main` gọi hai hàm: `allocateArray`, khởi tạo một mảng với độ dài (hoặc giới hạn) do người dùng chỉ định, và `genPrimeSequence`, tạo ra một dãy số nguyên tố trong giới hạn đã cho (lưu ý rằng với bất kỳ dãy số nào từ 2 đến *n*, số lượng số nguyên tố không thể vượt quá *n*, và thường ít hơn đáng kể).  
Hàm `main` trong [tệp C](_attachments/optExample.c) chứa mã đo thời gian thực thi của hai hàm trên. Khi biên dịch và chạy chương trình với `limit` = 5.000.000, ta thu được kết quả:

```
$ gcc -o optExample optExample.c -lm
$ time -p ./optExample 5000000
Time to allocate: 5.5e-05
Time to generate primes: 3.85525
348513 primes found.
real 3.85
user 3.86
sys 0.00
```

Chương trình `optExample` mất khoảng 3,86 giây để hoàn thành, với gần như toàn bộ thời gian nằm trong hàm `genPrimeSequence`.  
Không có lý do gì để tốn công tối ưu `allocateArray`, vì mọi cải thiện ở đây sẽ không đáng kể đối với thời gian chạy tổng thể.  
Trong các ví dụ tiếp theo, chúng ta sẽ tập trung hơn vào hàm `genPrimeSequence` và các hàm liên quan. Các hàm này được liệt kê lại dưới đây để tiện theo dõi:

```c
// helper function: checks to see if a number is prime
int isPrime(int x) {
    int i;
    for (i = 2; i < sqrt(x) + 1; i++) { //no prime number is less than 2
        if (x % i == 0) { //if the number is divisible by i
            return 0; //it is not prime
        }
    }
    return 1; //otherwise it is prime
}

// finds the next prime
int getNextPrime(int prev) {
    int next = prev + 1;
    while (!isPrime(next)) { //while the number is not prime
        next++; //increment and check again
    }
    return next;
}

// generates a sequence of primes
int genPrimeSequence(int *array, int limit) {
    int i;
    int len = limit;
    if (len == 0) return 0;
    array[0] = 2; //initialize the first number to 2
    for (i = 1; i < len; i++) {
        array[i] = getNextPrime(array[i-1]); //fill in the array
        if (array[i] > limit) {
            len = i;
            return len;
        }
    }
    return len;
}
```

Để tìm **hot spot** trong một chương trình, hãy tập trung vào các khu vực có nhiều vòng lặp nhất. Việc kiểm tra mã thủ công có thể giúp xác định các hot spot, mặc dù luôn cần xác minh lại bằng các công cụ đo hiệu năng trước khi tiến hành tối ưu.  
Khi kiểm tra thủ công chương trình `optExample`, ta có các nhận xét sau:

- Hàm `genPrimeSequence` cố gắng tạo tất cả các số nguyên tố từ 2 đến một số nguyên *n*. Vì số lượng số nguyên tố từ 2 đến *n* không thể vượt quá *n*, vòng lặp `for` trong `genPrimeSequence` chạy tối đa *n* lần. Mỗi vòng lặp gọi hàm `getNextPrime` một lần. Do đó, `getNextPrime` chạy tối đa *n* lần.

- Vòng lặp `while` trong `getNextPrime` sẽ tiếp tục chạy cho đến khi tìm được một số nguyên tố. Mặc dù khó xác định trước số lần vòng lặp `while` này chạy theo *n* (khoảng cách giữa các số nguyên tố liên tiếp có thể rất lớn), nhưng chắc chắn rằng `isPrime` được gọi ở mỗi lần lặp của `while`.

- Hàm `isPrime` chứa đúng một vòng lặp `for`. Giả sử vòng lặp này chạy tổng cộng *k* lần, thì phần thân vòng lặp sẽ chạy *k* lần.  
  Nhớ lại rằng [cấu trúc của vòng lặp for](../C1-C_intro/conditionals.html#_loops_in_c) gồm: *initialization statement* (câu lệnh khởi tạo biến lặp), *Boolean expression* (biểu thức điều kiện dừng vòng lặp), và *step expression* (biểu thức bước, cập nhật biến lặp mỗi vòng).  
  **Bảng 1** dưới đây mô tả số lần mỗi thành phần của vòng lặp được thực thi trong một vòng lặp `for` chạy *k* lần. Trong mọi vòng lặp `for`, phần khởi tạo chỉ chạy đúng một lần. Biểu thức điều kiện chạy *k + 1* lần (vì cần một lần kiểm tra cuối để thoát vòng lặp). Phần thân vòng lặp và biểu thức bước chạy *k* lần mỗi loại.

| Initialization statement | Boolean expression | Step expression | Loop body |
|--------------------------|--------------------|-----------------|-----------|
| 1                        | *k* + 1            | *k*             | *k*       |

**Bảng 1.** Các thành phần thực thi của vòng lặp (giả sử chạy *k* lần)

Qua kiểm tra thủ công, ta thấy chương trình dành phần lớn thời gian trong hàm `isPrime`, và hàm `sqrt` được gọi nhiều nhất. Tiếp theo, chúng ta sẽ sử dụng **code profiling** để xác minh giả thuyết này.

### 12.1.1. Sử dụng Callgrind để phân tích hiệu năng (Profile)

Trong chương trình nhỏ của chúng ta, việc kiểm tra mã thủ công để đưa ra giả thuyết rằng hàm `sqrt` xuất hiện trong một **hot spot** (điểm nóng) của mã là khá đơn giản. Tuy nhiên, việc xác định các hot spot có thể trở nên phức tạp hơn trong các chương trình lớn. Dù thế nào, việc sử dụng **profiling** (phân tích hiệu năng) để xác minh giả thuyết vẫn là một ý tưởng hay. Các công cụ phân tích hiệu năng mã như [Valgrind](http://valgrind.org/) cung cấp rất nhiều thông tin về quá trình thực thi chương trình. Trong phần này, chúng ta sẽ sử dụng công cụ `callgrind` để kiểm tra **call graph** (đồ thị lời gọi hàm) của chương trình `optExample`.


```
$ gcc -g -o optExample optExample.c -lm
$ valgrind --tool=callgrind ./optExample 100000
==32590== Callgrind, a call-graph generating cache profiler
==32590== Copyright (C) 2002-2015, and GNU GPL'd, by Josef Weidendorfer et al.
==32590== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==32590== Command: ./optExample 100000
==32590==
==32590== For interactive control, run 'callgrind_control -h'.
Time to allocate: 0.003869
Time to generate primes: 0.644743
9592 primes found.
==32590==
==32590== Events    : Ir
==32590== Collected : 68338759
==32590==
==32590== I   refs:      68,338,759
```


Để sử dụng `callgrind`, trước tiên hãy biên dịch lại chương trình `optExample` với cờ `-g` và chạy `callgrind` trên một phạm vi nhỏ hơn (từ 2 đến 100.000). Giống như các ứng dụng Valgrind khác, `callgrind` chạy như một lớp bao quanh chương trình, thêm các chú thích như số lần các hàm được thực thi và tổng số lệnh được thực thi. Do đó, chương trình `optExample` sẽ chạy lâu hơn khi kết hợp với `callgrind`.

Gõ `ls` trong terminal sẽ xuất hiện một tệp mới có tên `callgrind.out.xxxxx`, trong đó `xxxxx` là một ID duy nhất. Trong trường hợp này, tệp là `callgrind.out.32590` (tức là số hiển thị ở cột bên trái trong kết quả trước đó). Chạy `callgrind_annotate` trên tệp này sẽ cho thông tin bổ sung về ba hàm mà chúng ta quan tâm:


```
$ callgrind_annotate --auto=yes callgrind.out.32590
    ----------------------------------------------------------------
Profile data file 'callgrind.out.32393' (creator: callgrind-3.11.0)
    ----------------------------------------------------------------
...
    .  //helper function: checks to see if a number is prime
    400,004  int isPrime(int x) {
            .      int i;
36,047,657      for (i = 2; i < sqrt(x)+1; i++) { //no prime is less than 2
13,826,015  => ???:sqrt (2765204x)
16,533,672          if (x % i == 0) { //if the number is divisible by i
    180,818              return 0; //it is not prime
            .          }
            .      }
        9,592      return 1; //otherwise it is prime
    200,002  }
            .
            .  // finds the next prime
    38,368  int getNextPrime(int prev) {
    28,776      int next = prev + 1;
    509,597      while (!isPrime(next)) { //while the number is not prime
67,198,556  => optExample.c:isPrime (100001x)
    90,409          next++; //increment and check again
            .      }
        9,592      return next;
    19,184  }
            .
            .  // generates a sequence of primes
            6  int genPrimeSequence(int * array, int limit) {
            .      int i;
            2      int len = limit;
            2      if (len == 0) return 0;
            2      array[0]=2; //initialize the first number to 2
    38,369      for (i = 1; i < len; i++) {
    143,880          array[i] = getNextPrime(array[i-1]); //fill in the array
67,894,482  => optExample.c:getNextPrime (9592x)
    76,736          if (array[i] > limit){
            2              len = i;
            2              return len;
            .          }
            .      }
            .      return len;
            4  }

```

Các con số ở cột bên trái biểu thị tổng số lệnh được thực thi liên quan đến từng dòng mã. Các con số trong ngoặc đơn cho biết số lần một hàm cụ thể được chạy. Dựa vào các con số này, chúng ta có thể xác minh kết quả của việc kiểm tra thủ công.  
Trong hàm `genPrimeSequence`, hàm `getNextPrime` tạo ra số lượng lệnh thực thi nhiều nhất — 67,8 triệu lệnh, tương ứng với 9.592 lần gọi hàm (để tạo các số nguyên tố từ 2 đến 100.000).  
Kiểm tra `getNextPrime` cho thấy phần lớn số lệnh này (67,1 triệu, tức 99%) đến từ lời gọi hàm `isPrime`, được gọi tổng cộng 100.001 lần.  
Cuối cùng, kiểm tra `isPrime` cho thấy 13 triệu lệnh (20,5%) đến từ hàm `sqrt`, được thực thi tổng cộng 2,7 triệu lần.

Các kết quả này xác nhận giả thuyết ban đầu rằng chương trình dành phần lớn thời gian trong hàm `isPrime`, và hàm `sqrt` là hàm được gọi nhiều nhất. Giảm tổng số lệnh thực thi sẽ giúp chương trình chạy nhanh hơn; phân tích trên gợi ý rằng nỗ lực ban đầu nên tập trung vào việc cải thiện hàm `isPrime`, và có thể giảm số lần gọi `sqrt`.

### 12.1.2. Loop-Invariant Code Motion (Di chuyển mã bất biến ra khỏi vòng lặp)

**Loop-invariant code motion** là một kỹ thuật tối ưu hóa di chuyển các phép tính tĩnh (không thay đổi) bên trong vòng lặp ra ngoài vòng lặp mà không ảnh hưởng đến hành vi của vòng lặp. Các **optimizing compiler** (trình biên dịch tối ưu) có thể tự động thực hiện hầu hết các tối ưu hóa dạng này.  
Cụ thể, cờ biên dịch `-fmove-loop-invariants` trong GCC (được bật ở mức `-O1`) sẽ cố gắng xác định các trường hợp loop-invariant code và di chuyển chúng ra ngoài vòng lặp tương ứng.

Tuy nhiên, trình biên dịch không phải lúc nào cũng xác định được các trường hợp loop-invariant, đặc biệt là với **function call** (lời gọi hàm). Vì lời gọi hàm có thể vô tình gây ra **side effect** (tác dụng phụ — hành vi không mong muốn), hầu hết các trình biên dịch sẽ tránh cố gắng xác định xem một hàm có luôn trả về cùng một kết quả hay không.  
Do đó, mặc dù lập trình viên biết rằng `sqrt(x)` luôn trả về căn bậc hai của `x`, GCC sẽ không luôn giả định điều đó.  
Hãy xem xét trường hợp hàm `sqrt` cập nhật một biến toàn cục bí mật `g`. Khi đó, việc gọi `sqrt` một lần bên ngoài vòng lặp (*một* lần cập nhật `g`) không giống với việc gọi nó ở mỗi vòng lặp (*n* lần cập nhật `g`). Nếu trình biên dịch không thể xác định rằng một hàm luôn trả về cùng kết quả, nó sẽ không tự động di chuyển lời gọi hàm đó ra ngoài vòng lặp.

Tuy nhiên, lập trình viên biết rằng việc di chuyển phép tính `sqrt(x) + 1` ra ngoài vòng lặp `for` sẽ không ảnh hưởng đến hành vi của vòng lặp. Hàm đã chỉnh sửa được hiển thị dưới đây và có sẵn [trong tệp này](_attachments/optExample2.c):

```c
//helper function: checks to see if a number is prime
int isPrime(int x) {
    int i;
    int max = sqrt(x)+1;
    for (i = 2; i < max; i++) { //no prime number is less than 2
        if (x % i == 0) { //if the number is divisible by i
            return 0; //it is not prime
        }
    }
    return 1; //otherwise it is prime
}
```

**Bảng 2** cho thấy thay đổi đơn giản này giúp giảm hẳn 2 giây (47%) thời gian chạy của `optExample2`, ngay cả trước khi sử dụng các cờ tối ưu của trình biên dịch. Hơn nữa, trình biên dịch dường như dễ dàng tối ưu `optExample2` hơn một chút.

| Phiên bản       | Unoptimized | `-O1` | `-O2` | `-O3` |
|-----------------|-------------|-------|-------|-------|
| Original        | 3.86        | 2.32  | 2.14  | 2.15  |
| With loop-invariant code motion | 1.83 | 1.63  | 1.71  | 1.63  |

**Bảng 2.** Thời gian (giây) để tạo các số nguyên tố từ 2 đến 5.000.000

Chạy lại `callgrind` trên file thực thi `optExample2` cho thấy lý do tại sao thời gian chạy được cải thiện nhiều như vậy. Đoạn mã dưới đây giả định rằng tệp `callgrind.out.30086` chứa các chú thích khi chạy `callgrind` trên `optExample2`:


```
$ gcc -g -o optExample2 optExample2.c -lm
$ valgrind --tool=callgrind ./optExample2 100000
$ callgrind_annotate --auto=yes callgrind.out.30086
    ------------------------------------------------------------------
Profile data file 'callgrind.out.30086' (creator: callgrind-3.11.0)
    ------------------------------------------------------------------
    ...
    400,004  int isPrime(int x) {
            .      int i;
    900,013      int max = sqrt(x)+1;
    500,000  => ???:sqrt (100001x)
11,122,449      for (i = 2; i < max; i++) { //no prime number is less than 2
16,476,120          if (x % i == 0) { //if the number is divisible by i
    180,818              return 0; //it is not prime
            .          }
            .      }
        9,592      return 1; //otherwise it is prime
    200,002  }
            .
            .  // finds the next prime
    38,368  int getNextPrime(int prev) {
    28,776      int next = prev + 1;
    509,597      while (!isPrime(next)) { //while the number is not prime
29,789,794  => optExample2.c:isPrime (100001x)
    90,409          next++; //increment and check again
            .      }
        9,592      return next;
    19,184  }

```

Việc di chuyển lời gọi `sqrt` ra ngoài vòng lặp `for` đã giảm số lần gọi hàm `sqrt` trong chương trình từ 2,7 triệu xuống còn 100.000 (giảm 96%). Con số này tương ứng với số lần hàm `isPrime` được gọi, xác nhận rằng hàm `sqrt` chỉ thực thi một lần cho mỗi lần gọi `isPrime`.

Lưu ý rằng trình biên dịch có thể thực hiện mức tối ưu hóa đáng kể khi bật các cờ tối ưu, ngay cả khi lập trình viên không tự tay di chuyển mã. Trong trường hợp này, lý do là nhờ một lệnh đặc biệt gọi là `fsqrt` được quy định trong **x86 ISA**. Khi bật các cờ tối ưu, trình biên dịch thay thế tất cả các lời gọi hàm `sqrt` bằng lệnh `fsqrt`.  
Quá trình này được gọi là **inlining** (nội tuyến), và chúng ta sẽ tìm hiểu chi tiết hơn trong phần tiếp theo. Vì `fsqrt` không còn là một hàm, trình biên dịch dễ dàng nhận ra tính loop-invariant của nó và di chuyển nó ra ngoài thân vòng lặp.