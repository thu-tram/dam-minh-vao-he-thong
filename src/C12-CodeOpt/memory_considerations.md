Dưới đây là bản dịch tiếng Việt của mục **12.3. Memory Considerations**, tuân thủ đầy đủ các quy ước bạn đã nêu và áp dụng cách trình bày tách riêng code block như yêu cầu:

---

## 12.3. Các yếu tố cần lưu ý về bộ nhớ (Memory Considerations)

Lập trình viên cần đặc biệt chú ý đến việc sử dụng bộ nhớ, nhất là khi dùng các **data structure** (cấu trúc dữ liệu) tiêu tốn nhiều bộ nhớ như **matrix** (ma trận) và **array** (mảng).  
Mặc dù trình biên dịch cung cấp nhiều tính năng tối ưu hóa mạnh mẽ, nhưng không phải lúc nào nó cũng có thể thực hiện các tối ưu hóa giúp cải thiện việc sử dụng bộ nhớ của chương trình.  

Trong phần này, chúng ta sẽ sử dụng một chương trình cài đặt phép nhân ma trận–vector ([matrixVector.c](_attachments/matrixVector.c)) để dẫn dắt thảo luận về các kỹ thuật và công cụ cải thiện việc sử dụng bộ nhớ.

Hàm `main` của chương trình thực hiện hai bước:  
1. Cấp phát và khởi tạo ma trận đầu vào, vector đầu vào và ma trận đầu ra.  
2. Thực hiện phép nhân ma trận–vector.  

Khi chạy chương trình với kích thước ma trận–vector là 10.000 × 10.000, kết quả cho thấy hàm `matrixVectorMultiply` chiếm phần lớn thời gian thực thi:

```bash
$ gcc -o matrixVector matrixVector.c
$ ./matrixVector 10000 10000
Time to allocate and fill matrices: 1.2827
Time to allocate vector: 9.6e-05
Time to matrix-vector multiply: 1.98402
```

Vì vậy, phần thảo luận sẽ tập trung vào hàm `matrixVectorMultiply`.

---

### 12.3.1. Loop Interchange

**Loop interchange** (hoán đổi vòng lặp) là kỹ thuật tối ưu hóa thay đổi thứ tự của vòng lặp ngoài và vòng lặp trong trong các vòng lặp lồng nhau để tối đa hóa **cache locality** (tính cục bộ của cache).  
Việc thực hiện tự động kỹ thuật này là khó đối với trình biên dịch. Trong GCC, có cờ `-floop-interchange` nhưng hiện tại không được bật mặc định.  
Do đó, lập trình viên nên chú ý đến cách mã truy cập các cấu trúc dữ liệu phức hợp trong bộ nhớ như mảng và ma trận.

Ví dụ, hãy xem xét kỹ hơn hàm `matrixVectorMultiply` trong [matrixVector.c](_attachments/matrixVector.c):

#### Phiên bản gốc ([matrixVector.c](_attachments/matrixVector.c))

```c
void matrixVectorMultiply(int **m, int *v, int **res, int row, int col) {
    int i, j;
    // cycles through every matrix column in inner-most loop (inefficient)
    for (j = 0; j < col; j++) {
        for (i = 0; i < row; i++) {
            res[i][j] = m[i][j] * v[j];
        }
    }
}
```

#### Phiên bản hoán đổi vòng lặp ([matrixVector2.c](_attachments/matrixVector2.c))

```c
void matrixVectorMultiply(int **m, int *v, int **res, int row, int col) {
    int i, j;
    // cycles through every row of matrix in inner-most loop
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            res[i][j] = m[i][j] * v[j];
        }
    }
}
```

**Bảng 1.** Hoán đổi vòng lặp trong hàm `matrixVectorMultiply()`.

---

Các ma trận đầu vào và đầu ra được cấp phát động (theo phương pháp thứ hai đã thảo luận trong chương C).  
Kết quả là các hàng trong ma trận **không nằm liên tiếp nhau** trong bộ nhớ, trong khi các phần tử trong mỗi hàng thì **liên tiếp**.  
Thứ tự vòng lặp hiện tại khiến chương trình duyệt qua từng **cột** thay vì từng **hàng**.  

