## 8.4. Điều khiển rẽ nhánh và vòng lặp (Conditional Control and Loops)

Phần này đề cập đến các lệnh assembly cho [câu lệnh điều kiện và vòng lặp](../C1-C_intro/conditionals.html#_conditionals_and_loops).  
Hãy nhớ rằng **câu lệnh điều kiện** cho phép lập trình viên thay đổi luồng thực thi của chương trình dựa trên kết quả của một biểu thức điều kiện.  

Trình biên dịch sẽ dịch các câu lệnh điều kiện thành các lệnh assembly thay đổi **instruction pointer** (`%eip`) để trỏ tới một địa chỉ **không phải** là địa chỉ kế tiếp trong chuỗi lệnh của chương trình.