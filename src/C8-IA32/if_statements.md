### 8.4.2. Câu lệnh if trong Assembly

Hãy cùng xem hàm `getSmallest` ở dạng assembly.  
Để tiện theo dõi, hàm được nhắc lại dưới đây:

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

```asm
(gdb) disas getSmallest
Dump of assembler code for function getSmallest:
  0x8048411 <+6>:   mov    0x8(%ebp),%eax
  0x8048414 <+9>:   cmp    0xc(%ebp),%eax
  0x8048417 <+12>:  jle    0x8048421 <getSmallest+22>
  0x8048419 <+14>:  mov    0xc(%ebp),%eax
  0x804841f <+20>:  jmp    0x8048427 <getSmallest+28>
  0x8048421 <+22>:  mov    0x8(%ebp),%eax
  0x8048427 <+28>:  ret
```

Đây là một cách hiển thị khác của mã assembly so với trước đây.  
Ở đây, ta thấy **địa chỉ** gắn với mỗi lệnh, nhưng không thấy **byte** mã máy.  
Đoạn assembly này đã được chỉnh sửa nhẹ để đơn giản hơn: các lệnh thường xuất hiện khi tạo/kết thúc hàm (như `push %ebp` và `mov %esp, %ebp`) và lệnh cấp phát bộ nhớ trên stack đã được lược bỏ.  

Theo quy ước, GCC đặt tham số thứ nhất và thứ hai của hàm tại các vị trí `%ebp+8` và `%ebp+0xc` (hoặc `%ebp+12`).  
Trong phần giải thích này, ta gọi chúng lần lượt là `x` và `y`.

Bây giờ, hãy lần theo các dòng đầu tiên của đoạn assembly trên.  
Lưu ý: ví dụ này sẽ **không** vẽ stack minh họa; bạn đọc có thể tự luyện tập kỹ năng này.

- Lệnh `mov` sao chép giá trị tại địa chỉ `%ebp+8` (tham số `x`) vào thanh ghi `%eax`. Con trỏ lệnh (`%eip`) được đặt tới địa chỉ của lệnh tiếp theo: `0x08048414`.
- Lệnh `cmp` so sánh giá trị tại `%ebp+12` (tham số `y`) với `x` và thiết lập các cờ điều kiện. `%eip` chuyển sang `0x08048417`.
- Lệnh `jle` (jump if less or equal) cho biết: nếu `x <= y`, lệnh tiếp theo sẽ là tại `<getSmallest+22>` (`mov 0x8(%ebp), %eax`), và `%eip` sẽ được đặt thành `0x8048421`. Ngược lại, `%eip` sẽ trỏ tới lệnh kế tiếp theo thứ tự: `0x8048419`.

Các lệnh tiếp theo phụ thuộc vào việc nhánh tại `<getSmallest+12>` có được thực hiện hay không.

**Trường hợp 1 – Nhánh *không* được thực hiện** (`x > y`):

- `mov 0xc(%ebp),%eax` tại `<getSmallest+14>`: sao chép `y` vào `%eax`. `%eip` → `0x804841f`.
- `jmp` đặt `%eip` → `0x8048427`.
- `ret` kết thúc hàm. Lúc này `%eax` chứa `y`, và `getSmallest` trả về `y`.

**Trường hợp 2 – Nhánh được thực hiện** (`x <= y`):

- `mov 0x8(%ebp),%eax` tại `0x8048421`: sao chép `x` vào `%eax`. `%eip` → `0x8048427`.
- `ret` kết thúc hàm. Lúc này `%eax` chứa `x`, và `getSmallest` trả về `x`.

Chúng ta có thể chú thích đoạn assembly như sau:

```asm
0x8048411 <+6>:  mov 0x8(%ebp),%eax             # copy x to %eax
0x8048414 <+9>:  cmp 0xc(%ebp),%eax             # compare x with y
0x8048417 <+12>: jle 0x8048421 <getSmallest+22> # if x <= y goto <getSmallest+22>
0x8048419 <+14>: mov 0xc(%ebp),%eax             # copy y to %eax
0x804841f <+20>: jmp 0x8048427 <getSmallest+28> # goto <getSmallest+28>
0x8048421 <+22>: mov 0x8(%ebp),%eax             # copy x to %eax
0x8048427 <+28>: ret                            # exit function (return %eax)
```

Khi dịch ngược lại sang C, ta có thể viết:

#### **Phiên bản dùng goto**

