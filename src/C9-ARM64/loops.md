### 9.4.3. Vòng lặp trong Assembly  

Tương tự như câu lệnh `if`, các vòng lặp trong assembly cũng được triển khai bằng **branch instruction** (lệnh rẽ nhánh).  
Tuy nhiên, vòng lặp cho phép các lệnh được **thực thi lại** dựa trên kết quả của một điều kiện được đánh giá.

Hàm `sumUp` trong ví dụ dưới đây tính tổng tất cả các số nguyên dương từ 1 đến một số nguyên *n* do người dùng nhập.  
Đoạn code này được viết **không tối ưu** để minh họa cách hoạt động của vòng lặp `while` trong C.

```c
int sumUp(int n) {
    // khởi tạo total và i
    int total = 0;
    int i = 1;

    while (i <= n) {  // khi i nhỏ hơn hoặc bằng n
        total += i;   // cộng i vào total
        i++;          // tăng i thêm 1
    }
    return total;
}
```

Khi biên dịch và dùng GDB để disassemble, ta thu được mã assembly sau:

```
Dump of assembler code for function sumUp:
0x0724 <+0>:   sub   sp, sp, #0x20
0x0728 <+4>:   str   w0, [sp, #12]
0x072c <+8>:   str   wzr, [sp, #24]
0x0730 <+12>:  mov   w0, #0x1
0x0734 <+16>:  str   w0, [sp, #28]
0x0738 <+20>:  b     0x758 <sumUp+52>
0x073c <+24>:  ldr   w1, [sp, #24]
0x0740 <+28>:  ldr   w0, [sp, #28]
0x0744 <+32>:  add   w0, w1, w0
0x0748 <+36>:  str   w0, [sp, #24]
0x074c <+40>:  ldr   w0, [sp, #28]
0x0750 <+44>:  add   w0, w0, #0x1
0x0754 <+48>:  str   w0, [sp, #28]
0x0758 <+52>:  ldr   w1, [sp, #28]
0x075c <+56>:  ldr   w0, [sp, #12]
0x0760 <+60>:  cmp   w1, w0
0x0764 <+64>:  b.le  0x73c <sumUp+24>
0x0768 <+68>:  ldr   w0, [sp, #24]
0x076c <+72>:  add   sp, sp, #0x20
0x0770 <+76>:  ret
```

Trong ví dụ này, chúng ta sẽ **không** vẽ sơ đồ stack chi tiết, nhưng bạn nên tự thực hành để hiểu rõ hơn.

#### Năm lệnh đầu tiên  

Năm lệnh đầu tiên của hàm này thiết lập stack để thực thi hàm và lưu trữ một số giá trị tạm thời:

```
0x0724 <+0>:  sub  sp, sp, #0x20   // mở rộng stack thêm 32 byte (tạo stack frame mới)
0x0728 <+4>:  str  w0, [sp, #12]   // lưu n tại sp+12 (n)
0x072c <+8>:  str  wzr, [sp, #24]  // lưu 0 tại sp+24 (total)
0x0730 <+12>: mov  w0, #0x1        // w0 = 1
0x0734 <+16>: str  w0, [sp, #28]   // lưu 1 tại sp+28 (i)
```

Cụ thể, chúng thực hiện:

- Mở rộng call stack thêm 32 byte, đánh dấu frame mới.
- Lưu tham số đầu tiên (`n`) tại vị trí `sp + 12`.
- Lưu giá trị 0 tại `sp + 24` (biến `total`).
- Gán giá trị 1 vào thanh ghi `w0`.
- Lưu giá trị 1 tại `sp + 28` (biến `i`).

Hãy nhớ rằng các vị trí trên stack lưu **biến tạm thời** trong hàm.  
Để đơn giản, ta sẽ gọi vị trí `sp + 24` là `total` và `sp + 28` là `i`.  
Tham số đầu vào `n` của `sumUp` nằm tại địa chỉ `sp + 12`.  

Mặc dù các biến tạm thời được đặt trên stack, nhưng stack pointer **không thay đổi** sau khi thực thi lệnh đầu tiên (`sub sp, sp, #0x20`).

