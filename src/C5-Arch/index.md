## 5. Những gì von Neumann đã biết: Kiến trúc Máy tính

Thuật ngữ **computer architecture** (kiến trúc máy tính) có thể dùng để chỉ toàn bộ tầng phần cứng của máy tính. Tuy nhiên, nó thường được dùng để chỉ việc thiết kế và triển khai phần bộ xử lý số trong phần cứng máy tính, và trong chương này ta sẽ tập trung vào kiến trúc bộ xử lý máy tính.

**Central processing unit** (CPU – "bộ xử lý trung tâm", hay processor – "bộ xử lý") là phần của máy tính thực thi các lệnh chương trình trên dữ liệu chương trình. Lệnh chương trình và dữ liệu được lưu trong bộ nhớ truy cập ngẫu nhiên (RAM) của máy tính. Một bộ xử lý số cụ thể sẽ triển khai một **instruction set architecture** (ISA – "kiến trúc tập lệnh"), định nghĩa tập hợp các lệnh và cách mã hóa nhị phân của chúng, tập hợp các thanh ghi của CPU, và ảnh hưởng của việc thực thi lệnh lên trạng thái của bộ xử lý. Có nhiều ISA khác nhau, bao gồm SPARC, IA32, MIPS, ARM, ARC, PowerPC và x86 (trong đó x86 bao gồm IA32 và x86-64). Một **microarchitecture** (vi kiến trúc) định nghĩa mạch điện của một triển khai cụ thể của ISA. Các triển khai microarchitecture của cùng một ISA có thể khác nhau miễn là chúng tuân thủ định nghĩa của ISA. Ví dụ, Intel và AMD đều sản xuất các bộ vi xử lý triển khai ISA IA32 theo cách riêng.

Một số ISA định nghĩa **reduced instruction set computer** (RISC – "máy tính tập lệnh rút gọn"), trong khi số khác định nghĩa **complex instruction set computer** (CISC – "máy tính tập lệnh phức tạp"). ISA kiểu RISC có một tập nhỏ các lệnh cơ bản, mỗi lệnh thực thi rất nhanh; mỗi lệnh thường hoàn tất trong một chu kỳ xung nhịp của bộ xử lý, và trình biên dịch sẽ kết hợp nhiều lệnh RISC cơ bản để triển khai các chức năng cấp cao hơn. Ngược lại, lệnh trong ISA kiểu CISC cung cấp chức năng cấp cao hơn so với lệnh RISC. Kiến trúc CISC cũng định nghĩa tập lệnh lớn hơn RISC, hỗ trợ các chế độ định địa chỉ phức tạp hơn (cách biểu diễn vị trí dữ liệu trong bộ nhớ chương trình), và hỗ trợ lệnh có độ dài biến đổi. Một lệnh CISC đơn lẻ có thể thực hiện một chuỗi các chức năng cấp thấp và mất nhiều chu kỳ xung nhịp để hoàn tất. Cùng chức năng đó trên kiến trúc RISC sẽ cần nhiều lệnh hơn.

> Lịch sử của RISC và CISC

Vào đầu thập niên 1980, các nhà nghiên cứu tại Đại học Berkeley và Stanford đã phát triển RISC thông qua dự án Berkeley RISC và dự án Stanford MIPS. David Patterson (Berkeley) và John Hennessy (Stanford) đã nhận giải thưởng Turing năm 2017[^1] – giải thưởng cao nhất trong ngành điện toán – cho công trình phát triển kiến trúc RISC.

Tại thời điểm đó, kiến trúc RISC là một bước ngoặt so với quan điểm phổ biến rằng ISA cần ngày càng phức tạp để đạt hiệu năng cao. “Cách tiếp cận RISC khác với các máy tính CISC phổ biến lúc bấy giờ ở chỗ nó yêu cầu một tập nhỏ các lệnh đơn giản và tổng quát (các chức năng mà máy tính cần thực hiện), sử dụng ít transistor hơn so với tập lệnh phức tạp và giảm khối lượng công việc mà máy tính phải xử lý.”[^2]

