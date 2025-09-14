## 7.4. Điều khiển có điều kiện và Vòng lặp (Conditional Control and Loops)

Phần này đề cập đến các lệnh assembly x86 dành cho [conditionals và loops](../C1-C_intro/conditionals.html#_conditionals_and_loops).  
Hãy nhớ rằng các câu lệnh điều kiện cho phép lập trình viên thay đổi luồng thực thi của chương trình dựa trên kết quả của một biểu thức điều kiện. **Compiler** sẽ dịch các câu lệnh điều kiện thành các lệnh assembly điều chỉnh **instruction pointer** (`%rip`) để trỏ tới một địa chỉ **không** phải là địa chỉ tiếp theo trong chuỗi lệnh của chương trình.