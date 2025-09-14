## 12. Tối ưu hóa mã (Code Optimization)

**Code optimization** (tối ưu hóa mã) là quá trình cải thiện một chương trình bằng cách giảm kích thước mã, độ phức tạp, mức sử dụng bộ nhớ hoặc thời gian chạy (hoặc kết hợp các yếu tố này) mà không thay đổi chức năng vốn có của chương trình.  
Nhiều hệ thống biên dịch bao gồm một **code optimizer** (bộ tối ưu mã) như một bước trung gian. Cụ thể, một **optimizing compiler** (trình biên dịch tối ưu) sẽ áp dụng các phép biến đổi cải thiện mã như một phần của quá trình biên dịch. Hầu như tất cả các trình biên dịch hiện đại (bao gồm GCC) đều là optimizing compiler.  
Trình biên dịch C của GCC triển khai nhiều **optimization flag** (cờ tối ưu hóa) cho phép lập trình viên truy cập trực tiếp một tập con các tối ưu hóa đã được triển khai. Các cờ tối ưu hóa này giúp tối ưu mã nhưng phải đánh đổi với thời gian biên dịch và khả năng gỡ lỗi.  
Để đơn giản, GCC gói một tập con các cờ tối ưu hóa này thành các **optimization level** (mức tối ưu hóa) khác nhau mà lập trình viên có thể gọi trực tiếp. Ví dụ, lệnh sau biên dịch một chương trình mẫu với mức tối ưu hóa cấp 1:

```bash
$ gcc -O1 -o program program.c
```

Mức tối ưu hóa 1 (`-O1` hoặc `-O`) trong GCC thực hiện các tối ưu hóa cơ bản để giảm kích thước mã và thời gian thực thi, đồng thời cố gắng giữ thời gian biên dịch ở mức tối thiểu.  
Mức tối ưu hóa 2 (`-O2`) bao gồm hầu hết các tối ưu hóa đã được GCC triển khai mà không liên quan đến sự đánh đổi giữa dung lượng và hiệu năng.  
Cuối cùng, mức tối ưu hóa 3 (`-O3`) thực hiện thêm các tối ưu hóa bổ sung (chẳng hạn như **function inlining** — nội tuyến hàm, sẽ được thảo luận sau trong chương này) và có thể khiến chương trình mất nhiều thời gian biên dịch hơn đáng kể.  
[Tài liệu GCC](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) mô tả chi tiết các cờ tối ưu hóa đã được triển khai.

Việc thảo luận chi tiết về optimizing compiler và cách xây dựng, vận hành của chúng nằm ngoài phạm vi của cuốn sách này; chúng tôi khuyến khích bạn đọc quan tâm tìm hiểu cuốn sách kinh điển *Compilers: Principles, Techniques, and Tools* của Aho, Sethi và Ullman.  
Mục đích của chương này là làm nổi bật một số điều mà hầu hết các trình biên dịch có thể (và không thể) làm, cũng như cách lập trình viên có thể phối hợp với trình biên dịch và các công cụ profiling để giúp cải thiện mã của mình.

### Những gì trình biên dịch đã làm sẵn

Một số tối ưu hóa phổ biến được hầu hết mọi trình biên dịch thực hiện sẽ được mô tả ngắn gọn trong các phần tiếp theo.  
Sinh viên *không bao giờ* nên tự tay triển khai các tối ưu hóa này, vì chúng đã được trình biên dịch thực hiện sẵn.

**Constant Folding** (gộp hằng)

:   Các hằng số trong mã được tính toán ngay tại thời điểm biên dịch để giảm số lượng lệnh sinh ra.  
    Ví dụ, trong đoạn mã dưới đây, **macro expansion** (mở rộng macro) thay câu lệnh `int debug = N-5` bằng `int debug = 5-5`. Sau đó, **constant folding** cập nhật câu lệnh này thành `int debug = 0`.

```c
#define N 5
int debug = N - 5; // constant folding thay đổi câu lệnh này thành debug = 0;
```

**Constant Propagation** (truyền hằng)

:   **Constant propagation** thay thế biến bằng một giá trị hằng nếu giá trị đó được biết tại thời điểm biên dịch.  
    Xem đoạn mã sau:

