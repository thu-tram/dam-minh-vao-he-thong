## 8. Assembly x86 32-bit (IA32)

Trong chương này, chúng ta sẽ tìm hiểu về **Intel Architecture 32-bit (IA32)** — kiến trúc tập lệnh (instruction set architecture) của Intel cho bộ xử lý 32-bit.  
Hãy nhớ rằng một [instruction set architecture](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture) (ISA) định nghĩa tập hợp các lệnh và cách mã hóa nhị phân của một chương trình ở cấp độ máy.  

Để chạy các ví dụ trong chương này, bạn sẽ cần truy cập vào một máy có bộ xử lý x86 hoặc một trình biên dịch có khả năng tạo ra các tệp thực thi 32-bit.  
Thuật ngữ **"x86"** thường được dùng đồng nghĩa với kiến trúc IA32.  
Kiến trúc x86 và biến thể 64-bit của nó (**x86-64**) hiện diện rộng rãi trong các máy tính hiện đại.

Rất ít máy tính hiện đại còn sử dụng bộ xử lý 32-bit; hầu hết các hệ thống Intel và AMD sản xuất từ năm 2007 trở đi đều dùng bộ xử lý 64-bit.  
Để kiểm tra loại bộ xử lý bạn đang dùng, hãy sử dụng lệnh:

```bash
$ uname -m
i686
```

Nếu gõ `uname -m` và kết quả trả về là `i686` hoặc `i386`, hệ thống của bạn đang dùng bộ xử lý 32-bit.  
Tuy nhiên, nếu kết quả trả về là `x86_64`, hệ thống của bạn đang dùng bộ xử lý 64-bit mới hơn.  

Lưu ý rằng vì **x86-64** là một *mở rộng* của ISA IA32 cũ, nên hầu như tất cả các hệ thống 64-bit đều chứa một **hệ thống con 32-bit** cho phép chạy các tệp thực thi 32-bit.

Nếu bạn đang dùng hệ thống Linux 64-bit, đôi khi cần cài đặt thêm các gói bổ sung để có thể *tạo* tệp thực thi 32-bit, như chúng ta sẽ làm trong chương này.  
Ví dụ, trên máy Ubuntu, bạn cần cài đặt thư viện phát triển 32-bit và các gói bổ sung để mở rộng GCC với tính năng cross-compiling:

```bash
sudo apt-get install libc6-dev-i386 gcc-multilib
```

### Nhánh cú pháp của x86

Kiến trúc x86 thường tuân theo một trong hai nhánh cú pháp khác nhau:

- **AT&T syntax**: thường được dùng trên các máy UNIX, vì UNIX được phát triển tại AT&T Bell Labs. Trình assembler tương ứng là **GNU Assembler (GAS)**. Vì chúng ta sử dụng GCC cho hầu hết các ví dụ trong sách này, chương này sẽ dùng cú pháp AT&T.
- **Intel syntax**: thường được dùng trên các máy Windows, với trình assembler **Microsoft Macro Assembler (MASM)**. **Netwide Assembler (NASM)** là một ví dụ về assembler trên Linux sử dụng cú pháp Intel.

Cuộc tranh luận về việc cú pháp nào “tốt hơn” là một trong những “cuộc chiến kinh điển” của lĩnh vực này.  
Tuy nhiên, việc quen thuộc với cả hai cú pháp là hữu ích, vì lập trình viên có thể gặp bất kỳ cú pháp nào trong các tình huống khác nhau.

