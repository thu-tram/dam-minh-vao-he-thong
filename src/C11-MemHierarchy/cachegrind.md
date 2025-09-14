## 11.5. Phân tích Cache và Valgrind

Vì cache ảnh hưởng đáng kể đến hiệu năng của chương trình, hầu hết các hệ thống đều cung cấp các công cụ profiling để đo lường mức độ sử dụng cache của chương trình.  
Một công cụ như vậy là chế độ `cachegrind` của Valgrind, và phần này sẽ sử dụng nó để đánh giá hiệu năng cache.

---

Xét chương trình sau, chương trình này tạo ra một ma trận ngẫu nhiên kích thước *N*×*N*:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int **genRandomMatrix(int n, int max) {
    int i, j;
    int **mat = malloc(n * sizeof(int *));

    for (i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));

        for (j = 0; j < n; j++) {
            mat[i][j] = 1 + rand() % max;
        }
    }

    return mat;
}

void free_all(int **mat, int n) {
    int i;

    for (i = 0; i < n; i++) {
        free(mat[i]);
    }

    free(mat);
}

int main(int argc, char **argv) {
    int i, n;
    int **matrix;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "where <n> is the dimension of the matrix\n");
        return 1;
    }

    n = strtol(argv[1], NULL, 10);
    srand(time(NULL));

    matrix = genRandomMatrix(n, 100);

    free_all(matrix, n);
    return 0;
}
```

---

Ở các phần trước của chương này, chúng ta đã giới thiệu hai hàm để tính giá trị trung bình của tất cả các phần tử trong một ma trận.  
Chúng chỉ khác nhau ở cách truy cập chỉ số phần tử trong ma trận:

```c
float averageMat_v1(int **mat, int n) {
    int i, j, total = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Lưu ý cách truy cập: [i][j]
            total += mat[i][j];
        }
    }

    return (float) total / (n * n);
}
```

```c
float averageMat_v2(int **mat, int n) {
    int i, j, total = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Lưu ý cách truy cập: [j][i]
            total += mat[j][i];
        }
    }

    return (float) total / (n * n);
}
```

Phần này sẽ sử dụng công cụ **cache profiling** để định lượng sự khác biệt giữa hai cách tiếp cận này.

---

### 11.5.1. Phân tích lý thuyết và Benchmark ban đầu

Một phân tích lý thuyết dựa trên tính cục bộ (locality) và hệ thống phân cấp bộ nhớ cho thấy phiên bản đầu tiên có **spatial locality** (tính cục bộ không gian) tốt hơn trên ma trận `mat`, vì `mat` được lưu trữ trong bộ nhớ theo [row-major order](../C2-C_depth/arrays.html#_two_dimensional_array_memory_layout) (theo hàng).  

Phiên bản thứ hai có spatial locality kém hơn vì mỗi phần tử trong ma trận được truy cập theo **column-major order** (theo cột).  
Hãy nhớ rằng dữ liệu được nạp vào cache theo *block*.  
Việc duyệt ma trận theo column-major order có khả năng dẫn đến nhiều **cache miss** hơn, từ đó làm giảm hiệu năng.

---

Chúng ta sẽ chỉnh sửa hàm `main` để thêm lời gọi hàm `gettimeofday` nhằm đo lường chính xác sự khác biệt về hiệu năng giữa hai phiên bản.

```c
int main(int argc, char** argv) {
   /* Validate command line parameters. */
   if (argc != 2) {
       fprintf(stderr, "usage: %s <n>\n", argv[0]);
       fprintf(stderr, "where <n> is the dimension of the matrix\n");
       return 1;
   }

   /* Declare and initialize variables. */
   int i;
   float res;
   double timer;
   int n = strtol(argv[1], NULL, 10);
   srand(time(NULL));
   struct timeval tstart, tend;
   int ** matrix = genRandomMatrix(n, 100);

   /* Time version 1. */
   gettimeofday(&tstart, NULL);
   res = averageMat_v1(matrix, n);
   gettimeofday(&tend, NULL);
   timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
   printf("v1 average is: %.2f; time is %g\n", res, timer);

   /* Time version 2. */
   gettimeofday(&tstart, NULL);
   res = averageMat_v2(matrix, n);
   gettimeofday(&tend, NULL);
   timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
   printf("v2 average is: %.2f; time is %g\n", res, timer);

   /* Clean up. */
   free_all(matrix, n);
   return 0;
}
```


Biên dịch chương trình và chạy sẽ cho kết quả sau (lưu ý: thời gian sẽ thay đổi tùy vào máy chạy):

```
$ gcc -o cachex cachex.c
$ ./cachex 5000
v1 average is: 50.49; time is 0.053641
v2 average is: 50.49; time is 0.247644
```

Đó là một sự khác biệt lớn! Thực tế, cách duyệt **row-major order** nhanh hơn cách thứ hai tới **4,61 lần**.

---

### 11.5.2. Phân tích cache trong thực tế: Cachegrind

Phân tích lý thuyết hai giải pháp và chạy thử xác nhận rằng phiên bản đầu tiên nhanh hơn phiên bản thứ hai.  
Tuy nhiên, điều này chưa xác nhận chi tiết phân tích cache.  
May mắn là bộ công cụ Valgrind có thể giúp.  
Ở phần trước, chúng ta đã nói về cách Valgrind giúp [tìm memory leak](../C3-C_debug/valgrind.html#_debugging_memory_with_valgrind) trong chương trình.  
Phần này sẽ giới thiệu **Cachegrind**, trình mô phỏng cache của Valgrind.  
Cachegrind cho phép lập trình viên nghiên cứu cách một chương trình hoặc một hàm cụ thể tác động đến cache.

---

Cachegrind mô phỏng cách chương trình tương tác với hệ thống phân cấp cache của máy tính.  
Trong nhiều trường hợp, Cachegrind có thể tự động phát hiện cấu trúc cache của máy.  
Nếu không, Cachegrind vẫn mô phỏng **L1 cache** (cấp 1) và **LL cache** (last level – cấp cuối).  
Nó giả định L1 cache có hai thành phần độc lập: **instruction cache** và **data cache**.  
Lý do là LL cache có ảnh hưởng quan trọng nhất đến thời gian chạy, còn L1 cache có mức độ kết hợp (associativity) thấp nhất, nên cần đảm bảo chương trình tương tác tốt với nó.  
Những giả định này phù hợp với cấu trúc của hầu hết các máy hiện đại.

---

Cachegrind thu thập và xuất ra các thông tin sau:

- Instruction cache reads (`Ir`)
- L1 instruction cache read misses (`I1mr`) và LL cache instruction read misses (`ILmr`)
- Data cache reads (`Dr`)
- D1 cache read misses (`D1mr`) và LL cache data misses (`DLmr`)
- Data cache writes (`Dw`)
- D1 cache write misses (`D1mw`) và LL cache data write misses (`DLmw`)

Lưu ý: Tổng số truy cập D1 được tính bằng `D1 = D1mr + D1mw` và tổng số truy cập LL được tính bằng `ILmr + DLmr + DLmw`.

---

Hãy xem phiên bản 1 của chương trình hoạt động thế nào dưới Cachegrind.  
Chạy Valgrind trên mã đã biên dịch với lệnh:

```
$ valgrind --tool=cachegrind --cache-sim=yes ./cachex 1000
```

Trong lệnh này, công cụ `cachegrind` của Valgrind đóng vai trò như một lớp bao quanh file thực thi `cachex`.  
Chọn kích thước ma trận nhỏ hơn giúp Cachegrind chạy nhanh hơn.  
Cachegrind sẽ xuất thông tin về số lượng cache hit và miss trong toàn bộ chương trình:

```
==28657== Cachegrind, a cache and branch-prediction profiler
==28657== Copyright (C) 2002-2017, and GNU GPL'd by Nicholas Nethercote et al.
==28657== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==28657== Command: ./cachex 1000
==28657==
--28657-- warning: L3 cache found, using its data for the LL simulation.
average is: 50.49; time is 0.080304
average is: 50.49; time is 0.09733
==28657==
==28657== I   refs:      122,626,329
==28657== I1  misses:          1,070
==28657== LLi misses:          1,053
==28657== I1  miss rate:        0.00%
==28657== LLi miss rate:        0.00%
==28657==
==28657== D   refs:       75,292,076  (56,205,598 rd   + 19,086,478 wr)
==28657== D1  misses:      1,192,118  ( 1,129,099 rd   +     63,019 wr)
==28657== LLd misses:         64,399  (     1,543 rd   +     62,856 wr)
==28657== D1  miss rate:         1.6% (       2.0%     +        0.3%  )
==28657== LLd miss rate:         0.1% (       0.0%     +        0.3%  )
==28657==
==28657== LL refs:         1,193,188  ( 1,130,169 rd   +     63,019 wr)
==28657== LL misses:          65,452  (     2,596 rd   +     62,856 wr)
==28657== LL miss rate:          0.0% (       0.0%     +        0.3%  )
```

---

Tuy nhiên, ở đây chúng ta quan tâm **cụ thể** đến số hit và miss của hai phiên bản hàm tính trung bình.  
Để xem thông tin này, dùng công cụ `cg_annotate` của Cachegrind.  
Khi chạy Cachegrind, nó sẽ tạo ra một file trong thư mục hiện tại, có dạng `cachegrind.out.n` (trong đó `n` là PID).  
Chạy `cg_annotate` với lệnh (thay `cachegrind.out.28657` bằng tên file thực tế):

```
$ cg_annotate cachegrind.out.28657