#### Trọng tâm của vòng lặp (The Heart of the Loop)  

12 lệnh tiếp theo trong hàm `sumUp` chính là **trọng tâm** của vòng lặp:

```
0x0738 <+20>: b     0x758 <sumUp+52>  // nhảy tới <sumUp+52>
0x073c <+24>: ldr   w1, [sp, #24]     // w1 = total
0x0740 <+28>: ldr   w0, [sp, #28]     // w0 = i
0x0744 <+32>: add   w0, w1, w0        // w0 = i + total
0x0748 <+36>: str   w0, [sp, #24]     // lưu (total + i) vào total
0x074c <+40>: ldr   w0, [sp, #28]     // w0 = i
0x0750 <+44>: add   w0, w0, #0x1      // w0 = i + 1
0x0754 <+48>: str   w0, [sp, #28]     // lưu (i+1) vào i (i++)
0x0758 <+52>: ldr   w1, [sp, #28]     // w1 = i
0x075c <+56>: ldr   w0, [sp, #12]     // w0 = n
0x0760 <+60>: cmp   w1, w0            // so sánh i và n
0x0764 <+64>: b.le  0x73c <sumUp+24>  // nếu (i <= n) thì nhảy tới <sumUp+24>
```

- Lệnh đầu tiên là một cú nhảy trực tiếp tới `<sumUp+52>`, đặt thanh ghi **program counter** (`pc`) thành địa chỉ `0x758`.  
- Hai lệnh tiếp theo (tại `<sumUp+52>` và `<sumUp+56>`) nạp `i` và `n` vào các thanh ghi `w1` và `w0`.  
- Lệnh `cmp` tại `<sumUp+60>` so sánh `i` và `n`, thiết lập các cờ điều kiện thích hợp. Thanh ghi `pc` sau đó trỏ tới lệnh tiếp theo (`0x764`).  
- Lệnh `b.le` tại `<sumUp+64>` thay giá trị của `pc` bằng `0x73c` nếu `i <= n`.

Nếu **nhánh được thực hiện** (tức `i <= n`), chương trình nhảy tới `<sumUp+24>` và thực thi các lệnh sau:

- `ldr` tại `<sumUp+24>` và `<sumUp+28>` nạp `total` và `i` vào `w1` và `w0`.  
- `add` tại `<sumUp+32>` cộng `total` với `i` và lưu kết quả vào `w0`.  
- `str` tại `<sumUp+36>` cập nhật `total` bằng giá trị trong `w0`.  
- `ldr` tại `<sumUp+40>` nạp `i` vào `w0`.  
- `add` tại `<sumUp+44>` cộng 1 vào `i` và lưu vào `w0`.  
- `str` tại `<sumUp+48>` cập nhật `i` bằng giá trị trong `w0`.  
- `ldr` tại `<sumUp+52>` và `<sumUp+56>` nạp `i` và `n` vào `w1` và `w0`.  
- `cmp` tại `<sumUp+60>` so sánh `i` với `n` và thiết lập cờ điều kiện.  
- `b.le` thực thi: nếu `i <= n`, chương trình quay lại `<sumUp+24>` và lặp lại các lệnh từ `<sumUp+24>` đến `<sumUp+64>`. Nếu không, `pc` được đặt thành `0x768` (`<sumUp+68>`).

Nếu **nhánh không được thực hiện** (`i > n`), các lệnh sau chạy:

```
0x0768 <+68>:  ldr   w0, [sp, #24]   // w0 = total
0x076c <+72>:  add   sp, sp, #0x20   // khôi phục stack
0x0770 <+76>:  ret                   // trả về w0 (total)
```

Các lệnh này sao chép `total` vào thanh ghi trả về `w0`, thu nhỏ stack (`sp`) để khôi phục call stack, và thoát hàm.  
Kết quả là hàm trả về `total`.

**Bảng 1** dưới đây so sánh mã assembly và dạng C sử dụng `goto` của hàm `sumUp`:

#### Assembly

