## 11.1. Hệ phân cấp bộ nhớ (The Memory Hierarchy)

Khi chúng ta tìm hiểu về các thiết bị lưu trữ trong máy tính hiện đại, một mô hình chung xuất hiện: các thiết bị có dung lượng lớn hơn thì lại có hiệu năng thấp hơn. Nói cách khác, hệ thống sử dụng cả những thiết bị nhanh và những thiết bị có khả năng lưu trữ lượng dữ liệu lớn, nhưng không có thiết bị nào vừa nhanh vừa lưu trữ được nhiều. Sự đánh đổi giữa hiệu năng và dung lượng này được gọi là **memory hierarchy** (hệ phân cấp bộ nhớ), và **Hình 1** minh họa trực quan hệ phân cấp này.

Các thiết bị lưu trữ cũng có sự đánh đổi tương tự giữa chi phí và mật độ lưu trữ: thiết bị nhanh hơn thì đắt hơn, cả về chi phí trên mỗi byte và chi phí vận hành (ví dụ: tiêu thụ năng lượng).  
Chẳng hạn, mặc dù **cache** mang lại hiệu năng tuyệt vời, nhưng chi phí (và thách thức sản xuất) để chế tạo một CPU có cache đủ lớn để loại bỏ nhu cầu dùng **main memory** là điều không khả thi.  
Các hệ thống thực tế phải kết hợp nhiều loại thiết bị để đáp ứng yêu cầu về hiệu năng và dung lượng của chương trình, và một hệ thống điển hình ngày nay thường tích hợp hầu hết, nếu không muốn nói là tất cả, các thiết bị được mô tả trong **Hình 1**.

![In order, from (high performance, high cost, low capacity) to (low performance, low cost, high capacity): registers, cache, main memory, flash disk, traditional disk, and remote secondary storage.](_images/MemoryHierarchy.png)

**Hình 1.** Hệ phân cấp bộ nhớ

Thực tế của hệ phân cấp bộ nhớ là điều không mấy dễ chịu đối với lập trình viên, những người thường muốn không phải bận tâm đến tác động hiệu năng của vị trí dữ liệu được lưu trữ.  
Ví dụ, khi khai báo một biến số nguyên *trong hầu hết các ứng dụng*, lập trình viên lý tưởng sẽ không phải lo lắng về sự khác biệt giữa dữ liệu được lưu trong cache hay trong main memory.  
Việc yêu cầu lập trình viên quản lý chi tiết loại bộ nhớ mà mỗi biến chiếm dụng sẽ là một gánh nặng, mặc dù đôi khi điều này có thể đáng làm đối với một số đoạn code nhỏ, quan trọng về hiệu năng.

Lưu ý rằng **Hình 1** phân loại *cache* như một thực thể duy nhất, nhưng hầu hết các hệ thống đều có nhiều cấp cache tạo thành một hệ phân cấp nhỏ hơn bên trong.  
Ví dụ, CPU thường tích hợp một **level one (L1) cache** rất nhỏ và nhanh, nằm khá gần **ALU** (Arithmetic Logic Unit), và một **level two (L2) cache** lớn hơn nhưng chậm hơn, nằm xa hơn.  
Nhiều CPU đa nhân (**multicore CPU**) còn chia sẻ dữ liệu giữa các nhân thông qua một **level three (L3) cache** lớn hơn.  
Mặc dù sự khác biệt giữa các cấp cache có thể quan trọng đối với các ứng dụng nhạy cảm về hiệu năng, cuốn sách này sẽ chỉ xem xét một cấp cache duy nhất để đơn giản hóa.

Mặc dù chương này chủ yếu tập trung vào việc di chuyển dữ liệu giữa **register**, **CPU cache** và **main memory**, [phần tiếp theo](devices.html#_storage_devices) sẽ mô tả các thiết bị lưu trữ phổ biến trong toàn bộ **memory hierarchy**.  
Chúng ta sẽ tìm hiểu về ổ đĩa và vai trò của chúng trong bức tranh tổng thể của quản lý bộ nhớ ở **Chương 13**, trong phần thảo luận về [virtual memory](../C13-OS/vm.html#_virtual_memory).
