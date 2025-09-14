## 9.9. structs trong Assembly  

Một `struct` là một cách khác để tạo ra một tập hợp các kiểu dữ liệu trong C.  
Không giống như mảng, `struct` cho phép nhóm nhiều kiểu dữ liệu khác nhau lại với nhau.  
C lưu trữ một `struct` giống như một mảng một chiều, trong đó các phần tử dữ liệu (các **field**) được lưu trữ liên tiếp nhau trong bộ nhớ.

Hãy cùng xem lại `struct studentT` từ Chương 1:

```c
struct studentT {
    char name[64];
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentT student;
```

**Hình 1** cho thấy cách `student` được bố trí trong bộ nhớ. Mỗi a~i~ biểu thị một offset trong bộ nhớ.

![structArray](_images/structArray.png)  

**Hình 1.** Sơ đồ bố trí bộ nhớ của một struct `studentT`.

Mỗi field được lưu liên tiếp nhau trong bộ nhớ theo đúng thứ tự khai báo.  
Trong Hình 1, field `age` được cấp phát ngay sau field `name` (tại byte offset a~64~), tiếp theo là `grad_yr` (byte offset a~68~) và `gpa` (byte offset a~72~).  
Cách tổ chức này cho phép truy cập các field một cách hiệu quả về mặt bộ nhớ.

Để hiểu cách compiler sinh code assembly làm việc với một `struct`, hãy xem xét hàm `initStudent`:

```c
void initStudent(struct studentT *s, char *nm, int ag, int gr, float g) {
    strncpy(s->name, nm, 64);
    s->grad_yr = gr;
    s->age = ag;
    s->gpa = g;
}
```

Hàm `initStudent` nhận địa chỉ cơ sở của một `struct studentT` làm tham số đầu tiên,  
và các giá trị mong muốn cho từng field làm các tham số còn lại.  
Danh sách dưới đây là code assembly tương ứng của hàm:

```
Dump of assembler code for function initStudent:
0x7f4 <+0>:  stp  x29, x30, [sp, #-48]!  // sp -= 48; lưu x29, x30 tại sp, sp+4
0x7f8 <+4>:  mov  x29, sp                // x29 = sp (frame pointer = stack pointer)
0x7fc <+8>:  str  x0, [x29, #40]         // lưu s tại x29 + 40
0x800 <+12>: str  x1, [x29, #32]         // lưu nm tại x29 + 32
0x804 <+16>: str  w2, [x29, #28]         // lưu ag tại x29 + 28
0x808 <+20>: str  w3, [x29, #24]         // lưu gr tại x29 + 24
0x80c <+24>: str  s0, [x29, #20]         // lưu g tại x29 + 20
0x810 <+28>: ldr  x0, [x29, #40]         // x0 = s
0x814 <+32>: mov  x2, #0x40              // x2 = 0x40 (64)
0x814 <+36>: ldr  x1, [x29, #32]         // x1 = nm
0x818 <+40>: bl   0x6e0 <strncpy@plt>    // gọi strncpy(s, nm, 64) (s->name)
0x81c <+44>: ldr  x0, [x29, #40]         // x0 = s
0x820 <+48>: ldr  w1, [x29, #24]         // w1 = gr
0x824 <+52>: str  w1, [x0, #68]          // lưu gr tại (s + 68) (s->grad_yr)
0x828 <+56>: ldr  x0, [x29, #40]         // x0 = s
0x82c <+60>: ldr  w1, [x29, #28]         // w1 = ag
0x830 <+64>: str  w1, [x0, #64]          // lưu ag tại (s + 64) (s->age)
0x834 <+68>: ldr  x0, [x29, #40]         // x0 = s
0x838 <+72>: ldr  s0, [x29, #20]         // s0 = g
0x83c <+80>: str  s0, [x0, #72]          // lưu g tại (s + 72) (s->gpa)
0x844 <+84>: ldp  x29, x30, [sp], #48    // khôi phục x29, x30; sp += 48
0x848 <+88>: ret                         // return (void)
```

Việc chú ý đến **byte offset** của từng field là chìa khóa để hiểu đoạn code này.  
Dưới đây là một vài điểm cần lưu ý:

- Lời gọi `strncpy` nhận ba đối số: địa chỉ cơ sở của field `name` trong `s`, địa chỉ của mảng `nm`, và một **length specifier** (chỉ định độ dài).  
  Hãy nhớ rằng vì `name` là field đầu tiên trong `struct studentT`, nên địa chỉ của `s` cũng chính là địa chỉ của `s→name`.

