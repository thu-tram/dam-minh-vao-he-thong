### 4.4.2. Phép trừ (Subtraction)

Phép trừ kết hợp hai thao tác quen thuộc: **negation** (phủ định) và **addition** (cộng).  
Nói cách khác, phép trừ \(7 - 3\) tương đương với việc viết lại thành \(7 + (-3)\).  
Cách diễn đạt này phù hợp với cách phần cứng hoạt động — CPU vốn đã có mạch thực hiện **negation** và **addition**, nên việc tái sử dụng các mạch này hợp lý hơn là xây dựng hẳn một mạch trừ riêng.  
Hãy nhớ rằng một **procedure** (thủ tục) đơn giản để phủ định một số nhị phân là **[flip the bits and add one](signed.html#_negation)** (đảo tất cả các bit rồi cộng thêm 1).

Xét ví dụ \(0b0111\) (7) − \(0b0011\) (3).  
Bước đầu tiên là đưa số 3 vào mạch đảo bit.  
Để thực hiện phần “+1”, ta tận dụng **carry in** (số nhớ vào) của mạch cộng.  
Tức là, thay vì số nhớ được truyền từ chữ số này sang chữ số khác, phép trừ sẽ đưa một **carry in** vào *d~0~* của mạch cộng.  
Đặt **carry in** bằng 1 sẽ tăng giá trị ở hàng đơn vị thêm một đơn vị — chính xác là phần “+1” cần thiết trong phép phủ định.  
Kết hợp tất cả lại, ví dụ sẽ như sau:

> ```
>      1 (carry in)         1 (carry in)
>   0111                 0111
> - 0011               + 1100 (bits flipped)
> 
>   Result:   0100
>   Carry out: 1
> ```

Mặc dù kết quả đầy đủ của phép cộng này tạo ra một bit nhớ sang chữ số ngoài cùng bên trái, nhưng khi bỏ bớt (truncate) bit đó, ta thu được \(0b0100\), đúng với kết quả mong đợi là 4.  
Không giống như ví dụ phép cộng trước, **carry out** từ bit có trọng số cao nhất trong phép trừ **không nhất thiết** là dấu hiệu của lỗi **overflow** (tràn số).

Thực hiện phép trừ bằng cách phủ định rồi cộng cũng hoạt động khi trừ một số âm.  
Ví dụ, \(7 - (-3)\) cho kết quả 10:

> ```
>      1 (carry in)         1 (carry in)
>   0111                 0111
> - 1101               + 0010 (bits flipped)
> 
>   Result:   1010
>   Carry out: 0
> ```

Chúng ta sẽ tìm hiểu sâu hơn về ý nghĩa của việc có hoặc không có **carry out** trong [phần overflow](overflow.html#_integer_overflow).
