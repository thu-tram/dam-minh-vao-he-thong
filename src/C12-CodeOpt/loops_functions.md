## 12.2. Các tối ưu hóa khác của trình biên dịch: Loop Unrolling và Function Inlining

Kỹ thuật tối ưu **loop-invariant code motion** (di chuyển mã bất biến ra khỏi vòng lặp) được mô tả ở phần trước là một thay đổi đơn giản nhưng mang lại sự giảm đáng kể thời gian thực thi. Tuy nhiên, các tối ưu hóa như vậy phụ thuộc vào từng tình huống cụ thể và không phải lúc nào cũng cải thiện hiệu năng. Trong hầu hết các trường hợp, việc di chuyển mã bất biến ra khỏi vòng lặp đã được trình biên dịch tự động xử lý.

Ngày nay, mã nguồn thường được **đọc** nhiều hơn là **viết**. Trong đa số trường hợp, những cải thiện hiệu năng nhỏ lẻ không đáng để đánh đổi khả năng dễ đọc của mã. Nói chung, lập trình viên nên để trình biên dịch tự tối ưu bất cứ khi nào có thể. Trong phần này, chúng ta sẽ tìm hiểu một số kỹ thuật tối ưu hóa mà trước đây lập trình viên thường tự thực hiện thủ công, nhưng hiện nay thường được trình biên dịch tự động áp dụng.

Có nhiều nguồn tài liệu trực tuyến khuyến khích việc tự tay áp dụng các kỹ thuật được mô tả trong các phần sau. Tuy nhiên, chúng tôi khuyến nghị bạn nên kiểm tra xem trình biên dịch của mình có hỗ trợ các tối ưu hóa này hay không trước khi cố gắng tự triển khai chúng trong mã. Tất cả các tối ưu hóa được mô tả trong phần này đều được GCC hỗ trợ, nhưng có thể không có trong các trình biên dịch cũ.

---

### 12.2.1. Function Inlining

Một bước tối ưu hóa mà trình biên dịch thường cố gắng thực hiện là **function inlining** (nội tuyến hàm), tức là thay thế lời gọi hàm bằng phần thân của hàm đó.  
Ví dụ, trong hàm `main`, nếu trình biên dịch nội tuyến hàm `allocateArray`, nó sẽ thay lời gọi `allocateArray` bằng lời gọi trực tiếp tới `malloc`:

| Phiên bản gốc | Phiên bản với `allocateArray` được inline |
|---------------|-------------------------------------------|
| ```c | ```c |
| int main(int argc, char **argv) { | int main(int argc, char **argv) { |
|     // omitted for brevity |     // omitted for brevity |
|     // some variables shortened for space considerations |     // some variables shortened for space considerations |
|     int lim = strtol(argv[1], NULL, 10); |     int lim = strtol(argv[1], NULL, 10); |
|     // allocation of array | // allocation of array (in-lined) |
|     int *a = allocateArray(lim); |     int *a = malloc(lim * sizeof(int)); |
|     // generates sequence of primes |     // generates sequence of primes |
|     int len = genPrimeSequence(a, lim); |     int len = genPrimeSequence(a, lim); |
|     return 0; |     return 0; |
| } | } |
| ``` | ``` |

**Bảng 1.** Ví dụ trình biên dịch nội tuyến hàm `allocateArray`.

Việc nội tuyến hàm có thể giúp chương trình tiết kiệm thời gian chạy. Hãy nhớ rằng mỗi lần chương trình gọi một hàm, sẽ có nhiều lệnh liên quan đến việc **tạo** và **hủy** ngữ cảnh hàm được sinh ra. Nội tuyến hàm cho phép trình biên dịch loại bỏ các lời gọi dư thừa này, đồng thời giúp nó dễ dàng phát hiện các cơ hội tối ưu khác như **constant propagation** (truyền hằng), **constant folding** (gộp hằng), và **dead code elimination** (loại bỏ mã chết).  
Trong trường hợp của chương trình `optExample`, việc nội tuyến có thể cho phép trình biên dịch thay lời gọi `sqrt` bằng lệnh `fsqrt`, và sau đó di chuyển nó ra ngoài vòng lặp.

Cờ `-finline-functions` gợi ý cho GCC rằng các hàm nên được nội tuyến. Tối ưu hóa này được bật ở mức `-O3`. Mặc dù `-finline-functions` có thể được sử dụng độc lập với `-O3`, nhưng đây chỉ là *gợi ý* để trình biên dịch tìm các hàm có thể nội tuyến. Tương tự, từ khóa `static inline` có thể được dùng để gợi ý rằng một hàm cụ thể nên được nội tuyến.  
Cần lưu ý rằng trình biên dịch sẽ không nội tuyến tất cả các hàm, và việc nội tuyến không đảm bảo sẽ làm mã chạy nhanh hơn.

Lập trình viên nhìn chung nên tránh nội tuyến hàm thủ công. Việc này tiềm ẩn nguy cơ cao làm giảm đáng kể khả năng đọc mã, tăng khả năng xuất hiện lỗi, và khiến việc cập nhật, bảo trì hàm trở nên khó khăn hơn. Ví dụ, cố gắng nội tuyến hàm `isPrime` vào trong `getNextPrime` sẽ làm giảm mạnh khả năng đọc của `getNextPrime`.

---

### 12.2.2. Loop Unrolling

