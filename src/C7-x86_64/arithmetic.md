
## 7.3. Các lệnh số học (Arithmetic Instructions)

**x86 ISA** (Instruction Set Architecture — kiến trúc tập lệnh) triển khai một số **instructions** (lệnh) tương ứng với các phép toán số học được thực hiện bởi **ALU** (Arithmetic Logic Unit — đơn vị số học và logic). **Bảng 1** liệt kê một số lệnh số học thường gặp khi đọc mã assembly.

| Instruction  | Translation         |
|--------------|---------------------|
| `add S, D`   | S + D → D           |
| `sub S, D`   | D - S → D           |
| `inc D`      | D + 1 → D           |
| `dec D`      | D - 1 → D           |
| `neg D`      | -D → D              |
| `imul S, D`  | S × D → D           |
| `idiv S`     | `%rax` / S: quotient → `%rax`, remainder → `%rdx` |

**Bảng 1.** Các lệnh số học phổ biến.

Lệnh `add` và `sub` tương ứng với phép cộng và phép trừ, mỗi lệnh nhận hai **operand** (toán hạng). Ba lệnh tiếp theo là các lệnh thao tác trên một thanh ghi duy nhất, tương ứng với các phép **increment** (`x++`), **decrement** (`x--`) và **negation** (`-x`) trong C. Lệnh nhân (`imul`) hoạt động trên hai toán hạng và đặt kết quả vào toán hạng đích. Nếu kết quả cần nhiều hơn 64 bit để biểu diễn, giá trị sẽ bị cắt ngắn xuống 64 bit.

Lệnh chia (`idiv`) hoạt động hơi khác. Trước khi thực thi `idiv`, giả định rằng thanh ghi `%rax` chứa **dividend** (số bị chia). Gọi `idiv` với toán hạng S sẽ chia nội dung của `%rax` cho S, đặt **quotient** (thương) vào `%rax` và **remainder** (phần dư) vào `%rdx`.

### 7.3.1. Các lệnh dịch bit (Bit Shifting Instructions)

Các lệnh dịch bit cho phép **compiler** thực hiện các phép dịch bit. Các lệnh nhân và chia thường mất nhiều thời gian để thực thi. Dịch bit mang lại cho compiler một cách rút gọn khi nhân hoặc chia với các số là lũy thừa của 2.  

Ví dụ: để tính `77 * 4`, hầu hết compiler sẽ dịch phép toán này thành `77 << 2` để tránh dùng lệnh `imul`. Tương tự, để tính `77 / 4`, compiler thường dịch thành `77 >> 2` để tránh dùng `idiv`.

Cần lưu ý rằng dịch trái và dịch phải sẽ được dịch sang các lệnh khác nhau tùy thuộc vào mục tiêu là **arithmetic shift** (dịch số học, có dấu) hay **logical shift** (dịch logic, không dấu).

| Instruction  | Translation   | Arithmetic or Logical? |
|--------------|--------------|------------------------|
| `sal v, D`   | D `<<` v → D | arithmetic             |
| `shl v, D`   | D `<<` v → D | logical                |
| `sar v, D`   | D `>>` v → D | arithmetic             |
| `shr v, D`   | D `>>` v → D | logical                |

**Bảng 2.** Các lệnh dịch bit.

Mỗi lệnh dịch nhận hai toán hạng: một thường là **register** (thanh ghi, ký hiệu D) và một là giá trị dịch (*v*). Trên hệ thống 64-bit, giá trị dịch được mã hóa trong một byte (vì không hợp lý khi dịch quá 63 bit). Giá trị dịch *v* phải là hằng số hoặc được lưu trong thanh ghi `%cl`.

> **Different Versions of Instructions Help Distinguish Types at an Assembly Level**  
> Ở mức assembly, không tồn tại khái niệm kiểu dữ liệu. Tuy nhiên, hãy nhớ rằng compiler sẽ sử dụng các **component register** dựa trên kiểu dữ liệu. Tương tự, hãy nhớ rằng dịch phải hoạt động khác nhau tùy thuộc vào việc giá trị là signed hay unsigned. Ở mức assembly, compiler sử dụng các lệnh khác nhau để phân biệt giữa dịch logic và dịch số học.

### 7.3.2. Các lệnh thao tác bit (Bitwise Instructions)

