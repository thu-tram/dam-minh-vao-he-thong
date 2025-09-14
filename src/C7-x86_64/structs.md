## 7.9. structs trong Assembly

**Struct** là một cách khác để tạo một tập hợp các kiểu dữ liệu trong C.  
Không giống như mảng, struct cho phép nhóm các kiểu dữ liệu khác nhau lại với nhau.  
C lưu trữ một `struct` giống như một mảng một chiều, trong đó các phần tử dữ liệu (field) được lưu **liên tiếp** trong bộ nhớ.

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

**Hình 1** cho thấy cách `student` được bố trí trong bộ nhớ.  
Mỗi x~i~ biểu thị địa chỉ của một field cụ thể.

![structArray](_images/structArray.png)  
**Hình 1.** Cách bố trí bộ nhớ của struct `studentT`

Các field được lưu liên tiếp nhau trong bộ nhớ theo đúng thứ tự khai báo.  
Trong **Hình 1**, field `age` được cấp phát ngay sau field `name` (tại byte offset x~64~), tiếp theo là `grad_yr` (offset x~68~) và `gpa` (offset x~72~).  
Cách tổ chức này cho phép truy cập field hiệu quả về mặt bộ nhớ.

Để hiểu cách compiler sinh code assembly làm việc với một `struct`, hãy xét hàm `initStudent`:

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
Đoạn code assembly dưới đây thể hiện hàm này:

```
Dump of assembler code for function initStudent:
0x4006aa <+0>:  push  %rbp                   # lưu rbp
0x4006ab <+1>:  mov   %rsp,%rbp              # cập nhật rbp (stack frame mới)
0x4006ae <+4>:  sub   $0x20,%rsp             # thêm 32 byte vào stack frame
0x4006b2 <+8>:  mov   %rdi,-0x8(%rbp)        # copy tham số 1 vào %rbp-0x8 (s)
0x4006b6 <+12>: mov   %rsi,-0x10(%rbp)       # copy tham số 2 vào %rbp-0x10 (nm)
0x4006ba <+16>: mov   %edx,-0x14(%rbp)       # copy tham số 3 vào %rbp-0x14 (ag)
0x4006bd <+19>: mov   %ecx,-0x18(%rbp)       # copy tham số 4 vào %rbp-0x18 (gr)
0x4006c0 <+22>: movss %xmm0,-0x1c(%rbp)      # copy tham số 5 vào %rbp-0x1c (g)
0x4006c5 <+27>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006c9 <+31>: mov   -0x10(%rbp),%rcx       # copy nm vào %rcx
0x4006cd <+35>: mov   $0x40,%edx             # copy 0x40 (64) vào %edx
0x4006d2 <+40>: mov   %rcx,%rsi              # copy nm vào %rsi
0x4006d5 <+43>: mov   %rax,%rdi              # copy s vào %rdi
0x4006d8 <+46>: callq 0x400460 <strncpy@plt> # gọi strncpy(s->name, nm, 64)
0x4006dd <+51>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006e1 <+55>: mov   -0x18(%rbp),%edx       # copy gr vào %edx
0x4006e4 <+58>: mov   %edx,0x44(%rax)        # copy gr vào %rax+0x44 (s->grad_yr)
0x4006e7 <+61>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006eb <+65>: mov   -0x14(%rbp),%edx       # copy ag vào %edx
0x4006ee <+68>: mov   %edx,0x40(%rax)        # copy ag vào %rax+0x40 (s->age)
0x4006f1 <+71>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006f5 <+75>: movss -0x1c(%rbp),%xmm0      # copy g vào %xmm0
0x4006fa <+80>: movss %xmm0,0x48(%rax)       # copy g vào %rax+0x48 (s->gpa)
0x400700 <+86>: leaveq                       # chuẩn bị thoát hàm
0x400701 <+87>: retq                         # return (void, %rax bị bỏ qua)
```

Việc chú ý tới **byte offset** của từng field là chìa khóa để hiểu đoạn code này.  
Một số điểm cần lưu ý:

