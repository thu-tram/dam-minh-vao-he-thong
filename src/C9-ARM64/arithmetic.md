## 9.3. Các lệnh số học (Arithmetic Instructions) 

### 9.3.1. Các lệnh số học thông dụng (Common Arithmetic Instructions) 

**A64 ISA** (Instruction Set Architecture — “kiến trúc tập lệnh”) triển khai một số lệnh tương ứng với các phép toán số học được thực hiện bởi **ALU** (Arithmetic Logic Unit — “bộ số học và logic”). [Bảng 1](#OtherArithmetica64) liệt kê một số lệnh số học thường gặp khi đọc code Assembly của ARM.

| Instruction | Translation |
| --- | --- |
| `add D, O1, O2` | D = O1 + O2 |
| `sub D, O1, O2` | D = O1 - O2 |
| `neg D, O1` | D = -(O1) |

**Bảng 1.** Các lệnh số học thông dụng

Lệnh `add` và `sub` tương ứng với phép cộng và phép trừ, yêu cầu hai toán hạng ngoài thanh ghi đích. Ngược lại, lệnh `neg` chỉ yêu cầu một toán hạng ngoài thanh ghi đích.

Ba lệnh trong Bảng 1 cũng có các dạng *carry* cho phép lệnh sử dụng cờ điều kiện **carry** tùy chọn, `C`. Cờ carry là một bit được đặt khi một phép toán không dấu bị tràn. Chúng ta sẽ đề cập đến các cờ điều kiện khác ở phần tiếp theo, nhưng ở đây mô tả cờ carry để giới thiệu các lệnh số học bổ sung. Các dạng carry và bản dịch tương ứng được thể hiện trong Bảng 2.

| Instruction | Translation |
| --- | --- |
| `adc D, O1, O2` | D = O1 + O2 + `C` |
| `sbc D, O1, O2` | D = O1 - O2 - `~C` |
| `ngc D, O1` | D = -(O1) - `~C` |

**Bảng 2.** Các dạng carry của các lệnh số học thông dụng

Các lệnh trên cũng có hậu tố tùy chọn `s`. Khi hậu tố `s` được sử dụng (ví dụ: `adds`), nó cho biết phép toán số học sẽ thiết lập các cờ điều kiện.

#### Phép nhân và phép chia (Multiplication and Division) 

| Instruction | Translation |
| --- | --- |
| `mul D, O1, O2` | D = O1 × O2 |
| `udiv D, O1, O2` | D = O1 / O2 (32-bit unsigned) |
| `sdiv D, O1, O2` | D = O1 / O2 (64-bit signed) |

**Bảng 3.** Các lệnh nhân và chia thông dụng

Các lệnh nhân và chia thông dụng được thể hiện trong Bảng 3. Lệnh `mul` hoạt động trên hai toán hạng và đặt tích vào thanh ghi đích D. Phép chia **không** có dạng tổng quát; `udiv` và `sdiv` lần lượt hoạt động trên dữ liệu 32-bit và 64-bit. Lưu ý rằng bạn không thể nhân thanh ghi 32-bit với thanh ghi 64-bit.

Ngoài ra, **ARMv8-A** cung cấp các dạng nhân hợp thành (composite forms), cho phép CPU thực hiện các phép toán phức tạp hơn trong một lệnh duy nhất. Các lệnh này được thể hiện trong [Bảng 4](#CompositeMultiply).

| Instruction | Translation |
| --- | --- |
| `madd D, O1, O2, O3` | D = O3 + (O1 × O2) |
| `msub D, O1, O2, O3` | D = O3 - (O1 × O2) |
| `mneg D, O1, O2` | D = -(O1 × O2) |

**Bảng 4.** Các lệnh nhân hợp thành

### 9.3.2. Các lệnh dịch bit (Bit Shifting Instructions) 

Các lệnh dịch bit cho phép **compiler** (trình biên dịch) thực hiện các phép dịch bit. Các lệnh nhân và chia thường mất nhiều thời gian để thực thi. Dịch bit mang lại cho compiler một cách tối ưu hơn khi nhân hoặc chia với các số là lũy thừa của 2. Ví dụ, để tính `77 * 4`, hầu hết compiler sẽ dịch phép toán này thành `77 << 2` để tránh sử dụng lệnh `mul`. Tương tự, để tính `77 / 4`, compiler thường dịch thành `77 >> 2` để tránh dùng lệnh `sdiv`.

Cần lưu ý rằng dịch bit sang trái và sang phải sẽ được dịch thành các lệnh khác nhau tùy thuộc vào mục tiêu là dịch số học (signed) hay dịch logic (unsigned).

| Instruction | Translation | Arithmetic or Logical? |
| --- | --- | --- |
| `lsl D, R, #v` | D = R `<<` v | logical hoặc arithmetic |
| `lsr D, R, #v` | D = R `>>` v | logical |
| `asr D, R, #v` | D = R `>>` v | arithmetic |
| `ror D, R, #v` | D = R `>>>` v | neither (rotate) |

**Bảng 5.** Các lệnh dịch bit

Ngoài thanh ghi đích, mỗi lệnh dịch bit nhận hai toán hạng; một thường là thanh ghi (ký hiệu R) và một là giá trị dịch 6-bit (v). Trên hệ thống 64-bit, giá trị dịch được code hóa thành một byte (vì không có ý nghĩa khi dịch quá 63 bit). Giá trị dịch v phải là hằng số hoặc được lưu trong một thanh ghi thành phần.

Lệnh dịch bit cuối cùng, `ror`, cần được thảo luận riêng. Lệnh `ror` *xoay* các bit, thay thế các bit có trọng số lớn nhất bằng các bit có trọng số nhỏ nhất. Chúng ta ký hiệu phép xoay bit này bằng ký hiệu `>>>`.


> **Các phiên bản khác nhau của lệnh giúp chúng ta phân biệt kiểu dữ liệu ở mức assembly**  
>  
> Ở mức assembly, không tồn tại khái niệm *type* (kiểu dữ liệu). Tuy nhiên, hãy nhớ rằng **compiler** (trình biên dịch) có thể chọn sử dụng các *component register* (thanh ghi thành phần) dựa trên kiểu dữ liệu xuất hiện ở mức mã nguồn. Tương tự, hãy nhớ rằng phép dịch phải (*shift right*) hoạt động khác nhau tùy thuộc vào việc giá trị là **signed** (có dấu) hay **unsigned** (không dấu). Ở mức assembly, compiler sử dụng các lệnh riêng biệt để phân biệt giữa dịch logic (*logical shift*) và dịch số học (*arithmetic shift*).

### 9.3.3. Các lệnh thao tác bit (Bitwise Instructions) 

**Bitwise instruction** cho phép compiler thực hiện các phép toán bit trên dữ liệu. Một cách mà compiler sử dụng phép toán bit là để tối ưu hóa trong một số trường hợp. Ví dụ, compiler có thể chọn thực hiện `77 mod 4` bằng phép toán `77 & 3` thay vì sử dụng lệnh `sdiv` vốn tốn kém hơn.

Bảng 6 liệt kê các lệnh bitwise thông dụng và các lệnh bitwise hợp thành (*composite*) có sử dụng phép phủ định (*negation*).

| Instruction | Translation |
| --- | --- |
| `and D, O1, O2` | D = O1 `&` O2 |
| `orr D, O1, O2` | D = O1 `|` O2 |
| `eor D, O1, O2` | D = O1 `^` O2 |
| `mvn D, O` | D = `~`O |
| `bic D, O1, O2` | D = O1 `&` `~`O2 |
| `orn D, O1, O2` | D = O1 `|` `~`O2 |
| `eon D, O1, O2` | D = O1 `^` `~`O2 |

**Bảng 6.** Các phép toán bitwise

Hãy nhớ rằng **bitwise not** khác với phép **negation** (`neg`). Lệnh `mvn` đảo tất cả các bit của toán hạng nhưng **không** cộng thêm 1. Cần cẩn thận để không nhầm lẫn hai lệnh này.

> **Chỉ sử dụng phép toán bitwise khi thực sự cần trong code C của bạn!**  
>  
> Sau khi đọc phần này, bạn có thể sẽ muốn thay thế các phép toán số học thông thường trong code C của mình bằng các phép dịch bit hoặc các phép toán bit khác. Điều này **không** được khuyến khích. Hầu hết các compiler hiện đại đủ thông minh để thay thế các phép toán số học đơn giản bằng các phép toán bit khi điều đó hợp lý, vì vậy lập trình viên không cần phải tự làm điều đó. Nguyên tắc chung là lập trình viên nên ưu tiên **tính dễ đọc của code** bất cứ khi nào có thể và tránh tối ưu hóa sớm (*premature optimization*).
