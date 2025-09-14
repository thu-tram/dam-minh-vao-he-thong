### 14.3.3. Các cấu trúc đồng bộ hóa khác (Other Synchronization Constructs)

**Mutex** và **semaphore** không phải là những ví dụ duy nhất về cấu trúc đồng bộ hóa có thể được sử dụng trong ngữ cảnh các chương trình đa luồng.  
Trong tiểu mục này, chúng ta sẽ thảo luận ngắn gọn về hai cấu trúc đồng bộ hóa khác là **barrier** và **condition variable**, cả hai đều là một phần của thư viện **Pthreads**.

---

#### Barriers

**Barrier** là một loại cấu trúc đồng bộ hóa buộc **tất cả** các thread phải đạt đến một điểm chung trong quá trình thực thi trước khi cho phép chúng tiếp tục chạy song song.  
Pthreads cung cấp một primitive đồng bộ hóa dạng barrier.  
Để sử dụng barrier của Pthreads, cần thực hiện các bước sau:

- Khai báo một biến barrier toàn cục (ví dụ: `pthread_barrier_t barrier`)
- Khởi tạo barrier trong `main` (`pthread_barrier_init(&barrier)`)
- Hủy barrier trong `main` sau khi sử dụng (`pthread_barrier_destroy(&barrier)`)
- Sử dụng hàm `pthread_barrier_wait` để tạo một điểm đồng bộ hóa.

---

Chương trình sau minh họa cách sử dụng barrier trong một hàm có tên `threadEx`:

```c
void *threadEx(void *args){
    // parse args
    // ...
    long myid = myargs->id;
    int nthreads = myargs->numthreads;
    int *array = myargs->array;

    printf("Thread %ld starting thread work!\n", myid);
    pthread_barrier_wait(&barrier); // forced synchronization point
    printf("All threads have reached the barrier!\n");
    for (i = start; i < end; i++) {
        array[i] = array[i] * 2;
    }
    printf("Thread %ld done with work!\n", myid);

    return NULL;
}
```

Trong ví dụ này, **không thread nào** có thể bắt đầu xử lý phần mảng được gán cho đến khi **tất cả** các thread đã in ra thông báo rằng chúng bắt đầu làm việc.  
Nếu không có barrier, có thể một thread đã hoàn thành công việc trước khi các thread khác kịp in thông báo bắt đầu.  
Lưu ý rằng **vẫn** có thể xảy ra trường hợp một thread in thông báo hoàn thành công việc trước khi một thread khác thực sự kết thúc.

---

#### Condition Variables

**Condition variable** buộc một thread phải **block** (chặn) cho đến khi một điều kiện cụ thể được thỏa mãn.  
Cấu trúc này hữu ích trong các tình huống mà một điều kiện phải được đáp ứng trước khi thread thực hiện công việc.  
Nếu không có condition variable, thread sẽ phải liên tục kiểm tra xem điều kiện đã được đáp ứng chưa, gây tốn CPU.  

Condition variable **luôn** được sử dụng cùng với mutex.  
Trong loại cấu trúc đồng bộ hóa này, mutex đảm bảo **mutual exclusion** (loại trừ lẫn nhau), trong khi condition variable đảm bảo rằng điều kiện cụ thể được đáp ứng trước khi một thread giành được mutex.

---

Condition variable trong POSIX có kiểu `pthread_cond_t`.  
Giống như mutex và barrier, condition variable phải được **khởi tạo trước khi sử dụng** và **hủy sau khi sử dụng**.

- Để khởi tạo condition variable, dùng hàm `pthread_cond_init`.
- Để hủy condition variable, dùng hàm `pthread_cond_destroy`.

---

Hai hàm thường được gọi khi sử dụng condition variable là `pthread_cond_wait` và `pthread_cond_signal`.  
Cả hai hàm này đều yêu cầu **địa chỉ của một mutex** ngoài địa chỉ của condition variable:

- `pthread_cond_wait(&cond, &mutex)`: nhận địa chỉ của condition variable `cond` và mutex `mutex` làm tham số.  
  Hàm này khiến thread gọi nó bị block trên condition variable `cond` cho đến khi một thread khác **signal** (đánh thức) nó.

- `pthread_cond_signal(&cond)`: khiến thread gọi hàm này **đánh thức** một thread khác đang chờ trên condition variable `cond` (theo ưu tiên lập lịch).  
  Nếu không có thread nào đang bị block trên condition đó, hàm sẽ không có tác dụng.  
  Khác với `pthread_cond_wait`, hàm `pthread_cond_signal` có thể được gọi bởi một thread **bất kể** nó có đang giữ mutex liên quan hay không.

---

