Dưới đây là bản dịch tiếng Việt của mục **14.7. Implicit Threading with OpenMP**, tuân thủ đầy đủ các quy ước đã nêu:

---

## 14.7. Đa luồng ngầm định với OpenMP (Implicit Threading with OpenMP)

Cho đến nay, chúng ta đã trình bày lập trình bộ nhớ chia sẻ bằng **POSIX threads** (Pthreads).  
Mặc dù Pthreads rất hữu ích cho các ứng dụng đơn giản, nhưng khi chương trình trở nên phức tạp hơn, chúng lại ngày càng khó sử dụng.  
Pthreads là một ví dụ của **explicit parallel programming** (lập trình song song tường minh) với threads, yêu cầu lập trình viên phải chỉ rõ **chính xác** mỗi thread cần làm gì và khi nào thread đó bắt đầu hoặc kết thúc.

---

Với Pthreads, việc *bổ sung dần* (incrementally) khả năng song song vào một chương trình tuần tự hiện có cũng là một thách thức.  
Nói cách khác, ta thường phải viết lại toàn bộ chương trình để sử dụng threads — điều này thường không mong muốn khi cần song song hóa một codebase lớn và đã tồn tại từ trước.

---

Thư viện **Open Multiprocessing** (**OpenMP**) cung cấp một giải pháp *ngầm định* thay thế cho Pthreads.  
OpenMP được tích hợp sẵn trong GCC và các trình biên dịch phổ biến khác như LLVM và Clang, và có thể dùng với các ngôn ngữ C, C++ và Fortran.  
Ưu điểm chính của OpenMP là cho phép lập trình viên song song hóa các thành phần của mã C tuần tự hiện có bằng cách thêm **pragma** (chỉ thị đặc biệt cho trình biên dịch) vào các phần của mã.  
Các pragma dành riêng cho OpenMP bắt đầu bằng `#pragma omp`.

---

Việc trình bày chi tiết OpenMP nằm ngoài phạm vi của cuốn sách này, nhưng chúng ta sẽ đề cập đến một số pragma thường gặp và minh họa cách sử dụng chúng trong bối cảnh một số ứng dụng ví dụ.

---

### 14.7.1. Các pragma thường dùng (Common Pragmas)

Dưới đây là một số pragma được sử dụng phổ biến trong các chương trình OpenMP:

---

`#pragma omp parallel`  

