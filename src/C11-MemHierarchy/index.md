## 11. Lưu trữ và Hệ phân cấp bộ nhớ (Storage and the Memory Hierarchy)

Mặc dù việc thiết kế và triển khai các **algorithm** (thuật toán) hiệu quả thường là yếu tố *quan trọng nhất* để viết các chương trình có hiệu năng cao, vẫn còn một yếu tố khác — thường bị bỏ qua — có thể tác động lớn đến hiệu suất: **memory** (bộ nhớ).  
Có thể bạn sẽ ngạc nhiên khi biết rằng hai thuật toán có **asymptotic performance** (hiệu năng tiệm cận — số bước trong trường hợp xấu nhất) giống nhau, chạy trên cùng một dữ liệu đầu vào, lại có thể cho hiệu năng thực tế rất khác nhau do cách tổ chức phần cứng mà chúng chạy trên đó.  
Sự khác biệt này thường bắt nguồn từ cách các thuật toán truy cập bộ nhớ, đặc biệt là vị trí lưu trữ dữ liệu và kiểu mẫu (pattern) truy cập dữ liệu. Các kiểu mẫu này được gọi là **memory locality** (tính cục bộ bộ nhớ), và để đạt hiệu năng tốt nhất, kiểu truy cập của chương trình cần phù hợp với cách bố trí bộ nhớ của phần cứng.

Ví dụ, hãy xem xét hai biến thể của một hàm tính trung bình các giá trị trong ma trận *N*×*N*, như minh họa trong [Bảng 1](#TabMatrixVersions).  
Mặc dù cả hai phiên bản đều truy cập cùng số lượng vị trí bộ nhớ như nhau (N²), nhưng mã bên trái chạy nhanh hơn khoảng 5 lần trên hệ thống thực tế so với mã bên phải.  
Sự khác biệt đến từ cách chúng truy cập các vị trí bộ nhớ đó. Ở cuối chương này, chúng ta sẽ phân tích ví dụ này bằng công cụ **Cachegrind** (công cụ phân tích hiệu năng cache).

#### averageMat_v1

```c
float averageMat_v1(int **mat, int n) {
    int i, j, total = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Note indexing: [i][j]
            total += mat[i][j];
        }
    }
    return (float) total / (n * n);
}
```

#### averageMat_v2

```c
float averageMat_v2(int **mat, int n) {
    int i, j, total = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            // Note indexing: [j][i]
            total += mat[j][i];
        }
    }
    return (float) total / (n * n);
}
```

**Bảng 1.** Hai phiên bản của một hàm truy cập mọi phần tử của ma trận *N*×*N*. Chúng chỉ khác nhau ở cách đánh chỉ số (indexing) khi truy cập bộ nhớ.


Các vị trí lưu trữ như **register**, **CPU cache**, **main memory** (bộ nhớ chính), và **file** trên **disk** (đĩa) có thời gian truy cập, tốc độ truyền và dung lượng lưu trữ rất khác nhau.  
Khi lập trình một ứng dụng hiệu năng cao, điều quan trọng là phải xem xét dữ liệu được lưu ở đâu và chương trình truy cập dữ liệu từ thiết bị đó với tần suất như thế nào.  
Ví dụ, việc truy cập một ổ đĩa chậm chỉ một lần khi chương trình khởi động thường không phải vấn đề lớn. Nhưng nếu truy cập ổ đĩa thường xuyên, chương trình sẽ bị chậm đáng kể.

Chương này mô tả đặc điểm của nhiều loại thiết bị bộ nhớ khác nhau và cách chúng được tổ chức trong một máy tính cá nhân hiện đại.  
Với bối cảnh đó, chúng ta sẽ thấy cách kết hợp nhiều loại thiết bị bộ nhớ để khai thác **locality** (tính cục bộ) trong các mẫu truy cập bộ nhớ điển hình của chương trình.
