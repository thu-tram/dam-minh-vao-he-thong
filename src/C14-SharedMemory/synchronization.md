## 14.3. Đồng bộ hóa các Thread (Synchronizing Threads)

Trong các ví dụ mà chúng ta đã xem cho đến nay, mỗi **thread** thực thi mà **không chia sẻ dữ liệu** với bất kỳ thread nào khác.  
Ví dụ, trong chương trình **nhân vô hướng** (scalar multiplication), mỗi phần tử của mảng hoàn toàn độc lập với các phần tử khác, nên không cần thiết để các thread chia sẻ dữ liệu.

---

Tuy nhiên, khả năng **dễ dàng chia sẻ dữ liệu** với các thread khác lại là một trong những đặc điểm chính của thread.  
Hãy nhớ rằng tất cả các thread của một **process đa luồng** (multithreaded process) đều **chia sẻ vùng heap** chung của process đó.  
Trong phần này, chúng ta sẽ nghiên cứu chi tiết các cơ chế chia sẻ và bảo vệ dữ liệu mà thread có thể sử dụng.

---

**Thread synchronization** (đồng bộ hóa thread) đề cập đến việc **ép** các thread thực thi theo một **thứ tự nhất định**.  
Mặc dù việc đồng bộ hóa thread có thể làm tăng thời gian chạy của chương trình, nhưng nó thường **cần thiết** để đảm bảo **tính đúng đắn** của chương trình.  

Trong phần này, chúng ta sẽ tập trung thảo luận cách một cấu trúc đồng bộ hóa — **mutex** — giúp đảm bảo tính đúng đắn của chương trình đa luồng.  
Chúng ta sẽ kết thúc phần này bằng việc thảo luận một số cấu trúc đồng bộ hóa phổ biến khác: **semaphore**, **barrier** và **condition variable**.

---

### CountSort

Hãy nghiên cứu một ví dụ phức tạp hơn một chút có tên **CountSort**.  
Thuật toán CountSort là một thuật toán sắp xếp tuyến tính đơn giản **O(*N*)** dùng để sắp xếp một tập giá trị nhỏ đã biết có kích thước *R*, trong đó *R* nhỏ hơn rất nhiều so với *N*.  

Để minh họa cách CountSort hoạt động, hãy xét một mảng `A` gồm 15 phần tử, mỗi phần tử chứa giá trị ngẫu nhiên từ 0 đến 9 (tức có 10 giá trị khả dĩ):

```
A = [9, 0, 2, 7, 9, 0, 1, 4, 2, 2, 4, 5, 0, 9, 1]
```

---

Với một mảng cụ thể, CountSort hoạt động như sau:

1. **Đếm tần suất** xuất hiện của mỗi giá trị trong mảng.
2. **Ghi đè** mảng ban đầu bằng cách liệt kê mỗi giá trị theo đúng tần suất của nó.

---

Sau bước 1, tần suất của mỗi giá trị được lưu trong mảng `counts` có độ dài 10, trong đó `counts[i]` là số lần giá trị *i* xuất hiện trong mảng `A`.  
Ví dụ: vì có 3 phần tử có giá trị 2 trong mảng `A`, nên `counts[2]` = 3.

Mảng `counts` tương ứng với ví dụ trên như sau:

```
counts = [3, 2, 3, 0, 2, 1, 0, 1, 0, 3]
```

Lưu ý rằng tổng tất cả các phần tử trong mảng `counts` bằng đúng độ dài của `A`, tức **15**.

---

Bước 2 sử dụng mảng `counts` để ghi đè `A`, dựa vào tần suất để xác định tập chỉ số trong `A` lưu trữ mỗi giá trị liên tiếp theo thứ tự đã sắp xếp.  
Ví dụ: vì mảng `counts` cho biết có 3 phần tử giá trị 0 và 2 phần tử giá trị 1 trong `A`, nên 3 phần tử đầu tiên của mảng kết quả sẽ là 0, và 2 phần tử tiếp theo sẽ là 1.

---