```c
int debug = 0;

// sums up all the elements in an array
int doubleSum(int *array, int length){
    int i, total = 0;
    for (i = 0; i < length; i++){
        total += array[i];
        if (debug) {
            printf("array[%d] is: %d\n", i, array[i]);
        }
    }
    return 2 * total;
}
```

Một trình biên dịch áp dụng constant propagation sẽ thay `if (debug)` thành `if (0)`.

**Dead Code Elimination** (loại bỏ mã chết)

:   Không hiếm khi một chương trình chứa các biến, phép gán hoặc câu lệnh không được sử dụng.  
    Mặc dù các câu lệnh không cần thiết này hiếm khi được đưa vào một cách cố ý, nhưng chúng thường là sản phẩm phụ tự nhiên của quá trình phát triển và tinh chỉnh phần mềm liên tục.  
    Nếu không được phát hiện, các đoạn mã gọi là **dead code** này có thể khiến trình biên dịch sinh ra các lệnh assembly không cần thiết, từ đó lãng phí thời gian xử lý.  
    Hầu hết các trình biên dịch sử dụng các kỹ thuật như **dataflow analysis** (phân tích luồng dữ liệu) để xác định các đoạn mã không thể truy cập và loại bỏ chúng.  
    **Dead code elimination** thường giúp chương trình chạy nhanh hơn bằng cách thu nhỏ kích thước mã và tập lệnh liên quan.  
    Ví dụ, hãy xem lại hàm `doubleSum` trong đó trình biên dịch đã áp dụng constant propagation để thay `debug` bằng `0` trong câu lệnh `if`:

```c
int debug = 0;

// sums up all the elements in an array
int doubleSum(int *array, int length){
    int i, total = 0;
    for (i = 0; i < length; i++){
        total += array[i];
        if (0) { // debug đã được thay bằng hằng số 0 bởi trình biên dịch
            printf("array[%d] is: %d\n", i, array[i]);
        }
    }
    return 2 * total;
}
```

Một trình biên dịch sử dụng dataflow analysis sẽ nhận ra rằng câu lệnh `if` này luôn sai và câu lệnh `printf` sẽ không bao giờ được thực thi.  
Do đó, trình biên dịch sẽ loại bỏ câu lệnh `if` và lời gọi `printf` trong file thực thi đã biên dịch.  
Một lượt tối ưu khác cũng sẽ loại bỏ câu lệnh `debug = 0`.

**Simplifying expressions** (đơn giản hóa biểu thức)

:   Một số lệnh tốn nhiều chi phí hơn các lệnh khác.  
    Ví dụ, các lệnh số học `imul` và `idiv` trong assembly mất nhiều thời gian để thực thi.  
    Trình biên dịch thường cố gắng giảm số lượng các lệnh tốn kém này bằng cách đơn giản hóa các phép toán bất cứ khi nào có thể.  
    Ví dụ, trong hàm `doubleSum`, trình biên dịch có thể thay biểu thức `2 * total` bằng `total + total` vì lệnh cộng ít tốn chi phí hơn phép nhân:

```
//declaration of debug removed through dead-code elimination

//sums up all the elements in an array
int doubleSum(int *array, int length){
    int i, total = 0;
    for (i = 0; i < length; i++){
        total += array[i];
        //if statement removed through data-flow analysis
    }
    return total + total; //simplifying expression
}
```


Tương tự, trình biên dịch cũng sẽ biến đổi các đoạn mã sử dụng **bit-shifting** (dịch bit) và các toán tử bitwise khác để đơn giản hóa biểu thức.  
Ví dụ, trình biên dịch có thể thay biểu thức `total * 8` bằng `total << 3`, hoặc thay biểu thức `total % 8` bằng `total & 7`, vì các phép toán bitwise được thực hiện chỉ với một lệnh nhanh duy nhất.

### Những gì trình biên dịch không phải lúc nào cũng làm được: Lợi ích của việc học tối ưu hóa mã

Với những lợi ích mà **optimizing compiler** (trình biên dịch tối ưu) mang lại, có thể sẽ không rõ ràng ngay lập tức tại sao việc học tối ưu hóa mã lại hữu ích.  
Bạn có thể dễ dàng nghĩ về trình biên dịch như một “hộp đen” thông minh kỳ diệu. Nhưng cuối cùng, trình biên dịch cũng chỉ là một phần mềm thực hiện một loạt các phép biến đổi mã nhằm tăng tốc độ chạy.  
Trình biên dịch cũng bị giới hạn trong các loại tối ưu hóa mà nó có thể thực hiện.

