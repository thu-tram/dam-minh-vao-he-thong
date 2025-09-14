### 4.4.3. Phép nhân và phép chia (Multiplication and Division)

Phần này mô tả ngắn gọn về phép nhân và phép chia nhị phân với số nguyên.  
Cụ thể, nó trình bày các phương pháp tính toán bằng tay và **không** phản ánh hành vi của phần cứng hiện đại.  
Mô tả này không nhằm mục đích bao quát toàn bộ, vì phần còn lại của chương chủ yếu tập trung vào phép cộng và phép trừ.

#### Phép nhân (Multiplication)

Để nhân các số nhị phân, ta sử dụng chiến lược quen thuộc khi làm trên giấy: xét từng chữ số một và cộng các kết quả lại.  
Ví dụ, nhân \(0b0101\) (5) và \(0b0011\) (3) tương đương với việc cộng:

- Kết quả của việc nhân **d~0~** với \(0b0101\) (5): \(0b0101\) (5)  
- Kết quả của việc nhân **d~1~** với \(0b0101\) (5) và dịch trái kết quả một chữ số: \(0b1010\) (10)

> ```
>   0101       0101       0101
> x 0011  =  x    1  +  x   10  =  101 + 1010  =  1111 (15)
> ```

#### Phép chia số nguyên ((Integer) Division)

Không giống như các phép toán vừa mô tả ở trên, phép chia có khả năng tạo ra kết quả **không nguyên**.  
Điều quan trọng nhất cần nhớ khi chia số nguyên là trong hầu hết các ngôn ngữ lập trình (ví dụ: C, Python 2, và Java), phần thập phân của kết quả sẽ bị **truncation** (cắt bỏ).  
Ngoài ra, phép chia nhị phân sử dụng cùng phương pháp chia dài (long division) mà hầu hết học sinh được học ở tiểu học.  
Ví dụ, dưới đây là cách tính \(11 / 3\) cho kết quả nguyên bằng 3:

> ```
>     ____ 
> 11 |1011
> 
>     00__   11 (3) không chia hết cho 1 (1) hoặc 10 (2),
> 11 |1011   nên hai chữ số đầu tiên của kết quả là 00.
> 
>     001_   11 (3) chia vào 101 (5) được một lần.
> 11 |1011
> 
>     101    101 (5) - 11 (3) còn lại 10 (2).
>   -  11
>      10
> 
>     0011
> 11 |1011   11 (3) chia vào 101 (5) được một lần nữa.
>      101
> ```

Tại thời điểm này, phép toán đã cho ra kết quả nguyên mong đợi là \(0b0011\) (3), và phần cứng sẽ bỏ qua (truncate) bất kỳ phần thập phân nào.  
Nếu bạn muốn xác định **phần dư nguyên** (integral remainder), hãy sử dụng toán tử **modulus** (%);  
ví dụ: \(11 \% 3 = 2\).