Chiến lược tối ưu hóa cuối cùng của trình biên dịch mà chúng ta thảo luận trong phần này là **loop unrolling** (trải vòng lặp). Hãy cùng xem lại hàm `isPrime`:

```
// helper function: checks to see if a number is prime
int isPrime(int x) {
    int i;
    int max = sqrt(x) + 1;

    // no prime number is less than 2
    for (i = 2; i < max; i++) {
        // if the number is divisible by i
        if (x % i == 0) {
            return 0; // it's not prime
        }
    }
    return 1; // otherwise it is
}
```

Vòng lặp `for` thực thi tổng cộng `max` lần, trong đó `max` bằng căn bậc hai của số nguyên `x` cộng thêm một.  
Ở cấp độ **assembly**, mỗi lần thực thi vòng lặp sẽ kiểm tra xem `i` có nhỏ hơn `max` hay không. Nếu có, **instruction pointer** (con trỏ lệnh) sẽ nhảy vào phần thân vòng lặp, nơi thực hiện phép toán modulo. Nếu phép modulo cho kết quả bằng 0, chương trình lập tức thoát khỏi vòng lặp và trả về 0. Ngược lại, vòng lặp tiếp tục chạy.  
Mặc dù **branch predictor** (bộ dự đoán nhánh) thường dự đoán khá chính xác giá trị của biểu thức điều kiện (đặc biệt là bên trong vòng lặp), nhưng các dự đoán sai có thể làm giảm hiệu năng do gây gián đoạn **instruction pipeline** (đường ống lệnh).

**Loop unrolling** (trải vòng lặp) là một kỹ thuật tối ưu hóa mà trình biên dịch thực hiện để giảm tác động của các dự đoán sai. Trong loop unrolling, mục tiêu là giảm số vòng lặp đi một hệ số *n* bằng cách tăng khối lượng công việc mà mỗi vòng lặp thực hiện lên *n* lần.  
Khi một vòng lặp được unroll với hệ số 2, số vòng lặp sẽ giảm **một nửa**, trong khi lượng công việc mỗi vòng tăng **gấp đôi**.

Hãy áp dụng thủ công kỹ thuật loop unrolling hệ số 2 vào hàm `isPrime` của chúng ta (có trong [optExample3.c](_attachments/optExample3.c)):

```c
// helper function: checks to see if a number is prime
int isPrime(int x) {
    int i;
    int max = sqrt(x)+1;

    // no prime number is less than 2
    for (i = 2; i < max; i+=2) {
        // if the number is divisible by i or i+1
        if ( (x % i == 0) || (x % (i+1) == 0) ) {
            return 0; // it's not prime
        }
    }
    return 1; // otherwise it is
}
```

Lưu ý rằng mặc dù chúng ta đã giảm một nửa số vòng lặp `for`, nhưng mỗi vòng lặp giờ thực hiện hai phép kiểm tra modulo, tức là khối lượng công việc mỗi vòng tăng gấp đôi.  
Khi biên dịch và chạy lại chương trình, thời gian chạy chỉ cải thiện một chút (xem [Bảng 2](#NextTimes)). Đồng thời, khả năng đọc của mã cũng giảm.  
Một cách tốt hơn để tận dụng loop unrolling là sử dụng cờ tối ưu hóa `-funroll-loops` của trình biên dịch, cờ này yêu cầu trình biên dịch unroll các vòng lặp có thể xác định số lần lặp ngay tại thời điểm biên dịch.  
Cờ `-funroll-all-loops` là một tùy chọn mạnh mẽ hơn, unroll tất cả các vòng lặp bất kể trình biên dịch có chắc chắn về số lần lặp hay không.  
[Bảng 2](#NextTimes) cho thấy thời gian chạy của kỹ thuật loop unrolling thủ công hệ số 2 (trong `optExample3.c`) so với việc thêm các cờ tối ưu `-funroll-loops` và `-funroll-all-loops` vào chương trình trước đó ([optExample2.c](_attachments/optExample2.c)).

| Phiên bản | Unoptimized | `-O1` | `-O2` | `-O3` |
|-----------|-------------|-------|-------|-------|
| Original ([optExample.c](_attachments/optExample.c)) | 3.86 | 2.32 | 2.14 | 2.15 |
| With loop-invariant code motion ([optExample2.c](_attachments/optExample2.c)) | 1.83 | 1.63 | 1.71 | 1.63 |
| With manual 2-factor loop unrolling ([optExample3.c](_attachments/optExample3.c)) | 1.65 | 1.53 | 1.45 | 1.45 |
| With `-funroll-loops` ([optExample2.c](_attachments/optExample2.c)) | 1.82 | 1.48 | 1.46 | 1.46 |
| With `-funroll-all-loops` ([optExample2.c](_attachments/optExample2.c)) | 1.81 | 1.47 | 1.47 | 1.46 |

**Bảng 2.** Thời gian (giây) để tạo 5.000.000 số nguyên tố

Mặc dù loop unrolling thủ công mang lại một chút cải thiện hiệu năng, nhưng các cờ loop unrolling tích hợp sẵn của trình biên dịch, khi kết hợp với các cờ tối ưu hóa khác, cho hiệu năng tương đương.  
Nếu lập trình viên muốn áp dụng tối ưu hóa loop unrolling vào mã của mình, họ nên ưu tiên sử dụng các cờ của trình biên dịch, *không* nên tự unroll vòng lặp thủ công.