Sau khi chạy bước 2, mảng cuối cùng sẽ như sau:

```
A = [0, 0, 0, 1, 1, 2, 2, 2, 4, 4, 5, 7, 9, 9, 9]
```

---

Dưới đây là phần cài đặt **tuần tự** của thuật toán CountSort, với hai hàm `count` (bước 1) và `overwrite` (bước 2) được phân tách rõ ràng.  
Để ngắn gọn, chúng tôi không đưa toàn bộ chương trình ở đây, nhưng bạn có thể tải mã nguồn tại: [countSort.c](_attachments/countSort.c).


```c
#define MAX 10 //the maximum value of an element. (10 means 0-9)

/*step 1:
 * compute the frequency of all the elements in the input array and store
 * the associated counts of each element in array counts. The elements in the
 * counts array are initialized to zero prior to the call to this function.
*/
void countElems(int *counts, int *array_A, long length) {
    int val, i;
    for (i = 0; i < length; i++) {
      val = array_A[i]; //read the value at index i
      counts[val] = counts[val] + 1; //update corresponding location in counts
    }
}

/* step 2:
 * overwrite the input array (array_A) using the frequencies stored in the
 *  array counts
*/
void writeArray(int *counts, int *array_A) {
    int i, j = 0, amt;

    for (i = 0; i < MAX; i++) { //iterate over the counts array
        amt = counts[i]; //capture frequency of element i
        while (amt > 0) { //while all values aren't written
            array_A[j] = i; //replace value at index j of array_A with i
            j++; //go to next position in array_A
            amt--; //decrease the amount written by 1
        }
    }
}

/* main function:
 * gets array length from command line args, allocates a random array of that
 * size, allocates the counts array, the executes step 1 of the CountSort
 * algorithm (countsElem) followed by step 2 (writeArray).
*/
int main( int argc, char **argv ) {
    //code ommitted for brevity -- download source to view full file

    srand(10); //use of static seed ensures the output is the same every run

    long length = strtol( argv[1], NULL, 10 );
    int verbose = atoi(argv[2]);

    //generate random array of elements of specified length
    int *array = malloc(length * sizeof(int));
    genRandomArray(array, length);

    //print unsorted array (commented out)
    //printArray(array, length);

    //allocate counts array and initializes all elements to zero.
    int counts[MAX] = {0};

    countElems(counts, array, length); //calls step 1
    writeArray(counts, array); //calls step2

    //print sorted array (commented out)
    //printArray(array, length);

    free(array); //free memory

    return 0;
}
```

Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu:

---

Chạy chương trình này trên một mảng có kích thước 15 cho ra kết quả sau:

```bash
$ ./countSort 15 1
array before sort:
5 8 8 5 8 7 5 1 7 7 3 3 8 3 4
result after sort:
1 3 3 3 4 5 5 5 7 7 7 8 8 8 8
```


Tham số thứ hai của chương trình là cờ *verbose*, cho biết chương trình có in kết quả ra hay không.  
Tùy chọn này hữu ích với các mảng lớn, khi ta muốn chạy chương trình nhưng **không nhất thiết** phải in toàn bộ kết quả.

---

### Song song hóa `countElems`: Thử nghiệm ban đầu

Thuật toán **CountSort** gồm hai bước chính, và cả hai đều có thể hưởng lợi từ việc song song hóa.  
Trong phần còn lại của chương này, chúng ta sẽ tập trung vào song song hóa **bước 1**, tức hàm `countElems`.  
Việc song song hóa hàm `writeArray` được để lại như một bài tập cho người đọc.

---

Đoạn mã dưới đây minh họa **lần thử đầu tiên** tạo phiên bản đa luồng của hàm `countElems`.  
Một số phần của mã (phân tích tham số, xử lý lỗi) được lược bỏ để ngắn gọn, nhưng bạn có thể tải toàn bộ mã nguồn tại: [countElems_p.c](_attachments/countElems_p.c).

