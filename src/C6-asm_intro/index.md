## 6. Dưới lớp C: Lặn sâu vào Assembly

*Under the C, under the C*  

*Don't you know it's better*  

*Dealing with registers*  

*And assembly?*  

— Sebastian, có lẽ vậy  

Trước khi **compiler** (trình biên dịch) ra đời trong những ngày đầu của ngành khoa học máy tính, nhiều lập trình viên viết mã bằng **assembly language** (ngôn ngữ hợp ngữ), ngôn ngữ mô tả trực tiếp tập lệnh mà máy tính sẽ thực thi. Assembly language là cấp độ gần nhất mà lập trình viên có thể tiếp cận khi lập trình ở mức máy mà không phải viết trực tiếp bằng các chuỗi 1 và 0, và nó là dạng dễ đọc hơn của **machine code** (mã máy). Để viết được mã assembly hiệu quả, lập trình viên phải hiểu tường tận cách thức hoạt động của kiến trúc máy tính bên dưới.  

Sự ra đời của **compiler** đã thay đổi căn bản cách lập trình viên viết mã. Compiler dịch một ngôn ngữ lập trình có thể đọc được bởi con người (thường được viết bằng các từ tiếng Anh) sang một ngôn ngữ mà máy tính có thể hiểu (tức là machine code). Compiler thực hiện việc dịch này dựa trên các quy tắc của ngôn ngữ lập trình, đặc tả của hệ điều hành, và tập lệnh (instruction set) của máy, đồng thời cung cấp khả năng phát hiện lỗi và kiểm tra kiểu dữ liệu (type checking) trong quá trình dịch. Hầu hết các compiler hiện đại tạo ra mã assembly hiệu quả tương đương với mã assembly viết tay của những năm trước đây.  

### Lợi ích của việc học Assembly

Với tất cả những lợi ích mà compiler mang lại, có thể không rõ ràng tại sao việc học assembly lại hữu ích. Tuy nhiên, có một số lý do thuyết phục để học và hiểu mã assembly. Dưới đây là một vài ví dụ.  

#### 1. Mức trừu tượng cao che giấu các chi tiết giá trị của chương trình

Sự trừu tượng mà các ngôn ngữ lập trình bậc cao cung cấp là một lợi thế lớn trong việc giảm độ phức tạp của lập trình. Tuy nhiên, sự đơn giản hóa này cũng khiến lập trình viên dễ đưa ra các quyết định thiết kế mà không hiểu đầy đủ tác động của chúng ở mức máy. Thiếu kiến thức về assembly thường khiến lập trình viên không nắm bắt được những thông tin giá trị về cách chương trình chạy, và hạn chế khả năng hiểu rõ mã của mình thực sự đang làm gì.  

Ví dụ, hãy xem chương trình sau:  

```
#include <stdio.h>

int adder() {
    int a;
    return a + 2;
}

int assign() {
    int y = 40;
    return y;
}

int main(void) {
    int x;
    assign();
    x = adder();
    printf("x is: %d\n", x);
    return 0;
}
```

Kết quả của chương trình này là gì? Nhìn qua, hàm `assign` dường như không có tác dụng, vì giá trị trả về của nó không được lưu vào bất kỳ biến nào trong `main`. Hàm `adder` trả về giá trị `a + 2`, mặc dù biến `a` chưa được khởi tạo (dù trên một số máy, compiler sẽ khởi tạo `a` bằng 0). Việc in ra `x` lẽ ra sẽ cho kết quả không xác định. Tuy nhiên, khi biên dịch và chạy trên hầu hết các máy 64-bit, kết quả lại luôn là `42`:  

```
$ gcc -o example example.c
$ ./example
x is: 42
```

Kết quả này thoạt nhìn có vẻ vô lý, vì `adder` và `assign` dường như không liên quan. Việc hiểu về **stack frame** và cách các hàm thực thi ở tầng thấp sẽ giúp bạn lý giải tại sao kết quả lại là `42`. Chúng ta sẽ quay lại ví dụ này trong các chương tiếp theo.  

#### 2. Một số hệ thống máy tính bị giới hạn tài nguyên, không thể dùng compiler

Những loại “máy tính” phổ biến nhất lại thường là những thứ mà ta không dễ nhận ra là máy tính. Chúng xuất hiện ở khắp nơi, từ ô tô, máy pha cà phê, máy giặt cho đến đồng hồ thông minh. Các cảm biến, vi điều khiển (microcontroller) và các bộ xử lý nhúng (embedded processor) ngày càng đóng vai trò quan trọng trong cuộc sống, và tất cả đều cần phần mềm để hoạt động.  

Tuy nhiên, các bộ xử lý trong những thiết bị này thường rất nhỏ, đến mức không thể chạy mã đã biên dịch từ các ngôn ngữ lập trình bậc cao. Trong nhiều trường hợp, các thiết bị này yêu cầu các chương trình assembly độc lập, không phụ thuộc vào các thư viện runtime mà các ngôn ngữ lập trình phổ biến cần.  

---

Nếu bạn muốn, ở phần tiếp theo mình có thể dịch luôn các mục còn lại của chương này để bạn có bản tiếng Việt hoàn chỉnh và thống nhất về thuật ngữ. Bạn có muốn mình tiếp tục không?

#### 3. Phân tích lỗ hổng bảo mật (Vulnerability Analysis)

Một nhóm nhỏ các chuyên gia bảo mật dành thời gian của họ để tìm kiếm các **vulnerabilities** (lỗ hổng) trong nhiều loại hệ thống máy tính khác nhau. Nhiều hướng tấn công vào một chương trình liên quan đến cách chương trình lưu trữ thông tin **runtime** (thời gian chạy). Việc học **assembly** giúp các chuyên gia bảo mật hiểu được cách các lỗ hổng xuất hiện và cách chúng có thể bị khai thác.  

