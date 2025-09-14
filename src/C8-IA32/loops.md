### 8.4.3. Vòng lặp trong assembly (Loops in assembly)

Tương tự như câu lệnh `if`, các vòng lặp trong assembly cũng được triển khai bằng **lệnh nhảy** (jump instruction).  
Tuy nhiên, vòng lặp cho phép **các lệnh được thực thi lặp lại** dựa trên kết quả của một điều kiện được đánh giá.

Hàm `sumUp` trong ví dụ dưới đây tính tổng tất cả các số nguyên dương từ 1 đến một số nguyên do người dùng nhập vào.  
Đoạn code này được viết **không tối ưu** để minh họa cách hoạt động của vòng lặp `while` trong C.

```c
int sumUp(int n) {
    // khởi tạo total và i
    int total = 0;
    int i = 1;

    while (i <= n) {  // khi i nhỏ hơn hoặc bằng n
        total += i;   // cộng i vào total
        i += 1;       // tăng i thêm 1
    }
    return total;
}
```

Khi biên dịch đoạn code này với tùy chọn `-m32` và dùng GDB để disassemble, ta thu được code assembly sau:

```asm
(gdb) disas sumUp
Dump of assembler code for function sumUp:
  0x804840b <+0>:   push   %ebp
  0x804840c <+1>:   mov    %esp,%ebp
  0x804840e <+3>:   sub    $0x10,%esp
  0x8048411 <+6>:   movl   $0x0,-0x8(%ebp)
  0x8048418 <+13>:  movl   $0x1,-0x4(%ebp)
  0x804841f <+20>:  jmp    0x804842b <sumUp+32>
  0x8048421 <+22>:  mov    -0x4(%ebp),%eax
  0x8048424 <+25>:  add    %eax,-0x8(%ebp)
  0x8048427 <+28>:  add    $0x1,-0x4(%ebp)
  0x804842b <+32>:  mov    -0x4(%ebp),%eax
  0x804842e <+35>:  cmp    0x8(%ebp),%eax
  0x8048431 <+38>:  jle    0x8048421 <sumUp+22>
  0x8048433 <+40>:  mov    -0x8(%ebp),%eax
  0x8048436 <+43>:  leave
  0x8048437 <+44>:  ret
```

Chúng ta sẽ không vẽ stack minh họa trong ví dụ này, nhưng bạn đọc nên tự thực hành để hiểu rõ hơn.  
Bây giờ, hãy phân tích đoạn assembly này theo từng phần.

#### **Năm lệnh đầu tiên**

Năm lệnh đầu tiên của hàm thiết lập stack để thực thi hàm:

```asm
0x804840b <+0>:   push   %ebp                 # lưu ebp lên stack
0x804840c <+1>:   mov    %esp,%ebp            # cập nhật ebp (tạo stack frame mới)
0x804840e <+3>:   sub    $0x10,%esp           # cấp phát 16 byte cho stack frame
0x8048411 <+6>:   movl   $0x0,-0x8(%ebp)      # gán 0 vào ebp-0x8 (total)
0x8048418 <+13>:  movl   $0x1,-0x4(%ebp)      # gán 1 vào ebp-0x4 (i)
```

Nhớ rằng các vị trí trên stack lưu **biến tạm** trong hàm.  
Trong phần giải thích này, ta gọi `%ebp - 0x8` là `total` và `%ebp - 0x4` là `i`.  
Tham số đầu vào `n` của hàm `sumUp` nằm tại `%ebp + 0x8`.

#### **Phần lõi của vòng lặp**

Bảy lệnh tiếp theo trong hàm `sumUp` là phần lõi của vòng lặp:

```asm
0x804841f <+20>:  jmp    0x804842b <sumUp+32>  # nhảy tới <sumUp+32>
0x8048421 <+22>:  mov    -0x4(%ebp),%eax       # copy i vào eax
0x8048424 <+25>:  add    %eax,-0x8(%ebp)       # total += i
0x8048427 <+28>:  add    $0x1,-0x4(%ebp)       # i += 1
0x804842b <+32>:  mov    -0x4(%ebp),%eax       # copy i vào eax
0x804842e <+35>:  cmp    0x8(%ebp),%eax        # so sánh i với n
0x8048431 <+38>:  jle    0x8048421 <sumUp+22>  # nếu i <= n thì quay lại <sumUp+22>
```