Các lệnh thao tác bit cho phép compiler thực hiện các phép toán bit trên dữ liệu. Một cách compiler sử dụng các phép toán bit là để tối ưu hóa. Ví dụ, compiler có thể chọn triển khai `77 mod 4` bằng phép `77 & 3` thay vì dùng lệnh `idiv` vốn tốn kém hơn.

**Bảng 3** liệt kê các lệnh bitwise phổ biến.

| Instruction  | Translation  |
|--------------|--------------|
| `and S, D`   | S `&` D → D  |
| `or S, D`    | S `|` D → D  |
| `xor S, D`   | S `^` D → D  |
| `not D`      | `~`D → D     |

**Bảng 3.** Các phép toán bitwise.

Hãy nhớ rằng phép bitwise `not` khác với phép **negation** (`neg`). Lệnh `not` đảo tất cả các bit nhưng không cộng thêm 1. Cần cẩn thận để không nhầm lẫn hai lệnh này.

> **Chỉ sử dụng các phép toán bitwise khi thật sự cần thiết trong mã C của bạn!**  
> Sau khi đọc xong phần này, bạn có thể sẽ bị cám dỗ muốn thay thế các phép toán số học thông thường trong mã C của mình bằng các phép dịch bit hoặc các phép toán bitwise khác. Điều này *không* được khuyến khích. Hầu hết các **compiler** (trình biên dịch) hiện đại đủ thông minh để thay thế các phép toán số học đơn giản bằng các phép toán bitwise khi điều đó hợp lý, khiến lập trình viên không cần phải tự làm điều này. Nguyên tắc chung là lập trình viên nên ưu tiên khả năng dễ đọc của mã nguồn bất cứ khi nào có thể và tránh tối ưu hóa sớm một cách không cần thiết.

### 7.3.3. Lệnh Load Effective Address

*What's lea got to do (got to do) with it?*  

*What's lea, but an effective address loading?*  

~ Xin lỗi Tina Turner

Chúng ta cuối cùng cũng đến với **load effective address** hay lệnh `lea`, có lẽ là lệnh số học khiến sinh viên bối rối nhất. Lệnh này truyền thống được dùng như một cách nhanh để tính địa chỉ của một vị trí trong bộ nhớ. Lệnh `lea` hoạt động trên cùng cấu trúc **operand** (toán hạng) mà chúng ta đã thấy cho đến giờ, nhưng *không* bao gồm việc truy xuất bộ nhớ. Bất kể kiểu dữ liệu chứa trong toán hạng là gì (dù là một hằng số hay một địa chỉ), `lea` đơn giản chỉ thực hiện phép toán số học.

Ví dụ: giả sử thanh ghi `%rax` chứa giá trị hằng 0x5, thanh ghi `%rdx` chứa giá trị hằng 0x4, và thanh ghi `%rcx` chứa giá trị 0x808 (tình cờ đây là một địa chỉ). [Bảng 4](#leaEx) minh họa một số thao tác `lea` ví dụ, bản dịch của chúng và giá trị tương ứng.

| Instruction              | Translation               | Value           |
|--------------------------|---------------------------|-----------------|
| `lea 8(%rax), %rax`      | 8 + `%rax` → `%rax`       | 13 → `%rax`     |
| `lea (%rax, %rdx), %rax` | `%rax` + `%rdx` → `%rax`  | 9 → `%rax`      |
| `lea (,%rax,4), %rax`    | `%rax` × 4 → `%rax`       | 20 → `%rax`     |
| `lea -0x8(%rcx), %rax`   | `%rcx` - 8 → `%rax`       | 0x800 → `%rax`  |
| `lea -0x4(%rcx, %rdx, 2), %rax` | `%rcx` + `%rdx` × 2 - 4 → `%rax` | 0x80c → `%rax` |

**Bảng 4.** Ví dụ về các thao tác `lea`.

Trong tất cả các trường hợp, lệnh `lea` thực hiện phép toán số học trên toán hạng nguồn S và đặt kết quả vào toán hạng đích D. Lệnh `mov` giống hệt `lea` *ngoại trừ* việc `mov` *bắt buộc* phải coi nội dung trong toán hạng nguồn là một vị trí bộ nhớ nếu nó ở dạng địa chỉ bộ nhớ. Ngược lại, `lea` thực hiện cùng phép toán (đôi khi phức tạp) trên toán hạng *mà không* truy xuất bộ nhớ, cho phép compiler khéo léo sử dụng `lea` như một sự thay thế cho một số loại phép toán số học.

