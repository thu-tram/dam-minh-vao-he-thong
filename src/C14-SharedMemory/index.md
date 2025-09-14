
## 14. Tận dụng bộ nhớ chia sẻ trong kỷ nguyên đa lõi (Leveraging Shared Memory in the Multicore Era)

*Thế giới đã thay đổi.*  

*Tôi cảm nhận điều đó trong lớp silica.*  

*Tôi cảm nhận điều đó trong transistor.*  

*Tôi nhìn thấy điều đó trong lõi.*  

\~ Xin lỗi Galadriel (*Chúa tể những chiếc nhẫn: Hiệp hội nhẫn thần*)  

Cho đến nay, phần thảo luận về kiến trúc của chúng ta tập trung vào một thế giới thuần **single-CPU** (CPU đơn).  
Nhưng thế giới đã thay đổi. CPU ngày nay có nhiều **core** (lõi), hay đơn vị tính toán (**compute unit**).  
Trong chương này, chúng ta sẽ thảo luận về **kiến trúc đa lõi** (**multicore architectures**) và cách tận dụng chúng để tăng tốc độ thực thi chương trình.

> **CPUs, Processors, and Cores**  
>  
> Trong nhiều trường hợp ở chương này, các thuật ngữ *processor* và *CPU* được dùng thay thế cho nhau.  
> Ở mức cơ bản, **processor** (bộ xử lý) là bất kỳ mạch điện nào thực hiện một số phép tính trên dữ liệu bên ngoài.  
> Theo định nghĩa này, **central processing unit** (CPU – bộ xử lý trung tâm) là một ví dụ của processor.  
> Một processor hoặc CPU có nhiều **compute core** được gọi là **multicore processor** hoặc **multicore CPU**.  
> **Core** là một đơn vị tính toán chứa nhiều thành phần tạo nên CPU cổ điển: một ALU, các thanh ghi (**registers**), và một phần bộ nhớ đệm (**cache**).  
> Mặc dù *core* khác với *processor*, nhưng không hiếm khi thấy hai thuật ngữ này được dùng thay thế cho nhau trong các tài liệu (đặc biệt là những tài liệu ra đời khi multicore processor vẫn còn được coi là mới mẻ).

Năm 1965, nhà sáng lập Intel – **Gordon Moore** – dự đoán rằng số lượng transistor trong một **mạch tích hợp** sẽ tăng gấp đôi mỗi năm.  
Dự đoán này, nay được biết đến với tên **Định luật Moore** (**Moore’s Law**), sau đó được điều chỉnh thành số lượng transistor tăng gấp đôi mỗi **hai** năm.  

Bất chấp sự tiến hóa của các công tắc điện tử từ transistor của Bardeen đến các transistor siêu nhỏ trên chip hiện đại, Định luật Moore vẫn đúng trong suốt 50 năm qua.  
Tuy nhiên, bước sang thiên niên kỷ mới, thiết kế bộ xử lý đã gặp phải một số **giới hạn hiệu năng** quan trọng:

- **Memory wall**: Sự cải tiến của công nghệ bộ nhớ không theo kịp tốc độ tăng của xung nhịp CPU, khiến bộ nhớ trở thành **nút thắt cổ chai** về hiệu năng.  
  Do đó, việc liên tục tăng tốc độ thực thi của CPU **không còn** cải thiện hiệu năng tổng thể của hệ thống.

- **Power wall**: Việc tăng số lượng transistor trên một bộ xử lý tất yếu làm tăng nhiệt độ và mức tiêu thụ điện năng, kéo theo chi phí cấp điện và làm mát hệ thống.  
  Với sự phổ biến của hệ thống đa lõi, **điện năng** giờ đây trở thành mối quan tâm hàng đầu trong thiết kế hệ thống máy tính.

Hai “bức tường” về điện năng và bộ nhớ đã buộc các kiến trúc sư máy tính phải thay đổi cách thiết kế bộ xử lý.  
Thay vì thêm nhiều transistor để tăng tốc độ thực thi một luồng lệnh duy nhất, họ bắt đầu thêm nhiều **compute core** vào một CPU.  

Compute core là các đơn vị xử lý đơn giản hơn, chứa ít transistor hơn CPU truyền thống và thường dễ chế tạo hơn.  
Kết hợp nhiều core trên một CPU cho phép CPU thực thi **nhiều** luồng lệnh độc lập cùng lúc.