- Lệnh đầu tiên (`jmp`) nhảy thẳng tới `<sumUp+32>`, đặt `%eip` thành `0x804842b`.
- Tại `<sumUp+32>` và `<sumUp+35>`, giá trị `i` được copy vào `%eax` và so sánh với tham số `n`.
- Lệnh `cmp` thiết lập các cờ điều kiện để chuẩn bị cho lệnh `jle` tại `<sumUp+38>`.
- Nếu `i <= n`, lệnh `jle` sẽ nhảy về `<sumUp+22>` và thực hiện:
  - `mov -0x4(%ebp),%eax` → copy `i` vào `%eax`
  - `add %eax,-0x8(%ebp)` → cộng `i` vào `total`
  - `add $0x1,-0x4(%ebp)` → tăng `i` thêm 1
  - Quay lại so sánh `i` với `n` và lặp lại.

Nếu điều kiện `i <= n` **không** thỏa coden, `total` sẽ được đưa vào thanh ghi trả về `%eax` và hàm kết thúc.

#### **Bảng so sánh Assembly và dạng C dùng goto**

**Assembly**

```asm
<sumUp>:
<+0>:   push   %ebp
<+1>:   mov    %esp,%ebp
<+3>:   sub    $0x10,%esp
<+6>:   movl   $0x0,-0x8(%ebp)
<+13>:  movl   $0x1,-0x4(%ebp)
<+20>:  jmp    <sumUp+32>
<+22>:  mov    -0x4(%ebp),%eax
<+25>:  add    %eax,-0x8(%ebp)
<+28>:  addl   $0x1,-0x4(%ebp)
<+32>:  mov    -0x4(%ebp),%eax
<+35>:  cmp    0x8(%ebp),%eax
<+38>:  jle    <sumUp+22>
<+40>:  mov    -0x8(%ebp),%eax
<+43>:  leave
<+44>:  ret
```

**Dạng C dùng goto**

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


**Bảng 1.** Dịch hàm `sumUp` sang dạng C dùng `goto`.

Đoạn code trước đó cũng tương đương với phiên bản C **không** dùng câu lệnh `goto` như sau:

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

Vòng lặp chính trong hàm `sumUp` cũng có thể được viết lại dưới dạng vòng lặp `for`:

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

Phiên bản này tạo ra code assembly **giống hệt** với ví dụ vòng lặp `while`.  
Dưới đây là code assembly và chú thích từng dòng:

```asm
0x8048438 <+0>:  push   %ebp                  # lưu ebp
0x8048439 <+1>:  mov    %esp,%ebp             # cập nhật ebp (tạo stack frame mới)
0x804843b <+3>:  sub    $0x10,%esp            # cấp phát 16 byte cho stack frame
0x804843e <+6>:  movl   $0x0,-0x8(%ebp)       # gán 0 vào ebp-0x8 (total)
0x8048445 <+13>: movl   $0x1,-0x4(%ebp)       # gán 1 vào ebp-0x4 (i)
0x804844c <+20>: jmp    0x8048458 <sumUp2+32> # nhảy tới <sumUp2+32>
0x804844e <+22>: mov    -0x4(%ebp),%eax       # copy i vào %eax
0x8048451 <+25>: add    %eax,-0x8(%ebp)       # total += i
0x8048454 <+28>: addl   $0x1,-0x4(%ebp)       # i += 1
0x8048458 <+32>: mov    -0x4(%ebp),%eax       # copy i vào %eax
0x804845b <+35>: cmp    0x8(%ebp),%eax        # so sánh i với n
0x804845e <+38>: jle    0x804844e <sumUp2+22> # nếu i <= n thì quay lại <sumUp2+22>
0x8048460 <+40>: mov    -0x8(%ebp),%eax       # copy total vào %eax
0x8048463 <+43>: leave                        # chuẩn bị thoát hàm
0x8048464 <+44>: ret                          # trả về total
```

Để hiểu tại sao phiên bản `for` tạo ra code assembly giống hệt với phiên bản `while`, hãy nhớ rằng vòng lặp `for` có dạng:

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

Vì [mọi vòng lặp `for` đều có thể được biểu diễn bằng vòng lặp `while`](../C1-C_intro/conditionals.html#_for_loops), nên hai chương trình C dưới đây là các biểu diễn tương đương của đoạn assembly trên:

| **For loop** | **While loop** |
|--------------|----------------|
| ```c         | ```c           |
| int sumUp2(int n) {               | int sumUp(int n) {                 |
|     int total = 0;                |     int total = 0;                  |
|     int i = 1;                    |     int i = 1;                      |
|     for (i; i <= n; i++) {        |     while (i <= n) {                |
|         total += i;               |         total += i;                 |
|     }                             |         i += 1;                     |
|     return total;                 |     }                               |
| }                                 |     return total;                   |
| ```                               | }                                   |

**Bảng 2.** Các cách viết tương đương của hàm `sumUp`.

