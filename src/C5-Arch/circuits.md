## 5.4. Mạch điện

Các mạch số (digital circuits – "mạch số") thực hiện các chức năng cốt lõi của kiến trúc hệ thống. Chúng triển khai **Instruction Set Architecture** (ISA – "kiến trúc tập lệnh") dưới dạng phần cứng, đồng thời đảm nhiệm chức năng lưu trữ và điều khiển trong toàn bộ hệ thống.

Thiết kế mạch số đòi hỏi phải áp dụng nhiều cấp độ trừu tượng: các mạch thực hiện chức năng phức tạp được xây dựng từ các mạch nhỏ hơn đảm nhiệm một phần chức năng, và các mạch nhỏ này lại được xây dựng từ những mạch đơn giản hơn nữa, cho đến cấp độ các cổng logic cơ bản – là khối xây dựng nền tảng của mọi mạch số. [Hình 1](#Figcircuitabstraction) minh họa một mạch được trừu tượng hóa khỏi phần triển khai chi tiết. Mạch được biểu diễn dưới dạng một *hộp đen* (black box – "hộp đen") được gắn nhãn theo chức năng hoặc tên gọi, chỉ hiển thị đầu vào và đầu ra, còn phần triển khai bên trong được ẩn đi.

![an example circuit](_images/circuit.png)

**Hình 1. Một mạch được triển khai bằng cách liên kết các mạch con và cổng logic.** Chức năng của nó được trừu tượng hóa khỏi phần triển khai chi tiết và có thể được sử dụng như một khối xây dựng để tạo ra các mạch khác.

Có ba loại chính của các khối xây dựng mạch:

- mạch số học / logic,
- mạch điều khiển,
- và mạch lưu trữ.

Ví dụ, một vi mạch bộ xử lý (processor integrated circuit – "vi mạch xử lý") chứa cả ba loại mạch con này: tập hợp các thanh ghi sử dụng mạch lưu trữ; chức năng cốt lõi để thực hiện các phép toán số học và logic sử dụng mạch số học và logic; và các mạch điều khiển được sử dụng xuyên suốt trong bộ xử lý để điều khiển quá trình thực thi lệnh cũng như việc nạp và lưu giá trị vào các thanh ghi.

Trong phần này, ta sẽ tìm hiểu ba loại mạch nói trên, bắt đầu từ cách thiết kế một mạch cơ bản từ các cổng logic, sau đó là cách xây dựng các mạch lớn hơn từ các mạch cơ bản và cổng logic.

- [Mạch Số học / Logic](arithlogiccircs.html#_arithmetic_and_logic_circuits)
- Mạch Điều khiển
- Mạch Lưu trữ