Hãy nhớ rằng [dữ liệu được nạp vào cache theo *block*](../C11-MemHierarchy/caching.html#_direct_mapped_caches) chứ không phải từng phần tử riêng lẻ.  
Do đó, khi một phần tử *x* trong mảng `res` hoặc `m` được truy cập, **các phần tử liền kề với x** cũng được nạp vào cache.  
Việc duyệt qua từng “cột” của ma trận gây ra nhiều **cache miss** hơn, vì cache buộc phải nạp block mới ở mỗi lần truy cập.

**Bảng 2** cho thấy việc thêm cờ tối ưu hóa **không** làm giảm thời gian chạy của hàm.  
Tuy nhiên, chỉ cần đổi thứ tự vòng lặp (như trong ví dụ mã ở trên và trong [matrixVector2.c](_attachments/matrixVector2.c)) đã giúp hàm chạy nhanh hơn gần **8 lần** và cho phép trình biên dịch thực hiện thêm các tối ưu hóa khác.

#### Thời gian thực thi (giây) cho phép nhân ma trận 10.000 × 10.000

| Phiên bản | Chương trình       | Unoptimized | `-O1` | `-O2` | `-O3` |
|-----------|--------------------|-------------|-------|-------|-------|
| Original  | `matrixVector`     | 2.01        | 2.05  | 2.07  | 2.08  |
| With Loop Interchange | `matrixVector2` | 0.27        | 0.08  | 0.06  | 0.06  |

**Bảng 2.** Thời gian (giây) thực hiện phép nhân ma trận 10.000 × 10.000 phần tử.

---

Công cụ `cachegrind` của Valgrind (được thảo luận trong [Chương 11](../C11-MemHierarchy/cachegrind.html#_cache_analysis_and_valgrind)) là một cách tuyệt vời để xác định các vấn đề về **data locality** (tính cục bộ dữ liệu), và cho thấy sự khác biệt trong truy cập cache giữa hai phiên bản của hàm `matrixVectorMultiply` ở ví dụ trên.

---
Dưới đây là bản dịch tiếng Việt của mục **12.3.2. Some Other Compiler Optimizations for Improving Locality: Fission and Fusion**, tuân thủ đầy đủ các quy ước bạn đã nêu và áp dụng cách trình bày tách riêng code block như yêu cầu:

---

### 12.3.2. Một số tối ưu hóa khác của trình biên dịch để cải thiện locality: Fission và Fusion

Chạy lại chương trình đã cải tiến với dữ liệu 10.000 × 10.000 phần tử cho kết quả thời gian như sau:

```bash
$ gcc -o matrixVector2 matrixVector2.c
$ ./matrixVector2 10000 10000
Time to allocate and fill matrices: 1.29203
Time to allocate vector: 0.000107
Time to matrix-vector multiply: 0.271369
```

Bây giờ, việc cấp phát và điền dữ liệu cho ma trận chiếm nhiều thời gian nhất.  
Đo đạc chi tiết hơn cho thấy phần **điền dữ liệu** cho ma trận mới là nguyên nhân chính.  
Hãy xem kỹ đoạn mã này:

```c
// fill matrices
for (i = 0; i < rows; i++){
    fillArrayRandom(matrix[i], cols);
    fillArrayZeros(result[i], cols);
}
```

Để điền dữ liệu cho ma trận đầu vào và đầu ra, vòng lặp `for` duyệt qua tất cả các hàng và gọi hàm `fillArrayRandom` và `fillArrayZeros` cho từng hàng.  
Trong một số trường hợp, sẽ có lợi nếu trình biên dịch tách vòng lặp này thành hai vòng lặp riêng biệt — kỹ thuật này gọi là **loop fission**.

#### Phiên bản gốc

```c
for (i = 0; i < rows; i++) {
    fillArrayRandom(matrix[i], cols);
    fillArrayZeros(result[i], cols);
}
```

#### Phiên bản với loop fission

```c
for (i = 0; i < rows; i++) {
    fillArrayRandom(matrix[i], cols);
}

for (i = 0; i < rows; i++) {
    fillArrayZeros(result[i], cols);
}
```

**Bảng 3.** Ví dụ loop fission trên vòng lặp điền dữ liệu ma trận trong `main`.

---

Quá trình kết hợp hai vòng lặp chạy trên cùng một phạm vi thành một vòng lặp duy nhất (ngược lại với loop fission) được gọi là **loop fusion**.  
Loop fission và loop fusion là những ví dụ tối ưu hóa mà trình biên dịch có thể thực hiện để cải thiện **data locality**.  

Các trình biên dịch cho bộ xử lý đa nhân cũng có thể dùng loop fission hoặc fusion để cho phép vòng lặp chạy hiệu quả hơn trên nhiều nhân.  
Ví dụ: trình biên dịch có thể dùng loop fission để gán hai vòng lặp cho hai nhân khác nhau.  
Ngược lại, nó có thể dùng loop fusion để gộp các thao tác phụ thuộc vào nhau vào cùng một vòng lặp, rồi phân phối cho mỗi nhân một tập con các lần lặp (giả sử dữ liệu giữa các lần lặp là độc lập).

Trong trường hợp của chúng ta, áp dụng loop fission thủ công **không** cải thiện trực tiếp hiệu năng; thời gian điền dữ liệu mảng hầu như không thay đổi.  
Tuy nhiên, nó giúp lộ ra một tối ưu hóa tinh tế hơn: vòng lặp chứa `fillArrayZeros` là **không cần thiết**.  
Hàm `matrixVectorMultiply` sẽ gán giá trị cho từng phần tử trong mảng `result`, nên việc khởi tạo trước tất cả bằng 0 là dư thừa.

#### Phiên bản trước ([matrixVector2.c](_attachments/matrixVector2.c))

```c
for (i = 0; i < rows; i++) {
    matrix[i] = allocateArray(cols);
    result[i] = allocateArray(cols);
}

for (i = 0; i < rows; i++) {
    fillArrayRandom(matrix[i], cols);
    fillArrayZeros(result[i], cols);
}
```

#### Phiên bản cập nhật với `calloc()` ([matrixVector3.c](_attachments/matrixVector3.c))

```c
for (i = 0; i < rows; i++) {
    matrix[i] = allocateArray(cols);
    result[i] = allocateArray(cols);
}

for (i = 0; i < rows; i++) {
    fillArrayRandom(matrix[i], cols);
    // fillArrayZeros(result[i], cols); // no longer needed
}
```

**Bảng 4.** Loại bỏ lời gọi `fillArrayZeros` không cần thiết.


Dưới đây là bản dịch tiếng Việt của mục **12.3.3. Memory Profiling with Massif**, tuân thủ đầy đủ các quy ước bạn đã nêu:

---

### 12.3.3. Phân tích bộ nhớ với Massif

Việc thay đổi ở bước trước chỉ giúp giảm nhẹ thời gian chạy.  
Mặc dù đã loại bỏ bước điền toàn bộ phần tử trong ma trận kết quả bằng số 0, nhưng vẫn cần một lượng thời gian đáng kể để điền ma trận đầu vào bằng các số ngẫu nhiên:

```bash
$ gcc -o matrixVector3 matrixVector3.c
$ ./matrixVector3 10000 10000
Time to allocate matrices: 0.049073
Time to fill matrices: 0.946801
Time to allocate vector: 9.3e-05
Time to matrix-vector multiply: 0.359525
```

Mặc dù mỗi mảng được lưu **không liên tiếp** trong bộ nhớ, nhưng mỗi mảng chiếm `10,000 × sizeof(int)` byte, tức 40.000 byte.  
Vì có tổng cộng 20.000 mảng (10.000 cho ma trận đầu vào và 10.000 cho ma trận kết quả) được cấp phát, điều này tương ứng với **800 triệu byte**, hay khoảng **762 MB** dung lượng.  
Việc điền 762 MB dữ liệu bằng số ngẫu nhiên rõ ràng sẽ tốn nhiều thời gian.  
Với ma trận, mức sử dụng bộ nhớ tăng **theo hàm bậc hai** với kích thước đầu vào, và có thể ảnh hưởng lớn đến hiệu năng.

Công cụ `massif` của Valgrind có thể giúp bạn **profile** (phân tích) việc sử dụng bộ nhớ.  
Giống như các công cụ Valgrind khác đã đề cập trong sách này ([memcheck](../C3-C_debug/valgrind.html#_debugging_memory_with_valgrind), [cachegrind](../C11-MemHierarchy/cachegrind.html#_cache_analysis_and_valgrind), và callgrind), `massif` chạy như một lớp bao quanh file thực thi của chương trình.  
Cụ thể, `massif` sẽ chụp lại các **snapshot** (ảnh chụp) về việc sử dụng bộ nhớ của chương trình trong suốt quá trình chạy, và phân tích cách mức sử dụng bộ nhớ thay đổi.  
Lập trình viên có thể thấy `massif` hữu ích để theo dõi cách chương trình sử dụng **heap memory** (bộ nhớ heap), và xác định cơ hội cải thiện việc sử dụng bộ nhớ.

Hãy chạy `massif` trên file thực thi `matrixVector3`:

```bash
$ valgrind --tool=massif ./matrixVector3 10000 10000
==7030== Massif, a heap profiler
==7030== Copyright (C) 2003-2015, and GNU GPL'd, by Nicholas Nethercote
==7030== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==7030== Command: ./matrixVector3 10000 10000
==7030==
Time to allocate matrices: 0.049511
Time to fill matrices: 4.31627
Time to allocate vector: 0.001015
Time to matrix-vector multiply: 0.62672
==7030==
```

Khi chạy `massif`, nó sẽ tạo ra một file `massif.out.xxxx`, trong đó `xxxx` là một ID duy nhất.  
Nếu bạn đang thực hành theo, hãy gõ `ls` để xem file massif tương ứng.  
Trong ví dụ này, file tương ứng là `massif.out.7030`.  
Dùng lệnh `ms_print` để xem kết quả của `massif`:

```bash
$ ms_print massif.out.7030
-----------------------------------------------------------------------------
Command:            ./matrixVector3 10000 10000
Massif arguments:   (none)
ms_print arguments: massif.out.7030
-----------------------------------------------------------------------------

    MB
763.3^                                                ::::::::::::::::::::::#
     |:::::::::::::::::::::::::::::::::::::::::::::::::                     #
     |:                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
     |@                                               :                     #
   0 +-------------------------------------------------------------------->Gi
     0                                                                  9.778

Number of snapshots: 80
Detailed snapshots: [3, 12, 17, 22, 49, 59, 69, 79 (peak)]
```


Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu:

---

Ở đầu phần kết quả là **biểu đồ sử dụng bộ nhớ**.  
Trục *x* biểu diễn số lượng lệnh (**instructions**) đã thực thi.  
Trục *y* biểu diễn mức sử dụng bộ nhớ.  

Biểu đồ trên cho thấy tổng cộng **9,778 tỷ** (Gi) lệnh đã được thực thi trong lần chạy `matrixVector3`.  
Trong quá trình thực thi, `massif` đã chụp tổng cộng **80 snapshot** để đo mức sử dụng **heap**.  
Mức sử dụng bộ nhớ đạt đỉnh ở snapshot cuối cùng (79).  
Mức sử dụng bộ nhớ cực đại của chương trình là **763,3 MB**, và duy trì tương đối ổn định trong suốt quá trình chạy.

---

Phần tóm tắt của tất cả các snapshot xuất hiện ngay sau biểu đồ.  
Ví dụ, bảng dưới đây tương ứng với các snapshot xung quanh snapshot 79:

```
....
------------------------------------------------------------------------------
  n        time(i)         total(B)   useful-heap(B) extra-heap(B)   stacks(B)
------------------------------------------------------------------------------
 70      1,081,926      727,225,400      727,080,000       145,400          0
 71      1,095,494      737,467,448      737,320,000       147,448          0
 72      1,109,062      747,709,496      747,560,000       149,496          0
 73      1,122,630      757,951,544      757,800,000       151,544          0
 74      1,136,198      768,193,592      768,040,000       153,592          0
 75      1,149,766      778,435,640      778,280,000       155,640          0
 76      1,163,334      788,677,688      788,520,000       157,688          0
 77      1,176,902      798,919,736      798,760,000       159,736          0
 78  7,198,260,935      800,361,056      800,201,024       160,032          0
 79 10,499,078,349      800,361,056      800,201,024       160,032          0
99.98% (800,201,024B) (heap allocations) malloc/new/new[], --alloc-fns, etc.
->99.96% (800,040,000B) 0x40089D: allocateArray (in matrixVector3)
```

---

Mỗi hàng trong bảng tương ứng với một snapshot cụ thể, bao gồm:

- **Thời điểm** snapshot được chụp (`time(i)`).
- **Tổng mức tiêu thụ bộ nhớ heap** tại thời điểm đó (`total(B)`).
- **Số byte chương trình yêu cầu** tại thời điểm đó (`useful-heap`).
- **Số byte được cấp phát vượt quá** so với yêu cầu của chương trình (`extra-heap`).
- **Kích thước stack** (`stacks(B)`).

Theo mặc định, việc **profiling stack** bị tắt (vì nó làm `massif` chạy chậm đáng kể).  
Để bật profiling stack, hãy dùng tùy chọn `--stacks=yes` khi chạy `massif`.

---

Công cụ `massif` cho thấy **99,96%** mức sử dụng bộ nhớ heap của chương trình xảy ra trong hàm `allocateArray`, và tổng cộng **800 triệu byte** đã được cấp phát — phù hợp với phép tính ước lượng nhanh mà chúng ta đã thực hiện trước đó.  

Người đọc có thể thấy `massif` là một công cụ hữu ích để xác định các khu vực sử dụng nhiều bộ nhớ heap trong chương trình — điều thường làm chậm chương trình.  
Ví dụ, **memory leak** (rò rỉ bộ nhớ) có thể xảy ra khi lập trình viên thường xuyên gọi `malloc` nhưng không gọi `free` tại thời điểm thích hợp đầu tiên.  
Công cụ `massif` đặc biệt hữu ích để phát hiện các rò rỉ như vậy.
