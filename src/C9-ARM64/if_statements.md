### 9.4.2. Câu lệnh if trong Assembly  

Hãy cùng xem hàm `getSmallest` dưới dạng assembly. Để tiện theo dõi, hàm được nhắc lại dưới đây:

```c
int getSmallest(int x, int y) {
    int smallest;
    if ( x > y ) {
        smallest = y;
    }
    else {
        smallest = x;
    }
    return smallest;
}
```

Mã assembly tương ứng được trích xuất từ GDB trông như sau:

```
(gdb) disas getSmallest
Dump of assembler code for function getSmallest:
0x07f4 <+0>:    sub  sp, sp, #0x20
0x07f8 <+4>:    str  w0, [sp, #12]
0x07fc <+8>:    str  w1, [sp, #8]
0x0800 <+12>:   ldr  w1, [sp, #12]
0x0804 <+16>:   ldr  w0, [sp, #8]
0x0808 <+20>:   cmp  w1, w0
0x080c <+24>:   b.le 0x81c <getSmallest+40>
0x0810 <+28>:   ldr  w0, [sp, #8]
0x0814 <+32>:   str  w0, [sp, #28]
0x0818 <+36>:   b    0x824 <getSmallest+48>
0x081c <+40>:   ldr  w0, [sp, #12]
0x0820 <+44>:   str  w0, [sp, #28]
0x0824 <+48>:   ldr  w0, [sp, #28]
0x0828 <+52>:   add  sp, sp, #0x20
0x082c <+56>:   ret
```

Đây là một cách hiển thị khác của mã assembly so với những gì ta đã thấy trước đây. Ở đây, ta thấy **địa chỉ** gắn với mỗi lệnh, nhưng không thấy **byte** mã máy. Đoạn assembly này đã được chỉnh sửa nhẹ để đơn giản hơn. Theo quy ước, GCC đặt tham số thứ nhất và thứ hai của hàm vào các thanh ghi `x0` và `x1`. Vì các tham số của `getSmallest` có kiểu `int`, compiler sẽ đặt chúng vào *component register* tương ứng là `w0` và `w1`. Để dễ theo dõi, ta sẽ gọi các tham số này lần lượt là `x` và `y`.

Hãy lần theo một vài dòng đầu của đoạn assembly trên. Lưu ý rằng trong ví dụ này, chúng ta sẽ **không** vẽ stack ra minh họa. Đây là một bài tập để bạn tự thực hành kỹ năng theo dõi stack bằng cách tự vẽ.

- Lệnh `sub` mở rộng call stack thêm 32 byte (`0x20`).
- Các lệnh `str` tại `<getSmallest+4>` và `<getSmallest+8>` lưu `x` và `y` vào các vị trí `sp + 12` và `sp + 8` trên stack.
- Các lệnh `ldr` tại `<getSmallest+12>` và `<getSmallest+16>` nạp `x` và `y` vào các thanh ghi `w1` và `w0`. Lưu ý rằng nội dung ban đầu của `w0` và `w1` đã bị hoán đổi.
- Lệnh `cmp` so sánh `w1` với `w0` (tức so sánh `x` với `y`) và thiết lập các cờ điều kiện thích hợp.
- Lệnh `b.le` tại `<getSmallest+24>` cho biết nếu `x <= y` thì lệnh tiếp theo sẽ thực thi ở `<getSmallest+40>` (`pc = 0x81c`). Ngược lại, `pc` sẽ trỏ tới lệnh kế tiếp trong tuần tự, tức `0x810`.

Các lệnh tiếp theo sẽ phụ thuộc vào việc chương trình **có** thực hiện nhánh tại `<getSmallest+24>` hay không.

**Trường hợp 1:** Nhánh **không** được thực hiện. Khi đó, `pc = 0x810` (`<getSmallest+28>`), và các lệnh sau sẽ chạy:

- `ldr` tại `<getSmallest+28>` nạp `y` vào `w0`.
- `str` tại `<getSmallest+32>` lưu `y` vào `sp + 28`.
- `b` tại `<getSmallest+36>` đặt `pc = 0x824`.
- `ldr` tại `<getSmallest+48>` nạp `y` vào `w0`.
- Hai lệnh cuối thu nhỏ stack về kích thước ban đầu và trả về từ hàm. Lúc này, `y` nằm trong thanh ghi trả về `w0`, và `getSmallest` trả về `y`.

