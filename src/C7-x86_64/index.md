## 7. Assembly x86 64-bit (x86-64)

Trong chương này, chúng ta sẽ tìm hiểu về **Intel Architecture 64-bit** (x86-64) — **instruction set architecture** (ISA — kiến trúc tập lệnh). Hãy nhớ rằng một [instruction set architecture](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture) định nghĩa tập hợp các lệnh và cách code hóa nhị phân của một chương trình ở cấp độ máy.  
Để chạy các ví dụ trong chương này, bạn cần có quyền truy cập vào một máy tính với bộ xử lý x86 64-bit. Thuật ngữ “x86” thường được dùng đồng nghĩa với kiến trúc IA-32. Phần mở rộng 64-bit của kiến trúc này được gọi là **x86-64** (hoặc x64) và hiện diện phổ biến trong các máy tính hiện đại. Cả IA32 và x86-64 đều thuộc họ kiến trúc x86.

Để kiểm tra xem máy Linux của bạn có bộ xử lý Intel 64-bit hay không, hãy chạy lệnh `uname -m`. Nếu hệ thống của bạn là x86-64, bạn sẽ thấy kết quả như sau:

```
$ uname -m
x86_64
```

Vì x86-64 là phần mở rộng của ISA IA32 nhỏ hơn, một số bạn đọc có thể muốn tìm hiểu về IA32 trước. Để đọc thêm về IA32, hãy xem **Chương 8**.

> **Các nhánh cú pháp của x86**

Kiến trúc x86 thường tuân theo một trong hai nhánh cú pháp khác nhau.  
Các máy UNIX thường sử dụng cú pháp **AT&T**, vì UNIX được phát triển tại AT&T Bell Labs. **Assembler** (trình hợp dịch) tương ứng là **GNU Assembler** (GAS). Vì chúng ta sử dụng GCC cho hầu hết các ví dụ trong sách này, nên chương này sẽ trình bày cú pháp AT&T.  
Các máy Windows thường sử dụng cú pháp **Intel**, được dùng bởi **Microsoft Macro Assembler** (MASM). **Netwide Assembler** (NASM) là một ví dụ về assembler trên Linux sử dụng cú pháp Intel.  
Tranh luận về việc cú pháp nào “tốt hơn” là một trong những “cuộc chiến kinh điển” trong lĩnh vực này. Tuy nhiên, việc quen thuộc với cả hai cú pháp là hữu ích, vì lập trình viên có thể gặp bất kỳ cú pháp nào trong các tình huống khác nhau.