Một số chuyên gia bảo mật khác dành thời gian để **reverse engineering** (kỹ thuật đảo ngược) mã độc trong **malware** (phần mềm độc hại) và các phần mềm nguy hại khác. Kiến thức thực hành về assembly là điều thiết yếu để các kỹ sư phần mềm này có thể nhanh chóng phát triển các **countermeasures** (biện pháp đối phó) nhằm bảo vệ hệ thống trước các cuộc tấn công. Cuối cùng, các lập trình viên không hiểu cách mã họ viết được dịch sang assembly có thể vô tình viết ra mã dễ bị tấn công.  

---

#### 4. Các đoạn mã quan trọng trong phần mềm cấp hệ thống (Critical Code Sequences in System-Level Software)

Cuối cùng, có một số thành phần của hệ thống máy tính mà compiler không thể tối ưu hóa đủ tốt và cần phải viết assembly thủ công. Một số phần ở cấp hệ thống có mã assembly viết tay tại những khu vực mà tối ưu hóa chi tiết, phụ thuộc vào kiến trúc máy là rất quan trọng cho hiệu năng.  

Ví dụ, **boot sequence** (trình tự khởi động) trên tất cả các máy tính đều được viết bằng assembly. **Operating system** (hệ điều hành) thường chứa mã assembly viết tay cho việc **thread** hoặc **process context-switching** (chuyển ngữ cảnh luồng hoặc tiến trình). Con người thường có thể tạo ra mã assembly được tối ưu hóa tốt hơn compiler cho những đoạn mã ngắn nhưng quan trọng về hiệu năng này.  

---

### Bạn sẽ học gì trong các chương tiếp theo

Ba chương tiếp theo sẽ đề cập đến ba “hương vị” khác nhau của assembly.  
[Chương 7](../C7-x86_64/index.html#_x64_assembly_chapter) và [Chương 8](../C8-IA32/index.html#_IA32_assembly_chapter) nói về **x86_64** và dạng trước đó của nó, **IA32**.  
[Chương 9](../C9-ARM64/index.html#_a64_assembly_chapter) nói về **ARMv8-A assembly**, đây là **ISA** (Instruction Set Architecture — kiến trúc tập lệnh) được sử dụng trên hầu hết các thiết bị ARM hiện đại, bao gồm cả các máy tính bo mạch đơn như **Raspberry Pi**.  
[Chương 10](../C10-asm_takeaways/index.html#_assembly_summary) chứa phần tóm tắt và một số điểm then chốt khi học assembly.  

Mỗi “hương vị” assembly này triển khai một **ISA** khác nhau. Hãy nhớ rằng một [ISA](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture) định nghĩa tập lệnh và cách mã hóa nhị phân của chúng, tập các **CPU registers** (thanh ghi CPU), và tác động của việc thực thi các lệnh lên trạng thái của CPU và bộ nhớ.  

Trong ba chương tiếp theo, bạn sẽ thấy những điểm tương đồng chung giữa tất cả các ISA, bao gồm việc CPU registers được dùng làm toán hạng cho nhiều lệnh, và mỗi ISA cung cấp các loại lệnh tương tự nhau:  

1. **Instructions** cho các phép toán số học và logic, như phép cộng hoặc **bitwise AND**.  
2. **Instructions** cho điều khiển luồng (control flow) dùng để triển khai rẽ nhánh như `if-else`, vòng lặp, và lời gọi/trả về hàm.  
3. **Instructions** cho di chuyển dữ liệu (data movement) để nạp và lưu giá trị giữa CPU registers và bộ nhớ.  
4. **Instructions** để **push** và **pop** giá trị từ **stack**. Các lệnh này được dùng để triển khai **execution call stack**, nơi một **stack frame** mới (lưu trữ biến cục bộ và tham số của hàm đang chạy) được thêm vào đỉnh stack khi gọi hàm, và một frame được gỡ bỏ khỏi đỉnh stack khi hàm trả về.  

Một **C compiler** dịch mã nguồn C sang tập lệnh ISA cụ thể. Compiler dịch các câu lệnh C, bao gồm vòng lặp, `if-else`, lời gọi hàm và truy cập biến, sang một tập lệnh cụ thể được định nghĩa bởi ISA và được thực thi bởi CPU được thiết kế để chạy các lệnh từ ISA đó. Ví dụ, compiler dịch C sang lệnh **x86** để chạy trên bộ xử lý Intel x86, hoặc dịch C sang lệnh **ARM** để chạy trên bộ xử lý ARM.  

Khi đọc các chương thuộc phần assembly của cuốn sách, bạn có thể nhận thấy một số thuật ngữ quan trọng được định nghĩa lại và một số hình minh họa được lặp lại. Để hỗ trợ tốt nhất cho các giảng viên ngành Khoa học Máy tính, chúng tôi thiết kế mỗi chương có thể được sử dụng độc lập tại các trường cao đẳng và đại học. Mặc dù phần lớn nội dung trong mỗi chương là duy nhất, chúng tôi hy vọng những điểm chung giữa các chương sẽ giúp củng cố sự tương đồng giữa các “hương vị” assembly khác nhau trong tâm trí người đọc.  

Sẵn sàng học assembly chưa? Hãy bắt đầu ngay thôi! Truy cập các liên kết dưới đây để đến những chương mà bạn quan tâm:

(Chưa thêm đâu nhé...)