```c
int getSmallest(int x, int y) {
    int smallest;
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

#### **Phiên bản C thông thường**

```c
int getSmallest(int x, int y) {
    int smallest;
    if (x <= y) {
        smallest = x;
    }
    else {
        smallest = y;
    }
    return smallest;
}
```

**Bảng 1.** Dịch `getSmallest` sang dạng C dùng `goto` và dạng C thông thường.


Trong **Bảng 1**, biến `smallest` tương ứng với thanh ghi `%eax`.  
Nếu `x` nhỏ hơn hoặc bằng `y`, mã sẽ thực thi câu lệnh `smallest = x`, câu lệnh này gắn với nhãn `goto` là `assign_x` trong phiên bản hàm dạng `goto`.  
Ngược lại, câu lệnh `smallest = y` sẽ được thực thi.  
Nhãn `goto` là `done` được dùng để chỉ ra rằng giá trị trong `smallest` sẽ được trả về.

Lưu ý rằng bản dịch C từ mã assembly ở trên hơi khác so với hàm `getSmallest` gốc.  
Những khác biệt này không quan trọng; khi xem xét kỹ cả hai hàm, ta thấy chúng tương đương về mặt logic.  
Tuy nhiên, trình biên dịch trước tiên sẽ chuyển bất kỳ câu lệnh `if` nào sang dạng `goto` tương đương, dẫn đến phiên bản hơi khác nhưng vẫn tương đương.  

**Bảng 2** dưới đây cho thấy dạng chuẩn của câu lệnh `if` và dạng `goto` tương đương:

**Dạng C `if`-statement chuẩn**

```c
if (condition) {
    then_statement;
}
else {
    else_statement;
}
```

**Dạng `goto` tương đương do compiler tạo ra**

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

**Bảng 2.** Dạng chuẩn của câu lệnh `if` và dạng `goto` tương đương.

Khi dịch code sang assembly, compiler sẽ tạo một lệnh nhảy (jump) khi điều kiện **đúng**.  
Điều này khác với cấu trúc của câu lệnh `if`, nơi “nhảy” (tới `else`) xảy ra khi điều kiện **không đúng**.  
Dạng `goto` thể hiện rõ sự khác biệt logic này.

Xét bản dịch `goto` của hàm `getSmallest`, ta thấy:

- `x <= y` tương ứng với `!condition`.
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

Phiên bản này giống hệt với hàm `getSmallest` ban đầu.  
Hãy nhớ rằng một hàm viết theo nhiều cách khác nhau trong C vẫn có thể được dịch ra cùng một tập lệnh assembly.

#### Lệnh `cmov`

Nhóm lệnh điều kiện cuối cùng mà chúng ta đề cập là **conditional move** (`cmov`).  
Các lệnh `cmp`, `test` và `jmp` thực hiện **chuyển điều khiển có điều kiện** (*conditional transfer of control*) trong chương trình.  
Nói cách khác, luồng thực thi của chương trình sẽ rẽ nhánh theo nhiều hướng.  
Điều này có thể gây bất lợi cho việc tối ưu hóa mã, vì các nhánh này thường tốn kém.

Ngược lại, lệnh `cmov` thực hiện **chuyển dữ liệu có điều kiện** (*conditional transfer of data*).  
Nói cách khác, cả `then_statement` và `else_statement` của câu lệnh điều kiện đều được thực thi, và dữ liệu sẽ được đặt vào thanh ghi thích hợp dựa trên kết quả của điều kiện.

Việc sử dụng **biểu thức ba ngôi** (ternary expression) trong C thường khiến compiler sinh ra lệnh `cmov` thay vì các lệnh nhảy.  
Với câu lệnh if-then-else chuẩn, biểu thức ba ngôi có dạng:

```c
condition ? then_statement : else_statement;
```


Biểu thức ba ngôi (ternary expression) trong C có dạng:

```c
result = (condition) ? then_statement : else_statement;
```

Hãy sử dụng dạng này để viết lại hàm `getSmallest` dưới dạng biểu thức ba ngôi.  
Lưu ý rằng phiên bản mới này hoạt động **hoàn toàn giống** với hàm `getSmallest` ban đầu:

```c
int getSmallest_cmov(int x, int y) {
    return x > y ? y : x;
}
```

Mặc dù thay đổi này có vẻ không lớn, nhưng hãy xem mã assembly được tạo ra.  
Nhắc lại rằng tham số thứ nhất (`x`) và tham số thứ hai (`y`) được lưu tại các địa chỉ trên stack lần lượt là `%ebp+0x8` và `%ebp+0xc`.

```asm
0x08048441 <+0>:   push   %ebp              # save ebp
0x08048442 <+1>:   mov    %esp,%ebp         # update ebp
0x08048444 <+3>:   mov    0xc(%ebp),%eax    # copy y to %eax
0x08048447 <+6>:   cmp    %eax,0x8(%ebp)    # compare x with y
0x0804844a <+9>:   cmovle 0x8(%ebp),%eax    # if (x <= y) copy x to %eax
0x0804844e <+13>:  pop    %ebp              # restore %ebp
0x0804844f <+14>:  ret                      # return %eax
```

Đoạn mã assembly này **không có lệnh nhảy**.  
Sau khi so sánh `x` và `y`, giá trị `x` chỉ được chuyển vào thanh ghi trả về nếu `x <= y`.  
Tương tự như các lệnh nhảy, hậu tố (suffix) của lệnh `cmov` cho biết điều kiện mà việc di chuyển dữ liệu có điều kiện sẽ xảy ra.  

**Bảng 3** liệt kê tập hợp các lệnh `cmov`:

| Signed               | Unsigned             | Mô tả                                    |
|----------------------|----------------------|------------------------------------------|
| `cmove` (`cmovz`)    |                      | move if equal (==)                       |
| `cmovne` (`cmovnz`)  |                      | move if not equal (!=)                   |
| `cmovs`              |                      | move if negative                         |
| `cmovns`             |                      | move if non-negative                     |
| `cmovg` (`cmovnle`)  | `cmova` (`cmovnbe`)  | move if greater (>)                      |
| `cmovge` (`cmovnl`)  | `cmovae` (`cmovnb`)  | move if greater than or equal (>=)       |
| `cmovl` (`cmovnge`)  | `cmovb` (`cmovnae`)  | move if less (<)                         |
| `cmovle` (`cmovng`)  | `cmovbe` (`cmovna`)  | move if less than or equal (<=)          |

**Bảng 3.** Các lệnh `cmov`.

Compiler rất thận trọng khi chuyển đổi các lệnh nhảy thành lệnh `cmov`, đặc biệt trong các trường hợp có **side effect** (tác dụng phụ) hoặc liên quan đến giá trị con trỏ.  

**Bảng 4** sẽ cho thấy hai cách viết tương đương của một hàm `incrementX`.


### **Bảng 4.** Hai hàm cố gắng tăng giá trị của số nguyên `x`

#### **Dạng C thông thường**

```c
int incrementX(int *x) {
    if (x != NULL) { // nếu x không NULL
        return (*x)++; // tăng giá trị mà x trỏ tới và trả về
    }
    else { // nếu x là NULL
        return 1; // trả về 1
    }
}
```

#### **Dạng C dùng biểu thức ba ngôi (ternary form)**

```c
int incrementX2(int *x) {
    return x ? (*x)++ : 1;
}
```

Mỗi hàm nhận vào một con trỏ tới số nguyên và kiểm tra xem nó có phải `NULL` hay không.  
Nếu `x` không phải `NULL`, hàm sẽ tăng giá trị mà `x` trỏ tới và trả về giá trị đó.  
Ngược lại, hàm sẽ trả về giá trị `1`.

Có thể bạn sẽ nghĩ rằng `incrementX2` sử dụng lệnh `cmov` vì nó dùng biểu thức ba ngôi.  
Tuy nhiên, cả hai hàm đều tạo ra **chính xác cùng một mã assembly**:

```asm
0x80484cf <+0>:   push   %ebp
0x80484d0 <+1>:   mov    %esp,%ebp
0x80484d2 <+3>:   cmpl   $0x0,0x8(%ebp)
0x80484d6 <+7>:   je     0x80484e7 <incrementX2+24>
0x80484d8 <+9>:   mov    0x8(%ebp),%eax
0x80484db <+12>:  mov    (%eax),%eax
0x80484dd <+14>:  lea    0x1(%eax),%ecx
0x80484e0 <+17>:  mov    0x8(%ebp),%edx
0x80484e3 <+20>:  mov    %ecx,(%edx)
0x80484e5 <+22>:  jmp    0x80484ec <incrementX2+29>
0x80484e7 <+24>:  mov    $0x1,%eax
0x80484ec <+29>:  pop    %ebp
0x80484ed <+30>:  ret
```

Hãy nhớ rằng lệnh `cmov` sẽ **thực thi cả hai nhánh của điều kiện**.  
Nói cách khác, `x` sẽ luôn bị dereference (giải tham chiếu) bất kể điều kiện đúng hay sai.  

Xét trường hợp `x` là con trỏ null: việc dereference một con trỏ null sẽ gây ra **null pointer exception** trong chương trình, dẫn đến **segmentation fault**.  
Để tránh hoàn toàn khả năng này, compiler chọn cách an toàn hơn là sử dụng **lệnh nhảy** (jump) thay vì `cmov`.
