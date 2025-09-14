### 7.4.3. Vòng lặp trong Assembly

Tương tự như câu lệnh `if`, các vòng lặp trong assembly cũng được triển khai bằng các lệnh nhảy (**jump instructions**). Tuy nhiên, vòng lặp cho phép các lệnh được *thực thi lại* dựa trên kết quả của một điều kiện được đánh giá.

Hàm `sumUp` trong ví dụ dưới đây tính tổng tất cả các số nguyên dương từ 1 đến một số nguyên do người dùng xác định. Đoạn code này được viết **không tối ưu** để minh họa vòng lặp `while` trong C.

```c
int sumUp(int n) {
    //initialize total and i
    int total = 0;
    int i = 1;

    while (i <= n) {  //while i is less than or equal to n
        total += i;   //add i to total
        i++;          //increment i by 1
    }
    return total;
}
```

Biên dịch đoạn code này và dùng GDB để disassemble sẽ cho ra code assembly sau:

```
Dump of assembler code for function sumUp:
0x400526 <+0>:   push   %rbp
0x400527 <+1>:   mov    %rsp,%rbp
0x40052a <+4>:   mov    %edi,-0x14(%rbp)
0x40052d <+7>:   mov    $0x0,-0x8(%rbp)
0x400534 <+14>:  mov    $0x1,-0x4(%rbp)
0x40053b <+21>:  jmp    0x400547 <sumUp+33>
0x40053d <+23>:  mov    -0x4(%rbp),%eax
0x400540 <+26>:  add    %eax,-0x8(%rbp)
0x400543 <+29>:  add    $0x1,-0x4(%rbp)
0x400547 <+33>:  mov    -0x4(%rbp),%eax
0x40054a <+36>:  cmp    -0x14(%rbp),%eax
0x40054d <+39>:  jle    0x40053d <sumUp+23>
0x40054f <+41>:  mov    -0x8(%rbp),%eax
0x400552 <+44>:  pop    %rbp
0x400553 <+45>:  retq
```

Một lần nữa, chúng ta sẽ không vẽ stack một cách tường minh trong ví dụ này. Tuy nhiên, bạn đọc nên tự vẽ stack để luyện tập.

#### Năm lệnh đầu tiên

Năm lệnh đầu tiên của hàm này thiết lập stack để thực thi hàm và khởi tạo các giá trị tạm thời:

```
0x400526 <+0>:  push %rbp              # save %rbp onto the stack
0x400527 <+1>:  mov  %rsp,%rbp         # update the value of %rbp (new frame)
0x40052a <+4>:  mov  %edi,-0x14(%rbp)  # copy n to %rbp-0x14
0x40052d <+7>:  mov  $0x0,-0x8(%rbp)   # copy 0 to %rbp-0x8 (total)
0x400534 <+14>: mov  $0x1,-0x4(%rbp)   # copy 1 to %rbp-0x4 (i)
```

Hãy nhớ rằng các vị trí trên stack lưu trữ *các biến tạm thời* trong một hàm. Để đơn giản, ta sẽ gọi vị trí `%rbp-0x8` là `total` và `%rbp-0x4` là `i`. Tham số đầu vào `n` của `sumUp` được lưu tại `%rbp-0x14`. Mặc dù các biến tạm được đặt trên stack, lưu ý rằng stack pointer không thay đổi sau khi thực thi lệnh đầu tiên (`push %rbp`).

#### Trái tim của vòng lặp

Bảy lệnh tiếp theo trong hàm `sumUp` là phần lõi của vòng lặp:

```
0x40053b <+21>:  jmp    0x400547 <sumUp+33>  # goto <sumUp+33>
0x40053d <+23>:  mov    -0x4(%rbp),%eax      # copy i to %eax
0x400540 <+26>:  add    %eax,-0x8(%rbp)      # add i to total (total += i)
0x400543 <+29>:  add    $0x1,-0x4(%rbp)      # add 1 to i (i += 1)
0x400547 <+33>:  mov    -0x4(%rbp),%eax      # copy i to %eax
0x40054a <+36>:  cmp    -0x14(%rbp),%eax     # compare i to n
0x40054d <+39>:  jle    0x40053d <sumUp+23>  # if (i <= n) goto <sumUp+23>
```

- Lệnh đầu tiên là một **jump** trực tiếp tới `<sumUp+33>`, đặt `%rip` thành `0x400547`.
- Lệnh tiếp theo thực thi là `mov -0x4(%rbp),%eax`, đưa giá trị của `i` vào `%eax`. `%rip` được cập nhật thành `0x40054a`.
- Lệnh `cmp` tại `<sumUp+36>` so sánh `i` với `n` và thiết lập các **condition code register** phù hợp. `%rip` được đặt thành `0x40054d`.

Sau đó, lệnh `jle` được thực thi. Các lệnh tiếp theo phụ thuộc vào việc nhánh có được thực hiện hay không.

**Trường hợp nhánh được thực hiện** (`i <= n` là đúng): `%rip` được đặt thành `0x40053d` và chương trình nhảy tới `<sumUp+23>`. Các lệnh sau sẽ chạy tuần tự:

- `mov` tại `<sumUp+23>` sao chép `i` vào `%eax`.
- `add %eax,-0x8(%rbp)` cộng `i` vào `total` (`total += i`).
- `add` tại `<sumUp+29>` cộng 1 vào `i` (`i += 1`).
- `mov` tại `<sumUp+33>` sao chép giá trị mới của `i` vào `%eax`.
- `cmp` so sánh `i` với `n` và thiết lập các cờ điều kiện.
- `jle` thực thi. Nếu `i <= n`, chương trình lại nhảy về `<sumUp+23>` và vòng lặp (từ `<sumUp+23>` đến `<sumUp+39>`) lặp lại.

