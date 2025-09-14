### 7.4.1. Kiến thức chuẩn bị (Preliminaries)

### Các lệnh so sánh có điều kiện (Conditional Comparison Instructions)

Các **comparison instruction** (lệnh so sánh) thực hiện một phép toán số học nhằm phục vụ cho việc điều khiển thực thi có điều kiện của chương trình. [Bảng 1](#ConditionalControl) liệt kê các lệnh cơ bản liên quan đến **conditional control** (điều khiển có điều kiện).

| Instruction   | Translation |
|---------------|-------------|
| `cmp R1, R2`  | So sánh R2 với R1 (tức tính R2 - R1) |
| `test R1, R2` | Tính toán R1 & R2 (bitwise AND) |

**Bảng 1.** Các lệnh điều khiển có điều kiện.

Lệnh `cmp` so sánh giá trị của hai thanh ghi R2 và R1. Cụ thể, nó thực hiện phép trừ R1 khỏi R2.  
Lệnh `test` thực hiện phép **bitwise AND**. Một ví dụ thường gặp:

```
test %rax, %rax
```

Trong ví dụ này, phép AND từng bit của `%rax` với chính nó sẽ cho kết quả bằng 0 **chỉ khi** `%rax` chứa giá trị 0. Nói cách khác, đây là cách kiểm tra giá trị zero và tương đương với:

```
cmp $0, %rax
```

Không giống các lệnh số học đã đề cập trước đó, `cmp` và `test` **không** thay đổi thanh ghi đích. Thay vào đó, cả hai lệnh này thay đổi một tập hợp các giá trị 1-bit gọi là **condition code flags** (cờ code điều kiện).  
Ví dụ, `cmp` sẽ thay đổi các cờ này dựa trên việc giá trị R2 - R1 là dương (greater), âm (less) hay bằng 0 (equal). Hãy nhớ rằng [condition code](../C5-Arch/cpu.html#_the_alu) lưu trữ thông tin về một phép toán trong ALU. Các condition code flags là một phần của thanh ghi `FLAGS` trên hệ thống x86.

| Flag  | Translation |
|-------|-------------|
| `ZF`  | Bằng 0 (1: đúng; 0: sai) |
| `SF`  | Âm (1: đúng; 0: sai) |
| `OF`  | Xảy ra tràn số (1: đúng; 0: sai) |
| `CF`  | Xảy ra carry trong phép toán (1: đúng; 0: sai) |

**Bảng 2.** Các cờ condition code thông dụng.

Xét lại lệnh `cmp R1, R2`:

- `ZF` được đặt thành 1 nếu R1 và R2 bằng nhau.
- `SF` được đặt thành 1 nếu R2 *nhỏ hơn* R1 (R2 - R1 cho kết quả âm).
- `OF` được đặt thành 1 nếu phép R2 - R1 gây tràn số nguyên (hữu ích cho so sánh số có dấu).
- `CF` được đặt thành 1 nếu phép R2 - R1 gây carry (hữu ích cho so sánh số không dấu).

`SF` và `OF` được dùng cho so sánh số nguyên có dấu, trong khi `CF` được dùng cho so sánh số nguyên không dấu.  
Mặc dù việc tìm hiểu sâu về condition code flags vượt ra ngoài phạm vi cuốn sách này, nhưng việc `cmp` và `test` thiết lập các cờ này là điều kiện cần để nhóm lệnh tiếp theo (*jump instructions*) hoạt động chính xác.

### Các lệnh nhảy (Jump Instructions)

**Jump instruction** cho phép chương trình “nhảy” tới một vị trí mới trong code lệnh. Trong các chương trình assembly mà ta đã phân tích, `%rip` luôn trỏ tới lệnh kế tiếp trong bộ nhớ chương trình. Lệnh nhảy cho phép `%rip` được đặt tới một lệnh mới chưa từng thực thi (như trong câu lệnh `if`) hoặc tới một lệnh đã thực thi trước đó (như trong vòng lặp).

#### Lệnh nhảy trực tiếp (Direct jump instructions)

| Instruction  | Description |
|--------------|-------------|
| `jmp L`      | Nhảy tới vị trí được chỉ định bởi nhãn L |
| `jmp *addr`  | Nhảy tới địa chỉ được chỉ định |

**Bảng 3.** Các lệnh nhảy trực tiếp.

Trong đó, `L` là **symbolic label** (nhãn ký hiệu), đóng vai trò định danh trong tệp đối tượng của chương trình.  
Tất cả nhãn gồm một chuỗi chữ và số, theo sau là dấu hai chấm. Nhãn có thể là *local* hoặc *global* trong phạm vi tệp đối tượng. Nhãn của hàm thường là *global* và thường gồm tên hàm kèm dấu hai chấm, ví dụ `main:` (hoặc `<main>:`) dùng để đánh dấu hàm `main` do người dùng định nghĩa. Theo quy ước, nhãn *local* thường bắt đầu bằng dấu chấm, ví dụ `.L1` trong ngữ cảnh câu lệnh `if` hoặc vòng lặp.

Mỗi nhãn đều có một địa chỉ gắn liền. Khi CPU thực thi lệnh `jmp`, nó sẽ thay đổi `%rip` thành địa chỉ chương trình được chỉ định bởi nhãn `L`. Lập trình viên cũng có thể chỉ định trực tiếp một địa chỉ để nhảy tới bằng `jmp *`.  
Đôi khi, nhãn local được hiển thị dưới dạng offset so với đầu hàm. Ví dụ, một lệnh cách đầu hàm `main` 28 byte có thể được biểu diễn là `<main+28>`.

Ví dụ:  
`jmp 0x8048427 <main+28>` nghĩa là nhảy tới địa chỉ `0x8048427`, nhãn `<main+28>` cho biết nó cách địa chỉ bắt đầu của hàm `main` 28 byte. Thực thi lệnh này sẽ đặt `%rip` thành `0x8048427`.

#### Lệnh nhảy có điều kiện (Conditional jump instructions)

Hành vi của **conditional jump instruction** phụ thuộc vào các thanh ghi condition code được thiết lập bởi lệnh `cmp`. **Bảng 4** liệt kê các lệnh nhảy có điều kiện thông dụng.  
Mỗi lệnh bắt đầu bằng chữ `j` (jump). Hậu tố của lệnh cho biết *điều kiện* để nhảy. Hậu tố này cũng xác định việc so sánh số được hiểu là có dấu hay không dấu.

| Signed Comparison | Unsigned Comparison | Description |
|-------------------|---------------------|-------------|
| `je` (`jz`)       |                     | Nhảy nếu bằng nhau (==) hoặc nếu zero |
| `jne` (`jnz`)     |                     | Nhảy nếu không bằng (!=) |
| `js`              |                     | Nhảy nếu âm |
| `jns`             |                     | Nhảy nếu không âm |
| `jg` (`jnle`)     | `ja` (`jnbe`)        | Nhảy nếu lớn hơn (>) |
| `jge` (`jnl`)     | `jae` (`jnb`)        | Nhảy nếu lớn hơn hoặc bằng (>=) |
| `jl` (`jnge`)     | `jb` (`jnae`)        | Nhảy nếu nhỏ hơn (<) |
| `jle` (`jng`)     | `jbe` (`jna`)        | Nhảy nếu nhỏ hơn hoặc bằng (<=) |

**Bảng 4.** Các lệnh nhảy có điều kiện thông dụng.

**Bảng 4.** Các lệnh nhảy có điều kiện; các từ đồng nghĩa được hiển thị trong ngoặc đơn.

Thay vì ghi nhớ tất cả các lệnh nhảy có điều kiện khác nhau này, sẽ hữu ích hơn nếu bạn “đọc thành tiếng” các hậu tố (suffix) của lệnh. [Bảng 5](#JmpSuffixes) liệt kê các chữ cái thường gặp trong lệnh nhảy và từ tương ứng của chúng.

| Letter | Word |
|--------|------|
| `j`    | jump |
| `n`    | not |
| `e`    | equal |
| `s`    | signed |
| `g`    | greater (diễn giải theo số có dấu) |
| `l`    | less (diễn giải theo số có dấu) |
| `a`    | above (diễn giải theo số không dấu) |
| `b`    | below (diễn giải theo số không dấu) |

**Bảng 5.** Hậu tố của lệnh nhảy.

Khi đọc thành tiếng, ta có thể thấy `jg` tương ứng với *jump greater* và từ đồng nghĩa trong so sánh có dấu của nó là `jnl`, nghĩa là *jump not less*. Tương tự, phiên bản so sánh không dấu `ja` nghĩa là *jump above*, trong khi từ đồng nghĩa `jnbe` nghĩa là *jump not below or equal*.

Nếu bạn đọc thành tiếng các lệnh này, điều đó sẽ giúp giải thích tại sao một số từ đồng nghĩa lại tương ứng với những lệnh cụ thể. Một điều khác cần nhớ là các thuật ngữ *greater* và *less* yêu cầu CPU diễn giải phép so sánh số dưới dạng **có dấu**, trong khi *above* và *below* cho biết phép so sánh số là **không dấu**.

### Câu lệnh `goto`

Trong các tiểu mục tiếp theo, chúng ta sẽ xem xét các câu lệnh điều kiện và vòng lặp trong assembly và phân tích ngược chúng trở lại thành C. Khi dịch code assembly của các câu lệnh điều kiện và vòng lặp về C, sẽ hữu ích nếu hiểu được dạng `goto` tương ứng trong ngôn ngữ C.  

Câu lệnh `goto` là một primitive trong C, buộc chương trình chuyển luồng thực thi sang một dòng khác trong code. Lệnh assembly tương ứng với câu lệnh `goto` là `jmp`.

Câu lệnh `goto` bao gồm từ khóa `goto` theo sau là một **goto label** (nhãn goto), là một loại nhãn chương trình cho biết nơi thực thi sẽ tiếp tục. Ví dụ, `goto done` nghĩa là chương trình sẽ nhảy tới dòng được đánh dấu bằng nhãn `done`.  

Các ví dụ khác về nhãn chương trình trong C bao gồm [nhãn của câu lệnh switch](../C2-C_depth/advanced_switch.html#_c_switch_stmt_) đã được đề cập trong Chương 2.

<table>
  <thead>
    <tr>
      <th>Regular C version</th>
      <th>Goto version</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>
<pre><code>
int getSmallest(int x, int y) {
    int smallest;

    if ( x > y ) { //if (conditional)
        smallest = y; //then statement
    }
    else {
        smallest = x; //else statement
    }
    return smallest;
}
</code></pre>
      </td>
      <td>
<pre><code>
int getSmallest(int x, int y) {
    int smallest;

    if (x &lt;= y ) { //if (!conditional)
        goto else_statement;
    }

    smallest = y; //then statement
    goto done;
else_statement:
    smallest = x; //else statement
done:
    return smallest;
}
</code></pre>

  </tbody>
</table>


**Bảng 6.** So sánh một hàm C và dạng `goto` tương ứng.

**Bảng 6** minh họa hàm `getSmallest()` được viết bằng C thông thường và dạng `goto` tương ứng trong C. Hàm `getSmallest()` so sánh giá trị của hai số nguyên (`x` và `y`), và gán giá trị nhỏ hơn cho biến `smallest`.

Dạng `goto` của hàm này có thể trông hơi phản trực giác, nhưng hãy phân tích chính xác điều gì đang diễn ra. Câu lệnh điều kiện kiểm tra xem biến `x` có nhỏ hơn hoặc bằng `y` hay không.

- Nếu `x` nhỏ hơn hoặc bằng `y`, chương trình sẽ chuyển quyền điều khiển tới nhãn `else_statement`, nơi chứa câu lệnh duy nhất `smallest = x`. Vì chương trình thực thi tuần tự, nó sẽ tiếp tục thực thi phần code dưới nhãn `done`, trả về giá trị của `smallest` (`x`).

- Nếu `x` lớn hơn `y`, `smallest` được gán giá trị `y`. Sau đó, chương trình thực thi câu lệnh `goto done`, chuyển quyền điều khiển tới nhãn `done`, trả về giá trị của `smallest` (`y`).

Mặc dù câu lệnh `goto` từng được sử dụng phổ biến trong những ngày đầu của lập trình, nhưng việc sử dụng `goto` trong code hiện đại được coi là **thói quen xấu**, vì nó làm giảm khả năng đọc hiểu của code. Thực tế, nhà khoa học máy tính Edsger Dijkstra đã viết một bài báo nổi tiếng chỉ trích việc sử dụng `goto` với tiêu đề *Go To Statement Considered Harmful*[^1].

Nói chung, các chương trình C được thiết kế tốt sẽ không sử dụng `goto`, và lập trình viên được khuyến cáo tránh dùng để không viết ra code khó đọc, khó gỡ lỗi và khó bảo trì. Tuy nhiên, câu lệnh `goto` trong C vẫn quan trọng để hiểu, vì GCC thường chuyển đổi code C có điều kiện sang dạng `goto` trước khi dịch sang assembly, bao gồm cả code chứa câu lệnh `if` và vòng lặp.

Các tiểu mục tiếp theo sẽ trình bày chi tiết hơn về cách biểu diễn câu lệnh `if` và vòng lặp trong assembly:

- [If Statements](if_statements.html#_if_statements_in_assembly)  
- Loops

### Tài liệu tham khảo

[^1]: Edsger Dijkstra. "Go To Statement Considered Harmful". *Communications of the ACM* 11(3), trang 147–148, 1968.


