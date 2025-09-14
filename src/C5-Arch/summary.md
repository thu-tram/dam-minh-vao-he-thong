## 5.10. Tóm tắt

Trong chương này, chúng ta đã trình bày kiến trúc của máy tính, tập trung vào thiết kế và triển khai bộ xử lý (CPU) nhằm hiểu cách nó thực thi một chương trình. Các bộ xử lý hiện đại ngày nay đều dựa trên kiến trúc von Neumann, vốn định nghĩa một máy tính đa dụng với chương trình được lưu trữ trong bộ nhớ. Thiết kế đa dụng của kiến trúc von Neumann cho phép nó thực thi bất kỳ loại chương trình nào.

Để hiểu cách CPU thực thi các lệnh chương trình, ta đã xây dựng một CPU ví dụ, bắt đầu từ các khối xây dựng cơ bản là cổng logic để tạo thành các mạch số, từ đó triển khai một bộ xử lý số. Chức năng của bộ xử lý số được hình thành bằng cách kết hợp các mạch điều khiển, mạch lưu trữ và mạch số học/logic, và được điều khiển bởi mạch xung nhịp, vốn dẫn dắt các pha Fetch, Decode, Execute và WriteBack trong quá trình thực thi lệnh chương trình.

Mọi kiến trúc bộ xử lý đều triển khai một **instruction set architecture** (ISA – "kiến trúc tập lệnh"), định nghĩa tập hợp các lệnh CPU, tập hợp các thanh ghi CPU, và ảnh hưởng của việc thực thi lệnh lên trạng thái của bộ xử lý. Có rất nhiều ISA khác nhau, và thường có nhiều phiên bản vi xử lý khác nhau cùng triển khai một ISA nhất định. Các vi xử lý hiện đại ngày nay cũng sử dụng nhiều kỹ thuật để cải thiện hiệu năng, bao gồm thực thi kiểu pipeline, song song ở cấp độ lệnh (instruction-level parallelism), và thiết kế đa nhân (multicore).

Để tìm hiểu sâu và rộng hơn về kiến trúc máy tính, chúng tôi khuyến nghị bạn đọc thêm các giáo trình chuyên sâu về kiến trúc máy tính[^1].

### Ghi chú

[^1]: Một gợi ý là cuốn *Computer Organization and Design: The Hardware and Software Interface* của David A. Patterson và John L. Hennessy.