## 10. Những điểm rút ra chính về Assembly

Phần này của cuốn sách đã trình bày những kiến thức cơ bản về assembly.  
Mặc dù ngày nay hầu hết mọi người lập trình bằng ngôn ngữ bậc cao, nhưng việc hiểu assembly giúp lập trình viên nắm rõ hơn chương trình và compiler của họ đang thực sự làm gì.  
Kiến thức về assembly cũng rất cần thiết cho những ai thiết kế phần mềm cho **embedded systems** (hệ thống nhúng) và các môi trường hạn chế tài nguyên, cũng như cho những người làm trong lĩnh vực phân tích lỗ hổng bảo mật.  
Các chương trong phần assembly của sách này đã bao quát **64-bit Intel assembly (x86-64)**, **32-bit Intel Assembly (IA32)** và **64-bit ARM assembly (ARMv8-A)**.

### Các đặc điểm chung

Bất kể bạn học loại assembly nào, vẫn có một số đặc điểm chung của *mọi* ngôn ngữ assembly đáng chú ý:

**ISA định nghĩa ngôn ngữ assembly**  
Ngôn ngữ assembly cụ thể trên một máy được xác định bởi **instruction set architecture** (ISA – kiến trúc tập lệnh) của máy đó.  
Để xác định kiến trúc nền tảng của một máy Linux, có thể dùng lệnh:

```
uname -m
```

**Registers lưu trữ dữ liệu**  
Mỗi ISA định nghĩa một tập các **register** cơ bản mà CPU dùng để thao tác dữ liệu.  
Một số register là *general purpose* (đa dụng) và có thể chứa bất kỳ loại dữ liệu nào, trong khi một số khác là *special purpose* (chuyên dụng) và thường được compiler dành riêng cho các mục đích cụ thể (ví dụ: stack pointer, base pointer).  
Các register đa dụng có thể đọc và ghi, nhưng một số register chuyên dụng chỉ đọc (ví dụ: instruction pointer).

**Instructions xác định CPU có thể làm gì**  
ISA cũng định nghĩa một tập các **instruction** (lệnh) chỉ rõ các thao tác mà CPU có thể thực hiện.  
Mỗi lệnh có một **operation code** (opcode) xác định hành động của lệnh, và một hoặc nhiều **operand** (toán hạng) xác định dữ liệu được sử dụng.  
ISA mô tả các lệnh cụ thể cho việc di chuyển dữ liệu, các phép toán số học, điều kiện, rẽ nhánh và truy cập bộ nhớ.  
Những lệnh cốt lõi này thường được kết hợp để biểu diễn các cấu trúc dữ liệu phức tạp hơn như mảng, struct và ma trận.

**Program stack lưu trữ biến cục bộ của một hàm cụ thể**  
Compiler sử dụng stack (hoặc stack memory) trong không gian địa chỉ ảo của process để lưu dữ liệu tạm thời.  
Trên tất cả các hệ thống hiện đại, program stack phát triển về phía **địa chỉ thấp hơn**.  
Compiler dùng stack pointer và base pointer để xác định **stack frame** – vùng stack gắn với một hàm hoặc procedure cụ thể.  
Mỗi lần gọi hàm sẽ thêm một stack frame mới vào stack, xác định vùng stack của hàm callee.  
Stack frame của một hàm sẽ bị loại bỏ khi hàm đó trả về.  
Thông thường, stack pointer và base pointer sẽ trở lại giá trị ban đầu khi hàm kết thúc.  
Mặc dù điều này gợi ý rằng các biến cục bộ đã được “xóa” khỏi stack, nhưng dữ liệu cũ thường vẫn tồn tại dưới dạng giá trị rác, đôi khi gây ra các hành vi khó debug.  
Kẻ tấn công cũng có thể lợi dụng kiến thức về cách ISA quản lý stack để tạo ra các exploit nguy hiểm, như **buffer overflow**.

**Bảo mật**  
Mặc dù mọi hệ thống đều có thể bị tấn công bởi các lỗ hổng như buffer overflow, nhưng kiến trúc ARMv8-A ra đời sau đã có cơ hội học hỏi từ các lỗi bảo mật từng ảnh hưởng đến các kiến trúc Intel cũ hơn.  
Tuy nhiên, tuyến phòng thủ đầu tiên luôn là lập trình viên.  
Ngay cả khi có thêm các cơ chế bảo vệ, không ISA nào miễn nhiễm hoàn toàn với lỗi bảo mật.  
Khi lập trình bằng C, lập trình viên nên sử dụng **length specifier** bất cứ khi nào có thể để giảm nguy cơ lỗ hổng bảo mật do tràn bộ nhớ (xem **Bảng 1**).

| Thay vì:                  | Hãy dùng:                           |
|---------------------------|--------------------------------------|
| `gets(buf)`               | `fgets(buf, 12, stdin)`              |
| `scanf("%s", buf)`        | `scanf("%12s", buf)`                 |
| `strcpy(buf2, buf)`       | `strncpy(buf2, buf, 12)`             |
| `strcat(buf2, buf)`       | `strncat(buf2, buf, 12)`             |
| `sprintf(buf, "%d", num)` | `snprintf(buf, 12, "%d", num)`       |

**Bảng 1.** Các hàm C với length specifier.

### Tài liệu đọc thêm

Cuốn sách này chỉ giới thiệu một phần nhỏ về một số ngôn ngữ assembly phổ biến.  
Để hiểu sâu hơn về assembly, bạn nên tham khảo các tài liệu đặc tả ISA:

- [Intel 64 and IA32 Manuals](https://software.intel.com/en-us/articles/intel-sdm#architecture)  
- [ARM Cortex-A Programmer's Guide](https://developer.arm.com/docs/den0024/a/preface)

Các tài nguyên miễn phí sau cũng hữu ích cho những ai muốn học assembly 32-bit:

- [IA32 Programming Web Aside](http://csapp.cs.cmu.edu/3e/waside/waside-ia32.pdf) (Randal Bryant và David O'Hallaron)  
- [32-bit ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/) (Azeria Labs)

Các sách sau đây cũng có phần thảo luận chuyên sâu về assembly; tuy không miễn phí nhưng là nguồn tài liệu rất tốt:

- Intel systems: [*Computer Systems: A Programmer's Perspective*](http://csapp.cs.cmu.edu/) (Randal Bryant và David O'Hallaron)  
- ARMv8: [*Computer Organization and Design*](https://textbooks.elsevier.com/web/product_details.aspx?isbn=9780128017333) (David Patterson và John Hennessy)