## 14.8. Tóm tắt

Chương này đã cung cấp cái nhìn tổng quan về **bộ xử lý đa lõi** (multicore processors) và cách lập trình cho chúng.  
Cụ thể, chúng ta đã tìm hiểu về thư viện **POSIX threads** (hay **Pthreads**) và cách sử dụng nó để tạo ra các chương trình đa luồng đúng đắn, giúp tăng tốc hiệu năng của một chương trình đơn luồng.  
Các thư viện như POSIX và OpenMP sử dụng mô hình giao tiếp **shared memory** (bộ nhớ chia sẻ), trong đó các thread chia sẻ dữ liệu trong cùng một không gian bộ nhớ.

---

### Những điểm chính cần ghi nhớ

**Threads là đơn vị cơ bản của các chương trình concurrent**  

:   Để song song hóa một chương trình tuần tự, lập trình viên sử dụng các cấu trúc nhẹ gọi là **thread**.  
    Trong một **process đa luồng** (multithreaded process) cụ thể, mỗi thread có vùng **stack** riêng, nhưng chia sẻ dữ liệu chương trình, vùng **heap** và mã lệnh của process.  
    Giống như process, thread chạy **nondeterministically** (không xác định trước) trên CPU (tức là thứ tự thực thi thay đổi giữa các lần chạy, và việc thread nào được gán cho core nào là do hệ điều hành quyết định).

---

**Cấu trúc đồng bộ hóa đảm bảo chương trình hoạt động đúng**  

:   Hệ quả của việc dùng bộ nhớ chia sẻ là các thread có thể vô tình ghi đè dữ liệu trong vùng nhớ chung.  
    **Race condition** có thể xảy ra bất cứ khi nào hai thao tác cập nhật sai một giá trị chia sẻ.  
    Khi giá trị chia sẻ đó là dữ liệu, một dạng đặc biệt của race condition gọi là **data race** có thể xuất hiện.  
    Các cấu trúc đồng bộ hóa (mutex, semaphore, v.v.) giúp đảm bảo tính đúng đắn của chương trình bằng cách buộc các thread phải thực thi lần lượt khi cập nhật biến chia sẻ.

---

**Cẩn trọng khi sử dụng cấu trúc đồng bộ hóa**  

:   Đồng bộ hóa vốn dĩ tạo ra các điểm tính toán tuần tự trong một chương trình vốn song song.  
    Do đó, cần chú ý *cách* áp dụng các khái niệm đồng bộ hóa.  
    Tập hợp các thao tác cần chạy **atomic** (nguyên tử) được gọi là **critical section**.  
    Nếu critical section quá lớn, các thread sẽ thực thi tuần tự, không mang lại cải thiện về thời gian chạy.  
    Nếu dùng cấu trúc đồng bộ hóa một cách cẩu thả, các tình huống như **deadlock** có thể vô tình xảy ra.  
    Chiến lược tốt là để thread sử dụng biến cục bộ nhiều nhất có thể và chỉ cập nhật biến chia sẻ khi thật sự cần thiết.

---

**Không phải mọi thành phần của chương trình đều có thể song song hóa**  

:   Một số chương trình tất yếu có các thành phần tuần tự lớn, có thể cản trở hiệu năng của chương trình đa luồng trên nhiều core (ví dụ: **Amdahl’s Law**).  
    Ngay cả khi một tỷ lệ lớn của chương trình có thể song song hóa, speedup hiếm khi tuyến tính.  
    Người đọc cũng nên xem xét các chỉ số khác như **efficiency** (hiệu suất) và **scalability** (khả năng mở rộng) khi đánh giá hiệu năng chương trình.

---

### Tài liệu đọc thêm

Chương này nhằm giới thiệu khái niệm về **concurrency** (tính đồng thời) với threads; nó **không** bao quát toàn bộ chủ đề.  
Để tìm hiểu thêm về lập trình với POSIX threads và OpenMP, hãy tham khảo các hướng dẫn chất lượng cao của Blaise Barney từ Lawrence Livermore National Labs:  
- [Pthreads](https://hpc-tutorials.llnl.gov/posix/)  
- [OpenMP](https://hpc.llnl.gov/tuts/openMP/)

Đối với các công cụ tự động hỗ trợ gỡ lỗi chương trình song song, người đọc có thể tìm hiểu:  
- [Helgrind](https://valgrind.org/docs/manual/hg-manual.html)  
- [DRD](https://valgrind.org/docs/manual/drd-manual.html) trong bộ công cụ Valgrind.

---

Trong [chương cuối](../C15-Parallel/index.html#_looking_ahead_other_parallel_systems_and_parallel_programming_models) của cuốn sách, chúng ta sẽ có cái nhìn tổng quan ở mức cao về các kiến trúc song song phổ biến khác và cách lập trình cho chúng.  
[Xem tiếp để tìm hiểu thêm](../C15-Parallel/index.html#_looking_ahead_other_parallel_systems_and_parallel_programming_models).