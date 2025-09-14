### 9.4.1. Kiến thức cơ bản (Preliminaries)  

### Lệnh so sánh có điều kiện (Conditional Comparison Instructions)  

Các lệnh so sánh thực hiện một phép toán số học nhằm phục vụ cho việc điều khiển thực thi có điều kiện của chương trình.  
**[Bảng 1](#ConditionalControla64)** liệt kê các lệnh cơ bản liên quan đến điều khiển có điều kiện.

| Instruction   | Translation |
|---------------|-------------|
| `cmp O1, O2`  | So sánh O1 với O2 (tính O1 - O2) |
| `tst O1, O2`  | Tính O1 `&` O2 |

**Bảng 1.** Các lệnh điều khiển có điều kiện  

Lệnh `cmp` so sánh giá trị của hai toán hạng O1 và O2, cụ thể là trừ O2 khỏi O1.  
Lệnh `tst` thực hiện phép **bitwise AND**. Một ví dụ thường gặp:

```
tst x0, x0
```

Trong ví dụ này, phép AND bitwise của `x0` với chính nó chỉ cho kết quả bằng 0 khi `x0` chứa giá trị 0.  
Nói cách khác, đây là phép kiểm tra giá trị bằng 0, tương đương với:

```
cmp x0, #0
```

Không giống như các lệnh số học đã đề cập trước đó, `cmp` và `tst` **không** ghi kết quả vào một thanh ghi đích.  
Thay vào đó, cả hai lệnh này sẽ thay đổi một tập hợp các giá trị 1-bit gọi là **condition code flags**.  

Ví dụ, `cmp` sẽ thay đổi các cờ điều kiện dựa trên việc phép tính O1 - O2 cho kết quả dương (lớn hơn), âm (nhỏ hơn) hoặc bằng 0 (bằng nhau).  
Hãy nhớ rằng [condition code](../C5-Arch/cpu.html#_the_alu) lưu trữ thông tin về một phép toán trong ALU.  
Các cờ điều kiện là một phần của trạng thái bộ xử lý ARM (`PSTATE`), thay thế cho thanh ghi trạng thái chương trình hiện tại (`CPSR`) trong hệ ARMv7-A.

| Flag | Translation |
|------|-------------|
| `Z`  | Bằng 0 (1: đúng, 0: sai) |
| `N`  | Âm (1: đúng, 0: sai) |
| `V`  | Xảy ra tràn số có dấu (1: có, 0: không) |
| `C`  | Xảy ra carry số học / tràn số không dấu (1: có, 0: không) |

**Bảng 2.** Các cờ điều kiện thường dùng  

Xét lại lệnh `cmp O1, O2`:

- Cờ `Z` được đặt thành 1 nếu O1 và O2 bằng nhau.  
- Cờ `N` được đặt thành 1 nếu O1 *nhỏ hơn* O2 (tức O1 - O2 cho kết quả âm).  
- Cờ `V` được đặt thành 1 nếu phép O1 - O2 gây tràn số (hữu ích cho so sánh có dấu).  
- Cờ `C` được đặt thành 1 nếu phép O1 - O2 gây carry số học (hữu ích cho so sánh không dấu).  

Việc tìm hiểu sâu về các cờ điều kiện nằm ngoài phạm vi của sách này, nhưng cần lưu ý rằng việc `cmp` và `tst` thiết lập các cờ này cho phép các lệnh tiếp theo (lệnh *branch*) hoạt động chính xác.

### Lệnh nhánh (The Branch Instructions)  

Lệnh nhánh cho phép chương trình “nhảy” tới một vị trí mới trong mã lệnh.  
Trong các chương trình assembly mà ta đã phân tích, `pc` luôn trỏ tới lệnh kế tiếp trong bộ nhớ chương trình.  
Lệnh nhánh cho phép `pc` được đặt tới một lệnh mới chưa thực thi (như trong câu lệnh `if`) hoặc tới một lệnh đã thực thi trước đó (như trong vòng lặp).

#### Lệnh nhánh trực tiếp (Direct branch instructions)  

| Instruction        | Description |
|--------------------|-------------|
| `b addr L`         | `pc` = addr |
| `br A`             | `pc` = A |
| `cbz R, addr L`    | Nếu R bằng 0, `pc` = addr (nhánh có điều kiện) |
| `cbnz R, addr L`   | Nếu R khác 0, `pc` = addr (nhánh có điều kiện) |
| `b.c addr L`       | Nếu điều kiện c đúng, `pc` = addr (nhánh có điều kiện) |

**Bảng 3.** Các lệnh nhánh thường dùng  

**Bảng 3** liệt kê tập hợp các lệnh nhánh thường dùng; trong đó `L` là một **symbolic label** (nhãn ký hiệu), đóng vai trò như một định danh trong tệp đối tượng (object file) của chương trình.  
Tất cả các nhãn đều bao gồm một số chữ cái và chữ số, theo sau là dấu hai chấm.  
Nhãn có thể là *local* (cục bộ) hoặc *global* (toàn cục) trong phạm vi của một tệp đối tượng.  

- Nhãn của hàm thường là *global* và thường bao gồm tên hàm kèm dấu hai chấm. Ví dụ: `main:` (hoặc `<main>:`) được dùng để đánh dấu hàm `main` do người dùng định nghĩa.  
- Theo quy ước, các nhãn có phạm vi *local* thường được đặt dấu chấm ở đầu. Bạn có thể bắt gặp một nhãn local như `.L1` trong ngữ cảnh của một câu lệnh `if` hoặc vòng lặp.

Mỗi nhãn đều có một địa chỉ liên kết (`addr` trong Bảng 3).  
Khi CPU thực thi lệnh `b`, nó sẽ đặt thanh ghi `pc` thành `addr`.  
Lệnh `b` cho phép bộ đếm chương trình (program counter) thay đổi trong phạm vi 128 MB so với vị trí hiện tại.  
Người lập trình assembly cũng có thể chỉ định một địa chỉ cụ thể để nhảy tới bằng lệnh `br`.  
Không giống `b`, lệnh `br` không bị giới hạn về phạm vi địa chỉ.

Đôi khi, các nhãn local cũng được hiển thị dưới dạng offset so với điểm bắt đầu của một hàm.  
Ví dụ, một lệnh nằm cách điểm bắt đầu của `main` 28 byte có thể được biểu diễn bằng nhãn `<main+28>`.  
Lệnh `b 0x7d0 <main+28>` nghĩa là nhảy tới địa chỉ `0x7d0`, nhãn `<main+28>`, tức là cách điểm bắt đầu của hàm `main` 28 byte.  
Khi thực thi lệnh này, `pc` sẽ được đặt thành `0x7d0`.

Ba lệnh cuối trong Bảng 3 là **conditional branch instructions** (lệnh nhánh có điều kiện).  
Nói cách khác, thanh ghi `pc` chỉ được đặt thành `addr` nếu điều kiện cho trước được đánh giá là đúng.  

- Với `cbz`, nếu thanh ghi R bằng 0, nhánh sẽ được thực hiện và `pc` được đặt thành `addr`.  
- Với `cbnz`, nếu thanh ghi R khác 0, nhánh sẽ được thực hiện và `pc` được đặt thành `addr`.

Mạnh mẽ nhất trong các lệnh nhánh có điều kiện là `b.c`, cho phép compiler hoặc lập trình viên assembly chọn một hậu tố (suffix) tùy chỉnh để chỉ ra điều kiện thực hiện nhánh.

#### Hậu tố lệnh nhánh có điều kiện (Conditional branch instruction suffixes)  

**Bảng 4** liệt kê các hậu tố nhánh có điều kiện thường gặp (ký hiệu `c`).  
Khi dùng với lệnh nhánh, mỗi lệnh bắt đầu bằng chữ `b` và dấu chấm, biểu thị đây là lệnh nhánh.  
Hậu tố `c` chỉ ra **điều kiện** để thực hiện nhánh.  
Các hậu tố này cũng xác định việc so sánh số sẽ được hiểu là có dấu (signed) hay không dấu (unsigned).  

Lưu ý: lệnh nhánh có điều kiện có phạm vi ngắn hơn nhiều (1 MB) so với lệnh `b`.  
Những hậu tố này cũng được dùng cho lệnh **conditional select** (`csel`), sẽ được đề cập ở phần tiếp theo.

| Signed Comparison | Unsigned Comparison | Mô tả |
|-------------------|---------------------|-------|
| `eq`              | `eq`                | Nhảy nếu bằng nhau (==) hoặc nếu bằng 0 |
| `ne`              | `ne`                | Nhảy nếu khác nhau (!=) |
| `mi`              | `mi`                | Nhảy nếu âm (negative) |
| `pl`              | `pl`                | Nhảy nếu không âm (>= 0) |
| `gt`              | `hi`                | Nhảy nếu lớn hơn (>) / cao hơn (higher) |
| `ge`              | `cs` (`hs`)         | Nhảy nếu lớn hơn hoặc bằng (>=) |
| `lt`              | `lo` (`cc`)         | Nhảy nếu nhỏ hơn (<) |
| `le`              | `ls`                | Nhảy nếu nhỏ hơn hoặc bằng (<=) |

**Bảng 4.** Hậu tố lệnh nhánh có điều kiện (các từ đồng nghĩa được ghi trong ngoặc)

### Câu lệnh goto  

Trong các phần tiếp theo, chúng ta sẽ xem xét các câu lệnh điều kiện và vòng lặp trong assembly và dịch ngược chúng về C.  
Khi dịch ngược mã assembly của các câu lệnh điều kiện và vòng lặp về C, việc hiểu dạng `goto` tương ứng trong C là rất hữu ích.  

Câu lệnh `goto` là một primitive trong C, buộc chương trình chuyển luồng thực thi sang một dòng khác trong mã.  
Lệnh assembly tương ứng với `goto` là `b`.

Cú pháp `goto` gồm từ khóa `goto` theo sau là một **goto label** — một loại nhãn chương trình chỉ ra rằng việc thực thi sẽ tiếp tục tại vị trí được đánh dấu bởi nhãn đó.  
Ví dụ: `goto done` nghĩa là chương trình sẽ nhảy tới dòng có nhãn `done`.  

Các ví dụ khác về nhãn trong C bao gồm nhãn của câu lệnh [switch](../C2-C_depth/advanced_switch.html#_c_switch_stmt_) đã được đề cập ở Chương 2.

Các đoạn mã dưới đây minh họa hàm `getSmallest` được viết bằng C thông thường (bên trái) và dạng `goto` tương ứng trong C (bên phải).  
Hàm `getSmallest` so sánh giá trị của hai số nguyên (`x` và `y`), và gán giá trị nhỏ hơn cho biến `smallest`.



#### Phiên bản C thông thường

```c
int getSmallest(int x, int y) {
    int smallest;

    if ( x > y ) { // if (conditional)
        smallest = y; // then statement
    }
    else {
        smallest = x; // else statement
    }
    return smallest;
}
```

#### Phiên bản dùng goto
```c
int getSmallest(int x, int y) {
    int smallest;

    if (x <= y) { // if (!conditional)
        goto else_statement;
    }

    smallest = y; // then statement
    goto done;

else_statement:
    smallest = x; // else statement

done:
    return smallest;
}
```

**Bảng 5.** So sánh một hàm C và dạng `goto` tương ứng.

Dạng `goto` của hàm này có thể trông hơi phản trực giác, nhưng hãy phân tích xem thực chất chuyện gì đang diễn ra.  
Câu lệnh điều kiện kiểm tra xem biến `x` có nhỏ hơn hoặc bằng `y` hay không.

- Nếu `x` nhỏ hơn hoặc bằng `y`, chương trình sẽ chuyển quyền điều khiển tới nhãn `else_statement`, nơi chứa duy nhất câu lệnh `smallest = x`. Vì chương trình thực thi tuần tự, nó sẽ tiếp tục chạy phần mã dưới nhãn `done`, trả về giá trị của `smallest` (`x`).

- Nếu `x` lớn hơn `y`, thì `smallest` được gán bằng `y`. Sau đó chương trình thực thi câu lệnh `goto done`, chuyển quyền điều khiển tới nhãn `done`, trả về giá trị của `smallest` (`y`).

Mặc dù câu lệnh `goto` từng được sử dụng phổ biến trong những ngày đầu của lập trình, nhưng trong mã hiện đại, việc dùng `goto` được xem là một **thói quen xấu**, vì nó làm giảm khả năng đọc hiểu của mã nguồn.  
Thực tế, nhà khoa học máy tính **Edsger Dijkstra** đã viết một bài báo nổi tiếng chỉ trích việc sử dụng `goto` với tiêu đề *Go To Statement Considered Harmful*[^1].

Nhìn chung, các chương trình C được thiết kế tốt sẽ **không** sử dụng `goto`, và lập trình viên được khuyến cáo tránh dùng nó để không tạo ra mã khó đọc, khó gỡ lỗi và khó bảo trì.  
Tuy nhiên, việc hiểu câu lệnh `goto` trong C vẫn quan trọng, vì GCC thường chuyển đổi mã C có chứa điều kiện sang dạng `goto` trước khi dịch sang assembly — bao gồm cả mã có câu lệnh `if` và vòng lặp.

Các phần tiếp theo sẽ trình bày chi tiết hơn về cách biểu diễn câu lệnh `if` và vòng lặp trong assembly:

- [If Statements](if_statements.html#_if_statements_in_assembly)  
- Loops  

### Tài liệu tham khảo

[^1]: Edsger Dijkstra. *Go To Statement Considered Harmful*. *Communications of the ACM* 11(3), trang 147–148, 1968.