**Trường hợp 2:** Nhánh **được** thực hiện tại `<getSmallest+24>`. Khi đó, `pc = 0x81c` (`<getSmallest+40>`), và các lệnh sau sẽ chạy:

- `ldr` tại `<getSmallest+40>` nạp `x` vào `w0`.
- `str` tại `<getSmallest+44>` lưu `x` vào `sp + 28`.
- `ldr` tại `<getSmallest+48>` nạp `x` vào `w0`.
- Hai lệnh cuối thu nhỏ stack về kích thước ban đầu và trả về từ hàm. Lúc này, `x` nằm trong thanh ghi trả về `w0`, và `getSmallest` trả về `x`.

Ta có thể chú thích đoạn assembly trên như sau:

```
0x07f4 <+0>:   sub  sp, sp, #0x20          // mở rộng stack thêm 32 byte
0x07f8 <+4>:   str  w0, [sp, #12]          // lưu x tại sp+12
0x07fc <+8>:   str  w1, [sp, #8]           // lưu y tại sp+8
0x0800 <+12>:  ldr  w1, [sp, #12]          // w1 = x
0x0804 <+16>:  ldr  w0, [sp, #8]           // w0 = y
0x0808 <+20>:  cmp  w1, w0                 // so sánh x và y
0x080c <+24>:  b.le 0x81c <getSmallest+40> // nếu (x <= y) thì nhảy tới <getSmallest+40>
0x0810 <+28>:  ldr  w0, [sp, #8]           // w0 = y
0x0814 <+32>:  str  w0, [sp, #28]          // lưu y tại sp+28 (smallest)
0x0818 <+36>:  b    0x824 <getSmallest+48> // nhảy tới <getSmallest+48>
0x081c <+40>:  ldr  w0, [sp, #12]          // w0 = x
0x0820 <+44>:  str  w0, [sp, #28]          // lưu x tại sp+28 (smallest)
0x0824 <+48>:  ldr  w0, [sp, #28]          // w0 = smallest
0x0828 <+52>:  add  sp, sp, #0x20          // thu gọn stack
0x082c <+56>:  ret                         // trả về smallest
```

Chuyển ngược đoạn assembly này về mã C thu được:


#### **Dạng goto của `getSmallest()`**

```c
int getSmallest(int x, int y) {
    int smallest = y;
    if (x <= y) {
        goto assign_x;
    }
    smallest = y;
    goto done;

assign_x:
    smallest = x;

done:
    return smallest;
}
```

#### **Mã C tương đương**

```c
int getSmallest(int x, int y) {
    int smallest = y;
    if (x <= y) {
        smallest = x;
    }
    else {
        smallest = y;
    }
    return smallest;
}
```

Trong các ví dụ trên, biến `smallest` tương ứng với thanh ghi `w0`. Nếu `x` nhỏ hơn hoặc bằng `y`, mã sẽ thực thi câu lệnh `smallest = x`, câu lệnh này gắn với nhãn `goto` là `assign_x` trong dạng goto của hàm. Ngược lại, câu lệnh `smallest = y` sẽ được thực thi. Nhãn `goto` `done` được dùng để chỉ ra rằng giá trị trong `smallest` sẽ được trả về.

Lưu ý rằng bản dịch C ở trên của mã assembly có hơi khác so với hàm `getSmallest` gốc. Những khác biệt này không quan trọng; khi xem xét kỹ cả hai hàm, ta thấy chúng tương đương về mặt logic. Tuy nhiên, compiler trước tiên sẽ chuyển mỗi câu lệnh `if` thành một dạng `goto` tương đương, dẫn đến một phiên bản hơi khác nhưng vẫn tương đương. Các ví dụ dưới đây cho thấy dạng chuẩn của câu lệnh `if` và dạng `goto` tương đương.

#### **Câu lệnh `if` trong C**

```c
if (condition) {
    then_statement;
}
else {
    else_statement;
}
```

