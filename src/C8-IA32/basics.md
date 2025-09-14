## 8.1. Bắt đầu với Assembly: Những điều cơ bản

Để có cái nhìn đầu tiên về assembly, chúng ta sẽ chỉnh sửa hàm `adder` từ đầu chương để đơn giản hóa hành vi của nó. Đây là phiên bản đã chỉnh sửa (`adder2`):

```c
#include <stdio.h>

//adds two to an integer and returns the result
int adder2(int a) {
    return a + 2;
}

int main(void) {
    int x = 40;
    x = adder2(x);
    printf("x is: %d\n", x);
    return 0;
}
```

Để biên dịch đoạn code này, sử dụng lệnh sau:

```
$ gcc -m32 -o modified modified.c
```

Cờ `-m32` yêu cầu GCC biên dịch code thành một file thực thi 32-bit. Nếu quên thêm cờ này, kết quả assembly có thể sẽ khác rất nhiều so với các ví dụ trong chương này; mặc định, GCC biên dịch sang assembly x86-64, biến thể 64-bit của x86. Tuy nhiên, hầu như tất cả các kiến trúc 64-bit đều có chế độ chạy 32-bit để tương thích ngược. Chương này đề cập đến IA32; các chương khác sẽ nói về x86-64 và ARM. Dù đã cũ, IA32 vẫn cực kỳ hữu ích để hiểu cách chương trình hoạt động và cách tối ưu hóa code.

Tiếp theo, hãy xem code assembly tương ứng của đoạn code này bằng cách gõ:

```
$ objdump -d modified > output
$ less output
```

Tìm đoạn code liên quan đến `adder2` bằng cách gõ `/adder2` khi đang xem file `output` với `less`. Phần liên quan đến `adder2` sẽ trông tương tự như sau:

**Kết quả assembly cho hàm `adder2`**:

```
0804840b <adder2>:
 804840b:       55                      push   %ebp
 804840c:       89 e5                   mov    %esp,%ebp
 804840e:       8b 45 08                mov    0x8(%ebp),%eax
 8048411:       83 c0 02                add    $0x2,%eax
 8048414:       5d                      pop    %ebp
 8048415:       c3                      ret
```

Đừng lo nếu bạn chưa hiểu chuyện gì đang diễn ra. Chúng ta sẽ tìm hiểu chi tiết hơn về assembly ở các phần sau. Hiện tại, chúng ta sẽ nghiên cứu cấu trúc của từng lệnh riêng lẻ.

Mỗi dòng trong ví dụ trên chứa:

- Địa chỉ của lệnh trong bộ nhớ chương trình
- Các byte tương ứng với lệnh
- Dạng văn bản (plaintext) của lệnh

Ví dụ: `55` là code máy (machine code) của lệnh `push %ebp`, và lệnh này nằm tại địa chỉ `0x804840b` trong bộ nhớ chương trình.

Điều quan trọng cần lưu ý là một dòng code C thường được dịch thành **nhiều** lệnh assembly.  
Ví dụ, phép toán `a + 2` được biểu diễn bằng hai lệnh:  
`mov 0x8(%ebp), %eax` và `add $0x2, %eax`.