- Lời gọi `strncpy` nhận địa chỉ cơ sở của field `name` trong `s`, địa chỉ mảng `nm`, và độ dài cần copy làm ba tham số.  
  Hãy nhớ rằng vì `name` là field đầu tiên trong `struct studentT`, nên địa chỉ của `s` cũng chính là địa chỉ của `s->name`.

```
0x4006b2 <+8>:  mov   %rdi,-0x8(%rbp)        # copy tham số 1 vào %rbp-0x8 (s)
0x4006b6 <+12>: mov   %rsi,-0x10(%rbp)       # copy tham số 2 vào %rbp-0x10 (nm)
0x4006ba <+16>: mov   %edx,-0x14(%rbp)       # copy tham số 3 vào %rbp-0x14 (ag)
0x4006bd <+19>: mov   %ecx,-0x18(%rbp)       # copy tham số 4 vào %rbp-0x18 (gr)
0x4006c0 <+22>: movss %xmm0,-0x1c(%rbp)      # copy tham số 5 vào %rbp-0x1c (g)
0x4006c5 <+27>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006c9 <+31>: mov   -0x10(%rbp),%rcx       # copy nm vào %rcx
0x4006cd <+35>: mov   $0x40,%edx             # copy 0x40 (64) vào %edx
0x4006d2 <+40>: mov   %rcx,%rsi              # copy nm vào %rsi
0x4006d5 <+43>: mov   %rax,%rdi              # copy s vào %rdi
0x4006d8 <+46>: callq 0x400460 <strncpy@plt> # gọi strncpy(s->name, nm, 64)
```

- Đoạn code này chứa một thanh ghi (`%xmm0`) và lệnh (`movss`) chưa được đề cập trước đó. `%xmm0` là ví dụ về thanh ghi dành riêng cho giá trị **floating-point**. Lệnh `movss` cho biết dữ liệu được di chuyển là kiểu **floating-point đơn chính xác** (single-precision).

- Phần tiếp theo của code (từ `<initStudent+51>` đến `<initStudent+58>`) đặt giá trị của tham số `gr` tại offset `0x44` (68) tính từ đầu `s`.  
  Xem lại bố cục bộ nhớ trong **Hình 1** cho thấy địa chỉ này tương ứng với `s->grad_yr`:

```
0x4006dd <+51>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006e1 <+55>: mov   -0x18(%rbp),%edx       # copy gr vào %edx
0x4006e4 <+58>: mov   %edx,0x44(%rax)        # copy gr vào %rax+0x44 (s->grad_yr)
```

- Phần tiếp theo của code (từ `<initStudent+61>` đến `<initStudent+68>`) đặt giá trị của tham số `ag` tại offset `0x40` (64) tính từ đầu `s`.  
  Trong **Hình 1**, địa chỉ này tương ứng với `s->age`:

```
0x4006e7 <+61>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006eb <+65>: mov   -0x14(%rbp),%edx       # copy ag vào %edx
0x4006ee <+68>: mov   %edx,0x40(%rax)        # copy ag vào %rax+0x40 (s->age)
```

- Cuối cùng, các lệnh từ `<initStudent+71>` đến `<initStudent+80>` đặt giá trị của tham số `g` (điểm GPA) vào offset `0x48` (72) tính từ đầu `s`.  
  Offset này khớp với vị trí của field `gpa` trong **Hình 1**:

```
0x4006f1 <+71>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006f5 <+75>: movss -0x1c(%rbp),%xmm0      # copy g vào %xmm0
0x4006fa <+80>: movss %xmm0,0x48(%rax)       # copy g vào %rax+0x48 (s->gpa)
```

- Lệnh `leaveq` tại `<initStudent+86>` chuẩn bị stack để thoát khỏi hàm, và `retq` tại `<initStudent+87>` trả quyền điều khiển về cho hàm gọi. Vì đây là hàm `void`, giá trị trong `%rax` sẽ bị bỏ qua.

Như vậy, bằng cách quan sát các **byte offset** và cách compiler sử dụng chúng trong các lệnh `mov`, ta có thể thấy rõ cách các field của struct được truy cập và gán giá trị trong assembly.  
Điều này cũng cho thấy lợi ích của việc khai báo các field liên tiếp trong bộ nhớ: compiler chỉ cần cộng thêm offset cố định vào địa chỉ cơ sở của struct để truy cập từng field, giúp việc truy cập dữ liệu nhanh và hiệu quả hơn.


