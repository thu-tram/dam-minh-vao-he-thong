## 4.9. Tóm tắt (Summary)

Chương này đã xem xét cách các máy tính hiện đại biểu diễn thông tin bằng **bit** và **byte**.  
Một điểm quan trọng cần ghi nhớ là bộ nhớ máy tính lưu trữ **mọi thông tin** dưới dạng các giá trị nhị phân 0 và 1 — việc **diễn giải ý nghĩa** của các bit này phụ thuộc vào chương trình hoặc con người đang sử dụng chúng.  
Chương này chủ yếu tập trung vào cách biểu diễn **số nguyên**, bắt đầu với số nguyên **unsigned** (không âm) trước khi xem xét số nguyên **signed** (có dấu).

Phần cứng máy tính hỗ trợ nhiều phép toán trên số nguyên, bao gồm các phép quen thuộc như **cộng**, **trừ**, **nhân** và **chia**.  
Hệ thống cũng cung cấp các phép toán bitwise như **AND**, **OR**, **NOT** và **dịch bit** (shifting).  
Khi thực hiện *bất kỳ* phép toán nào, cần lưu ý đến **số bit** được dùng để biểu diễn các toán hạng và kết quả.  
Nếu vùng lưu trữ dành cho kết quả **không đủ lớn**, hiện tượng **overflow** (tràn số) có thể xảy ra, dẫn đến việc giá trị kết quả bị biểu diễn sai.

Cuối cùng, chương này đã khám phá các phương pháp phổ biến để biểu diễn **số thực** trong hệ nhị phân, bao gồm cả chuẩn **IEEE 754**.  
Cần lưu ý rằng khi biểu diễn giá trị **floating-point**, chúng ta đánh đổi **độ chính xác** để có được **tính linh hoạt cao hơn** (tức là khả năng “di chuyển” dấu chấm thập phân).