:   Tạo một **team** (nhóm) các thread và cho mỗi thread chạy đoạn mã trong phạm vi của pragma (thường là một lời gọi hàm).  
    Việc gọi pragma này thường tương đương với việc gọi cặp hàm `pthread_create` và `pthread_join` [đã thảo luận trong phần Pthreads](posix.html#_creating_and_joining_threads).  
    Pragma này có thể đi kèm một số **clause** (mệnh đề), bao gồm:

    - `num_threads`: chỉ định số lượng thread cần tạo.
    - `private`: danh sách các biến sẽ là **private** (cục bộ) cho từng thread. Các biến private cũng có thể được khai báo trong phạm vi của pragma. Mỗi thread sẽ có bản sao riêng của từng biến.
    - `shared`: danh sách các biến sẽ được **shared** (chia sẻ) giữa các thread. Chỉ có **một bản sao** của biến được chia sẻ giữa tất cả các thread.
    - `default`: chỉ định việc xác định biến nào là shared sẽ do trình biên dịch quyết định hay không. Trong hầu hết các trường hợp, ta nên dùng `default(none)` và chỉ rõ biến nào là shared, biến nào là private.

---

`#pragma omp for`  

:   Chỉ định rằng mỗi thread sẽ thực thi một **tập con** các vòng lặp của một vòng `for`.  
    Mặc dù việc lập lịch (scheduling) vòng lặp phụ thuộc vào hệ thống, mặc định thường là phương pháp **chunking** (chia khối) đã đề cập trong [ví dụ nhân vô hướng](posix.html#_revisiting_scalar_multiplication).  
    Đây là dạng lập lịch **static**: mỗi thread được gán một khối cố định và xử lý các vòng lặp trong khối đó.  

    OpenMP cũng hỗ trợ lập lịch **dynamic**: mỗi thread nhận một số vòng lặp, và khi hoàn thành sẽ yêu cầu một nhóm vòng lặp mới.  
    Chính sách lập lịch có thể được đặt bằng clause:

    - `schedule(dynamic)`: chỉ định sử dụng lập lịch **dynamic**. Mặc dù hữu ích trong một số trường hợp, lập lịch static (mặc định) thường nhanh hơn.

---

`#pragma omp parallel for`  

:   Kết hợp giữa `omp parallel` và `omp for`.  
    Khác với `omp for`, pragma này **tạo team thread** trước, rồi mới gán cho mỗi thread một tập vòng lặp để thực thi.

---

`#pragma omp critical`  

:   Chỉ định rằng đoạn mã trong phạm vi của pragma là một **critical section** — chỉ một thread được thực thi đoạn mã này tại một thời điểm để đảm bảo tính đúng đắn.

---

Ngoài ra, OpenMP còn cung cấp một số **hàm** hữu ích cho thread khi thực thi, ví dụ:

- `omp_get_num_threads`: trả về số lượng thread trong team hiện tại.
- `omp_set_num_threads`: đặt số lượng thread mà một team sẽ có.
- `omp_get_thread_num`: trả về **ID** của thread đang gọi hàm.

---

> **Lưu ý:**  
> `omp parallel for` **chỉ hoạt động với vòng lặp `for`**.  
> Các loại vòng lặp khác như `while` hoặc `do`-`while` **không được hỗ trợ**.

---

Bạn có muốn tôi dịch tiếp sang phần **14.7.2. Ví dụ sử dụng OpenMP** để minh họa các pragma này trong thực tế không?

Dưới đây là bản dịch tiếng Việt của mục **14.7.2** và phần mở đầu **14.7.3**, tuân thủ đầy đủ các quy ước đã nêu:

---

### 14.7.2. Hello Threading: Phiên bản OpenMP

Hãy cùng xem lại chương trình “Hello World”  
([hellothreads.c](_attachments/hellothreads.c)), nhưng lần này sử dụng **OpenMP** thay vì **Pthreads**:


```c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void HelloWorld( void ) {
    long myid = omp_get_thread_num();
    printf( "Hello world! I am thread %ld\n", myid );
}

int main( int argc, char** argv ) {
    long nthreads;

    if (argc !=2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "where <n> is the number of threads\n");
        return 1;
    }

    nthreads = strtol( argv[1], NULL, 10 );

    #pragma omp parallel num_threads(nthreads)
        HelloWorld();

    return 0;
}
```

Lưu ý rằng chương trình OpenMP **ngắn hơn rất nhiều** so với phiên bản Pthreads.  
Để truy cập các hàm thư viện OpenMP, ta include file header `omp.h`.  
Pragma `omp parallel num_threads(nthreads)` trong hàm `main` sẽ tạo ra một nhóm thread, trong đó mỗi thread sẽ gọi hàm `HelloWorld`.  

Mệnh đề `num_threads(nthreads)` chỉ định tổng số thread sẽ được tạo.  
Pragma này cũng sẽ **join** (hợp nhất) tất cả các thread đã tạo về lại tiến trình đơn luồng.  
Nói cách khác, toàn bộ công việc cấp thấp như tạo và join thread được **trừu tượng hóa** khỏi lập trình viên và chỉ cần một pragma duy nhất để thực hiện.  
Vì lý do này, OpenMP được xem là một thư viện **implicit threading** (đa luồng ngầm định).

---

OpenMP cũng trừu tượng hóa việc quản lý **thread ID**.  
Trong ngữ cảnh của `HelloWorld`, hàm `omp_get_thread_num` sẽ lấy ra **ID duy nhất** gắn với thread đang chạy nó.

---

#### Biên dịch mã nguồn

Hãy biên dịch và chạy chương trình này bằng cách truyền cờ `-fopenmp` cho trình biên dịch, báo hiệu rằng chúng ta đang biên dịch với OpenMP:

```
$ gcc -o hello_mp hello_mp.c -fopenmp

$ ./hello_mp 4
Hello world! I am thread 2
Hello world! I am thread 3
Hello world! I am thread 0
Hello world! I am thread 1
```

Vì thứ tự thực thi của các thread có thể thay đổi ở những lần chạy sau, chạy lại chương trình này sẽ cho ra một chuỗi thông báo khác:

```
$ ./hello_mp 4
Hello world! I am thread 3
Hello world! I am thread 2
Hello world! I am thread 1
Hello world! I am thread 0
```

Hành vi này giống với [ví dụ dùng Pthreads](posix.html#_hello_threading_writing_your_first_multithreaded_program) mà chúng ta đã thấy trước đó.

---

### 14.7.3. Ví dụ phức tạp hơn: CountSort với OpenMP

Một ưu điểm mạnh mẽ của OpenMP là nó cho phép lập trình viên **song song hóa dần dần** (incrementally parallelize) mã nguồn của mình.  
Để thấy điều này trong thực tế, hãy song song hóa thuật toán **CountSort** phức tạp hơn mà chúng ta đã thảo luận trước đó trong chương này (mã tuần tự nằm tại: [countSort.c](_attachments/countSort.c)).  

Hãy nhớ rằng thuật toán này sắp xếp các mảng chứa một phạm vi giá trị nhỏ.  
Hàm `main` của chương trình tuần tự trông như sau:

```c
int main( int argc, char **argv ) {
    //parse args (omitted for brevity)

    srand(10); //use of static seed ensures the output is the same every run

    //generate random array of elements of specified length
    //(omitted for brevity)

    //allocate counts array and initializes all elements to zero.
    int counts[MAX] = {0};

    countElems(counts, array, length); //calls step 1
    writeArray(counts, array); //calls step2

    free(array); //free memory

    return 0;
}
```

Hàm `main`, sau khi phân tích các tham số dòng lệnh và tạo một mảng ngẫu nhiên, sẽ gọi hàm `countsElems` rồi đến hàm `writeArray`.

---

#### Song song hóa CountElems bằng OpenMP

Có nhiều cách để song song hóa chương trình trên.  
Một cách (được minh họa trong ví dụ sau) là sử dụng **pragma** `omp parallel` trong ngữ cảnh của các hàm `countElems` và `writeArray`.  
Kết quả là **không cần** thay đổi gì trong hàm `main`.  
Phiên bản đầy đủ của chương trình có tại: [countSort_mp.c](_attachments/countSort_mp.c).

---

Trước tiên, hãy xem cách song song hóa hàm `countElems` bằng OpenMP:


```c
void countElems(int *counts, int *array, long length) {

    #pragma omp parallel default(none) shared(counts, array, length)
    {
        int val, i, local[MAX] = {0};
        #pragma omp for
        for (i = 0; i < length; i++) {
            val = array[i];
            local[val]++;
        }

       #pragma omp critical
       {
           for (i = 0; i < MAX; i++) {
               counts[i] += local[i];
           }
       }
   }
}
```


Trong phiên bản này của mã, ba pragma được sử dụng:

- **`#pragma omp parallel`**: chỉ định rằng một **team** (nhóm) các thread sẽ được tạo.  
  Lệnh `omp_set_num_threads(nthreads)` trong `main` đặt kích thước mặc định của team thread là `nthreads`.  
  Nếu không dùng hàm `omp_set_num_threads`, số lượng thread sẽ mặc định bằng số lượng core trong hệ thống.  
  Nhắc lại: pragma `omp parallel` sẽ **ngầm định** tạo thread ở đầu khối và **join** chúng ở cuối khối.  
  Dấu ngoặc nhọn `{}` được dùng để xác định phạm vi.  
  Mệnh đề `shared` khai báo rằng các biến `counts`, `array` và `length` là **shared** (toàn cục) giữa tất cả các thread.  
  Do đó, các biến `val`, `i` và `local[MAX]` sẽ được khai báo **cục bộ** trong từng thread.

- **`#pragma omp for`**: song song hóa vòng lặp `for`, chia số vòng lặp cho các thread.  
  OpenMP sẽ tính toán cách chia vòng lặp tối ưu.  
  Như đã đề cập trước đó, chiến lược mặc định thường là phương pháp **chunking**, trong đó mỗi thread nhận xấp xỉ cùng số vòng lặp để xử lý.  
  Như vậy, mỗi thread sẽ đọc một phần của mảng `array` chia sẻ và cộng dồn kết quả vào mảng `local` cục bộ của nó.

- **`#pragma omp critical`**: chỉ định rằng đoạn mã trong phạm vi critical section chỉ được thực thi bởi **một thread** tại một thời điểm.  
  Điều này tương đương với việc dùng **mutex** trong phiên bản Pthreads của chương trình.  
  Ở đây, mỗi thread sẽ lần lượt cập nhật mảng `counts` chia sẻ.

---

Hãy đánh giá hiệu năng của hàm này khi chạy với 100 triệu phần tử:

```
$ ./countElems_mp 100000000 1
Run Time for Phase 1 is 0.249893

$ ./countElems_mp 100000000 2
Run Time for Phase 1 is 0.124462

$ ./countElems_mp 100000000 4
Run Time for Phase 1 is 0.068749
```

Kết quả rất tốt: hàm đạt **speedup** bằng 2 khi chạy với 2 thread, và **speedup** 3.63 khi chạy với 4 thread.  
Hiệu năng thậm chí còn tốt hơn cả phiên bản Pthreads!

---

#### Hàm `writeArray` trong OpenMP

Song song hóa hàm `writeArray` **khó hơn nhiều**.  
Đoạn mã sau minh họa một giải pháp khả thi:

```c
void writeArray(int *counts, int *array) {
    int i;

    // giả định số lượng thread không vượt quá MAX
    #pragma omp parallel for schedule(dynamic)
    for (i = 0; i < MAX; i++) {
        int j = 0, amt, start = 0;
        for (j = 0; j < i; j++) {  // tính toán vị trí bắt đầu "thực"
            start += counts[j];
        }

        amt = counts[i]; // số lượng phần tử cần ghi

        // ghi đè amt phần tử bằng giá trị i, bắt đầu từ vị trí start
        for (j = start; j < start + amt; j++) {
            array[j] = i;
        }
    }
}
```

---

Trước khi song song hóa, chúng tôi đã thay đổi hàm này vì [phiên bản cũ](_attachments/countSort.c) của `writeArray` khiến biến `j` phụ thuộc vào các vòng lặp trước đó.  
Trong phiên bản mới, mỗi thread sẽ tính toán giá trị `start` duy nhất của mình dựa trên tổng của tất cả các phần tử trước đó trong `counts`.

Khi loại bỏ sự phụ thuộc này, việc song song hóa trở nên khá đơn giản.  
Pragma `#pragma omp parallel for` sẽ tạo ra một nhóm thread và song song hóa vòng lặp `for` bằng cách gán cho mỗi thread một tập con các vòng lặp cần thực hiện.  
Nhắc lại, pragma này là sự kết hợp của `omp parallel` và `omp for` (đã được sử dụng trong phần song song hóa hàm `countElems`).

---

Cách lập lịch theo kiểu **chunking** (như trong hàm `countElems` trước đó) **không phù hợp** ở đây, vì có khả năng mỗi phần tử trong `counts` có tần suất rất khác nhau.  
Điều này dẫn đến việc các thread sẽ không có khối lượng công việc bằng nhau, khiến một số thread bị gán nhiều việc hơn các thread khác.  
Do đó, ta sử dụng mệnh đề `schedule(dynamic)`, để mỗi thread hoàn thành vòng lặp được gán trước khi yêu cầu một vòng lặp mới từ bộ quản lý thread.

---

Vì mỗi thread ghi vào các vị trí khác nhau trong mảng, nên **không cần** dùng mutual exclusion (loại trừ lẫn nhau) cho hàm này.

---

Hãy chú ý xem mã OpenMP gọn gàng hơn nhiều so với phiên bản dùng POSIX threads.  
Mã rất dễ đọc và chỉ cần chỉnh sửa rất ít.  
Đây là một trong những sức mạnh của **abstraction** (trừu tượng hóa), khi các chi tiết triển khai được ẩn khỏi lập trình viên.

---

Tuy nhiên, một sự đánh đổi cần thiết của abstraction là **quyền kiểm soát**.  
Lập trình viên giả định rằng trình biên dịch “đủ thông minh” để xử lý các chi tiết song song hóa, nhờ đó việc song song hóa ứng dụng trở nên dễ dàng hơn.  
Tuy nhiên, lập trình viên sẽ không còn đưa ra các quyết định chi tiết về cách song song hóa đó.  
Nếu không hiểu rõ cách các pragma OpenMP thực thi bên trong, sẽ khó để debug một ứng dụng OpenMP hoặc biết pragma nào là phù hợp nhất để dùng trong một tình huống cụ thể.

---

### 14.7.4. Tìm hiểu thêm về OpenMP

Phần thảo luận sâu hơn về OpenMP nằm ngoài phạm vi của cuốn sách này, nhưng có một số tài nguyên miễn phí hữu ích[^1][^2] để học OpenMP.

---

### Tài liệu tham khảo:

1. Blaise Barney. “OpenMP”.  
   [https://hpc.llnl.gov/tuts/openMP/](https://hpc.llnl.gov/tuts/openMP/)

2. Richard Brown và Libby Shoop. “Multicore Programming with OpenMP”.  
   *CSinParallel: Parallel Computing in the Computer Science curriculum*.  
   [http://selkie.macalester.edu/csinparallel/modules/MulticoreProgramming/build/html/index.html](http://selkie.macalester.edu/csinparallel/modules/MulticoreProgramming/build/html/index.html)



