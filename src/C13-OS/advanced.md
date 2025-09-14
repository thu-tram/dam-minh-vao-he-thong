## 13.5. Tóm tắt và các chức năng khác của Hệ điều hành

Trong chương này, chúng ta đã tìm hiểu **hệ điều hành** (operating system – OS) là gì, cách nó hoạt động và vai trò của nó trong việc chạy các chương trình ứng dụng trên máy tính.  
Là lớp **system software** (phần mềm hệ thống) nằm giữa **computer hardware** (phần cứng máy tính) và **application programs** (chương trình ứng dụng), OS quản lý phần cứng một cách hiệu quả và triển khai các **abstraction** (trừu tượng hóa) giúp máy tính dễ sử dụng hơn.

Hệ điều hành triển khai hai abstraction quan trọng — **process** (tiến trình) và **virtual memory** (bộ nhớ ảo) — để hỗ trợ **multiprogramming** (đa chương trình, cho phép nhiều chương trình chạy đồng thời trên hệ thống).  
OS theo dõi tất cả các process trong hệ thống và trạng thái của chúng, đồng thời thực hiện **context switching** (chuyển ngữ cảnh) giữa các process đang chạy trên các lõi CPU.  
OS cũng cung cấp cơ chế để các process tạo process mới, thoát (exit) và giao tiếp với nhau.

Thông qua virtual memory, OS triển khai abstraction về một **không gian bộ nhớ ảo riêng** cho mỗi process.  
Abstraction này bảo vệ các process khỏi việc nhìn thấy tác động của các process khác khi cùng chia sẻ bộ nhớ vật lý của máy tính.  
**Paging** (phân trang) là một cách triển khai virtual memory, ánh xạ từng **page** (trang) trong không gian địa chỉ ảo của mỗi process tới các **frame** (khung) trong RAM vật lý.  
Virtual memory cũng giúp OS sử dụng RAM hiệu quả hơn; bằng cách coi RAM như một **cache** cho ổ đĩa, nó cho phép các page của không gian bộ nhớ ảo được lưu trữ trong RAM hoặc trên đĩa.

Trọng tâm của chương này là vai trò của OS trong việc chạy một chương trình, bao gồm các abstraction và cơ chế mà nó triển khai để chạy chương trình hiệu quả. Tuy nhiên, đây **không phải** là toàn bộ bức tranh.  
Còn rất nhiều lựa chọn triển khai, chi tiết kỹ thuật và vấn đề về **policy** (chính sách) liên quan đến process, quản lý process, virtual memory và quản lý bộ nhớ.  

Ngoài ra, OS còn triển khai nhiều abstraction, chức năng và chính sách quan trọng khác để quản lý và sử dụng máy tính.  
Ví dụ: OS triển khai abstraction **filesystem** (hệ thống tệp) để truy cập dữ liệu lưu trữ, các cơ chế bảo vệ và **security policy** (chính sách bảo mật) để bảo vệ người dùng và hệ thống, cũng như các **scheduling policy** (chính sách lập lịch) cho các tài nguyên phần cứng và phần mềm khác nhau.

Các hệ điều hành hiện đại cũng hỗ trợ **interprocess communication** (giao tiếp liên tiến trình), **networking** (mạng máy tính), và **parallel & distributed computing** (tính toán song song và phân tán).  
Ngoài ra, hầu hết OS ngày nay đều bao gồm hỗ trợ **hypervisor**, cho phép **virtualize** (ảo hóa) phần cứng hệ thống và cho phép **host OS** (hệ điều hành chủ) chạy nhiều **virtual guest OS** (hệ điều hành khách ảo).  

Virtualization cho phép host OS — vốn quản lý phần cứng máy tính — khởi động và chạy nhiều hệ điều hành khác trên chính nó, mỗi hệ điều hành có một **virtualized view** (góc nhìn ảo hóa) riêng về phần cứng bên dưới.  
Hỗ trợ hypervisor của host OS quản lý quá trình ảo hóa, bao gồm việc bảo vệ và chia sẻ tài nguyên vật lý giữa các guest OS.

Cuối cùng, hầu hết các OS cung cấp một mức độ **extensibility** (khả năng mở rộng) nhất định, cho phép người dùng (thường là **system administrator** – quản trị hệ thống) tinh chỉnh OS.  
Ví dụ: hầu hết các hệ thống kiểu Unix cho phép người dùng (thường yêu cầu quyền **root** hoặc **superuser**) thay đổi kích thước **OS buffer**, **cache**, **swap partition**, và lựa chọn từ nhiều **scheduling policy** khác nhau trong các **OS subsystem** và thiết bị phần cứng.  
Thông qua các thay đổi này, người dùng có thể tinh chỉnh hệ thống cho phù hợp với loại **application workload** (khối lượng công việc ứng dụng) mà họ chạy.

Các OS loại này thường hỗ trợ **loadable kernel module** (mô-đun nhân có thể nạp), là các đoạn code thực thi có thể được nạp vào **kernel** và chạy ở **kernel mode**.  
Loadable kernel module thường được dùng để bổ sung abstraction hoặc chức năng mới vào kernel, cũng như để nạp **device driver** (trình điều khiển thiết bị) vào kernel nhằm quản lý một phần cứng cụ thể.

Để tìm hiểu sâu và rộng hơn về hệ điều hành, chúng tôi khuyến nghị đọc các giáo trình hệ điều hành, chẳng hạn như *Operating Systems: Three Easy Pieces*¹.

### 13.5.1. Tài liệu tham khảo

1. [Operating Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/),  
   tác giả: Remzi H. Arpaci-Dusseau và Andrea C. Arpaci-Dusseau,  
   Arpaci-Dusseau Books, tháng 8 năm 2018.