**Algorithmic Strength Reduction Is Impossible** (Không thể giảm độ phức tạp thuật toán một cách tự động)

:   Nguyên nhân hàng đầu dẫn đến hiệu năng kém là do lựa chọn sai **data structure** (cấu trúc dữ liệu) và **algorithm** (thuật toán).  
    Trình biên dịch không thể “phép màu” sửa chữa những quyết định sai này.  
    Ví dụ, một trình biên dịch sẽ không bao giờ tối ưu một chương trình đang cài đặt **bubble sort** thành một chương trình dùng **quick sort**.  
    Mặc dù độ tinh vi của trình biên dịch và các tối ưu hóa của nó ngày càng được cải thiện, nhưng *chất lượng* tối ưu hóa của từng trình biên dịch cụ thể vẫn khác nhau giữa các nền tảng.  
    Do đó, trách nhiệm thuộc về lập trình viên trong việc đảm bảo mã của mình sử dụng các thuật toán và cấu trúc dữ liệu tốt nhất.

**Compiler Optimization Flags Are Not Guaranteed to Make Code "Optimal" (or Consistent)**  
(Các cờ tối ưu hóa của trình biên dịch không đảm bảo mã sẽ “tối ưu” hoặc nhất quán)

:   Việc tăng mức tối ưu hóa của trình biên dịch (ví dụ: từ `-O2` lên `-O3`) không phải lúc nào cũng làm giảm thời gian chạy của chương trình.  
    Đôi khi, lập trình viên có thể phát hiện rằng việc nâng cờ tối ưu từ `-O2` lên `-O3` lại *làm chậm* chương trình, hoặc không mang lại cải thiện hiệu năng nào.  
    Trong một số trường hợp khác, lập trình viên có thể thấy rằng chương trình biên dịch **không** dùng cờ tối ưu thì chạy bình thường, nhưng khi biên dịch với `-O2` hoặc `-O3` lại gây ra **segmentation fault** hoặc lỗi khác.  
    Những lỗi lập trình kiểu này đặc biệt khó gỡ, vì cờ debug (`-g`) của GCC không tương thích với các cờ tối ưu (`-O`), do các phép biến đổi mà trình biên dịch thực hiện ở các mức `-O` gây cản trở khả năng phân tích mã của **debugger**.  
    Cờ `-g` lại là yêu cầu bắt buộc của nhiều công cụ profiling phổ biến như GDB và Valgrind.

Một nguyên nhân lớn gây ra hành vi không nhất quán là do tiêu chuẩn C/C++ không đưa ra hướng dẫn rõ ràng về cách xử lý **undefined behavior** (hành vi không xác định).  
Kết quả là, thường thì trình biên dịch sẽ tự quyết định cách xử lý sự mơ hồ này.  
Sự khác biệt trong cách các mức tối ưu hóa xử lý undefined behavior có thể khiến kết quả *thay đổi*.  
Xem ví dụ sau từ John Regehr¹:

```c
int silly(int a) {
  return (a + 1) > a;
}
```

Giả sử `silly` được chạy với `a = INT_MAX`. Trong trường hợp này, phép tính `a + 1` gây **integer overflow** (tràn số nguyên).  
Tuy nhiên, tiêu chuẩn C/C++ không định nghĩa *cách* trình biên dịch phải xử lý tràn số nguyên.  
Thực tế, biên dịch chương trình **không** dùng tối ưu hóa khiến hàm trả về 0, trong khi biên dịch với tối ưu hóa `-O3` lại khiến hàm trả về 1.

Tóm lại, các cờ tối ưu hóa nên được sử dụng một cách thận trọng, có cân nhắc và khi thật sự cần thiết.  
Việc hiểu rõ cờ tối ưu nào nên dùng cũng giúp lập trình viên **hợp tác** với trình biên dịch thay vì “đối đầu” với nó.