I1 cache:         32768 B, 64 B, 8-way associative
D1 cache:         32768 B, 64 B, 8-way associative
LL cache:         8388608 B, 64 B, 16-way associative
Command:          ./cachex 1000
Data file:        cachegrind.out.28657
Events recorded:  Ir I1mr ILmr Dr D1mr DLmr Dw D1mw DLmw
Events shown:     Ir I1mr ILmr Dr D1mr DLmr Dw D1mw DLmw
Event sort order: Ir I1mr ILmr Dr D1mr DLmr Dw D1mw DLmw
Thresholds:       0.1 100 100 100 100 100 100 100 100
Include dirs:
User annotated:
Auto-annotation:  off

 ----------------------------------------------------------------------------
         Ir  I1mr  ILmr         Dr      D1mr  DLmr         Dw   D1mw   DLmw
 ----------------------------------------------------------------------------
122,626,329 1,070 1,053 56,205,598 1,129,099 1,543 19,086,478 63,019 62,856  PROGRAM TOTALS

 ----------------------------------------------------------------------------
        Ir I1mr ILmr         Dr      D1mr DLmr        Dw   D1mw   DLmw  file:function
 ----------------------------------------------------------------------------
14,009,017    3    3  9,005,008    62,688    0     1,004      0      0  averageMat_v1
14,009,017    0    0  9,005,008 1,062,996    0     1,004      0      0  averageMat_v2
```

---

Chúng tôi đã chỉnh sửa đầu ra để tập trung vào hai phiên bản hàm tính trung bình.  
Kết quả cho thấy **phiên bản 2** có **1.062.996** data miss, so với chỉ **62.688** miss ở **phiên bản 1**.  
Cachegrind đã cung cấp bằng chứng rõ ràng rằng phân tích của chúng ta là chính xác.