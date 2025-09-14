## 8.9. struct trong Assembly 

Một **struct** là một cách khác để tạo ra một tập hợp các kiểu dữ liệu trong C. Khác với **array** (mảng), **struct** cho phép nhóm các kiểu dữ liệu khác nhau lại với nhau. Ngôn ngữ C lưu trữ một `struct` giống như một **single-dimension array** (mảng một chiều), trong đó các phần tử dữ liệu (*field*) được lưu trữ liên tiếp nhau trong bộ nhớ.

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

Hình 1 minh họa cách `student` được bố trí trong bộ nhớ.  
Để đơn giản, giả sử `student` bắt đầu tại địa chỉ x~0~. Mỗi x~i~ biểu thị địa chỉ của một *field*.

![structArray](_images/structArray.png)

**Hình 1.** Sơ đồ bố trí bộ nhớ của struct `student`

Các *field* được lưu trữ liên tiếp nhau trong bộ nhớ theo đúng thứ tự mà chúng được khai báo. Trong Hình 1, *field* `age` được cấp phát ngay sau *field* `name` (tại byte offset x~64~), tiếp theo là `grad_yr` (byte offset x~68~) và `gpa` (byte offset x~72~). Cách tổ chức này cho phép truy cập các *field* một cách hiệu quả về mặt bộ nhớ.

Để hiểu cách **compiler** (trình biên dịch) sinh code Assembly để làm việc với một `struct`, hãy xem xét hàm `initStudent`:

```c
void initStudent(struct studentT *s, char *nm, int ag, int gr, float g) {
    strncpy(s->name, nm, 64);
    s->grad_yr = gr;
    s->age = ag;
    s->gpa = g;
}
```

Hàm `initStudent` sử dụng địa chỉ cơ sở của một `struct studentT` làm tham số đầu tiên, và các giá trị mong muốn cho từng *field* làm các tham số còn lại. Danh sách dưới đây mô tả hàm này ở dạng Assembly.  
Nói chung, tham số thứ *i* của hàm `initStudent` nằm tại địa chỉ stack `(ebp+8)` + 4 × *i*.

```
<initStudent>:
 <+0>:   push  %ebp                     # lưu ebp
 <+1>:   mov   %esp,%ebp                # cập nhật ebp (tạo stack frame mới)
 <+3>:   sub   $0x18,%esp               # cấp thêm 24 byte cho stack frame
 <+6>:   mov   0x8(%ebp),%eax           # copy tham số 1 (s) vào eax
 <+9>:   mov   0xc(%ebp),%edx           # copy tham số 2 (nm) vào edx
 <+12>:  mov   $0x40,0x8(%esp)          # copy 0x40 (64) vào esp+8
 <+16>:  mov   %edx,0x4(%esp)           # copy nm vào esp+4
 <+20>:  mov   %eax,(%esp)              # copy s lên đỉnh stack (esp)
 <+23>:  call  0x8048320 <strncpy@plt>  # gọi strncpy(s->name, nm, 64)
 <+28>:  mov   0x8(%ebp),%eax           # copy s vào eax
 <+32>:  mov   0x14(%ebp),%edx          # copy tham số 4 (gr) vào edx
 <+35>:  mov   %edx,0x44(%eax)          # copy gr vào offset eax+68 (s->grad_yr)
 <+38>:  mov   0x8(%ebp),%eax           # copy s vào eax
 <+41>:  mov   0x10(%ebp),%edx          # copy tham số 3 (ag) vào edx
 <+44>:  mov   %edx,0x40(%eax)          # copy ag vào offset eax+64 (s->age)
 <+47>:  mov   0x8(%ebp),%edx           # copy s vào edx
 <+50>:  mov   0x18(%ebp),%eax          # copy g vào eax
 <+53>:  mov   %eax,0x48(%edx)          # copy g vào offset edx+72 (s->gpa)
 <+56>:  leave                          # chuẩn bị thoát hàm
 <+57>:  ret                            # trả về
```


Việc chú ý đến **byte offset** (độ lệch tính theo byte) của từng *field* là chìa khóa để hiểu đoạn code này. Dưới đây là một số điểm cần lưu ý:

- Lời gọi `strncpy` nhận ba đối số: địa chỉ cơ sở của *field* `name` trong `s`, địa chỉ của mảng `nm`, và một giá trị chỉ định độ dài. Hãy nhớ rằng vì `name` là *field* đầu tiên trong `struct studentT`, nên địa chỉ của `s` cũng chính là địa chỉ của `s→name`.

