Ok Khánh 👍  

## 4.2. Chuyển đổi giữa các hệ cơ số (Converting Between Bases)

Bạn sẽ thường xuyên gặp cả ba hệ cơ số mà chúng ta đã giới thiệu trong chương này ở nhiều ngữ cảnh khác nhau.  
Trong một số trường hợp, bạn sẽ cần chuyển đổi từ một hệ cơ số sang hệ khác.  
Phần này bắt đầu bằng việc chỉ ra cách chuyển đổi giữa **binary** (nhị phân) và **hexadecimal** (thập lục phân), vì hai hệ này có mối liên hệ trực tiếp.  
Sau đó, chúng ta sẽ tìm hiểu cách chuyển đổi sang và từ **decimal** (thập phân).

### 4.2.1. Chuyển đổi giữa Binary và Hexadecimal

Vì cơ số của cả binary và hexadecimal đều là lũy thừa của 2, việc chuyển đổi giữa chúng khá đơn giản.  
Cụ thể, mỗi chữ số hexadecimal biểu diễn một trong 16 giá trị khác nhau, và bốn bit cũng biểu diễn \(2^4 = 16\) giá trị khác nhau, nên chúng có khả năng biểu diễn tương đương.  
Bảng 1 liệt kê ánh xạ một-một giữa mọi nhóm 4 bit và một chữ số hexadecimal.

| Binary | Hexadecimal |   | Binary | Hexadecimal |
|--------|-------------|---|--------|-------------|
| 0000   | 0           |   | 1000   | 8           |
| 0001   | 1           |   | 1001   | 9           |
| 0010   | 2           |   | 1010   | A           |
| 0011   | 3           |   | 1011   | B           |
| 0100   | 4           |   | 1100   | C           |
| 0101   | 5           |   | 1101   | D           |
| 0110   | 6           |   | 1110   | E           |
| 0111   | 7           |   | 1111   | F           |

**Bảng 1.** Ánh xạ giữa tất cả các nhóm 4 bit và một chữ số hexadecimal.

Lưu ý rằng nội dung của Bảng 1 chỉ đơn giản là đếm từ 0 đến 15 ở cả hai hệ cơ số, nên bạn không cần phải ghi nhớ.  
Dựa vào bảng này, bạn có thể chuyển đổi bất kỳ số lượng bit hoặc chữ số hex liên tiếp nào theo cả hai hướng:

- **Ví dụ 1:** Chuyển 0xB491 sang binary: thay thế từng chữ số hex bằng giá trị binary tương ứng.

> ```
>   B    4    9    1
> 1011 0100 1001 0001  ->  0b1011010010010001
> ```

- **Ví dụ 2:** Chuyển 0b1111011001 sang hexadecimal: chia các bit thành nhóm 4 từ **phải sang trái**.  
  Nếu nhóm bên trái không đủ 4 bit, thêm các số 0 ở đầu. Sau đó thay thế bằng giá trị hex tương ứng.

> ```
> 1111011001  ->  11 1101 1001  ->  0011 1101 1001
>                                     ^ padding
> 
> 0011 1101 1001
>   3    D    9  ->  0x3D9
> ```

### 4.2.2. Chuyển đổi sang Decimal

Thực tế, việc chuyển đổi sang decimal chính là những gì chúng ta đã làm trong [các phần trước](bases.html#_unsigned_binary_numbers).  
Với một số ở **bất kỳ** cơ số **B** nào, đánh số các chữ số từ **phải sang trái** là d~0~, d~1~, d~2~, … cho phép áp dụng công thức tổng quát:

> \((d_{N-1} \times B^{N-1}) + (d_{N-2} \times B^{N-2}) + \dots + (d_2 \times B^2) + (d_1 \times B^1) + (d_0 \times B^0)\)

### 4.2.3. Chuyển đổi từ Decimal

Chuyển đổi từ decimal sang hệ khác cần nhiều bước hơn.  
Về nguyên tắc, đây là quá trình ngược lại của công thức trên: xác định giá trị của từng chữ số sao cho tổng của chúng (theo vị trí) bằng số decimal ban đầu.  
Bạn có thể hình dung mỗi chữ số trong hệ đích giống như các hàng đơn vị, hàng chục, hàng trăm… trong hệ thập phân.

**Ví dụ:** Chuyển từ decimal sang hexadecimal.  
Mỗi chữ số hex tương ứng với một lũy thừa của 16.  
Bảng 2 liệt kê một số lũy thừa đầu tiên của 16.

| 16⁴   | 16³  | 16² | 16¹ | 16⁰ |
|-------|------|-----|-----|-----|
| 65536 | 4096 | 256 | 16  | 1   |

**Bảng 2.** Lũy thừa của 16.

**Ví dụ:** Chuyển **9742** sang hexadecimal:

1. **65536** không vừa trong 9742 → d~4~ = 0.  
   Các chữ số cao hơn cũng bằng 0.

2. **4096** vừa 2 lần → d~3~ = 2.  
   Còn lại: 9742 − 8192 = 1550.

3. **256** vừa 6 lần → d~2~ = 6.  
   Còn lại: 1550 − 1536 = 14.

4. **16** không vừa → d~1~ = 0.

5. **1** vừa 14 lần → d~0~ = E (14 trong hex).

Kết quả: **0x260E**.

#### Decimal sang Binary: Lũy thừa của 2

Quy trình tương tự áp dụng cho binary, chỉ cần dùng lũy thừa của 2.  
Bảng 3 liệt kê một số lũy thừa đầu tiên của 2.

| 2⁸  | 2⁷  | 2⁶ | 2⁵ | 2⁴ | 2³ | 2² | 2¹ | 2⁰ |
|-----|-----|----|----|----|----|----|----|----|
| 256 | 128 | 64 | 32 | 16 | 8  | 4  | 2  | 1  |

**Bảng 3.** Lũy thừa của 2.

**Ví dụ:** Chuyển **422** sang binary:

- 256 vừa → d~8~ = 1, còn 166.  
- 128 vừa → d~7~ = 1, còn 38.  
- 64 không vừa → d~6~ = 0.  
- 32 vừa → d~5~ = 1, còn 6.  
- 16 không vừa → d~4~ = 0.  
- 8 không vừa → d~3~ = 0.  
- 4 vừa → d~2~ = 1, còn 2.  
- 2 vừa → d~1~ = 1, còn 0.  
- 1 không vừa → d~0~ = 0.

Kết quả: **0b110100110**.

#### Decimal sang Binary: Chia liên tiếp

Một phương pháp khác không cần biết trước lũy thừa của 2:  
Liên tục chia số decimal cho 2 (lấy phần nguyên), mỗi lần ghi lại **0** nếu số chẵn, **1** nếu số lẻ.  
Các bit được tạo từ **phải sang trái**.  
Khi kết quả chia bằng 0, quá trình kết thúc.

**Ví dụ:** 422

- 422 chẵn → d~0~ = 0  
- 211 lẻ → d~1~ = 1  
- 105 lẻ → d~2~ = 1  
- 52 chẵn → d~3~ = 0  
- 26 chẵn → d~4~ = 0  
- 13 lẻ → d~5~ = 1  
- 6 chẵn → d~6~ = 0  
- 3 lẻ → d~7~ = 1  
- 1 lẻ → d~8~ = 1 → chia tiếp được 0 → dừng.

Kết quả: **0b110100110** (giống phương pháp trước).