```
0x7fc <+8>:  str  x0, [x29, #40]         // lưu s tại x29 + 40
0x800 <+12>: str  x1, [x29, #32]         // lưu nm tại x29 + 32
0x804 <+16>: str  w2, [x29, #28]         // lưu ag tại x29 + 28
0x808 <+20>: str  w3, [x29, #24]         // lưu gr tại x29 + 24
0x80c <+24>: str  s0, [x29, #20]         // lưu g tại x29 + 20
0x810 <+28>: ldr  x0, [x29, #40]         // x0 = s
0x814 <+32>: mov  x2, #0x40              // x2 = 0x40 (64)
0x814 <+36>: ldr  x1, [x29, #32]         // x1 = nm
0x818 <+40>: bl   0x6e0 <strncpy@plt>    // gọi strncpy(s, nm, 64) (s->name)
```

- Đoạn code trên có sử dụng một thanh ghi chưa được đề cập trước đó (`s0`). Thanh ghi `s0` là ví dụ về thanh ghi dành riêng cho giá trị **floating point**.

- Phần tiếp theo (các lệnh `<initStudent+44>` đến `<initStudent+52>`) ghi giá trị của tham số `gr` vào vị trí cách đầu `s` 68 byte.  
  Xem lại sơ đồ bố trí bộ nhớ của struct trong Hình 1 cho thấy địa chỉ này tương ứng với `s→grad_yr`.

```
0x81c <+44>: ldr  x0, [x29, #40]         // x0 = s
0x820 <+48>: ldr  w1, [x29, #24]         // w1 = gr
0x824 <+52>: str  w1, [x0, #68]          // lưu gr tại (s + 68) (s->grad_yr)
```

- Phần tiếp theo (các lệnh `<initStudent+56>` đến `<initStudent+64>`) sao chép tham số `ag` vào field `s→age`, nằm tại offset 64 byte tính từ địa chỉ của `s`.

```
0x828 <+56>: ldr  x0, [x29, #40]         // x0 = s
0x82c <+60>: ldr  w1, [x29, #28]         // w1 = ag
0x830 <+64>: str  w1, [x0, #64]          // lưu ag tại (s + 64) (s->age)
```

- Cuối cùng, giá trị tham số `g` được sao chép vào field `s→gpa` (byte offset 72).  
  Lưu ý việc sử dụng thanh ghi `s0` vì dữ liệu tại vị trí `x29 + 20` là số thực dấu phẩy động đơn chính xác (*single-precision floating point*):

```
0x834 <+68>: ldr  x0, [x29, #40]         // x0 = s
0x838 <+72>: ldr  s0, [x29, #20]         // s0 = g
0x83c <+80>: str  s0, [x0, #72]          // lưu g tại (s + 72) (s->gpa)
```

### 9.9.1. Data Alignment và structs  

Xem xét khai báo `studentT` đã được chỉnh sửa như sau:

```c
struct studentTM {
    char name[63]; // thay đổi thành 63 thay vì 64
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentTM student2;
```

Kích thước của field `name` được thay đổi thành 63 byte thay vì 64 byte như ban đầu.  
Hãy xem điều này ảnh hưởng thế nào đến cách `struct` được bố trí trong bộ nhớ.  
Có thể bạn sẽ hình dung nó như trong [Hình 2](#incorrectLayouta64):

![struct2wrong](_images/struct2wrong.png)  

**Hình 2.** Bố trí bộ nhớ **sai** cho struct `studentTM` đã cập nhật. Lưu ý rằng field `"name"` giảm từ 64 xuống 63 byte.

Trong hình minh họa này, field `age` xuất hiện ngay sau field `name`. Nhưng điều này là **sai**.  
[Hình 3](#correctLayouta64) cho thấy bố trí thực tế trong bộ nhớ:

![struct2right](_images/struct2right.png)  

**Hình 3.** Bố trí bộ nhớ **đúng** cho struct `studentTM` đã cập nhật.  
Byte a~63~ được compiler thêm vào để đáp ứng yêu cầu **memory alignment**, nhưng nó không thuộc về bất kỳ field nào.

Chính sách **alignment** của A64 yêu cầu:
- Các kiểu dữ liệu 4 byte (ví dụ: `int`) phải nằm ở địa chỉ là bội số của 4.
- Các kiểu dữ liệu 64-bit (`long`, `double`, và con trỏ) phải nằm ở địa chỉ là bội số của 8.

Đối với một `struct`, compiler sẽ thêm các byte trống gọi là **padding** giữa các field để đảm bảo mỗi field thỏa coden yêu cầu alignment.  
Ví dụ, trong `struct` ở đoạn code trên, compiler thêm 1 byte padding tại byte a~63~ để đảm bảo field `age` bắt đầu ở địa chỉ là bội số của 4.  
Các giá trị được **align** đúng trong bộ nhớ có thể được đọc hoặc ghi chỉ với một thao tác, giúp tăng hiệu suất.

Xem điều gì xảy ra khi `struct` được định nghĩa như sau:

```c
struct studentTM {
    int  age;
    int  grad_yr;
    float gpa;
    char name[63];
};

struct studentTM student3;
```

Việc chuyển mảng `name` xuống cuối struct sẽ dời byte padding xuống cuối struct, đảm bảo `age`, `grad_yr` và `gpa` đều được **align** theo 4 byte.