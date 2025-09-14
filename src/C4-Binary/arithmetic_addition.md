# 4.4.1. Phép cộng (Addition)

Hãy nhớ rằng trong một số nhị phân, mỗi chữ số chỉ có thể là 0 hoặc 1.  
Do đó, khi cộng hai bit mà *cả hai* đều bằng 1, kết quả sẽ **carry out** (tạo số nhớ) sang chữ số kế tiếp (ví dụ: \(1 + 1 = 0b10\), cần hai bit để biểu diễn).  
Trong thực tế, các chương trình cộng các biến nhiều bit, trong đó kết quả **carry out** của một chữ số sẽ ảnh hưởng đến chữ số tiếp theo thông qua **carry in** (số nhớ vào).

Nói chung, khi cộng các chữ số từ hai số nhị phân (*A* và *B*), sẽ có **tám** khả năng xảy ra tùy thuộc vào giá trị của *Digit~A~*, *Digit~B~*, và **Carry~in~** từ chữ số trước.  
Bảng 1 liệt kê tám khả năng có thể xảy ra khi cộng một cặp bit. Cột **Carry~in~** chỉ số nhớ được đưa vào từ chữ số trước, và cột **Carry~out~** cho biết việc cộng cặp bit này có tạo số nhớ sang chữ số tiếp theo hay không.

| Digit~A~ | Digit~B~ | Carry~in~ | Result (Sum) | Carry~out~ |
|----------|----------|-----------|--------------|------------|
| 0        | 0        | 0         | 0            | 0          |
| 0        | 0        | 1         | 1            | 0          |
| 0        | 1        | 0         | 1            | 0          |
| 0        | 1        | 1         | 0            | 1          |
| 1        | 0        | 0         | 1            | 0          |
| 1        | 0        | 1         | 0            | 1          |
| 1        | 1        | 0         | 0            | 1          |
| 1        | 1        | 1         | 1            | 1          |

**Bảng 1.** Tám khả năng khi cộng hai bit (*A* và *B*) với khả năng có số nhớ từ chữ số trước.

Xét ví dụ cộng hai số nhị phân 4-bit.  
Bắt đầu bằng cách viết thẳng hàng các chữ số sao cho các bit tương ứng nằm cùng cột, sau đó cộng từng cặp bit theo thứ tự từ bit có trọng số thấp nhất (*d~0~*) đến bit có trọng số cao nhất (*d~3~*).  
Ví dụ, cộng \(0b0010 + 0b1011\):

> ```
>      1    <- Carry 1 từ digit 1 sang digit 2
>   0010
> + 1011
> --------
>   1101
> ```

Ví dụ trên cho thấy có một số nhớ 1 từ *d~1~* sang *d~2~*.  
Tình huống này tương tự như khi cộng hai chữ số thập phân mà tổng lớn hơn 9.  
Ví dụ: \(5 + 8 = 13\), chữ số hàng đơn vị là 3 và số 1 được nhớ sang hàng chục.

Toán hạng thứ nhất (\(0b0010\)) có bit đầu tiên là 0, nên biểu diễn giá trị 2 trong cả hai cách diễn giải: **two’s complement** (bù hai) và **unsigned** (không dấu).  
Toán hạng thứ hai (\(0b1011\)) biểu diễn giá trị -5 nếu hiểu theo **signed two’s complement**, hoặc 11 nếu hiểu theo **unsigned**.  
May mắn là cách diễn giải không ảnh hưởng đến các bước tính toán.  
Kết quả (\(0b1101\)) có thể hiểu là 13 (unsigned: \(2 + 11\)) hoặc -3 (signed: \(2 + (-5)\)), cả hai đều đúng tùy theo cách diễn giải toán hạng thứ hai.

Nói chung, một dãy 4-bit biểu diễn giá trị trong khoảng \([0, 15]\) nếu hiểu là **unsigned**, và \([-8, 7]\) nếu hiểu là **signed**.  
Trong ví dụ trước, kết quả nằm trong khoảng biểu diễn được, nhưng không phải lúc nào cũng vậy.  
Ví dụ, khi cộng \(0b1100\) (unsigned 12) + \(0b0111\) (7), kết quả đúng phải là 19, nhưng 4-bit không thể biểu diễn 19:

> ```
>    11     <- Carry 1 từ digit 2 sang digit 3, và digit 3 tràn ra ngoài giá trị 4-bit
>  1100
> +0111
> -----
>  0011
> Carry out: 1
> ```

Lưu ý rằng phép cộng này tạo ra số nhớ 1 từ bit có trọng số cao nhất — đây được gọi là **carry out** của toàn bộ phép toán.  
Trong ví dụ này, carry out cho thấy kết quả cần thêm một bit để lưu trữ.  
Tuy nhiên, khi thực hiện phép cộng 4-bit, phần cứng sẽ bỏ (truncate) bit nhớ này, để lại kết quả \(0b0011\).  
Nếu mục tiêu là cộng \(12 + 7\), kết quả 3 chắc chắn gây bất ngờ.  
Sự bất ngờ này là do **overflow** (tràn số).  
Chúng ta sẽ tìm hiểu cách phát hiện overflow và nguyên nhân gây ra nó trong [phần sau](overflow.html#_integer_overflow).

> Các mạch cộng nhiều bit (multibit adder circuits) cũng hỗ trợ **carry in**, hoạt động như số nhớ vào bit ngoài cùng bên phải (tức là đầu vào *Carry~in~* cho *d~0~*).  
> Trong phép cộng, carry in thường được đặt ngầm định bằng 0, nên không xuất hiện trong ví dụ trước.  
> Tuy nhiên, carry in trở nên quan trọng trong các phép toán khác sử dụng mạch cộng, đặc biệt là [subtraction](arithmetic_subtraction.html#_subtraction).