## 8.3. Arithmetic Instructions (Các lệnh số học)

**IA32 ISA** triển khai một số lệnh tương ứng với các phép toán số học được thực hiện bởi **ALU**. [Bảng 1](#OtherArithmetic32) liệt kê một số lệnh số học thường gặp khi đọc mã assembly.

| Instruction  | Translation |
|--------------|-------------|
| `add S, D`   | S + D → D |
| `sub S, D`   | D - S → D |
| `inc D`      | D + 1 → D |
| `dec D`      | D - 1 → D |
| `neg D`      | -D → D |
| `imul S, D`  | S × D → D |
| `idiv S`     | `%eax` / S : Q → `%eax`, R → `%edx` |

**Bảng 1.** Các lệnh số học thông dụng.

- Lệnh `add` và `sub` tương ứng với phép cộng và trừ, mỗi lệnh nhận hai toán hạng.  
- Ba lệnh tiếp theo là các lệnh thao tác trên một thanh ghi duy nhất, tương ứng với các phép tăng (`x++`), giảm (`x--`) và đổi dấu (`-x`) trong C.  
- Lệnh nhân (`imul`) hoạt động trên hai toán hạng và đặt kết quả vào toán hạng đích. Nếu kết quả cần nhiều hơn 32 bit để biểu diễn, giá trị sẽ bị cắt (truncate) xuống 32 bit.  

Lệnh chia (`idiv`) hoạt động hơi khác: trước khi thực thi `idiv`, giả định rằng thanh ghi `%eax` chứa số bị chia (dividend). Gọi `idiv` với toán hạng S sẽ chia nội dung `%eax` cho S, đưa thương số (quotient) vào `%eax` và số dư (remainder) vào `%edx`.

---

### 8.3.1. Bit Shifting Instructions (Các lệnh dịch bit)

Các lệnh dịch bit cho phép compiler thực hiện các phép dịch bit.  
Các lệnh nhân và chia thường tốn nhiều thời gian để thực thi.  
Dịch bit cung cấp cho compiler một cách tối ưu hơn khi nhân hoặc chia cho các số là lũy thừa của 2.  

Ví dụ: để tính `77 * 4`, hầu hết compiler sẽ dịch thành `77 << 2` để tránh dùng `imul`.  
Tương tự, để tính `77 / 4`, compiler thường dịch thành `77 >> 2` để tránh dùng `idiv`.

Cần lưu ý rằng dịch trái và dịch phải sẽ được dịch sang các lệnh khác nhau tùy thuộc vào mục tiêu là dịch số học (arithmetic – có dấu) hay dịch logic (logical – không dấu).

| Instruction | Translation | Arithmetic or Logical? |
|-------------|-------------|------------------------|
| `sal v, D`  | D `<<` v → D | arithmetic |
| `shl v, D`  | D `<<` v → D | logical |
| `sar v, D`  | D `>>` v → D | arithmetic |
| `shr v, D`  | D `>>` v → D | logical |

**Bảng 2.** Các lệnh dịch bit.

Mỗi lệnh dịch nhận hai toán hạng:  
- Một toán hạng thường là thanh ghi (ký hiệu D).  
- Toán hạng còn lại là giá trị dịch (*v*).  

Trên hệ thống 32-bit, giá trị dịch được mã hóa trong 1 byte (vì không có ý nghĩa khi dịch quá 31 bit).  
Giá trị dịch *v* phải là một hằng số hoặc được lưu trong thanh ghi `%cl`.

---

**Ghi chú:**  
Ở cấp độ assembly, không tồn tại khái niệm kiểu dữ liệu. Tuy nhiên, hãy nhớ rằng dịch phải hoạt động khác nhau tùy thuộc vào việc giá trị là số có dấu hay không dấu.  
Compiler sử dụng các lệnh khác nhau để phân biệt giữa dịch logic và dịch số học.

---

### 8.3.2. Bitwise Instructions (Các lệnh thao tác bit)

Các lệnh thao tác bit cho phép compiler thực hiện các phép toán bitwise trên dữ liệu.  
Một cách compiler sử dụng các phép toán bitwise là để tối ưu hóa.  
Ví dụ: compiler có thể chọn thực hiện `77 mod 4` bằng `77 & 3` thay vì dùng lệnh `idiv` tốn kém hơn.

**Bảng 3** liệt kê các lệnh bitwise thông dụng:

| Instruction | Translation |
|-------------|-------------|
| `and S, D`  | S `&` D → D |
| `or S, D`   | S `|` D → D |
| `xor S, D`  | S `^` D → D |
| `not D`     | `~`D → D |

**Bảng 3.** Các phép toán bitwise.

Lưu ý: **bitwise `not`** khác với **negation (`neg`)**.  
- `not` đảo tất cả các bit nhưng **không** cộng thêm 1.  
- `neg` đổi dấu số (two’s complement), tức là đảo bit rồi cộng thêm 1.  
Cần cẩn thận để không nhầm lẫn hai lệnh này.

>> **Chỉ sử dụng các phép toán bitwise khi thật sự cần thiết trong code C của bạn!**
>> 
>> Sau khi đọc xong phần này, bạn có thể sẽ bị cám dỗ muốn thay thế các phép toán số học thông thường trong code C của mình bằng các phép dịch bit hoặc các phép toán bitwise khác. Điều này **không** được khuyến khích. Hầu hết các compiler hiện đại đủ thông minh để tự thay thế các phép toán số học đơn giản bằng các phép toán bitwise khi thích hợp, vì vậy lập trình viên không cần phải làm điều đó. Nguyên tắc chung là lập trình viên nên ưu tiên khả năng dễ đọc của code bất cứ khi nào có thể và tránh tối ưu hóa quá sớm. |

---

### 8.3.3. Lệnh Load Effective Address

*What's lea got to do (got to do) with it?*  
*What's lea, but an effective address loading?*  
\~ Xin lỗi Tina Turner

Chúng ta cuối cùng cũng đến với **load effective address** hay lệnh `lea`, có lẽ là lệnh số học khiến sinh viên bối rối nhiều nhất. Lệnh này truyền thống được dùng như một cách nhanh để tính toán địa chỉ của một vị trí trong bộ nhớ. Lệnh `lea` hoạt động trên cùng cấu trúc toán hạng mà ta đã thấy từ trước đến giờ nhưng **không** bao gồm việc truy xuất bộ nhớ. Bất kể toán hạng chứa loại dữ liệu gì (dù là hằng số hay địa chỉ), `lea` chỉ đơn giản thực hiện phép toán số học.

Ví dụ: giả sử thanh ghi `%eax` chứa giá trị hằng 0x5, thanh ghi `%edx` chứa giá trị hằng 0x4, và thanh ghi `%ecx` chứa giá trị 0x808 (thực tế là một địa chỉ). [Bảng 4](#leaEx32) đưa ra một số ví dụ về các phép `lea`, bản dịch và giá trị tương ứng.

| Instruction | Translation | Value |
|-------------|-------------|-------|
| `lea 8(%eax), %eax` | 8 + `%eax` → `%eax` | 13 → `%eax` |
| `lea (%eax, %edx), %eax` | `%eax` + `%edx` → `%eax` | 9 → `%eax` |
| `lea (,%eax,4), %eax` | `%eax` × 4 → `%eax` | 20 → `%eax` |
| `lea -0x8(%ecx), %eax` | `%ecx` - 8 → `%eax` | 0x800 → `%eax` |
| `lea -0x4(%ecx, %edx, 2), %eax` | `%ecx` + `%edx` × 2 - 4 → `%eax` | 0x80c → `%eax` |

**Bảng 4.** Ví dụ về các phép `lea`.

Trong tất cả các trường hợp, lệnh `lea` thực hiện phép toán số học trên toán hạng nguồn S và đặt kết quả vào toán hạng đích D. Lệnh `mov` giống hệt `lea` **ngoại trừ** việc `mov` **bắt buộc** phải coi nội dung của toán hạng nguồn là một địa chỉ bộ nhớ nếu nó ở dạng địa chỉ bộ nhớ. Ngược lại, `lea` thực hiện cùng phép toán (đôi khi phức tạp) trên toán hạng **mà không** truy xuất bộ nhớ, cho phép compiler khéo léo dùng `lea` như một phép thay thế cho một số loại phép toán số học.
