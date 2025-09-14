Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu và giữ nguyên toàn bộ phần code:

---

### 7.4.2. Câu lệnh if trong Assembly

Hãy cùng xem hàm `getSmallest` ở dạng assembly. Để tiện theo dõi, hàm được trích lại dưới đây:

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

Mã assembly tương ứng được trích xuất từ GDB trông tương tự như sau:

```
(gdb) disas getSmallest
Dump of assembler code for function getSmallest:
   0x40059a <+4>:   mov    %edi,-0x14(%rbp)
   0x40059d <+7>:   mov    %esi,-0x18(%rbp)
   0x4005a0 <+10>:  mov    -0x14(%rbp),%eax
   0x4005a3 <+13>:  cmp    -0x18(%rbp),%eax
   0x4005a6 <+16>:  jle    0x4005b0 <getSmallest+26>
   0x4005a8 <+18>:  mov    -0x18(%rbp),%eax
   0x4005ae <+24>:  jmp    0x4005b9 <getSmallest+35>
   0x4005b0 <+26>:  mov    -0x14(%rbp),%eax
   0x4005b9 <+35>:  pop    %rbp
   0x4005ba <+36>:  retq
```

Đây là một cách hiển thị khác của mã assembly so với những gì ta đã thấy trước đây. Ở đây, ta có thể thấy **địa chỉ** gắn với mỗi lệnh, nhưng không thấy **byte** mã máy. Lưu ý rằng đoạn assembly này đã được chỉnh sửa nhẹ để đơn giản hóa. Các lệnh thường xuất hiện khi tạo hàm (tức `push %rbp`, `mov %rsp, %rbp`) đã được lược bỏ. Theo quy ước, GCC đặt tham số thứ nhất và thứ hai của hàm vào các thanh ghi `%rdi` và `%rsi`. Vì các tham số của `getSmallest` có kiểu `int`, compiler đặt chúng vào các **component register** tương ứng là `%edi` và `%esi`. Để dễ theo dõi, ta sẽ gọi các tham số này lần lượt là `x` và `y`.

Hãy lần lượt phân tích một số dòng đầu tiên của đoạn mã assembly trên. Lưu ý rằng trong ví dụ này, chúng ta sẽ không vẽ stack một cách tường minh. Đây là một bài tập để bạn tự thực hành kỹ năng theo dõi stack.

- Lệnh `mov` đầu tiên sao chép giá trị trong thanh ghi `%edi` (tham số thứ nhất, `x`) và đặt nó vào vị trí bộ nhớ `%rbp-0x14` trên call stack. Instruction pointer (`%rip`) được đặt tới địa chỉ của lệnh tiếp theo, 0x40059d.

- Lệnh `mov` thứ hai sao chép giá trị trong thanh ghi `%esi` (tham số thứ hai, `y`) và đặt nó vào vị trí bộ nhớ `%rbp-0x18` trên call stack. `%rip` được cập nhật trỏ tới địa chỉ của lệnh tiếp theo, 0x4005a0.

- Lệnh `mov` thứ ba sao chép `x` vào thanh ghi `%eax`. `%rip` được cập nhật trỏ tới địa chỉ của lệnh tiếp theo trong chuỗi.

- Lệnh `cmp` so sánh giá trị tại vị trí `%rbp-0x18` (tham số thứ hai, `y`) với `x` và thiết lập các **condition code flag register** phù hợp. `%rip` tăng tới địa chỉ của lệnh tiếp theo, 0x4005a6.

- Lệnh `jle` tại địa chỉ 0x4005a6 cho biết nếu `x` nhỏ hơn hoặc bằng `y`, lệnh tiếp theo sẽ được thực thi ở vị trí `<getSmallest+26>` và `%rip` sẽ được đặt thành 0x4005b0. Ngược lại, `%rip` sẽ được đặt tới lệnh tiếp theo trong chuỗi, 0x4005a8.

Các lệnh tiếp theo sẽ phụ thuộc vào việc chương trình có thực hiện nhánh (jump) tại địa chỉ 0x4005a6 hay không.

**Trường hợp 1:** Nhánh *không* được thực hiện. Khi đó, `%rip` được đặt thành 0x4005a8 (`<getSmallest+18>`) và chuỗi lệnh sau sẽ chạy:

- Lệnh `mov -0x18(%rbp), %eax` tại `<getSmallest+18>` sao chép `y` vào `%eax`. `%rip` tăng lên 0x4005ae.