#### **Dạng `goto` tương đương của compiler**
```c
    if (!condition) {
        goto else;
    }
    then_statement;
    goto done;
else:
    else_statement;
done:
```

**Bảng 2.** Dạng chuẩn của câu lệnh if và dạng goto tương đương.

Khi dịch mã sang assembly, compiler sẽ tạo một nhánh (*branch*) khi điều kiện đúng. Điều này khác với cấu trúc của câu lệnh `if`, nơi một “jump” (nhảy) tới `else` xảy ra khi điều kiện *không* đúng. Dạng `goto` thể hiện rõ sự khác biệt logic này.

Xét bản dịch `goto` ban đầu của hàm `getSmallest`, ta thấy:

- `x <= y` tương ứng với `!(condition)`.
- `smallest = x` là `else_statement`.
- `smallest = y` là `then_statement`.
- Dòng cuối của hàm là `return smallest`.

Viết lại phiên bản gốc của hàm với các chú thích trên:

```c
int getSmallest(int x, int y) {
    int smallest;
    if (x > y) {     // !(x <= y)
        smallest = y; // then_statement
    }
    else {
        smallest = x; // else_statement
    }
    return smallest;
}
```

Phiên bản này giống hệt với hàm `getSmallest` ban đầu. Hãy nhớ rằng một hàm được viết theo nhiều cách khác nhau ở mức mã C vẫn có thể được dịch ra cùng một tập lệnh assembly.


#### Lệnh Conditional Select  