> **More cores != better**  
>  
> Có thể bạn sẽ nghĩ rằng tất cả các core đều giống nhau và máy tính có càng nhiều core thì càng tốt.  
> Điều này **không hẳn** đúng!  
> Ví dụ: **graphics processing unit** (GPU – bộ xử lý đồ họa) có các core với số lượng transistor **ít hơn nhiều** so với CPU core, và được thiết kế chuyên biệt cho các tác vụ xử lý vector.  
> Một GPU điển hình có thể có **5.000** core hoặc hơn.  
> Tuy nhiên, GPU core bị giới hạn về loại phép toán mà chúng có thể thực hiện và **không phải lúc nào** cũng phù hợp cho tính toán tổng quát như CPU core.  
> Việc tính toán bằng GPU được gọi là **manycore computing**.  
> Trong chương này, chúng ta tập trung vào **multicore computing**.  
> Xem [Chương 15](../C15-Parallel/gpu.html#_GPUs) để tìm hiểu về manycore computing.




### Xem xét kỹ hơn: Có bao nhiêu lõi?

Hầu như tất cả các hệ thống máy tính hiện đại đều có nhiều **core** (lõi), bao gồm cả các thiết bị nhỏ như [Raspberry Pi](https://www.raspberrypi.org/).  
Xác định số lượng core trên một hệ thống là điều quan trọng để đo lường chính xác hiệu năng của các chương trình **multicore** (đa lõi).  

Trên máy tính Linux và macOS, lệnh `lscpu` cung cấp bản tóm tắt kiến trúc của hệ thống.  
Trong ví dụ sau, chúng tôi hiển thị kết quả của lệnh `lscpu` khi chạy trên một máy mẫu (một số phần được lược bỏ để nhấn mạnh các thông tin chính):

```bash
$ lscpu

Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                8
On-line CPU(s) list:   0-7
Thread(s) per core:    2
Core(s) per socket:    4
Socket(s):             1
Model name:            Intel(R) Core(TM) i7-3770 CPU @ 3.40GHz
CPU MHz:               1607.562
CPU max MHz:           3900.0000
CPU min MHz:           1600.0000
L1d cache:             32K
L1i cache:             32K
L2 cache:              256K
L3 cache:              8192K
...
```


Lệnh `lscpu` cung cấp nhiều thông tin hữu ích, bao gồm loại bộ xử lý, tốc độ core, và số lượng core.  
Để tính số lượng **physical core** (lõi vật lý/thực tế) trên hệ thống, nhân số lượng **socket** với số lượng core trên mỗi socket.  
Kết quả `lscpu` mẫu ở trên cho thấy hệ thống có **một socket** với **bốn core** trên mỗi socket, tức là tổng cộng **bốn physical core**.

#### Hyperthreading

Thoạt nhìn, có thể thấy hệ thống trong ví dụ trước có **tám core**.  
Xét cho cùng, đây là điều mà trường `"CPU(s)"` dường như ngụ ý.  
Tuy nhiên, trường này thực tế cho biết số lượng **hyperthreaded core** (lõi logic), chứ không phải số lượng physical core.  

**Hyperthreading** (hay **simultaneous multithreading – SMT**) cho phép xử lý hiệu quả nhiều **thread** trên cùng một core.  
Mặc dù hyperthreading có thể giảm tổng thời gian chạy của một chương trình, hiệu năng trên hyperthreaded core **không** tăng tỷ lệ thuận như trên physical core.  

Tuy nhiên, nếu một tác vụ bị **idle** (nhàn rỗi, ví dụ do [control hazard](../C5-Arch/pipelining_advanced.html#_pipelining_hazards_control_hazards)), một tác vụ khác vẫn có thể tận dụng core đó.  

Tóm lại, hyperthreading được giới thiệu để cải thiện **process throughput** (lượng tiến trình hoàn thành trong một đơn vị thời gian), chứ không phải **process speedup** (mức cải thiện thời gian chạy của một tiến trình đơn lẻ).  
Phần lớn nội dung thảo luận về hiệu năng trong chương tiếp theo sẽ tập trung vào **speedup**.

#### Performance Cores và Efficiency Cores

Trên một số kiến trúc mới hơn (chẳng hạn như bộ xử lý Intel thế hệ 12 trở lên), phép nhân số lượng socket với số lượng core và **hardware thread** cho ra một con số khác (thường nhỏ hơn) so với giá trị hiển thị trong trường `"CPU(s)"`.  
Điều gì đang xảy ra ở đây?  

Câu trả lời nằm ở các kiến trúc **heterogeneous** (không đồng nhất) mới đang được các hãng sản xuất chip phát triển.  
Ví dụ: bắt đầu từ thế hệ CPU thứ 12, Intel đã giới thiệu kiến trúc kết hợp giữa **Performance core** (**P-core**) và **Efficiency core** (**E-core**).  

Mục tiêu của thiết kế lai này là giao các tác vụ nền nhỏ hơn cho các E-core tiết kiệm điện, giải phóng các P-core mạnh mẽ nhưng tiêu thụ nhiều điện cho các tác vụ tính toán nặng.  
Nguyên tắc tương tự cũng được áp dụng trong kiến trúc di động **big.LITTLE** trước đây của Arm.

Trên các kiến trúc heterogeneous, kết quả mặc định của `lscpu` chỉ hiển thị các P-core khả dụng;  
số lượng E-core thường có thể tính bằng cách lấy tổng số core trong trường `"CPU(s)"` trừ đi số lượng P-core.  

Chạy lệnh `lscpu` với tùy chọn `--all` và `--extended` sẽ hiển thị **bản đồ đầy đủ** của P-core và E-core trên hệ thống, trong đó E-core có thể nhận diện nhờ tốc độ xử lý thấp hơn.

Bạn có muốn tôi dịch tiếp sang phần **14.1. Shared Memory Multiprocessing** để nối tiếp nội dung không?