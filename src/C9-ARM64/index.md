## 9. ARM Assembly  

Trong chương này, chúng ta sẽ tìm hiểu về kiến trúc **ARM version 8 application profile (ARMv8-A)** với **A64 ISA**, phiên bản ARM ISA mới nhất đang được sử dụng trên tất cả các máy tính ARM chạy hệ điều hành Linux.  
Hãy nhớ rằng một [instruction set architecture](../C5-Arch/index.html#_what_von_neumann_knew_computer_architecture) (ISA — “kiến trúc tập lệnh”) định nghĩa tập hợp các lệnh và cách mã hóa nhị phân của một chương trình ở cấp độ máy.  

Để chạy được các ví dụ trong chương này, bạn cần có một máy với bộ xử lý ARMv8-A và hệ điều hành 64-bit. Các ví dụ trong chương này được thực hiện trên **Raspberry Pi 3B+** chạy hệ điều hành **Ubuntu Mate 64-bit**. Lưu ý rằng mọi phiên bản Raspberry Pi phát hành từ năm 2016 đều có thể sử dụng A64 ISA. Tuy nhiên, **Raspberry Pi OS** (hệ điều hành mặc định của Raspberry Pi) vẫn là bản 32-bit tại thời điểm viết sách này.  

Bạn có thể kiểm tra xem hệ thống của mình đang chạy hệ điều hành 64-bit hay không bằng cách chạy lệnh `uname -m`. Một hệ điều hành 64-bit sẽ cho kết quả như sau:

```
$ uname -m
aarch64
```

Mặc dù có thể *biên dịch* (build) các tệp thực thi ARM trên máy Intel bằng [bộ công cụ cross-compilation GNU của ARM](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads), nhưng bạn **không thể** *chạy* trực tiếp các tệp ARM trên hệ thống x86.  
Nếu muốn học ARM assembly trực tiếp trên laptop, bạn có thể thử [QEMU](https://www.qemu.org/), một trình **giả lập** (emulator) hệ thống ARM. Trình giả lập khác với máy ảo ở chỗ nó mô phỏng cả phần cứng của hệ thống khác.  

Một lựa chọn khác là sử dụng [EC2 A1 instances](https://aws.amazon.com/ec2/instance-types/a1/) mà Amazon mới phát hành. Mỗi instance cung cấp cho bạn quyền truy cập vào bộ xử lý **Graviton 64-bit**, tuân theo đặc tả ARMv8-A.  

Tuy nhiên, cần lưu ý rằng các lệnh assembly cụ thể do compiler sinh ra phụ thuộc nhiều vào hệ điều hành và kiến trúc phần cứng chính xác. Do đó, mã assembly sinh ra trên AWS hoặc qua QEMU có thể hơi khác so với các ví dụ trong chương này.  

> **RISC và bộ xử lý ARM**  
>  
> Trong nhiều năm, kiến trúc **CISC** (*complex instruction set computer*) chiếm ưu thế trên thị trường máy tính cá nhân và máy chủ. Ví dụ phổ biến của CISC là các bộ xử lý Intel và AMD.  
> Tuy nhiên, kiến trúc **RISC** (*reduced instruction set computer*) đã phát triển mạnh trong thập kỷ qua nhờ nhu cầu từ lĩnh vực điện toán di động. ARM (viết tắt của *Acorn RISC Machine*) là một ví dụ của kiến trúc RISC, bên cạnh RISC-V và MIPS.  
> RISC đặc biệt hấp dẫn đối với điện toán di động nhờ hiệu suất năng lượng cao, giúp kéo dài tuổi thọ pin.  
> Trong những năm gần đây, ARM và các bộ xử lý RISC khác đã bắt đầu thâm nhập vào thị trường máy chủ và **HPC** (*high performance computing*). Ví dụ, siêu máy tính **Fugaku** của Nhật Bản — nhanh nhất thế giới vào năm 2020 — sử dụng bộ xử lý ARM.

