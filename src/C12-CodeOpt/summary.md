## 12.4. Những điểm chính và Tóm tắt

Hành trình ngắn (và có lẽ hơi gây nản) của chúng ta vào thế giới **code optimization** (tối ưu hóa code) nên truyền tải một thông điệp rất quan trọng tới người đọc: nếu bạn đang nghĩ đến việc tối ưu code thủ công, hãy cân nhắc kỹ xem điều gì đáng để bạn bỏ thời gian và điều gì nên để cho trình biên dịch xử lý.  
Dưới đây là một số lời khuyên quan trọng khi bạn muốn cải thiện hiệu năng code.

**Chọn cấu trúc dữ liệu và thuật toán tốt**

Không có gì có thể thay thế việc sử dụng đúng **algorithm** (thuật toán) và **data structure** (cấu trúc dữ liệu); việc không làm như vậy thường là nguyên nhân hàng đầu dẫn đến hiệu năng kém.  
Ví dụ, thuật toán nổi tiếng **Sieve of Eratosthenes** là một cách hiệu quả hơn nhiều để tạo ra các số nguyên tố so với thuật toán tùy chỉnh của chúng ta trong `optExample`, và mang lại cải thiện hiệu năng đáng kể.  
Đoạn dưới đây cho thấy thời gian cần để tạo tất cả các số nguyên tố từ 2 đến 5 triệu bằng một cài đặt của thuật toán sieve:

```bash
$ gcc -o genPrimes genPrimes.c
$ ./genPrimes 5000000
Found 348513 primes (0.122245 s)
```

Thuật toán sieve chỉ mất 0,12 giây để tìm tất cả các số nguyên tố từ 2 đến 5 triệu, so với 1,46 giây mà `optExample2` cần để tạo cùng tập số nguyên tố với optimization flag `-O3` được bật (nhanh hơn 12×).  
Việc cài đặt thuật toán sieve được để lại như một bài tập cho bạn đọc; tuy nhiên, rõ ràng là việc chọn một thuật toán tốt hơn ngay từ đầu sẽ tiết kiệm hàng giờ tối ưu hóa thủ công. Ví dụ này cho thấy tại sao kiến thức về cấu trúc dữ liệu và thuật toán là nền tảng đối với các nhà khoa học máy tính.

**Sử dụng hàm thư viện chuẩn bất cứ khi nào có thể**

Đừng “phát minh lại bánh xe”. Nếu trong quá trình lập trình bạn cần một hàm thực hiện một tác vụ rất phổ biến (ví dụ: tìm giá trị tuyệt đối, hoặc tìm giá trị lớn nhất/nhỏ nhất trong một danh sách số), hãy dừng lại và kiểm tra xem hàm đó đã tồn tại trong **standard library** (thư viện chuẩn) của ngôn ngữ hay chưa.  
Các hàm trong thư viện chuẩn thường được kiểm thử kỹ và tối ưu hóa cho hiệu năng.  
Ví dụ, nếu bạn tự viết một phiên bản `sqrt` của riêng mình, trình biên dịch có thể sẽ không biết để tự động thay thế lời gọi hàm đó bằng lệnh `fsqrt`.

**Tối ưu dựa trên dữ liệu, không dựa trên cảm giác**

Nếu sau khi đã chọn cấu trúc dữ liệu và thuật toán tốt *và* sử dụng các hàm thư viện chuẩn mà vẫn cần cải thiện hiệu năng, hãy dùng một công cụ **code profiler** (phân tích hiệu năng code) tốt như Valgrind.  
Tối ưu hóa *không bao giờ* nên dựa trên cảm giác. Tập trung quá nhiều vào những gì bạn *nghĩ* là nên tối ưu (mà không có dữ liệu chứng minh) thường dẫn đến lãng phí thời gian.

**Tách code phức tạp thành nhiều hàm**

Việc nội tuyến code thủ công thường không mang lại cải thiện hiệu năng đáng kể so với những gì trình biên dịch hiện đại có thể làm. Thay vào đó, hãy giúp trình biên dịch dễ dàng tối ưu hơn cho bạn.  
Trình biên dịch dễ tối ưu hơn với các đoạn code ngắn. Việc tách các thao tác phức tạp thành nhiều hàm vừa tăng khả năng đọc code, vừa giúp trình biên dịch tối ưu dễ hơn.  
Hãy kiểm tra xem trình biên dịch của bạn có tự động thử nội tuyến hay có cờ riêng để thử nội tuyến code hay không. Tốt hơn hết là để trình biên dịch thực hiện nội tuyến thay vì tự làm thủ công.

**Ưu tiên khả năng đọc code**

Trong nhiều ứng dụng ngày nay, khả năng đọc code là ưu tiên hàng đầu. Thực tế là code được đọc nhiều hơn là được viết.  
Nhiều công ty dành nhiều thời gian đào tạo kỹ sư phần mềm viết code theo một phong cách nhất định để tối đa hóa khả năng đọc.  
Nếu việc tối ưu code làm giảm đáng kể khả năng đọc, bạn cần cân nhắc xem cải thiện hiệu năng có xứng đáng với sự đánh đổi đó hay không.  
Ví dụ, nhiều trình biên dịch hiện nay có optimization flag cho phép **loop unrolling** (trải vòng lặp). Lập trình viên nên luôn sử dụng các optimization flag này thay vì tự unroll vòng lặp, vì điều đó có thể làm giảm mạnh khả năng đọc code.  
Giảm khả năng đọc code thường làm tăng khả năng xuất hiện lỗi không mong muốn, từ đó có thể dẫn đến lỗ hổng bảo mật.

**Chú ý đến việc sử dụng bộ nhớ**

Việc sử dụng bộ nhớ của chương trình thường ảnh hưởng lớn hơn đến thời gian thực thi so với số lượng lệnh mà nó chạy.  
[Ví dụ về loop interchange](memory_considerations.html#_loop_interchange) minh họa rõ điều này. Trong cả hai trường hợp, vòng lặp thực thi cùng số lượng lệnh, nhưng thứ tự vòng lặp lại ảnh hưởng đáng kể đến truy cập bộ nhớ và **locality** (tính cục bộ).  
Hãy nhớ sử dụng các công cụ phân tích bộ nhớ như `massif` và `cachegrind` khi tối ưu chương trình.

**Trình biên dịch luôn được cải tiến**

Các lập trình viên trình biên dịch liên tục cập nhật để thực hiện các tối ưu hóa phức tạp hơn một cách an toàn.  
Ví dụ, GCC đã chuyển sang dạng [static single assignment (SSA)](https://gcc.gnu.org/onlinedocs/gccint/SSA.html) từ phiên bản 4.0, giúp cải thiện đáng kể hiệu quả của một số tối ưu hóa.  
Nhánh `GRAPHITE` của mã nguồn GCC triển khai [polyhedral model](https://polyhedral.info/), cho phép trình biên dịch thực hiện các loại biến đổi vòng lặp phức tạp hơn.  
Khi trình biên dịch ngày càng tinh vi, lợi ích của tối ưu hóa thủ công giảm đi đáng kể.