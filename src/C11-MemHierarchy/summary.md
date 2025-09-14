
## 11.7. Tóm tắt (Summary)

Chương này đã khám phá các đặc điểm của thiết bị lưu trữ máy tính và sự đánh đổi giữa các yếu tố quan trọng như **access latency** (độ trễ truy cập), **storage capacity** (dung lượng lưu trữ), **transfer latency** (độ trễ truyền dữ liệu), và chi phí.  
Do các thiết bị có nhiều sự đánh đổi về thiết kế và hiệu năng, chúng tự nhiên hình thành một **memory hierarchy** (hệ phân cấp bộ nhớ), được sắp xếp theo dung lượng và thời gian truy cập.  
Ở đỉnh của hệ phân cấp, các thiết bị lưu trữ chính như **CPU cache** và **main memory** cung cấp dữ liệu nhanh chóng trực tiếp cho CPU, nhưng dung lượng hạn chế.  
Ở các cấp thấp hơn, các thiết bị lưu trữ phụ như **solid-state drive (SSD)** và **hard disk** cung cấp dung lượng lưu trữ lớn với mật độ cao, nhưng hiệu năng thấp hơn.

Vì các hệ thống hiện đại cần cả dung lượng lớn và hiệu năng tốt, các nhà thiết kế hệ thống xây dựng máy tính với nhiều dạng lưu trữ khác nhau.  
Điều quan trọng là hệ thống phải quản lý thiết bị lưu trữ nào sẽ chứa từng phần dữ liệu cụ thể.  
Hệ thống sẽ cố gắng lưu dữ liệu đang được sử dụng tích cực trong các thiết bị lưu trữ nhanh hơn, và chuyển dữ liệu ít được sử dụng sang các thiết bị lưu trữ chậm hơn.

Để xác định dữ liệu nào đang được sử dụng, hệ thống dựa vào các mẫu truy cập dữ liệu của chương trình, được gọi là **locality**.  
Chương trình thể hiện hai dạng locality quan trọng:

- **Temporal Locality**: Chương trình có xu hướng truy cập cùng một dữ liệu nhiều lần theo thời gian.
- **Spatial Locality**: Chương trình có xu hướng truy cập dữ liệu nằm gần dữ liệu đã được truy cập trước đó.

**Locality** là cơ sở cho **CPU cache**, nơi lưu trữ một phần nhỏ của **main memory** trong bộ nhớ nhanh nằm trực tiếp trên chip CPU.  
Khi chương trình cố gắng truy cập **main memory**, CPU sẽ kiểm tra dữ liệu trong cache trước; nếu tìm thấy, nó sẽ tránh được việc truy cập tốn kém hơn xuống bộ nhớ chính.

Khi chương trình gửi yêu cầu đọc hoặc ghi bộ nhớ, nó cung cấp **address** (địa chỉ) của vị trí bộ nhớ cần truy cập. **CPU cache** sử dụng ba phần của các bit trong địa chỉ bộ nhớ để xác định cache line lưu trữ phần nào của **main memory**:

1. Các bit *index* ở giữa ánh xạ địa chỉ tới một vị trí lưu trữ trong cache.
2. Các bit *tag* ở phần cao của địa chỉ xác định duy nhất phần bộ nhớ mà vị trí cache đó lưu trữ.
3. Các bit *offset* ở phần thấp của địa chỉ xác định byte cụ thể trong dữ liệu được lưu mà chương trình muốn truy cập.

Cuối cùng, chương này kết thúc bằng việc minh họa cách công cụ **Cachegrind** có thể hỗ trợ phân tích hiệu năng cache cho một chương trình đang chạy.  
**Cachegrind** mô phỏng sự tương tác của chương trình với hệ thống cache và thu thập thống kê về việc sử dụng cache của chương trình (ví dụ: tỷ lệ **hit** và **miss**).