```
 <+6>:   mov   0x8(%ebp),%eax           # copy tham số 1 (s) vào eax
 <+9>:   mov   0xc(%ebp),%edx           # copy tham số 2 (nm) vào edx
 <+12>:  mov   $0x40,0x8(%esp)          # copy 0x40 (64) vào esp+8
 <+16>:  mov   %edx,0x4(%esp)           # copy nm vào esp+4
 <+20>:  mov   %eax,(%esp)              # copy s lên đỉnh stack (esp)
 <+23>:  call  0x8048320 <strncpy@plt>  # gọi strncpy(s->name, nm, 64)
```

- Phần tiếp theo của code (các lệnh `<initStudent+28>` đến `<initStudent+35>`) đặt giá trị của tham số `gr` tại vị trí cách đầu `s` **68 byte**. Xem lại sơ đồ bố trí bộ nhớ trong Hình 1 cho thấy địa chỉ này tương ứng với `s→grad_yr`.

```
 <+28>:  mov   0x8(%ebp),%eax           # copy s vào eax
 <+32>:  mov   0x14(%ebp),%edx          # copy tham số 4 (gr) vào edx
 <+35>:  mov   %edx,0x44(%eax)          # copy gr vào offset eax+68 (s->grad_yr)
```

- Phần tiếp theo của code (các lệnh `<initStudent+38>` đến `<initStudent+53>`) sao chép tham số `ag` vào *field* `s→age`. Sau đó, giá trị của tham số `g` được sao chép vào *field* `s→gpa` (byte offset 72):

```
 <+38>:  mov   0x8(%ebp),%eax           # copy s vào eax
 <+41>:  mov   0x10(%ebp),%edx          # copy tham số 3 (ag) vào edx
 <+44>:  mov   %edx,0x40(%eax)          # copy ag vào offset eax+64 (s->age)
 <+47>:  mov   0x8(%ebp),%edx           # copy s vào edx
 <+50>:  mov   0x18(%ebp),%eax          # copy g vào eax
 <+53>:  mov   %eax,0x48(%edx)          # copy g vào offset edx+72 (s->gpa)
```

### 8.9.1. Data Alignment và struct

Xem xét khai báo `struct studentT` đã được chỉnh sửa như sau:

```c
struct studentTM {
    char name[63]; // cập nhật thành 63 thay vì 64
    int  age;
    int  grad_yr;
    float gpa;
};

struct studentTM student2;
```

Kích thước của *field* `name` được thay đổi thành 63 byte, thay vì 64 như ban đầu. Hãy xem điều này ảnh hưởng thế nào đến cách `struct` được bố trí trong bộ nhớ. Có thể bạn sẽ hình dung nó như trong [Hình 2](#wrongLayout32):

![struct2wrong](_images/struct2wrong.png)

**Hình 2.** Sơ đồ bố trí bộ nhớ **sai** cho `struct studentTM` đã chỉnh sửa. Lưu ý rằng *field* `name` của struct đã giảm từ 64 xuống 63 byte.

Trong hình minh họa này, *field* `age` chiếm ngay byte liền sau *field* `name`. Nhưng điều này là **sai**. [Hình 3](#correctLayout32) mô tả bố trí thực tế trong bộ nhớ:

![struct2right](_images/struct2right.png)

**Hình 3.** Sơ đồ bố trí bộ nhớ **đúng** cho `struct studentTM` đã chỉnh sửa. Byte x~63~ được **compiler** thêm vào để đáp ứng yêu cầu **memory alignment** (căn chỉnh bộ nhớ), nhưng nó không thuộc về bất kỳ *field* nào.

Chính sách căn chỉnh của IA32 yêu cầu:

- Kiểu dữ liệu 2 byte (ví dụ: `short`) phải nằm tại địa chỉ chia hết cho 2.
- Kiểu dữ liệu 4 byte (`int`, `float`, `long`, và các kiểu con trỏ) phải nằm tại địa chỉ chia hết cho 4.
- Kiểu dữ liệu 8 byte (`double`, `long long`) phải nằm tại địa chỉ chia hết cho 8.

Đối với một `struct`, **compiler** sẽ thêm các byte trống (**padding**) giữa các *field* để đảm bảo mỗi *field* đáp ứng yêu cầu căn chỉnh của nó. Ví dụ, trong `struct` ở đoạn code trên, **compiler** thêm một byte trống tại byte x~63~ để đảm bảo *field* `age` bắt đầu tại một địa chỉ chia hết cho 4. Khi dữ liệu được căn chỉnh đúng trong bộ nhớ, CPU có thể đọc hoặc ghi nó chỉ trong một thao tác, giúp tăng hiệu suất.

Xem xét trường hợp khi một `struct` được định nghĩa như sau:

```c
struct studentTM {
    int  age;
    int  grad_yr;
    float gpa;
    char name[63];
};

struct studentTM student3;
```

Việc chuyển mảng `name` xuống cuối sẽ dời byte **padding** về cuối `struct`, đảm bảo rằng `age`, `grad_yr` và `gpa` đều được căn chỉnh theo 4 byte.