- Lệnh `jmp` tại `<getSmallest+24>` đặt `%rip` thành 0x4005b9.

- Các lệnh cuối cùng được thực thi là `pop %rbp` và `retq`, dọn dẹp stack và trả về từ lời gọi hàm. Trong trường hợp này, `y` nằm trong thanh ghi trả về.

**Trường hợp 2:** Nhánh được thực hiện tại `<getSmallest+16>`. Nói cách khác, lệnh `jle` đặt `%rip` thành 0x4005b0 (`<getSmallest+26>`). Khi đó, các lệnh tiếp theo là:

- Lệnh `mov -0x14(%rbp), %eax` tại địa chỉ 0x4005b0 sao chép `x` vào `%eax`. `%rip` tăng lên 0x4005b9.

- Các lệnh cuối cùng được thực thi là `pop %rbp` và `retq`, dọn dẹp stack và trả về giá trị trong thanh ghi trả về. Trong trường hợp này, `%eax` chứa `x`, và `getSmallest` trả về `x`.

Chúng ta có thể chú thích đoạn assembly trên như sau:


```
0x40059a <+4>:  mov %edi,-0x14(%rbp)          # copy x to %rbp-0x14
0x40059d <+7>:  mov %esi,-0x18(%rbp)          # copy y to %rbp-0x18
0x4005a0 <+10>: mov -0x14(%rbp),%eax          # copy x to %eax
0x4005a3 <+13>: cmp -0x18(%rbp),%eax          # compare x with y
0x4005a6 <+16>: jle 0x4005b0 <getSmallest+26> # if x<=y goto <getSmallest+26>
0x4005a8 <+18>: mov -0x18(%rbp),%eax          # copy y to %eax
0x4005ae <+24>: jmp 0x4005b9 <getSmallest+35> # goto <getSmallest+35>
0x4005b0 <+26>: mov -0x14(%rbp),%eax          # copy x to %eax
0x4005b9 <+35>: pop %rbp                      # restore %rbp (clean up stack)
0x4005ba <+36>: retq                          # exit function (return %eax)
```


Chuyển ngược đoạn assembly này về mã C sẽ cho ra:

<h2>Bảng 1:</h2>

