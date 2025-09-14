
### 8.4.1. Mở đầu (Preliminaries)

### Các lệnh so sánh có điều kiện (Conditional Comparison Instructions)

Các lệnh so sánh thực hiện một phép toán số học nhằm phục vụ cho việc điều khiển thực thi có điều kiện của chương trình.  
**Bảng 1** liệt kê các lệnh cơ bản liên quan đến điều khiển có điều kiện.

| Instruction  | Translation                                      |
|--------------|--------------------------------------------------|
| `cmp R1, R2` | So sánh R2 với R1 (tức là tính R2 - R1)           |
| `test R1, R2`| Tính toán phép AND theo bit giữa R1 và R2         |

**Bảng 1.** Các lệnh điều khiển có điều kiện (Conditional Control Instructions)

Lệnh `cmp` so sánh giá trị của hai thanh ghi R2 và R1. Cụ thể, nó thực hiện phép trừ **R2 - R1**.  
Lệnh `test` thực hiện phép **AND theo bit**. Một ví dụ thường gặp là:

```asm
test %eax, %eax
```

Trong ví dụ này, phép AND theo bit của `%eax` với chính nó sẽ cho kết quả bằng 0 **chỉ khi** `%eax` chứa giá trị 0.  
Nói cách khác, đây là cách kiểm tra giá trị zero và tương đương với:

```asm
cmp $0, %eax
```

Không giống như các lệnh số học đã đề cập trước đây, `cmp` và `test` **không** thay đổi giá trị của thanh ghi đích.  
Thay vào đó, cả hai lệnh này sẽ thay đổi một tập hợp các giá trị 1-bit gọi là **cờ code điều kiện** (*condition code flags*).  