- Phần tiếp theo của đoạn code (từ `<initStudent+61>` đến `<initStudent+68>`) sao chép giá trị tham số `ag` vào field `s→age` của `struct`, field này nằm tại offset `0x40` (hoặc 64 byte) tính từ địa chỉ của `s`:

```
0x4006e7 <+61>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006eb <+65>: mov   -0x14(%rbp),%edx       # copy ag vào %edx
0x4006ee <+68>: mov   %edx,0x40(%rax)        # copy ag vào %rax+0x40 (s->age)
```

- Cuối cùng, giá trị tham số `g` được sao chép vào field `s→gpa` (byte offset 72 hoặc `0x48`) của `struct`. Lưu ý việc sử dụng thanh ghi `%xmm0` vì dữ liệu tại vị trí `%rbp-0x1c` là số thực dấu phẩy động đơn chính xác (single-precision floating point):

```
0x4006f1 <+71>: mov   -0x8(%rbp),%rax        # copy s vào %rax
0x4006f5 <+75>: movss -0x1c(%rbp),%xmm0      # copy g vào %xmm0
0x4006fa <+80>: movss %xmm0,0x48(%rax)       # copy g vào %rax+0x48
```

### 7.9.1. Data Alignment và structs

Xét khai báo `struct studentT` đã được chỉnh sửa như sau:

```c
struct studentTM {
    char name[63]; // thay đổi thành 63 thay vì 64
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentTM student2;
```

Kích thước của field `name` được thay đổi thành 63 byte, thay vì 64 byte như ban đầu. Hãy xem điều này ảnh hưởng thế nào đến cách `struct` được bố trí trong bộ nhớ. Có thể bạn sẽ hình dung nó như trong [Hình 2](#wrongLayout):

![struct2wrong](_images/struct2wrong.png)  
**Hình 2.** Cách bố trí bộ nhớ **sai** cho struct `studentTM` đã chỉnh sửa. Lưu ý rằng field `name` của struct được giảm từ 64 xuống 63 byte.

Trong hình minh họa này, field `age` xuất hiện ngay ở byte liền kề sau field `name`. Nhưng đây là **không đúng**. [Hình 3](#correctLayout) cho thấy bố trí thực tế trong bộ nhớ:

![struct2right](_images/struct2right.png)  
**Hình 3.** Cách bố trí bộ nhớ **đúng** cho struct `studentTM` đã chỉnh sửa. Byte x~63~ được compiler thêm vào để đáp ứng yêu cầu căn chỉnh bộ nhớ (memory alignment), nhưng nó không thuộc về bất kỳ field nào.

Chính sách căn chỉnh (alignment policy) của kiến trúc x64 yêu cầu:

- Các kiểu dữ liệu 2 byte (ví dụ `short`) phải nằm ở địa chỉ chia hết cho 2.
- Các kiểu dữ liệu 4 byte (ví dụ `int`, `float`, `unsigned`) phải nằm ở địa chỉ chia hết cho 4.
- Các kiểu dữ liệu lớn hơn (ví dụ `long`, `double`, và con trỏ) phải nằm ở địa chỉ chia hết cho 8.

Đối với một `struct`, compiler sẽ thêm các byte trống (**padding**) giữa các field để đảm bảo mỗi field thỏa coden yêu cầu căn chỉnh của nó.  
Ví dụ, trong `struct` được khai báo ở **Hình 3**, compiler thêm 1 byte padding tại byte x~63~ để đảm bảo field `age` bắt đầu ở một địa chỉ là bội số của 4.  
Các giá trị được căn chỉnh đúng trong bộ nhớ có thể được đọc hoặc ghi chỉ với một thao tác, giúp tăng hiệu suất.

Xét trường hợp khi một `struct` được định nghĩa như sau:

```c
struct studentTM {
    int  age;
    int  grad_yr;
    float gpa;
    char name[63];
};

struct studentTM student3;
```

Việc đưa mảng `name` xuống cuối struct sẽ dời byte padding xuống cuối struct, đảm bảo rằng `age`, `grad_yr` và `gpa` đều được căn chỉnh theo 4 byte.