**Trường hợp nhánh không được thực hiện** (`i` không nhỏ hơn hoặc bằng `n`): các lệnh sau sẽ chạy:

```
0x40054f <+41>:  mov    -0x8(%rbp),%eax     # copy total to %eax
0x400552 <+44>:  pop    %rbp                # restore rbp
0x400553 <+45>:  retq                       # return (total)
```

Các lệnh này sao chép `total` vào `%eax`, khôi phục `%rbp` về giá trị ban đầu và thoát khỏi hàm. Do đó, hàm trả về `total` khi kết thúc.

**Bảng 1** dưới đây sẽ trình bày dạng assembly và dạng C dùng `goto` của hàm `sumUp`.


**Assembly:**

```assembly
<sumUp>:
<+0>:  push %rbp
<+1>:  mov %rsp,%rbp
<+4>:  mov %edi,-0x14(%rbp)
<+7>:  mov $0x0,-0x8(%rbp)
<+14>: mov $0x1,-0x4(%rbp)
<+21>: jmp 0x400547 <sumUp+33>
<+23>: mov -0x4(%rbp),%eax
<+26>: add %eax,-0x8(%rbp)
<+29>: add $0x1,-0x4(%rbp)
<+33>: mov -0x4(%rbp),%eax
<+36>: cmp -0x14(%rbp),%eax
<+39>: jle 0x40053d <sumUp+23>
<+41>: mov -0x8(%rbp),%eax
<+44>: pop %rbp
<+45>: retq
```

**Dạng C dùng goto:**

```c
int sumUp(int n) {
    int total = 0;
    int i = 1;
    goto start;
body:
    total += i;
    i += 1;
start:
    if (i <= n) {
        goto body;
    }
    return total;
}
```

*Bảng 1. Dịch hàm `sumUp` sang dạng C dùng `goto`.*

Đoạn code trên cũng tương đương với đoạn C sau, không dùng câu lệnh `goto`:

```c
int sumUp(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) {
        total += i;
        i += 1;
    }
    return total;
}
```

#### Vòng lặp for trong Assembly

Vòng lặp chính trong hàm `sumUp` cũng có thể được viết dưới dạng vòng lặp `for`:

```c
int sumUp2(int n) {
    int total = 0;             // khởi tạo total = 0
    int i;
    for (i = 1; i <= n; i++) { // khởi tạo i = 1, tăng i thêm 1 khi i <= n
        total += i;            // cộng i vào total
    }
    return total;
}
```

Phiên bản này tạo ra code assembly **giống hệt** với ví dụ vòng lặp `while`. Dưới đây là code assembly và chú thích từng dòng:

```assembly
Dump of assembler code for function sumUp2:
0x400554 <+0>:   push   %rbp                   # lưu %rbp
0x400555 <+1>:   mov    %rsp,%rbp              # cập nhật %rbp (stack frame mới)
0x400558 <+4>:   mov    %edi,-0x14(%rbp)       # copy %edi vào %rbp-0x14 (n)
0x40055b <+7>:   movl   $0x0,-0x8(%rbp)        # copy 0 vào %rbp-0x8 (total)
0x400562 <+14>:  movl   $0x1,-0x4(%rbp)        # copy 1 vào %rbp-0x4 (i)
0x400569 <+21>:  jmp    0x400575 <sumUp2+33>   # goto <sumUp2+33>
0x40056b <+23>:  mov    -0x4(%rbp),%eax        # copy i vào %eax [loop]
0x40056e <+26>:  add    %eax,-0x8(%rbp)        # cộng i vào total (total += i)
0x400571 <+29>:  addl   $0x1,-0x4(%rbp)        # cộng 1 vào i (i++)
0x400575 <+33>:  mov    -0x4(%rbp),%eax        # copy i vào %eax [start]
0x400578 <+36>:  cmp    -0x14(%rbp),%eax       # so sánh i với n
0x40057b <+39>:  jle    0x40056b <sumUp2+23>   # nếu (i <= n) goto loop
0x40057d <+41>:  mov    -0x8(%rbp),%eax        # copy total vào %eax
0x400580 <+44>:  pop    %rbp                   # chuẩn bị thoát hàm
0x400581 <+45>:  retq                          # trả về total
```

Để hiểu vì sao phiên bản vòng lặp `for` này tạo ra code assembly giống hệt với phiên bản vòng lặp `while`, hãy nhớ rằng vòng lặp `for` có dạng:

```c
for (<khởi tạo>; <biểu thức điều kiện>; <bước lặp>) {
    <thân vòng lặp>
}
```

và tương đương với dạng vòng lặp `while` sau:

```c
<khởi tạo>
while (<biểu thức điều kiện>) {
    <thân vòng lặp>
    <bước lặp>
}
```

Vì [mọi vòng lặp `for` đều có thể được biểu diễn bằng vòng lặp `while`](../C1-C_intro/conditionals.html#_for_loops), nên hai chương trình C dưới đây là các cách viết tương đương cho đoạn assembly trên:

**For loop:**
```c
int sumUp2(int n) {
    int total = 0;
    int i = 1;
    for (i; i <= n; i++) {
        total += i;
    }
    return total;
}
```

**While loop:**
```c
int sumUp(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) {
        total += i;
        i += 1;
    }
    return total;
}
```

*Bảng 2. Các cách viết tương đương của hàm `sumUp`.*