Lệnh điều kiện cuối cùng mà chúng ta tìm hiểu là **conditional select** (`csel`).  
Các lệnh `cmp`, `tst` và `b` thực hiện **conditional transfer of control** (chuyển điều khiển có điều kiện) trong chương trình. Nói cách khác, luồng thực thi của chương trình sẽ rẽ nhánh theo nhiều hướng. Điều này có thể gây bất lợi cho việc tối ưu hóa mã, vì các lệnh nhánh thường tốn kém để thực thi do gây gián đoạn **instruction pipeline** (chi tiết sẽ được đề cập trong [mục Kiến trúc](../C5-Arch/pipelining_advanced.html#_pipelining_hazards_control_hazards)).  

Ngược lại, lệnh `csel` thực hiện **conditional transfer of data** (chuyển dữ liệu có điều kiện). Nói cách khác, CPU sẽ thực thi *cả* `then_statement` và `else_statement`, sau đó đặt dữ liệu vào thanh ghi thích hợp dựa trên kết quả của điều kiện.

Việc sử dụng **biểu thức ba ngôi** (*ternary expression*) trong C thường khiến compiler sinh ra lệnh `csel` thay cho các lệnh nhánh. Với câu lệnh if-then-else chuẩn, biểu thức ba ngôi có dạng:

```c
result = (condition) ? then_expression : else_expression;
```

Hãy dùng dạng này để viết lại hàm `getSmallest` dưới dạng biểu thức ba ngôi. Lưu ý rằng phiên bản mới này hoạt động giống hệt hàm `getSmallest` ban đầu:

```c
int getSmallest_csel(int x, int y) {
    return x > y ? y : x;
}
```

Mặc dù thay đổi này có vẻ không lớn, nhưng hãy xem mã assembly được tạo ra. Nhớ rằng tham số thứ nhất và thứ hai (`x` và `y`) lần lượt được lưu trong các thanh ghi `w0` và `w1`:

```
(gdb) disas getSmallest_csel
Dump of assembler code for function getSmallest_csel:
0x0860 <+0>:  sub  sp, sp, #0x10      // mở rộng stack thêm 16 byte
0x0864 <+4>:  str  w0, [sp, #12]      // lưu x tại sp+12
0x0868 <+8>:  str  w1, [sp, #8]       // lưu y tại sp+8
0x086c <+12>: ldr  w0, [sp, #8]       // w0 = y
0x0870 <+16>: ldr  w2, [sp, #12]      // w2 = x
0x0874 <+20>: ldr  w1, [sp, #12]      // w1 = x
0x0878 <+24>: cmp  w2, w0             // so sánh x và y
0x087c <+28>: csel w0, w1, w0, le     // nếu (x <= y) w0 = x, ngược lại w0 = y
0x0880 <+32>: add  sp, sp, #0x10      // khôi phục sp
0x0884 <+36>: ret                     // trả về (w0)
```

Mã assembly này **không có lệnh nhảy**. Sau khi so sánh `x` và `y`, `x` chỉ được đưa vào thanh ghi trả về `w0` nếu `x <= y`.

Cấu trúc của lệnh `csel` là:

```
csel D, R1, R2, C // nếu (C) D = R1, ngược lại D = R2
```

Trong đó:
- `D` là thanh ghi đích.
- `R1` và `R2` là hai thanh ghi chứa các giá trị cần chọn.
- `C` là điều kiện cần đánh giá.

Tương tự như lệnh nhánh, thành phần `C` trong lệnh `csel` chỉ ra điều kiện để thực hiện chọn giá trị. Các điều kiện này giống hệt như trong [bảng hậu tố lệnh nhánh có điều kiện](conditional_control_loops.html#_conditional_branch_instruction_suffixes).

Với hàm `getSmallest` gốc, **bộ tối ưu hóa nội bộ** của compiler (xem Chương 12) sẽ thay thế các lệnh `b` bằng một lệnh `csel` nếu bật tối ưu hóa mức 1 (`-O1`):

```
// biên dịch với: gcc -O1 -o getSmallest getSmallest.c
Dump of assembler code for function getSmallest:
0x0734 <+0>:  cmp  w0, w1            // so sánh x và y
0x0738 <+4>:  csel w0, w0, w1, le    // nếu (x <= y) w0 = x, ngược lại w0 = y
0x073c <+8>:  ret                    // trả về (w0)
```

Nói chung, compiler rất thận trọng khi tối ưu hóa lệnh nhánh thành `csel`, đặc biệt trong các trường hợp có **side effect** hoặc liên quan đến con trỏ. **Bảng 3** dưới đây cho thấy hai cách viết tương đương của một hàm `incrementX`:

#### Mã C
```c
int incrementX(int * x) {
    if (x != NULL) { // nếu x không NULL
        return (*x)++; // tăng giá trị *x
    }
    else { // nếu x là NULL
        return 1;
    }
}
```

#### Dạng ba ngôi trong C
```c
int incrementX2(int * x) {
    return x ? (*x)++ : 1;
}
```

**Bảng 3.** Hai hàm cố gắng tăng giá trị của số nguyên `x`.

Mỗi hàm nhận một con trỏ tới số nguyên làm tham số và kiểm tra xem nó có phải `NULL` hay không. Nếu `x` không phải `NULL`, hàm sẽ tăng giá trị mà `x` trỏ tới và trả về giá trị đó. Ngược lại, hàm trả về 1.

Có thể bạn sẽ nghĩ rằng `incrementX2` sẽ dùng lệnh `csel` vì nó dùng biểu thức ba ngôi. Tuy nhiên, cả hai hàm đều sinh ra **mã assembly giống hệt nhau**:

```
// tham số x nằm trong thanh ghi x0
Dump of assembler code for function incrementX2:
0x0774 <+0>:  mov  w1, #0x1                   // w1 = 0x1
0x0778 <+4>:  cbz  x0, 0x788 <incrementX2+20> // nếu (x == 0) nhảy tới <incrementX2+20>
0x077c <+8>:  ldr  w1, [x0]                   // w1 = *x
0x0780 <+12>: add  w2, w1, #0x1               // w2 = w1 + 1
0x0784 <+16>: str  w2, [x0]                   // *x = w2
0x0788 <+20>: mov  w0, w1                     // w0 = *x
0x078c <+24>: ret                             // trả về (w0)
```

Hãy nhớ rằng lệnh `csel` **thực thi cả hai nhánh** của điều kiện. Nói cách khác, `x` sẽ luôn bị dereference (giải tham chiếu) dù điều kiện đúng hay sai. Nếu `x` là con trỏ null, việc dereference sẽ gây **null pointer exception** và dẫn đến **segmentation fault**. Để tránh rủi ro này, compiler chọn cách an toàn là dùng lệnh nhánh thay vì `csel`.