#### Ví dụ về Condition Variable

Thông thường, condition variable hữu ích nhất khi một nhóm thread đang chờ một nhóm khác hoàn thành một hành động nào đó.  
Trong ví dụ sau, chúng ta sử dụng nhiều thread để mô phỏng một nhóm **nông dân** thu thập trứng từ một nhóm **gà**.  
"Chicken" và "Farmer" đại diện cho hai lớp thread riêng biệt.  
Toàn bộ mã nguồn của chương trình này có thể tải tại [layeggs.c](_attachments/layeggs.c).  
Lưu ý rằng đoạn mã liệt kê ở đây đã lược bỏ nhiều phần chú thích/xử lý lỗi để ngắn gọn.

```c
int main(int argc, char **argv){
    //... declarations omitted for brevity

    // these will be shared by all threads via pointer fields in t_args
    int num_eggs;           // number of eggs ready to collect
    pthread_mutex_t mutex;  // mutex associated with cond variable
    pthread_cond_t  eggs;   // used to block/wake-up farmer waiting for eggs

    //... args parsing removed for brevity

    num_eggs = 0; // number of eggs ready to collect
    ret = pthread_mutex_init(&mutex, NULL); //initialize the mutex
    pthread_cond_init(&eggs, NULL); //initialize the condition variable

    //... thread_array and thread_args creation/filling omitted for brevity

    // create some chicken and farmer threads
    for (i = 0; i < (2 * nthreads); i++) {
        if ( (i % 2) == 0 ) {
            ret = pthread_create(&thread_array[i], NULL,
                                 chicken, &thread_args[i]);
        }
        else {
            ret = pthread_create(&thread_array[i], NULL,
                                 farmer, &thread_args[i] );
        }
    }

    // wait for chicken and farmer threads to exit
    for (i = 0; i < (2 * nthreads); i++)  {
        ret = pthread_join(thread_array[i], NULL);
    }

    // clean-up program state
    pthread_mutex_destroy(&mutex); //destroy the mutex
    pthread_cond_destroy(&eggs);   //destroy the cond var

    return 0;
}
```

Hàm `main` tạo một biến chia sẻ `num_eggs` (biểu thị tổng số trứng hiện có tại bất kỳ thời điểm nào), một `mutex` chia sẻ (được sử dụng mỗi khi một thread truy cập `num_eggs`), và một condition variable chia sẻ `eggs`.  
Sau đó, nó tạo **hai thread Chicken** và **hai thread Farmer**.



Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu:

---

Mỗi **Chicken thread** (thread “gà”) chịu trách nhiệm đẻ một số lượng trứng nhất định:


```c
void *chicken(void *args ) {
    struct t_arg *myargs = (struct t_arg *)args;
    int *num_eggs, i, num;

    num_eggs = myargs->num_eggs;
    i = 0;

    // lay some eggs
    for (i = 0; i < myargs->total_eggs; i++) {
        usleep(EGGTIME); //chicken sleeps

        pthread_mutex_lock(myargs->mutex);
        *num_eggs = *num_eggs + 1;  // update number of eggs
        num = *num_eggs;
        pthread_cond_signal(myargs->eggs); // wake a sleeping farmer (squawk)
        pthread_mutex_unlock(myargs->mutex);

        printf("chicken %d created egg %d available %d\n",myargs->id,i,num);
    }
    return NULL;
}
```



Để đẻ một quả trứng, Chicken thread sẽ **ngủ** một lúc, sau đó **giành mutex** và cập nhật tổng số trứng hiện có (`num_eggs`) tăng thêm 1.  
Trước khi **nhả mutex**, Chicken thread sẽ **đánh thức** một Farmer thread đang ngủ (có thể là bằng tiếng kêu “cục tác”).  
Chicken thread lặp lại chu trình này cho đến khi đẻ hết số trứng mà nó dự định (`total_eggs`).

---

Mỗi **Farmer thread** (thread “nông dân”) chịu trách nhiệm thu thập `total_eggs` quả trứng từ đàn gà (có thể là để ăn sáng):


```c
void *farmer(void *args ) {
    struct t_arg * myargs = (struct t_arg *)args;
    int *num_eggs, i, num;

    num_eggs = myargs->num_eggs;

    i = 0;

    for (i = 0; i < myargs->total_eggs; i++) {
        pthread_mutex_lock(myargs->mutex);
        while (*num_eggs == 0 ) { // no eggs to collect
            // wait for a chicken to lay an egg
            pthread_cond_wait(myargs->eggs, myargs->mutex);
        }

        // we hold mutex lock here and num_eggs > 0
        num = *num_eggs;
        *num_eggs = *num_eggs - 1;
        pthread_mutex_unlock(myargs->mutex);

        printf("farmer %d gathered egg %d available %d\n",myargs->id,i,num);
    }
    return NULL;
}
```