Trong đoạn mã này, mỗi thread sẽ đếm tần suất xuất hiện của các phần tử trong **phần mảng** được gán cho nó từ mảng toàn cục, và cập nhật vào mảng đếm toàn cục `counts`:



```c
/*parallel version of step 1 (first cut) of CountSort algorithm:
 * extracts arguments from args value
 * calculates the portion of the array that thread is responsible for counting
 * computes the frequency of all the elements in assigned component and stores
 * the associated counts of each element in counts array
*/
void *countElems( void *args ) {
    struct t_arg * myargs = (struct t_arg *)args;
    //extract arguments (omitted for brevity)
    int *array = myargs->ap;
    long *counts = myargs->countp;
    //... (get nthreads, length, myid)

    //assign work to the thread
    long chunk = length / nthreads; //nominal chunk size
    long start = myid * chunk;
    long end = (myid + 1) * chunk;
    long val;
    if (myid == nthreads-1) {
        end = length;
    }

    long i;
    //heart of the program
    for (i = start; i < end; i++) {
        val = array[i];
        counts[val] = counts[val] + 1;
    }

    return NULL;
}
```

Hàm `main` gần như giống hệt các chương trình mẫu trước đây:

```c
int main(int argc, char **argv) {

    if (argc != 4) {
        //print out usage info (ommitted for brevity)
        return 1;
    }

    srand(10); //static seed to assist in correctness check

    //parse command line arguments
    long t;
    long length = strtol(argv[1], NULL, 10);
    int verbose = atoi(argv[2]);
    long nthreads = strtol(argv[3], NULL, 10);

    //generate random array of elements of specified length
    int *array = malloc(length * sizeof(int));
    genRandomArray(array, length);

    //specify counts array and initialize all elements to zero
    long counts[MAX] = {0};

    //allocate threads and args array
    pthread_t *thread_array; //pointer to future thread array
    thread_array = malloc(nthreads * sizeof(pthread_t)); //allocate the array
    struct t_arg *thread_args = malloc( nthreads * sizeof(struct t_arg) );

    //fill thread array with parameters
    for (t = 0; t < nthreads; t++) {
        //ommitted for brevity...
    }

    for (t = 0; t < nthreads; t++) {
        pthread_create(&thread_array[t], NULL, countElems, &thread_args[t]);
    }

    for (t = 0; t < nthreads; t++) {
        pthread_join(thread_array[t], NULL);
    }

    free(thread_array);
    free(array);

    if (verbose) {
        printf("Counts array:\n");
        printCounts(counts);
    }
    return 0;
}
```
---

Để đảm bảo khả năng tái lập kết quả, bộ sinh số ngẫu nhiên được **seed** với giá trị cố định (10), nhằm đảm bảo mảng `array` (và do đó `counts`) luôn chứa cùng một tập giá trị ở mỗi lần chạy.  
Một hàm bổ sung (`printCounts`) sẽ in nội dung của mảng `counts` toàn cục.  
Kỳ vọng là, **bất kể** số lượng thread sử dụng, nội dung mảng `counts` phải luôn giống nhau.  
Để ngắn gọn, phần xử lý lỗi đã được lược bỏ.

---

Biên dịch chương trình và chạy với 1, 2 và 4 thread trên mảng 10 triệu phần tử cho kết quả:

```bash
$ gcc -o countElems_p countElems_p.c -pthread

$./countElems_p 10000000 1 1
Counts array:
999170 1001044 999908 1000431 999998 1001479 999709 997250 1000804 1000207

$./countElems_p 10000000 1 2
Counts array:
661756 661977 657828 658479 657913 659308 658561 656879 658070 657276

$./countElems_p 10000000 1 4
Counts array:
579846 580814 580122 579772 582509 582713 582518 580917 581963 581094
```


---

Lưu ý rằng kết quả in ra **thay đổi đáng kể** ở mỗi lần chạy.  
Đặc biệt, chúng thay đổi khi ta thay đổi số lượng thread!  
Điều này **không nên xảy ra**, vì việc dùng seed tĩnh đảm bảo cùng một tập giá trị ở mỗi lần chạy.  