```asm
<sumUp>:
  <+0>:   sub   sp, sp, #0x20
  <+4>:   str   w0, [sp, #12]
  <+8>:   str   wzr, [sp, #24]
  <+12>:  mov   w0, #0x1
  <+16>:  str   w0, [sp, #28]
  <+20>:  b     0x758 <sumUp+52>
  <+24>:  ldr   w1, [sp, #24]
  <+28>:  ldr   w0, [sp, #28]
  <+32>:  add   w0, w1, w0
  <+36>:  str   w0, [sp, #24]
  <+40>:  ldr   w0, [sp, #28]
  <+44>:  add   w0, w0, #0x1
  <+48>:  str   w0, [sp, #28]
  <+52>:  ldr   w1, [sp, #28]
  <+56>:  ldr   w0, [sp, #12]
  <+60>:  cmp   w1, w0
  <+64>:  b.le  0x73c <sumUp+24>
  <+68>:  ldr   w0, [sp, #24]
  <+72>:  add   sp, sp, #0x20
  <+76>:  ret
```

#### Dạng C với goto

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

**Bảng 1.** Dịch `sumUp()` sang dạng C sử dụng `goto`.

Đoạn code ở trên cũng tương đương với đoạn mã C sau đây **không** sử dụng câu lệnh `goto`:

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

Phiên bản này tạo ra **mã assembly giống hệt** với ví dụ vòng lặp `while`.  
Dưới đây là mã assembly của `sumUp2` kèm chú thích từng dòng:

```
Dump of assembler code for function sumUp2:
0x0774 <+0>:  sub   sp, sp, #0x20     // mở rộng stack thêm 32 byte (frame mới)
0x0778 <+4>:  str   w0, [sp, #12]     // lưu n tại sp+12 (n)
0x077c <+8>:  str   wzr, [sp, #24]    // lưu 0 tại sp+24 (total)
0x0780 <+12>: mov   w0, #0x1          // w0 = 1
0x0784 <+16>: str   w0, [sp, #28]     // lưu 1 tại sp+28 (i)
0x0788 <+20>: b     0x7a8 <sumUp2+52> // nhảy tới <sumUp2+52>
0x078c <+24>: ldr   w1, [sp, #24]     // w1 = total
0x0790 <+28>: ldr   w0, [sp, #28]     // w0 = i
0x0794 <+32>: add   w0, w1, w0        // w0 = total + i
0x0798 <+36>: str   w0, [sp, #24]     // lưu (total+i) vào total
0x079c <+40>: ldr   w0, [sp, #28]     // w0 = i
0x07a0 <+44>: add   w0, w0, #0x1      // w0 = i + 1
0x07a4 <+48>: str   w0, [sp, #28]     // lưu (i+1) vào i (i += 1)
0x07a8 <+52>: ldr   w1, [sp, #28]     // w1 = i
0x07ac <+56>: ldr   w0, [sp, #12]     // w0 = n
0x07b0 <+60>: cmp   w1, w0            // so sánh i và n
0x07b4 <+64>: b.le  0x78c <sumUp2+24> // nếu (i <= n) nhảy tới <sumUp2+24>
0x07b8 <+68>: ldr   w0, [sp, #24]     // w0 = total
0x07bc <+72>: add   sp, sp, #0x20     // khôi phục stack
0x07c0 <+76>: ret                     // trả về w0 (total)
```

Để hiểu tại sao phiên bản vòng lặp `for` này tạo ra mã assembly giống hệt với phiên bản vòng lặp `while`, hãy nhớ rằng vòng lặp `for` có dạng:

```c
for (<khởi tạo>; <biểu thức điều kiện>; <bước lặp>) {
    <thân vòng lặp>
}
```

Điều này tương đương với vòng lặp `while` sau:

```c
<khởi tạo>
while (<biểu thức điều kiện>) {
    <thân vòng lặp>
    <bước lặp>
}
```

Vì [mọi vòng lặp `for` đều có thể được biểu diễn bằng vòng lặp `while`](../C1-C_intro/conditionals.html#_for_loops), nên hai chương trình C dưới đây là các cách viết tương đương cho cùng một đoạn assembly ở trên:

#### For loop
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
#### While loop
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

**Bảng 2.** Các cách viết tương đương của hàm `sumUp`.