Mỗi Farmer thread sẽ **giành mutex** trước khi kiểm tra biến chia sẻ `num_eggs` để xem có trứng nào sẵn sàng không (`*num_eggs == 0`).  
Trong khi **không có** trứng, Farmer thread sẽ **block** (tức là “ngủ”).

---

Sau khi Farmer thread được “đánh thức” bởi tín hiệu từ một Chicken thread, nó sẽ kiểm tra lại xem trứng vẫn còn không (vì có thể một Farmer khác đã lấy trước).  
Nếu còn, Farmer sẽ “thu thập” trứng (giảm `num_eggs` đi 1) và **nhả mutex**.

---

Theo cách này, Chicken và Farmer phối hợp nhịp nhàng để đẻ và thu thập trứng.  
**Condition variable** đảm bảo rằng không Farmer thread nào thu thập trứng cho đến khi trứng được đẻ bởi một Chicken thread.

---

#### Broadcasting

Một hàm khác được sử dụng với condition variable là `pthread_cond_broadcast`, hữu ích khi **nhiều thread** đang bị block trên cùng một điều kiện.  
Gọi `pthread_cond_broadcast(&cond)` sẽ **đánh thức tất cả** các thread đang bị block trên condition `cond`.  

Trong ví dụ tiếp theo, chúng ta sẽ thấy cách condition variable có thể triển khai cơ chế **barrier** đã được thảo luận trước đó:


```c
// mutex (initialized in main)
pthread_mutex_t mutex;

// condition variable signifying the barrier (initialized in main)
pthread_cond_t barrier;

void *threadEx_v2(void *args){
    // parse args
    // ...

    long myid = myargs->id;
    int nthreads = myargs->numthreads;
    int *array = myargs->array

    // counter denoting the number of threads that reached the barrier
    int *n_reached = myargs->n_reached;

    // start barrier code
    pthread_mutex_lock(&mutex);
    *n_reached++;

    printf("Thread %ld starting work!\n", myid)

    // if some threads have not reached the barrier
    while (*n_reached < nthreads) {
        pthread_cond_wait(&barrier, &mutex);
    }
    // all threads have reached the barrier
    printf("all threads have reached the barrier!\n");
    pthread_cond_broadcast(&barrier);

    pthread_mutex_unlock(&mutex);
    // end barrier code

    // normal thread work
    for (i = start; i < end; i++) {
        array[i] = array[i] * 2;
    }
    printf("Thread %ld done with work!\n", myid);

    return NULL;
}
```


Hàm `threadEx_v2` có chức năng giống hệt `threadEx`.  
Trong ví dụ này, condition variable được đặt tên là `barrier`.  
Khi mỗi thread giành được lock, nó sẽ tăng biến `n_reached` — số lượng thread đã đến điểm barrier.  
Trong khi số thread đã đến barrier **nhỏ hơn** tổng số thread, thread sẽ **chờ** trên condition variable `barrier` và mutex `mutex`.

---

Tuy nhiên, khi **thread cuối cùng** đến barrier, nó sẽ gọi `pthread_cond_broadcast(&barrier)`, giải phóng **tất cả** các thread khác đang chờ trên condition variable `barrier`, cho phép chúng tiếp tục thực thi.

---

Ví dụ này hữu ích để minh họa hàm `pthread_cond_broadcast`;  
tuy nhiên, tốt nhất là nên sử dụng **Pthreads barrier primitive** bất cứ khi nào cần barrier trong chương trình.

---

Một câu hỏi mà sinh viên thường đặt ra là:  
Vòng lặp `while` bao quanh lời gọi `pthread_cond_wait` trong code `farmer` và `threadEx_v2` có thể thay bằng câu lệnh `if` không?  

Câu trả lời là **không** — vòng lặp `while` này **hoàn toàn cần thiết** vì hai lý do chính:

1. **Điều kiện có thể thay đổi** trước khi thread vừa được đánh thức kịp tiếp tục thực thi.  
   Vòng lặp `while` đảm bảo điều kiện được kiểm tra lại một lần cuối.

2. Hàm `pthread_cond_wait` có thể gặp **spurious wakeups** — tình huống một thread bị đánh thức **nhầm** ngay cả khi điều kiện chưa được thỏa mãn.  

Vòng lặp `while` ở đây chính là một ví dụ của **predicate loop**, buộc phải kiểm tra lại condition variable trước khi nhả mutex.  
Vì vậy, việc sử dụng **predicate loop** là **thực hành đúng đắn** khi làm việc với condition variable.