> **The compiler is not required to handle undefined behavior**  
> Hàm `silly` khi chạy với `a = INT_MAX` là một ví dụ về undefined behavior.  
> Lưu ý rằng kết quả không nhất quán do trình biên dịch tạo ra **không** phải là lỗi thiết kế của trình biên dịch hay là hậu quả của việc dùng cờ tối ưu hóa.  
> Trình biên dịch được thiết kế để tuân theo **specification** (đặc tả) của ngôn ngữ.  
> Tiêu chuẩn ngôn ngữ C không quy định trình biên dịch phải làm gì khi gặp undefined behavior; chương trình có thể bị crash, không biên dịch được, hoặc tạo ra kết quả không nhất quán hoặc sai.  
> Cuối cùng, lập trình viên phải chịu trách nhiệm phát hiện và loại bỏ undefined behavior trong mã.  
> Việc `silly` nên trả về 0, 1 hay giá trị khác là quyết định mà lập trình viên phải đưa ra.  
> Để tìm hiểu thêm về undefined behavior và các vấn đề liên quan trong C, hãy xem **C FAQ²** hoặc blog của John Regehr¹.

**Pointers Can Prove Problematic** (Con trỏ có thể gây rắc rối)

:   Hãy nhớ rằng trình biên dịch chỉ thực hiện các phép biến đổi mà không làm thay đổi hành vi cơ bản của chương trình nguồn.  
    Nếu một phép biến đổi có nguy cơ thay đổi hành vi của chương trình, trình biên dịch sẽ không thực hiện nó.  
    Điều này đặc biệt đúng trong trường hợp **memory aliasing** (trùng địa chỉ bộ nhớ), khi hai con trỏ khác nhau trỏ đến cùng một địa chỉ trong bộ nhớ.  
    Ví dụ, hãy xem hàm `shiftAdd` dưới đây, nhận hai con trỏ số nguyên làm tham số. Hàm này nhân số thứ nhất với 10 và cộng số thứ hai vào.  
    Vì vậy, nếu `shiftAdd` được truyền vào hai số nguyên 5 và 6, kết quả sẽ là 56.

#### averageMat_v1

```c
float averageMat_v1(int **mat, int n) {
    int i, j, total = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Note indexing: [i][j]
            total += mat[i][j];
        }
    }
    return (float) total / (n * n);
}
```

#### averageMat_v2

```c
float averageMat_v2(int **mat, int n) {
    int i, j, total = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Note indexing: [j][i]
            total += mat[j][i];
        }
    }
    return (float) total / (n * n);
}
```

**Bảng 1.** So sánh hai hàm nhân số thứ nhất với 10 và cộng số thứ hai vào. Có tại [liên kết này](_attachments/shiftadd.c).

Hàm `shiftAddOpt` tối ưu hàm `shiftAdd` bằng cách loại bỏ một lần truy cập bộ nhớ bổ sung tới `a`, dẫn đến tập lệnh nhỏ hơn trong mã assembly đã biên dịch.  
Tuy nhiên, trình biên dịch sẽ **không bao giờ** tự thực hiện tối ưu hóa này do rủi ro **memory aliasing** (trùng địa chỉ bộ nhớ).  
Để hiểu lý do, hãy xem xét hàm `main` sau:

```c
int main(void){
    int x = 5;
    int y = 6;
    shiftAdd(&x, &y); // should produce 56
    printf("shiftAdd produces: %d\n", x);

    x = 5; // reset x
    shiftAddOpt(&x, &y); // should produce 56
    printf("shiftAddOpt produces: %d\n", x);

    return 0;
}
```

Biên dịch và chạy chương trình này cho kết quả như mong đợi:

```bash
$ gcc -o shiftadd shiftadd.c
$ ./shiftadd
shiftAdd produces: 56
shiftAddOpt produces: 56
```

Giả sử, thay vào đó, chương trình được sửa đổi để `shiftAdd` nhận **cùng một con trỏ tới `x`** cho cả hai tham số:

```c
int main(void){
    int x = 5;
    shiftAdd(&x, &x); // should produce 55
    printf("shiftAdd produces: %d\n", x);

    x = 5; // reset x
    shiftAddOpt(&x, &x); // should produce 55
    printf("shiftAddOpt produces: %d\n", x);

    return 0;
}
```

Kết quả mong đợi là **55**.  
Tuy nhiên, khi biên dịch và chạy lại mã đã cập nhật, ta nhận được **hai kết quả khác nhau**:

```bash
$ gcc -o shiftadd shiftadd.c
$ ./shiftadd
shiftAdd produces: 100
shiftAddOpt produces: 55
```

Lần theo quá trình thực thi của các hàm `shiftAdd` với giả định rằng `a` và `b` cùng trỏ tới **cùng một vị trí bộ nhớ** sẽ thấy vấn đề:  
Phép nhân `a` với 10 trong `shiftAdd` cập nhật `x` thành 50.  
Tiếp theo, phép cộng `a` với `b` trong `shiftAdd` khiến `x` tăng gấp đôi thành 100.  

Rủi ro **memory aliasing** cho thấy `shiftAdd` và `shiftAddOpt` **thực tế không tương đương**, mặc dù lập trình viên có thể đã dự định như vậy.  

Để khắc phục, cần nhận ra rằng tham số thứ hai của `shiftAdd` **không cần** truyền vào dưới dạng con trỏ.  
Thay thế tham số thứ hai bằng một số nguyên sẽ loại bỏ rủi ro aliasing và cho phép trình biên dịch tối ưu một hàm thành hàm còn lại:

#### shiftAdd (phiên bản chưa tối ưu - đã sửa)

```c
void shiftAdd(int *a, int b){
    *a = *a * 10; // multiply by 10
    *a += b;      // add b
}
```

#### shiftAddOpt (phiên bản tối ưu - đã sửa)

```c
void shiftAddOpt(int *a, int b){
    *a = (*a * 10) + b;
}
```

**Bảng 2.** Các hàm đã cải tiến, nhân số thứ nhất với 10 và cộng số thứ hai vào. Có tại [liên kết này](_attachments/shiftadd.c).

Việc loại bỏ truy cập bộ nhớ không cần thiết giúp lập trình viên giữ nguyên khả năng đọc của hàm `shiftAdd` gốc, đồng thời cho phép trình biên dịch tối ưu hàm.

### Hợp tác với trình biên dịch: Một chương trình mẫu

Trong các phần tiếp theo, chúng ta sẽ tập trung tìm hiểu thêm về các loại tối ưu hóa phổ biến và thảo luận các chiến lược lập trình, profiling để giúp trình biên dịch dễ dàng tối ưu mã hơn.  
Để dẫn dắt nội dung, chúng ta sẽ cùng tối ưu chương trình sau (được viết chưa tối ưu) nhằm tìm tất cả các số nguyên tố từ 2 đến *n* (mã nguồn có tại [liên kết này](_attachments/optExample.c)).


```c
//helper function: checks to see if a number is prime
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

int main(int argc, char **argv) {
  //omitted for brevity
  int *array = allocateArray(limit);
  int length = genPrimeSequence(array, limit);

  return 0;
}
```


**Bảng 3** cho thấy kết quả đo thời gian để tạo ra các số nguyên tố từ 2 đến 5.000.000 với các cờ tối ưu hóa khác nhau, sử dụng lệnh biên dịch cơ bản sau:

```bash
$ gcc -o optExample optExample.c -lm
```

#### Kết quả thời gian (giây)

| Unoptimized | `-O1` | `-O2` | `-O3` |
|-------------|-------|-------|-------|
| 3.86        | 2.32  | 2.14  | 2.15  |

**Bảng 3.** Thời gian (giây) để tạo các số nguyên tố từ 2 đến 5.000.000

Thời gian nhanh nhất quan sát được khi dùng cờ tối ưu hóa là khoảng **2,14 giây**.  
Mặc dù việc sử dụng các cờ tối ưu hóa giúp giảm hơn một giây so với thời gian chạy ban đầu của chương trình, nhưng việc tăng mức tối ưu hóa chỉ mang lại cải thiện rất nhỏ.  
Trong các phần tiếp theo, chúng ta sẽ thảo luận cách có thể chỉnh sửa chương trình để giúp trình biên dịch dễ dàng tối ưu hơn.

### Tài liệu tham khảo

1. John Regehr. *"A Guide to Undefined Behavior in C and C++, Part 1"*.  
   [https://blog.regehr.org/archives/213](https://blog.regehr.org/archives/213)

2. C FAQ. *"comp.lang.c FAQ list: Question 11.33"*.  
   [http://c-faq.com/ansi/undef.html](http://c-faq.com/ansi/undef.html)