Ví dụ, `cmp` sẽ thay đổi các cờ này dựa trên việc kết quả của **R2 - R1** là số dương (lớn hơn), số âm (nhỏ hơn) hay bằng 0 (bằng nhau).  
Hãy nhớ rằng [condition code](../C5-Arch/cpu.html#_the_alu) lưu trữ thông tin về một phép toán trong ALU.  
Các cờ code điều kiện là một phần của thanh ghi `FLAGS` trên hệ thống x86.

| Flag  | Translation                                               |
|-------|-----------------------------------------------------------|
| `ZF`  | Bằng 0 (1: đúng; 0: sai)                                  |
| `SF`  | Âm (1: đúng; 0: sai)                                      |
| `OF`  | Xảy ra tràn số (1: đúng; 0: sai)                          |
| `CF`  | Xảy ra carry trong phép toán số học (1: đúng; 0: sai)     |

**Bảng 2.** Các cờ code điều kiện thường gặp.

**Giải thích lại lệnh `cmp R1, R2`:**

- `ZF` được đặt thành 1 nếu R1 và R2 bằng nhau.  
- `SF` được đặt thành 1 nếu R2 **nhỏ hơn** R1 (tức là R2 - R1 < 0).  
- `OF` được đặt thành 1 nếu phép toán R2 - R1 gây tràn số nguyên (hữu ích cho so sánh số có dấu).  
- `CF` được đặt thành 1 nếu phép toán R2 - R1 tạo ra carry (hữu ích cho so sánh số không dấu).

Các cờ `SF` và `OF` được dùng cho so sánh số nguyên **có dấu**, trong khi `CF` được dùng cho so sánh số nguyên **không dấu**.  
Mặc dù việc tìm hiểu sâu về các cờ này nằm ngoài phạm vi của sách, nhưng việc `cmp` và `test` thiết lập các cờ này là điều kiện cần để nhóm lệnh tiếp theo (*jump instructions*) hoạt động chính xác.

### Các lệnh nhảy (Jump Instructions)

Lệnh nhảy cho phép chương trình “nhảy” tới một vị trí mới trong code lệnh.  
Trong các chương trình assembly mà ta đã phân tích, `%eip` luôn trỏ tới lệnh kế tiếp trong bộ nhớ chương trình.  
Các lệnh nhảy cho phép `%eip` được đặt tới một lệnh mới chưa từng thực thi (như trong câu lệnh `if`) hoặc tới một lệnh đã thực thi trước đó (như trong vòng lặp).

#### Lệnh nhảy trực tiếp (Direct jump instructions)

| Instruction  | Description                                 |
|--------------|---------------------------------------------|
| `jmp L`      | Nhảy tới vị trí được chỉ định bởi nhãn L     |
| `jmp *addr`  | Nhảy tới địa chỉ được chỉ định               |

**Bảng 3.** Các lệnh nhảy trực tiếp.

**Bảng 3** liệt kê tập hợp các lệnh nhảy trực tiếp; trong đó `L` là một **symbolic label** (nhãn ký hiệu), đóng vai trò như một định danh trong tệp đối tượng (object file) của chương trình.  
Mọi nhãn đều bao gồm một số chữ cái và chữ số, theo sau là dấu hai chấm (`:`).  
Nhãn có thể là *local* (cục bộ) hoặc *global* (toàn cục) trong phạm vi của một tệp đối tượng.  

- Nhãn của hàm thường là *global* và thường bao gồm tên hàm kèm dấu hai chấm. Ví dụ: `main:` (hoặc `<main>:`) được dùng để đánh dấu hàm `main` do người dùng định nghĩa.  
- Theo quy ước, các nhãn có phạm vi *local* sẽ bắt đầu bằng dấu chấm (`.`). Bạn có thể bắt gặp một nhãn local như `.L1` trong ngữ cảnh của một câu lệnh `if` hoặc vòng lặp.

Mỗi nhãn đều có một địa chỉ liên kết. Khi CPU thực thi lệnh `jmp`, nó sẽ thay đổi `%eip` để trỏ tới địa chỉ chương trình được chỉ định bởi nhãn `L`.  
Lập trình viên viết assembly cũng có thể chỉ định một địa chỉ cụ thể để nhảy tới bằng lệnh `jmp *`.  

Đôi khi, các nhãn local được hiển thị dưới dạng **offset** (độ lệch) so với điểm bắt đầu của một hàm.  
Ví dụ, một lệnh nằm cách điểm bắt đầu của `main` 28 byte có thể được biểu diễn bằng nhãn `<main+28>`.

Ví dụ: lệnh `jmp 0x8048427 <main+28>` cho biết nhảy tới địa chỉ `0x8048427`, địa chỉ này có nhãn `<main+28>`, nghĩa là nó cách điểm bắt đầu của hàm `main` 28 byte.  
Khi thực thi lệnh này, `%eip` sẽ được đặt thành `0x8048427`.

#### Conditional jump instructions (Lệnh nhảy có điều kiện)

Hành vi của các lệnh nhảy có điều kiện phụ thuộc vào các thanh ghi **condition code** được thiết lập bởi lệnh `cmp`.  
**Bảng 4** liệt kê tập hợp các lệnh nhảy có điều kiện thường gặp.  
Mỗi lệnh bắt đầu bằng chữ `j` để biểu thị đây là lệnh nhảy.  
Phần hậu tố (suffix) của mỗi lệnh cho biết *điều kiện* để thực hiện nhảy.  
Hậu tố này cũng xác định việc so sánh số sẽ được hiểu là **có dấu** (signed) hay **không dấu** (unsigned).

| Signed Comparison    | Unsigned Comparison  | Mô tả                                      |
|----------------------|----------------------|---------------------------------------------|
| `je` (`jz`)          |                      | nhảy nếu bằng nhau (==) hoặc nếu bằng 0     |
| `jne` (`jnz`)        |                      | nhảy nếu không bằng (!=)                    |
| `js`                 |                      | nhảy nếu âm                                 |
| `jns`                |                      | nhảy nếu không âm                           |
| `jg` (`jnle`)        | `ja` (`jnbe`)        | nhảy nếu lớn hơn (>)                        |
| `jge` (`jnl`)        | `jae` (`jnb`)        | nhảy nếu lớn hơn hoặc bằng (>=)             |
| `jl` (`jnge`)        | `jb` (`jnae`)        | nhảy nếu nhỏ hơn (<)                        |
| `jle` (`jng`)        | `jbe` (`jna`)        | nhảy nếu nhỏ hơn hoặc bằng (<=)             |

**Bảng 4.** Các lệnh nhảy có điều kiện; các từ đồng nghĩa được ghi trong ngoặc.

Thay vì học thuộc lòng tất cả các lệnh này, bạn có thể **đọc thành tiếng** phần hậu tố để dễ nhớ hơn.  
**Bảng 5** liệt kê các chữ cái thường gặp trong hậu tố lệnh nhảy và ý nghĩa của chúng:

| Chữ cái | Ý nghĩa                                      |
|---------|----------------------------------------------|
| `j`     | jump                                         |
| `n`     | not                                          |
| `e`     | equal                                        |
| `s`     | signed                                       |
| `g`     | greater (so sánh có dấu)                     |
| `l`     | less (so sánh có dấu)                        |
| `a`     | above (so sánh không dấu)                    |
| `b`     | below (so sánh không dấu)                    |

**Bảng 5.** Hậu tố của lệnh nhảy.

Ví dụ:  
- `jg` nghĩa là *jump greater* (nhảy nếu lớn hơn), và từ đồng nghĩa dạng signed của nó là `jnl` (*jump not less*).  
- Phiên bản unsigned `ja` nghĩa là *jump above*, còn từ đồng nghĩa `jnbe` nghĩa là *jump not below or equal*.

Việc đọc thành tiếng giúp bạn hiểu tại sao một số từ đồng nghĩa lại tương ứng với các lệnh cụ thể.  
Ngoài ra, cần nhớ rằng các từ *greater* và *less* yêu cầu CPU hiểu phép so sánh là **có dấu**, trong khi *above* và *below* yêu cầu hiểu phép so sánh là **không dấu**.

### Câu lệnh `goto`

Trong các phần tiếp theo, chúng ta sẽ xem xét các câu lệnh điều kiện và vòng lặp trong assembly, sau đó dịch ngược chúng về C.  
Khi dịch ngược code assembly của các câu lệnh điều kiện và vòng lặp về C, việc hiểu dạng `goto` tương ứng trong C là rất hữu ích.

Câu lệnh `goto` là một primitive trong C, buộc chương trình nhảy tới một dòng khác trong code.  
Lệnh assembly tương ứng với `goto` là `jmp`.

Câu lệnh `goto` bao gồm từ khóa `goto` theo sau là một **goto label** (nhãn goto), là một loại nhãn chương trình cho biết vị trí tiếp tục thực thi.  
Ví dụ: `goto done` nghĩa là chương trình sẽ nhảy tới dòng có nhãn `done`.  

Các ví dụ khác về nhãn trong C bao gồm nhãn của câu lệnh [switch](../C2-C_depth/advanced_switch.html#_c_switch_stmt_) đã được đề cập trong Chương 2.

**Ví dụ so sánh:**

**Phiên bản C thông thường**

```c
int getSmallest(int x, int y) {
    int smallest;
    if (x > y) { // if (conditional)
        smallest = y; // then statement
    }
    else {
        smallest = x; // else statement
    }
    return smallest;
}
```

**Phiên bản dùng `goto`**

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

**Bảng 6.** So sánh một hàm C và dạng `goto` tương ứng của nó.

**Bảng 6** minh họa hàm `getSmallest()` được viết bằng cú pháp C thông thường và dạng `goto` tương ứng trong C.  
Hàm `getSmallest()` so sánh giá trị của hai số nguyên (`x` và `y`), và gán giá trị nhỏ hơn cho biến `smallest`.

Dạng `goto` của hàm này có thể trông hơi phản trực giác, nhưng hãy phân tích chính xác điều gì đang diễn ra.  
Câu lệnh điều kiện kiểm tra xem biến `x` có nhỏ hơn hoặc bằng `y` hay không.

- Nếu `x` nhỏ hơn hoặc bằng `y`, chương trình sẽ chuyển quyền điều khiển tới nhãn `else_statement`, nơi chứa duy nhất câu lệnh `smallest = x`.  
  Vì chương trình thực thi tuần tự, nó sẽ tiếp tục chạy phần code dưới nhãn `done`, trả về giá trị của `smallest` (tức `x`).

- Nếu `x` lớn hơn `y`, thì `smallest` được gán giá trị `y`.  
  Sau đó chương trình thực thi câu lệnh `goto done`, chuyển quyền điều khiển tới nhãn `done`, nơi trả về giá trị của `smallest` (tức `y`).

Mặc dù câu lệnh `goto` từng được sử dụng phổ biến trong những ngày đầu của lập trình, nhưng việc dùng nó trong code hiện đại được coi là **thói quen xấu**, vì nó làm giảm khả năng đọc hiểu của code.  
Trên thực tế, nhà khoa học máy tính **Edsger Dijkstra** đã viết một bài báo nổi tiếng chỉ trích việc sử dụng `goto` với tiêu đề *Go To Statement Considered Harmful*[^1^].

Nhìn chung, các chương trình C được thiết kế tốt sẽ **không** sử dụng câu lệnh `goto`, và lập trình viên được khuyến cáo tránh dùng nó để không tạo ra code khó đọc, khó gỡ lỗi và khó bảo trì.  
Tuy nhiên, việc hiểu câu lệnh `goto` trong C vẫn quan trọng, vì **GCC** thường chuyển đổi code C có chứa điều kiện sang dạng `goto` trước khi dịch sang assembly, bao gồm cả code có câu lệnh `if` và vòng lặp.

Các phần tiếp theo sẽ trình bày chi tiết hơn về cách biểu diễn câu lệnh `if` và vòng lặp trong assembly:

- [If Statements](if_statements.html#_if_statements_in_assembly)  
- Loops

### Tài liệu tham khảo

[^1^]: Edsger Dijkstra. *Go To Statement Considered Harmful*. *Communications of the ACM* 11(3), trang 147–148, 1968.