<table>
  <thead>
    <tr>
      <th>Dạng goto</th>
      <th>Mã C đã dịch</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>
      <pre><code>int getSmallest(int x, int y) {
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
</code></pre>
      </td>
      <td>
      <pre><code>int getSmallest(int x, int y) {
    int smallest;
    if (x <= y) {
        smallest = x;
    } else {
        smallest = y;
    }
    return smallest;
}
</code></pre>
      </td>
    </tr>
  </tbody>
</table>

**Bảng 1.** Dịch `getSmallest()` sang dạng C dùng `goto` và dạng C thông thường.

Trong **Bảng 1**, biến `smallest` tương ứng với thanh ghi `%eax`. Nếu `x` nhỏ hơn hoặc bằng `y`, mã sẽ thực thi câu lệnh `smallest = x`, câu lệnh này gắn với nhãn `goto` là `assign_x` trong dạng `goto` của hàm. Ngược lại, câu lệnh `smallest = y` sẽ được thực thi. Nhãn `goto` `done` được dùng để chỉ ra rằng giá trị trong `smallest` sẽ được trả về.

Hãy lưu ý rằng bản dịch C ở trên của đoạn assembly có một chút khác biệt so với hàm `getSmallest` ban đầu. Những khác biệt này không quan trọng; khi xem xét kỹ cả hai hàm, ta thấy chúng tương đương về mặt logic. Tuy nhiên, compiler trước tiên sẽ chuyển bất kỳ câu lệnh `if` nào thành dạng `goto` tương đương, dẫn đến một phiên bản hơi khác nhưng vẫn tương đương. **Bảng 2** cho thấy dạng chuẩn của câu lệnh `if` và dạng `goto` tương đương:

<table>
  <thead>
    <tr>
      <th>C if statement</th>
      <th>Dạng goto tương đương của compiler</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>
      <pre><code>if (condition) {
    then_statement;
} else {
    else_statement;
}
</code></pre>
      </td>
      <td>
      <pre><code>if (!condition) {
    goto else;
}
    then_statement;
    goto done;
else:
    else_statement;
done:
</code></pre>
      </td>
    </tr>
  </tbody>
</table>

**Bảng 2.** Dạng chuẩn của câu lệnh `if` và dạng `goto` tương đương.

Khi dịch mã sang assembly, compiler sẽ tạo một lệnh nhảy (jump) khi điều kiện **đúng**. Điều này trái ngược với cấu trúc của câu lệnh `if`, nơi “nhảy” (tới `else`) xảy ra khi điều kiện **không** đúng. Dạng `goto` thể hiện rõ sự khác biệt về logic này.

Xét bản dịch `goto` ban đầu của hàm `getSmallest`, ta thấy rằng:

- `x <= y` tương ứng với `!condition`.
- `smallest = x` là `else_statement`.
- Dòng `smallest = y` là `then_statement`.
- Dòng cuối cùng của hàm là `return smallest`.

Viết lại phiên bản gốc của hàm với các chú thích ở trên sẽ cho:

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

Phiên bản này giống hệt với hàm `getSmallest` ban đầu. Hãy nhớ rằng một hàm được viết theo nhiều cách khác nhau ở mức mã C vẫn có thể được dịch ra cùng một tập lệnh assembly.

Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước đã nêu và giữ nguyên toàn bộ phần code:

---

#### Các lệnh cmov

Nhóm lệnh điều kiện cuối cùng mà chúng ta đề cập là **conditional move** (`cmov`). Các lệnh `cmp`, `test` và `jmp` thực hiện *conditional transfer of control* (chuyển điều khiển có điều kiện) trong một chương trình. Nói cách khác, luồng thực thi của chương trình sẽ rẽ nhánh theo nhiều hướng khác nhau. Điều này có thể gây bất lợi cho việc tối ưu hóa mã, vì các nhánh này thường tốn kém về hiệu năng.

Ngược lại, lệnh `cmov` thực hiện *conditional transfer of data* (chuyển dữ liệu có điều kiện). Nói cách khác, cả `then_statement` và `else_statement` của cấu trúc điều kiện đều được thực thi, và dữ liệu sẽ được đặt vào thanh ghi thích hợp dựa trên kết quả của điều kiện.

Việc sử dụng **biểu thức ba ngôi** (ternary expression) trong C thường khiến compiler sinh ra lệnh `cmov` thay vì các lệnh nhảy. Với câu lệnh if-then-else thông thường, biểu thức ba ngôi có dạng:

```c
result = (condition) ? then_statement : else_statement;
```

Hãy sử dụng dạng này để viết lại hàm `getSmallest` dưới dạng biểu thức ba ngôi. Lưu ý rằng phiên bản mới này hoạt động hoàn toàn giống với hàm `getSmallest` ban đầu:

```c
int getSmallest_cmov(int x, int y) {
    return x > y ? y : x;
}
```

Mặc dù thay đổi này có vẻ không lớn, nhưng hãy xem mã assembly được tạo ra. Hãy nhớ rằng tham số thứ nhất và thứ hai (`x` và `y`) lần lượt được lưu trong các thanh ghi `%edi` và `%esi`:

```
0x4005d7 <+0>:   push   %rbp             #save %rbp
0x4005d8 <+1>:   mov    %rsp,%rbp        #update %rbp
0x4005db <+4>:   mov    %edi,-0x4(%rbp)  #copy x to %rbp-0x4
0x4005de <+7>:   mov    %esi,-0x8(%rbp)  #copy y to %rbp-0x8
0x4005e1 <+10>:  mov    -0x8(%rbp),%eax  #copy y to %eax
0x4005e4 <+13>:  cmp    %eax,-0x4(%rbp)  #compare x and y
0x4005e7 <+16>:  cmovle -0x4(%rbp),%eax  #if (x <=y) copy x to %eax
0x4005eb <+20>:  pop    %rbp             #restore %rbp
0x4005ec <+21>:  retq                    #return %eax
```

Đoạn mã assembly này **không có lệnh nhảy**. Sau khi so sánh `x` và `y`, `x` sẽ được chuyển vào thanh ghi trả về chỉ khi `x` nhỏ hơn hoặc bằng `y`. Giống như các lệnh nhảy, hậu tố của lệnh `cmov` cho biết điều kiện mà việc chuyển dữ liệu có điều kiện sẽ xảy ra. **Bảng 3** liệt kê tập hợp các lệnh conditional move.

| Signed               | Unsigned             | Mô tả                              |
|----------------------|----------------------|-------------------------------------|
| `cmove` (`cmovz`)    |                      | move if equal (==)                  |
| `cmovne` (`cmovnz`)  |                      | move if not equal (!=)              |
| `cmovs`              |                      | move if negative                    |
| `cmovns`             |                      | move if non-negative                |
| `cmovg` (`cmovnle`)  | `cmova` (`cmovnbe`)  | move if greater (>)                 |
| `cmovge` (`cmovnl`)  | `cmovae` (`cmovnb`)  | move if greater than or equal (>=)  |
| `cmovl` (`cmovnge`)  | `cmovb` (`cmovnae`)  | move if less (<)                    |
| `cmovle` (`cmovng`)  | `cmovbe` (`cmovna`)  | move if less than or equal (<=)     |

**Bảng 3.** Các lệnh cmov.

Trong trường hợp của hàm `getSmallest` ban đầu, **trình tối ưu hóa nội bộ** của compiler (xem Chương 12) sẽ thay thế các lệnh nhảy bằng một lệnh `cmov` nếu bật tối ưu hóa cấp độ 1 (tức `-O1`):

```
#compiled with: gcc -O1 -o getSmallest getSmallest.c
<getSmallest>:
    0x400546 <+0>: cmp    %esi,%edi      #compare x and y
    0x400548 <+2>: mov    %esi,%eax      #copy y to %eax
    0x40054a <+4>: cmovle %edi,%eax      #if (x<=y) copy x to %eax
    0x40054d <+7>: retq                  #return %eax
```

Nói chung, **compiler** rất thận trọng khi tối ưu hóa các lệnh nhảy (**jump instructions**) thành các lệnh `cmov`, đặc biệt trong các trường hợp có liên quan đến **side effects** (tác dụng phụ) và giá trị con trỏ (**pointer values**). **Bảng 4** cho thấy hai cách viết tương đương của một hàm `incrementX`:


C code

```
int incrementX(int *x) {
    if (x != NULL) { //if x is not NULL
        return (*x)++; //increment x
    }
    else { //if x is NULL
        return 1; //return 1
    }
}
```

C ternary form:

```
int incrementX2(int *x){
    return x ? (*x)++ : 1;
}
```

**Bảng 4.** Hai hàm cố gắng tăng giá trị của số nguyên `x`.

Mỗi hàm nhận vào một con trỏ tới một số nguyên và kiểm tra xem nó có phải `NULL` hay không. Nếu `x` không phải `NULL`, hàm sẽ tăng giá trị được giải tham chiếu của `x` và trả về giá trị đó. Ngược lại, hàm sẽ trả về giá trị 1.

Thoạt nhìn, có thể bạn sẽ nghĩ rằng `incrementX2` sử dụng lệnh `cmov` vì nó dùng **biểu thức ba ngôi** (ternary expression). Tuy nhiên, cả hai hàm đều sinh ra **chính xác cùng một mã assembly**.

```
0x4005ed <+0>:   push   %rbp
0x4005ee <+1>:   mov    %rsp,%rbp
0x4005f1 <+4>:   mov    %rdi,-0x8(%rbp)
0x4005f5 <+8>:   cmpq   $0x0,-0x8(%rbp)
0x4005fa <+13>:  je     0x40060d <incrementX+32>
0x4005fc <+15>:  mov    -0x8(%rbp),%rax
0x400600 <+19>:  mov    (%rax),%eax
0x400602 <+21>:  lea    0x1(%rax),%ecx
0x400605 <+24>:  mov    -0x8(%rbp),%rdx
0x400609 <+28>:  mov    %ecx,(%rdx)
0x40060b <+30>:  jmp    0x400612 <incrementX+37>
0x40060d <+32>:  mov    $0x1,%eax
0x400612 <+37>:  pop    %rbp
0x400613 <+38>:  retq
```

Hãy nhớ rằng lệnh `cmov` *thực thi cả hai nhánh của điều kiện*. Nói cách khác, `x` sẽ luôn bị giải tham chiếu (**dereference**) bất kể điều kiện là gì. Xét trường hợp `x` là một con trỏ null: việc giải tham chiếu một con trỏ null sẽ dẫn đến **null pointer exception** trong mã, gây ra lỗi **segmentation fault**. Để loại bỏ hoàn toàn khả năng này, compiler chọn cách an toàn và sử dụng các lệnh nhảy.