>> **Assembly của bạn có thể trông khác!**
>>
>> Nếu bạn biên dịch code cùng với chúng tôi, bạn có thể nhận thấy một số đoạn assembly của mình trông khác so với trong sách. Các lệnh assembly chính xác mà compiler xuất ra phụ thuộc vào phiên bản compiler và hệ điều hành. Hầu hết các ví dụ assembly trong sách này được tạo trên hệ thống chạy Ubuntu hoặc Red Hat Enterprise Linux (RHEL).
>>
>> Trong các ví dụ tiếp theo, chúng tôi **không** sử dụng bất kỳ optimization flag hóa nào. Ví dụ, chúng tôi biên dịch bất kỳ file ví dụ nào (`example.c`) bằng lệnh: `gcc -m32 -o example example.c`. Do đó, sẽ có nhiều lệnh trông như dư thừa trong các ví dụ. Hãy nhớ rằng compiler không “thông minh” — nó chỉ đơn giản làm theo một loạt quy tắc để dịch code dễ đọc của con người sang ngôn ngữ máy. Trong quá trình dịch này, việc xuất hiện một số lệnh dư thừa là điều bình thường. Các compiler tối ưu hóa sẽ loại bỏ nhiều lệnh dư thừa này trong quá trình tối ưu hóa, nội dung này sẽ được đề cập ở [chương sau](../C12-CodeOpt/index.html#_code_optimization).

### 8.1.1. Thanh ghi (Registers) 

Hãy nhớ rằng **register** (thanh ghi) là một đơn vị lưu trữ có kích thước bằng một từ (word-sized) nằm trực tiếp trên CPU. Có thể có các thanh ghi riêng cho dữ liệu, lệnh và địa chỉ. Ví dụ, CPU Intel có tổng cộng tám thanh ghi để lưu trữ dữ liệu 32-bit:

`%eax`, `%ebx`, `%ecx`, `%edx`, `%edi`, `%esi`, `%esp`, và `%ebp`.

Chương trình có thể đọc hoặc ghi vào cả tám thanh ghi này. Sáu thanh ghi đầu tiên đều lưu dữ liệu **general-purpose** (đa dụng), trong khi hai thanh ghi cuối thường được compiler dành riêng để lưu dữ liệu địa chỉ. Mặc dù một chương trình có thể diễn giải nội dung của thanh ghi đa dụng là số nguyên hoặc địa chỉ, bản thân thanh ghi không phân biệt điều đó. Hai thanh ghi cuối (`%esp` và `%ebp`) lần lượt được gọi là **stack pointer** (con trỏ stack) và **frame pointer** (con trỏ khung stack). Compiler dành riêng các thanh ghi này cho các thao tác duy trì cấu trúc của **program stack**. Thông thường, `%esp` trỏ tới đỉnh của stack chương trình, trong khi `%ebp` trỏ tới đáy của stack frame hiện tại. Chúng ta sẽ bàn chi tiết hơn về stack frame và hai thanh ghi này trong phần nói về [functions](functions.html#_functions_in_assembly).

Thanh ghi cuối cùng đáng nhắc đến là `%eip` hay **instruction pointer** (đôi khi gọi là **program counter** – PC). Nó trỏ tới lệnh tiếp theo sẽ được CPU thực thi. Không giống tám thanh ghi kể trên, chương trình không thể ghi trực tiếp vào `%eip`.

### 8.1.2. Cú pháp nâng cao của thanh ghi (Advanced Register Notation) 

Với sáu thanh ghi đầu tiên vừa nêu, ISA cung cấp cơ chế truy cập 16 bit thấp của mỗi thanh ghi. ISA cũng cung cấp cơ chế riêng để truy cập các thành phần 8 bit của 16 bit thấp này đối với bốn thanh ghi đầu tiên. **Bảng 1** liệt kê từng thanh ghi và cơ chế (nếu có) để truy cập các byte thành phần.

| 32-bit register (bits 31–0) | Lower 16 bits (bits 15–0) | Bits 15–8 | Bits 7–0 |
|-----------------------------|---------------------------|-----------|----------|
| `%eax`                      | `%ax`                     | `%ah`     | `%al`    |
| `%ebx`                      | `%bx`                     | `%bh`     | `%bl`    |
| `%ecx`                      | `%cx`                     | `%ch`     | `%cl`    |
| `%edx`                      | `%dx`                     | `%dh`     | `%dl`    |
| `%edi`                      | `%di`                     |           |          |
| `%esi`                      | `%si`                     |           |          |

**Bảng 1.** Các thanh ghi x86 và cơ chế truy cập byte thấp.

- 16 bit thấp của bất kỳ thanh ghi nào kể trên có thể được truy cập bằng cách dùng hai ký tự cuối trong tên thanh ghi. Ví dụ: `%ax` truy cập 16 bit thấp của `%eax`.

- Byte *cao* và *thấp* trong 16 bit thấp của bốn thanh ghi đầu tiên có thể được truy cập bằng cách lấy hai ký tự cuối của tên thanh ghi và thay ký tự cuối bằng `h` (high – cao) hoặc `l` (low – thấp) tùy byte cần truy cập. Ví dụ: `%al` tham chiếu 8 bit thấp của `%ax`, còn `%ah` tham chiếu 8 bit cao của `%ax`. Các thanh ghi 8 bit này thường được compiler dùng để lưu giá trị 1 byte cho một số thao tác nhất định, như dịch bit (bitwise shift) – vì một thanh ghi 32-bit không thể dịch quá 31 vị trí và số 32 chỉ cần 1 byte để lưu. Nói chung, compiler sẽ dùng thành phần thanh ghi nhỏ nhất cần thiết để hoàn thành một phép toán.

### 8.1.3. Cấu trúc lệnh (Instruction Structure) 

Mỗi lệnh gồm một **opcode** (code thao tác) chỉ định nó làm gì, và một hoặc nhiều **operand** (toán hạng) cho biết cách thực hiện. Ví dụ: lệnh `add $0x2, %eax` có opcode là `add` và các toán hạng là `$0x2` và `%eax`.

Mỗi toán hạng tương ứng với một vị trí nguồn hoặc đích cho thao tác cụ thể. Có nhiều loại toán hạng:

- **Constant (literal)**: giá trị hằng, đứng trước bởi ký hiệu `$`. Ví dụ: trong `add $0x2, %eax`, `$0x2` là giá trị hằng ở hệ thập lục phân 0x2.
- **Register**: tham chiếu trực tiếp tới một thanh ghi. Ví dụ: `add $0x2, %eax` chỉ định `%eax` là nơi lưu kết quả của phép cộng.
- **Memory**: tham chiếu tới một giá trị trong bộ nhớ chính (RAM), thường dùng để tra cứu địa chỉ. Dạng địa chỉ bộ nhớ có thể kết hợp thanh ghi và giá trị hằng. Ví dụ: trong `mov 0x8(%ebp), %eax`, toán hạng `0x8(%ebp)` nghĩa là “cộng 0x8 vào giá trị trong `%ebp`, rồi truy xuất bộ nhớ tại địa chỉ đó”. Đây chính là thao tác dereference con trỏ.

### 8.1.4. Ví dụ với toán hạng (An Example with Operands) 

Cách tốt nhất để giải thích chi tiết về toán hạng là đưa ra một ví dụ nhanh. Giả sử bộ nhớ chứa các giá trị sau:

| Address | Value |
|---------|-------|
| 0x804   | 0xCA  |
| 0x808   | 0xFD  |
| 0x80c   | 0x12  |
| 0x810   | 0x1E  |

Giả sử các thanh ghi chứa giá trị:

| Register | Value |
|----------|-------|
| %eax     | 0x804 |
| %ebx     | 0x10  |
| %ecx     | 0x4   |
| %edx     | 0x1   |

Khi đó, các toán hạng trong **Bảng 2** sẽ được đánh giá (evaluate) thành các giá trị như hiển thị. Mỗi dòng trong bảng ghép một toán hạng với dạng của nó (ví dụ: constant, register, memory), cách nó được dịch, và giá trị của nó. Lưu ý: ký hiệu `M[x]` ở đây biểu thị giá trị tại vị trí bộ nhớ có địa chỉ x.

| Operand         | Form     | Translation                  | Value   |
|-----------------|----------|------------------------------|---------|
| `%ecx`          | Register | `%ecx`                       | 0x4     |
| `(%eax)`        | Memory   | M[`%eax`] hoặc M[0x804]       | 0xCA    |
| `$0x808`        | Constant | 0x808                         | 0x808   |
| `0x808`         | Memory   | M[0x808]                      | 0xFD    |
| `0x8(%eax)`     | Memory   | M[`%eax` + 8] hoặc M[0x80c]   | 0x12    |
| `(%eax, %ecx)`  | Memory   | M[`%eax` + `%ecx`] hoặc M[0x808] | 0xFD |
| `0x4(%eax, %ecx)` | Memory | M[`%eax` + `%ecx` + 4] hoặc M[0x80c] | 0x12 |
| `0x800(,%edx,4)` | Memory  | M[0x800 + `%edx`*4] hoặc M[0x804] | 0xCA |
| `(%eax, %edx, 8)` | Memory | M[`%eax` + `%edx`*8] hoặc M[0x80c] | 0x12 |

**Bảng 2.** Ví dụ về các toán hạng (operands)

Trong **Bảng 2**, ký hiệu `%ecx` biểu thị giá trị được lưu trong thanh ghi `%ecx`.  
Ngược lại, M[`%eax`] cho biết giá trị bên trong `%eax` sẽ được coi là một địa chỉ, và cần **dereference** (truy xuất) giá trị tại địa chỉ đó.  
Do đó, toán hạng `(%eax)` tương ứng với M[0x804], và giá trị tại địa chỉ này là 0xCA.

Một vài lưu ý quan trọng trước khi tiếp tục:  
Mặc dù **Bảng 2** cho thấy nhiều dạng toán hạng hợp lệ, nhưng không phải tất cả đều có thể dùng thay thế cho nhau trong mọi trường hợp.

Cụ thể:

- Toán hạng dạng **constant** không thể đóng vai trò là toán hạng đích (destination operand).
- Toán hạng dạng **memory** không thể đồng thời là cả nguồn (source) và đích (destination) trong cùng một lệnh.
- Trong các phép toán có **scaling** (xem hai toán hạng cuối trong [Bảng 2](#Operands32)), hệ số nhân (scaling factor) phải là một trong các giá trị 1, 2, 4 hoặc 8.

**Bảng 2** được cung cấp để tham khảo; tuy nhiên, việc nắm vững các dạng toán hạng chính sẽ giúp bạn đọc nhanh hơn khi phân tích code assembly.

### 8.1.5. Hậu tố của lệnh (Instruction Suffixes)

Trong một số trường hợp ở các ví dụ tiếp theo, các lệnh thông dụng và lệnh số học có thêm **hậu tố** (suffix) để chỉ **kích thước** (gắn với **kiểu dữ liệu**) của dữ liệu được thao tác ở cấp độ code lệnh.  
Compiler sẽ tự động dịch code sang các lệnh có hậu tố phù hợp.  
**Bảng 3** cho thấy các hậu tố phổ biến của lệnh x86.

| Suffix | C Type                  | Size (bytes) |
|--------|------------------------|--------------|
| b      | `char`                  | 1            |
| w      | `short`                 | 2            |
| l      | `int`, `long`, `unsigned` | 4          |

**Bảng 3.** Ví dụ về hậu tố của lệnh.

Lưu ý: các lệnh liên quan đến **thực thi có điều kiện** sẽ có hậu tố khác nhau tùy thuộc vào điều kiện được đánh giá.  
Chúng ta sẽ tìm hiểu các lệnh liên quan đến điều kiện trong [phần sau](conditional_control_loops.html#_conditional_control_and_loops).