### 14.3.2. Semaphores

**Semaphore** thường được sử dụng trong **hệ điều hành** và các chương trình **concurrent** (chạy đồng thời), với mục tiêu quản lý việc truy cập đồng thời vào một **pool** (bể) tài nguyên.  
Khi sử dụng semaphore, mục tiêu không phải là *ai* sở hữu cái gì, mà là *còn bao nhiêu* tài nguyên vẫn khả dụng.  
Semaphore khác với **mutex** ở một số điểm sau:

- Semaphore **không nhất thiết** phải ở trạng thái nhị phân (locked hoặc unlocked).  
  Một loại đặc biệt gọi là **counting semaphore** (semaphore đếm) có giá trị từ 0 đến *r*, trong đó *r* là số lượng tài nguyên tối đa có thể có.  
  Mỗi khi một tài nguyên được tạo ra, semaphore sẽ **tăng** giá trị.  
  Mỗi khi một tài nguyên được sử dụng, semaphore sẽ **giảm** giá trị.  
  Khi counting semaphore có giá trị 0, nghĩa là **không còn tài nguyên khả dụng**, và bất kỳ thread nào cố gắng lấy tài nguyên sẽ phải **chờ** (block).

- Semaphore có thể **mặc định ở trạng thái khóa**.

---

Mặc dù mutex và **condition variable** có thể mô phỏng chức năng của semaphore, nhưng trong một số trường hợp, sử dụng semaphore có thể **đơn giản** và **hiệu quả** hơn.  
Semaphore cũng có ưu điểm là **bất kỳ thread nào** cũng có thể mở khóa semaphore (khác với mutex, nơi thread gọi khóa phải là thread mở khóa).

---

Semaphore **không** phải là một phần của thư viện **Pthreads**, nhưng điều đó không có nghĩa là bạn không thể sử dụng chúng.  
Trên hệ thống Linux và macOS, các primitive semaphore có thể được truy cập từ `semaphore.h` (thường nằm trong `/usr/include`).  
Vì không có chuẩn chung, các lời gọi hàm có thể khác nhau giữa các hệ thống.  
Tuy nhiên, thư viện semaphore có các khai báo tương tự như mutex:

- **Khai báo** một semaphore (kiểu `sem_t`, ví dụ: `sem_t semaphore`).

- **Khởi tạo** semaphore bằng `sem_init` (thường trong `main`).  
  Hàm `sem_init` có 3 tham số:  
  1. Địa chỉ của semaphore.  
  2. Trạng thái khởi tạo (locked hoặc unlocked).  
  3. Cho biết semaphore sẽ được chia sẻ giữa các thread của **một process** (ví dụ: giá trị 0) hay **giữa các process** (ví dụ: giá trị 1).  

  Điều này hữu ích vì semaphore thường được dùng để đồng bộ hóa giữa các process.  
  Ví dụ: `sem_init(&semaphore, 1, 0)` nghĩa là semaphore ban đầu **đang khóa** (tham số thứ hai là 1) và được chia sẻ giữa các thread của cùng một process (tham số thứ ba là 0).  
  Ngược lại, mutex luôn bắt đầu ở trạng thái **unlocked**.  
  Lưu ý: trên macOS, hàm tương đương là `sem_open`.

- **Hủy** semaphore bằng `sem_destroy` (thường trong `main`).  
  Hàm này chỉ nhận con trỏ tới semaphore: `sem_destroy(&semaphore)`.  
  Lưu ý: trên macOS, hàm tương đương có thể là `sem_unlink` hoặc `sem_close`.

- Hàm `sem_wait` cho biết một tài nguyên đang được sử dụng và **giảm** giá trị semaphore.  
  Nếu giá trị semaphore > 0 (nghĩa là vẫn còn tài nguyên khả dụng), hàm sẽ trả về ngay và thread được phép tiếp tục.  
  Nếu giá trị semaphore = 0, thread sẽ **block** cho đến khi có tài nguyên khả dụng (tức semaphore > 0).  
  Ví dụ: `sem_wait(&semaphore)`.

- Hàm `sem_post` cho biết một tài nguyên được giải phóng và **tăng** giá trị semaphore.  
  Hàm này trả về ngay lập tức.  
  Nếu có thread đang chờ semaphore (tức giá trị semaphore trước đó là 0), thread đó sẽ nhận quyền sử dụng tài nguyên vừa được giải phóng.  
  Ví dụ: `sem_post(&semaphore)`.