Những kết quả này **vi phạm** một trong những nguyên tắc cơ bản của lập trình đa luồng:  
> **Kết quả của chương trình phải đúng và nhất quán, bất kể số lượng thread sử dụng.**

---

Vì lần thử đầu tiên song song hóa `countElems` dường như **không hoạt động đúng**, hãy đi sâu hơn để xem chương trình đang làm gì và cách khắc phục.

---

### Data Races (Tranh chấp dữ liệu)

Để hiểu chuyện gì đang xảy ra, hãy xét một ví dụ chạy với **hai thread** trên **hai core** riêng biệt của một hệ thống đa lõi.  
Hãy nhớ rằng việc thực thi của bất kỳ thread nào cũng có thể bị **OS** tạm dừng (preempt) tại bất kỳ thời điểm nào, nghĩa là mỗi thread có thể đang chạy **các lệnh khác nhau** của cùng một hàm tại một thời điểm nhất định (hoặc thậm chí cùng một lệnh).

Bảng 1 cho thấy một đường thực thi khả dĩ của hàm `countElems`.  
Để minh họa rõ hơn, chúng ta dịch dòng:

```c
counts[val] = counts[val] + 1;
```

thành chuỗi lệnh tương đương sau:

1. **Read** `counts[val]` và đưa vào một thanh ghi.  
2. **Modify** thanh ghi bằng cách tăng giá trị lên 1.  
3. **Write** nội dung của thanh ghi trở lại `counts[val]`.

---

Mẫu này được gọi là **read–modify–write**.  
Trong ví dụ ở Bảng 1, mỗi thread chạy trên một core riêng (Thread 0 trên Core 0, Thread 1 trên Core 1).  
Chúng ta bắt đầu quan sát quá trình tại thời điểm *i*, khi cả hai thread đều có `val` = 1.

Dưới đây là bản dịch tiếng Việt của nội dung bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu:

---

| Thời điểm | Thread 0 | Thread 1 |
|-----------|----------|----------|
| *i*       | Đọc `counts[1]` và đưa vào thanh ghi của Core 0 | ... |
| *i+1*     | Tăng giá trị trong thanh ghi lên 1 | Đọc `counts[1]` và đưa vào thanh ghi của Core 1 |
| *i+2*     | Ghi đè `counts[1]` bằng nội dung của thanh ghi | Tăng giá trị trong thanh ghi lên 1 |
| *i+3*     | ... | Ghi đè `counts[1]` bằng nội dung của thanh ghi |

**Bảng 1.** Một trình tự thực thi khả dĩ của hai thread khi chạy `countElems`

---

