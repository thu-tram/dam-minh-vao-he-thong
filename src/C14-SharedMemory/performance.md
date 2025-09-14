## 14.4. Đo lường hiệu năng của các chương trình song song (Measuring the Performance of Parallel Programs)

Cho đến nay, chúng ta đã sử dụng hàm `gettimeofday` để đo lượng thời gian mà chương trình cần để thực thi.  
Trong phần này, chúng ta sẽ thảo luận cách đo lường **mức độ hiệu quả** của một chương trình song song so với một chương trình tuần tự, cũng như các chủ đề khác liên quan đến việc đo lường hiệu năng của các chương trình song song.

Trước tiên, chúng ta sẽ đề cập đến một số khái niệm cơ bản liên quan đến hiệu năng song song:

- **Speedup** (tăng tốc)  
- **Efficiency** (hiệu suất)  
- **Amdahl’s Law** (Định luật Amdahl)

Mặc dù **Amdahl’s Law** và **speedup** là hai khái niệm rất quan trọng liên quan đến hiệu năng, nhưng việc hiểu rõ thêm các chủ đề sau sẽ giúp người đọc có cái nhìn toàn diện hơn về hiệu năng:

- [**Gustafson–Barsis Law**](performance_advanced.html#_gustafson_barsis_law)  
- **Scalability** (khả năng mở rộng)

Cụ thể, **Gustafson–Barsis Law** cung cấp một góc nhìn rõ hơn về **giới hạn** của Amdahl’s Law.