ISA kiểu CISC biểu diễn chương trình bằng ít lệnh hơn so với RISC, thường dẫn đến tệp thực thi chương trình nhỏ hơn. Trên các hệ thống có bộ nhớ chính nhỏ, kích thước tệp thực thi là yếu tố quan trọng ảnh hưởng đến hiệu năng chương trình, vì tệp lớn sẽ chiếm nhiều không gian RAM, làm giảm bộ nhớ dành cho các phần khác của chương trình đang chạy. Các vi kiến trúc dựa trên CISC cũng thường được tối ưu hóa để thực thi hiệu quả các lệnh CISC có độ dài biến đổi và chức năng cao. Mạch điện chuyên biệt để thực thi các lệnh phức tạp có thể giúp thực thi hiệu quả hơn các chức năng cấp cao cụ thể, nhưng phải đánh đổi bằng việc tăng độ phức tạp cho toàn bộ quá trình thực thi lệnh.

Khi so sánh RISC với CISC, chương trình RISC có nhiều lệnh hơn để thực thi, nhưng mỗi lệnh lại được thực thi hiệu quả hơn hầu hết các lệnh CISC, và RISC cho phép thiết kế vi kiến trúc đơn giản hơn CISC. Chương trình CISC có ít lệnh hơn, và vi kiến trúc CISC được thiết kế để thực thi hiệu quả các lệnh phức tạp hơn, nhưng lại yêu cầu thiết kế vi kiến trúc phức tạp hơn và xung nhịp cao hơn. Nhìn chung, bộ xử lý RISC mang lại thiết kế hiệu quả hơn và hiệu năng tốt hơn. Khi dung lượng bộ nhớ máy tính ngày càng tăng, kích thước tệp thực thi không còn là yếu tố quan trọng đối với hiệu năng chương trình. Tuy nhiên, CISC vẫn là ISA chiếm ưu thế, phần lớn là do được ngành công nghiệp triển khai và hỗ trợ.

Hiện nay, CISC vẫn là ISA chủ đạo cho máy tính để bàn và nhiều máy chủ cấp cao. Ví dụ, các ISA x86 của Intel đều dựa trên CISC. ISA kiểu RISC phổ biến hơn trong các máy chủ cao cấp (ví dụ: SPARC) và thiết bị di động (ví dụ: ARM) nhờ yêu cầu năng lượng thấp. Một triển khai vi kiến trúc cụ thể của ISA kiểu RISC hoặc CISC có thể kết hợp cả thiết kế RISC và CISC bên trong. Ví dụ, hầu hết các bộ xử lý CISC sử dụng microcode để mã hóa một số lệnh CISC thành tập lệnh giống RISC hơn mà bộ xử lý bên dưới sẽ thực thi, và một số tập lệnh RISC hiện đại cũng chứa một vài lệnh phức tạp hoặc chế độ định địa chỉ nâng cao hơn so với tập lệnh MIPS và Berkeley RISC ban đầu.

Tất cả các bộ xử lý hiện đại, bất kể ISA của chúng là gì, đều tuân theo mô hình kiến trúc von Neumann. Thiết kế đa dụng của kiến trúc von Neumann cho phép nó thực thi bất kỳ loại chương trình nào. Nó sử dụng mô hình lưu trữ chương trình, nghĩa là lệnh chương trình được lưu trong bộ nhớ máy tính cùng với dữ liệu chương trình, và cả hai đều là đầu vào cho bộ xử lý.

Chương này giới thiệu kiến trúc von Neumann cùng với nguồn gốc và các thành phần nền tảng của kiến trúc máy tính hiện đại. Ta sẽ xây dựng một bộ xử lý số (CPU) ví dụ dựa trên mô hình kiến trúc von Neumann, thiết kế CPU từ các mạch số được xây dựng từ các khối cổng logic, và trình bày cách CPU thực thi các lệnh chương trình.

[^1]: ACM Turing Award 2017 – David Patterson & John Hennessy.  
[^2]: Hennessy, John & Patterson, David. *Computer Architecture: A Quantitative Approach*. Morgan Kaufmann, 2017.