Giả sử trước khi thực hiện trình tự trong [Bảng 1](#ExecSequence), `counts[1]` chứa giá trị **60**.  
Tại thời điểm *i*, Thread 0 đọc `counts[1]` và đưa giá trị 60 vào thanh ghi của Core 0.  
Tại thời điểm *i+1*, trong khi Thread 0 tăng giá trị trong thanh ghi của Core 0 lên 1, thì **giá trị hiện tại** của `counts[1]` (60) được Thread 1 đọc và đưa vào thanh ghi của Core 1.  
Tại thời điểm *i+2*, Thread 0 cập nhật `counts[1]` thành 61, trong khi Thread 1 tăng giá trị trong thanh ghi cục bộ của nó (60) lên 1.  
Kết quả cuối cùng là tại thời điểm *i+3*, giá trị `counts[1]` bị Thread 1 ghi đè thành 61, **không phải** 62 như mong đợi!  
Điều này khiến `counts[1]` về cơ bản **bị mất** một lần tăng.

---

Chúng ta gọi tình huống hai thread cố gắng ghi vào cùng một vị trí bộ nhớ là **data race** (tranh chấp dữ liệu).  
Nói chung hơn, **race condition** là bất kỳ tình huống nào mà việc thực thi đồng thời của hai thao tác dẫn đến kết quả sai.  
Lưu ý rằng việc **đọc đồng thời** `counts[1]` **không** tự nó tạo thành race condition, vì việc đọc giá trị từ bộ nhớ thường không gây vấn đề.  
Chính sự kết hợp của bước đọc này với các thao tác ghi vào `counts[1]` mới gây ra kết quả sai.  
Mẫu **read–modify–write** này là một nguồn phổ biến gây ra **data race** trong hầu hết các chương trình đa luồng.  
Trong phần thảo luận về race condition và cách khắc phục, chúng ta sẽ tập trung vào **data race**.

---

> **Atomic operations**  
> Một thao tác được gọi là **atomic** nếu một thread nhận thấy nó được thực hiện **không bị gián đoạn** (nói cách khác, là một hành động “tất cả hoặc không gì cả”).  
> Trong một số thư viện, có từ khóa hoặc kiểu dữ liệu để chỉ định rằng một khối tính toán nên được xử lý như một thao tác atomic.  
> Trong ví dụ trước, dòng `counts[val] = counts[val] + 1` (ngay cả khi viết là `counts[val]++`) **không** phải là atomic, vì dòng này thực tế tương ứng với nhiều lệnh ở cấp độ máy.  
> Cần có một cấu trúc đồng bộ hóa như **mutual exclusion** (loại trừ lẫn nhau) để đảm bảo không xảy ra data race.  
> Nói chung, nên giả định rằng mọi thao tác đều **không atomic** trừ khi mutual exclusion được áp dụng một cách tường minh.

---

Hãy nhớ rằng **không phải** mọi trình tự thực thi của hai thread đều gây ra race condition.  
Xem ví dụ trình tự thực thi của Thread 0 và Thread 1 trong Bảng 2.

| Thời điểm | Thread 0 | Thread 1 |
|-----------|----------|----------|
| *i*       | Đọc `counts[1]` và đưa vào thanh ghi của Core 0 | ... |
| *i+1*     | Tăng giá trị trong thanh ghi lên 1 | ... |
| *i+2*     | Ghi đè `counts[1]` bằng nội dung của thanh ghi | ... |
| *i+3*     | ... | Đọc `counts[1]` và đưa vào thanh ghi của Core 1 |
| *i+4*     | ... | Tăng giá trị trong thanh ghi lên 1 |
| *i+5*     | ... | Ghi đè `counts[1]` bằng nội dung của thanh ghi |

**Bảng 2.** Một trình tự thực thi khác của hai thread khi chạy `countElems`

---

Trong trình tự này, Thread 1 **không** đọc `counts[1]` cho đến **sau** khi Thread 0 đã cập nhật giá trị mới (61).  
Kết quả là Thread 1 đọc giá trị 61 từ `counts[1]` và đưa vào thanh ghi của Core 1 tại thời điểm *i+3*, sau đó ghi giá trị 62 vào `counts[1]` tại thời điểm *i+5*.

---

Để khắc phục **data race**, trước tiên chúng ta phải xác định **critical section** — phần mã cần được thực thi **atomic** (cô lập) để đảm bảo hành vi đúng.  
Trong các chương trình đa luồng, các khối mã cập nhật tài nguyên chia sẻ thường được xác định là critical section.

Trong hàm `countElems`, các thao tác cập nhật mảng `counts` cần được đặt trong critical section để đảm bảo giá trị không bị mất do nhiều thread cùng cập nhật một vị trí bộ nhớ:


```c
long i;
for (i = start; i < end; i++) {
    val = array[i];
    counts[val] = counts[val] + 1; // dòng này cần được bảo vệ
}
```

Vì vấn đề cốt lõi trong `countElems` là việc **truy cập đồng thời** vào mảng `counts` bởi nhiều thread, nên cần có một cơ chế đảm bảo rằng **chỉ một thread** được thực thi bên trong **critical section** tại một thời điểm.  
Sử dụng một cấu trúc đồng bộ hóa (như **mutex**, sẽ được trình bày trong phần tiếp theo) sẽ buộc các thread phải **lần lượt** đi vào critical section.









