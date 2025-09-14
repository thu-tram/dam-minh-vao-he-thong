## 4.1. Các hệ cơ số và số nguyên không dấu (unsigned integers)

Sau khi đã thấy rằng [các dãy nhị phân (binary sequences) có thể được diễn giải theo nhiều cách phi số học khác nhau](index.html#_binary_and_data_representation), giờ chúng ta hãy chuyển sự chú ý sang các con số. Cụ thể, chúng ta sẽ bắt đầu với **unsigned** numbers (số nguyên không dấu), tức là các số có thể được diễn giải là bằng 0 hoặc dương, nhưng không bao giờ âm (chúng không có _sign_ — dấu).

### 4.1.1. Số thập phân (decimal numbers)

Thay vì bắt đầu với nhị phân, trước hết hãy xem xét một hệ thống số mà chúng ta đã quen thuộc: **decimal number system** (hệ số thập phân), sử dụng _base_ (cơ số) là 10. _Base 10_ (cơ số 10) ngụ ý hai đặc điểm quan trọng trong việc diễn giải và biểu diễn các giá trị thập phân:

1. Mỗi chữ số riêng lẻ trong một số ở cơ số 10 lưu trữ một trong 10 giá trị duy nhất (0–9). Để lưu trữ một giá trị lớn hơn 9, giá trị đó phải được **carry** (nhớ sang) một chữ số bổ sung ở bên trái. Ví dụ, nếu một chữ số đang ở giá trị tối đa (9) và ta cộng thêm 1, kết quả sẽ cần hai chữ số (9 + 1 = 10). Mẫu này đúng cho mọi chữ số, bất kể vị trí của nó trong số (ví dụ: 50 **8** 0 + **2** 0 = 5 **10** 0).

2. Vị trí của mỗi chữ số trong số quyết định tầm quan trọng của chữ số đó đối với giá trị tổng thể. Đánh số các chữ số từ _phải sang trái_ là d0, d1, d2, v.v., mỗi chữ số kế tiếp sẽ đóng góp một hệ số gấp _mười_ lần so với chữ số liền kề bên phải. Ví dụ, xét giá trị 8425 ([Hình 1](#FigBaseTen)).

![For the number 8425, digit 0 is the 5, which is in the "ones place".  Digit 1 is the 2, which is in the "tens place".  Digit 2 is the 4, in the "hundreds place".  Finally, digit 3 is the 8, in the "thousands place".](_images/BaseTen.png)

**Hình 1.** Tầm quan trọng của mỗi chữ số trong một số ở cơ số 10, sử dụng các tên gọi mà bạn có thể đã học ở tiểu học.

Với ví dụ 8425: số 5 ở vị trí "ones" (hàng đơn vị) đóng góp 5 (5 × 10⁰). Số 2 ở vị trí "tens" (hàng chục) đóng góp 20 (2 × 10¹). Số 4 ở vị trí "hundreds" (hàng trăm) đóng góp 400 (4 × 10²). Cuối cùng, số 8 ở vị trí "thousands" (hàng nghìn) đóng góp 8000 (8 × 10³). Một cách chính xác hơn, ta có thể biểu diễn 8425 như sau:

(8 × 10³)    +    (4 × 10²)    +    (2 × 10¹)    +    (5 × 10⁰)

Mẫu số mũ tăng dần áp dụng cho cơ số 10 này chính là lý do tại sao nó được gọi là _base 10_ (cơ số 10). Việc đánh số vị trí các chữ số từ phải sang trái bắt đầu với d0 ngụ ý rằng mỗi chữ số di đóng góp 10ⁱ vào giá trị tổng thể. Do đó, giá trị tổng thể của bất kỳ số thập phân N chữ số nào có thể được biểu diễn như:

(dN-1 × 10ᴺ⁻¹)    +    (dN-2 × 10ᴺ⁻²)    +    …    +    (d2 × 10²)    +    (d1 × 10¹)    +    (d0 × 10⁰)

May mắn thay, như chúng ta sẽ thấy ngay sau đây, một mẫu tương tự cũng áp dụng cho các hệ cơ số khác.

**Phân biệt các hệ cơ số**

Bây giờ, khi chúng ta sắp giới thiệu một hệ cơ số thứ hai, một vấn đề tiềm ẩn là sự thiếu rõ ràng trong cách diễn giải một con số. Ví dụ, xét giá trị 1000. Không rõ ngay lập tức liệu bạn nên hiểu nó là giá trị thập phân (tức là một nghìn) hay giá trị nhị phân (tức là tám, lý do sẽ được giải thích ngay).  
Để làm rõ, phần còn lại của chương này sẽ gắn một **prefix** (tiền tố) cho tất cả các số không phải thập phân. Chúng ta sẽ sớm giới thiệu nhị phân, với tiền tố **0b**, và hệ thập lục phân (hexadecimal), với tiền tố **0x**.

Vì vậy, nếu bạn thấy 1000, hãy giả định đó là số thập phân "một nghìn". Nếu bạn thấy 0b1000, hãy hiểu đó là số nhị phân, trong trường hợp này có giá trị là "tám".

### 4.1.2. Số nhị phân không dấu (unsigned binary numbers)

Mặc dù bạn có thể chưa từng nghĩ đến công thức cụ thể mô tả số thập phân dưới dạng lũy thừa của 10, nhưng khái niệm về các vị trí { _ones_, _tens_, _hundreds_, … } hẳn là quen thuộc. May mắn thay, thuật ngữ tương tự cũng áp dụng cho các hệ cơ số khác, như nhị phân. Tất nhiên, cơ số trong các hệ khác nhau sẽ khác nhau, nên mỗi vị trí chữ số sẽ đóng góp một giá trị khác nhau vào tổng số.

**Binary number system** (hệ số nhị phân) sử dụng cơ số 2 thay vì 10 như hệ thập phân. Phân tích nó theo cách tương tự như với thập phân sẽ cho thấy nhiều điểm song song (với 2 thay cho 10):

1. Mỗi **bit** trong một số ở cơ số 2 lưu trữ một trong hai giá trị duy nhất (0 hoặc 1). Để lưu trữ một giá trị lớn hơn 1, biểu diễn nhị phân phải **carry** sang một bit bổ sung ở bên trái. Ví dụ, nếu một bit đang ở giá trị tối đa (1) và ta cộng thêm 1, kết quả sẽ cần hai bit (1 + 1 = 0b10). Mẫu này đúng cho mọi bit, bất kể vị trí của nó trong số (ví dụ: 0b100 **1** 00 + 0b **1** 00 = 0b10 **10** 00).

2. Vị trí của mỗi bit trong số quyết định tầm quan trọng của bit đó đối với giá trị số. Đánh số các bit từ _phải sang trái_ là d0, d1, d2, v.v., mỗi bit kế tiếp sẽ đóng góp một hệ số gấp _hai_ lần so với bit liền kề bên phải.

Điểm thứ nhất cho thấy việc đếm trong nhị phân tuân theo cùng một mẫu như thập phân: chỉ cần liệt kê các giá trị và thêm chữ số (bit) khi cần. Vì phần này tập trung vào _unsigned_ numbers (chỉ gồm 0 và các số dương), nên việc bắt đầu đếm từ 0 là tự nhiên. [Bảng 1](#TabBinaryCounting) cho thấy cách đếm một vài số tự nhiên đầu tiên trong nhị phân. Như bạn có thể thấy từ bảng, việc đếm trong nhị phân nhanh chóng làm tăng số lượng chữ số. Điều này là hợp lý, vì mỗi chữ số nhị phân (chỉ có hai giá trị khả dĩ) chứa ít thông tin hơn một chữ số thập phân (10 giá trị khả dĩ).

Dưới đây là bản dịch tiếng Việt của đoạn bạn cung cấp, tuân thủ đầy đủ các quy ước bạn đặt ra: giữ nguyên thuật ngữ chuyên ngành bằng tiếng Anh, giải thích tiếng Việt khi xuất hiện lần đầu, đảm bảo câu văn tự nhiên, rõ ràng và chính xác về mặt kỹ thuật.

| **Binary value** (giá trị nhị phân) | **Decimal value** (giá trị thập phân) |
|-------------------------------------|----------------------------------------|
| 0                                   | 0                                      |
| 1                                   | 1                                      |
| 10                                  | 2                                      |
| 11                                  | 3                                      |
| 100                                 | 4                                      |
| 101                                 | 5                                      |
| ...                                 | ...                                    |

**Bảng 1.** So sánh cách đếm trong hệ nhị phân và hệ thập phân.

Điểm thứ hai về việc đánh số vị trí các chữ số nghe thật quen thuộc! Thực tế, nó giống với hệ thập phân đến mức dẫn đến một công thức gần như y hệt để diễn giải một số nhị phân. Chỉ cần thay số 10 ở cơ số của mỗi số mũ bằng số 2:

$$(d_{N-1} \times 2^{N-1}) + (d_{N-2} \times 2^{N-2}) + \dots + (d_2 \times 2^2) + (d_1 \times 2^1) + (d_0 \times 2^0)$$

Áp dụng công thức này sẽ cho ra cách diễn giải _unsigned_ (không dấu) của bất kỳ số nhị phân nào. Ví dụ, xét số **0b1000**:

$$(1 \times 2^3) + (0 \times 2^2) + (0 \times 2^1) + (0 \times 2^0)$$  
$$= 8 + 0 + 0 + 0 = 8$$

Một ví dụ dài hơn với một byte, **0b10110100**:

$$(1 \times 2^7) + (0 \times 2^6) + (1 \times 2^5) + (1 \times 2^4) + (0 \times 2^3) + (1 \times 2^2) + (0 \times 2^1) + (0 \times 2^0)$$  
$$= 128 + 0 + 32 + 16 + 0 + 4 + 0 + 0 = 180$$

### 4.1.3. Hệ thập lục phân (hexadecimal)

Cho đến giờ, chúng ta đã xem xét hai hệ cơ số: thập phân (decimal) và nhị phân (binary). Hệ thập phân nổi bật vì con người quen sử dụng, trong khi hệ nhị phân phù hợp với cách dữ liệu được lưu trữ trong phần cứng. Điều quan trọng cần lưu ý là chúng **tương đương** về khả năng biểu diễn: không có số nào mà một hệ có thể biểu diễn còn hệ kia thì không. Với sự tương đương này, có thể bạn sẽ ngạc nhiên khi chúng ta tiếp tục tìm hiểu thêm một hệ cơ số nữa: **hexadecimal** (hệ thập lục phân) với cơ số 16.

Với hai hệ cơ số vốn đã tốt, bạn có thể tự hỏi tại sao cần thêm một hệ nữa. Câu trả lời chủ yếu là **vì sự tiện lợi**. Như đã thấy ở [Bảng 1](#TabBinaryCounting), các dãy bit (bit sequences) trong nhị phân nhanh chóng trở nên rất dài. Con người thường gặp khó khăn khi đọc các chuỗi dài chỉ gồm 0 và 1. Trong khi đó, hệ thập phân gọn hơn, nhưng cơ số 10 lại không khớp với cơ số 2 của nhị phân.

Hệ thập phân cũng không dễ dàng biểu diễn phạm vi giá trị có thể thể hiện bằng một số lượng bit cố định. Ví dụ, giả sử một máy tính cũ sử dụng địa chỉ bộ nhớ (memory address) 16-bit. Các địa chỉ hợp lệ của nó trải từ **0b0000000000000000** đến **0b1111111111111111**. Nếu biểu diễn ở hệ thập phân, các địa chỉ này sẽ từ 0 đến 65535. Rõ ràng, biểu diễn thập phân gọn hơn so với chuỗi nhị phân dài, nhưng trừ khi bạn thuộc lòng cách chuyển đổi, việc suy luận từ số thập phân sẽ khó hơn. Cả hai vấn đề này còn nghiêm trọng hơn trên các thiết bị hiện đại, vốn dùng địa chỉ 32-bit hoặc 64-bit.

Chính ở những chuỗi bit dài này mà hệ thập lục phân (base 16) phát huy ưu thế. Cơ số lớn cho phép mỗi chữ số biểu diễn nhiều thông tin hơn, giúp số hexadecimal gọn hơn. Hơn nữa, vì cơ số 16 là lũy thừa của 2 ($2^4 = 16$), việc ánh xạ giữa hexadecimal và binary (và ngược lại) trở nên dễ dàng. Để đầy đủ, hãy phân tích hệ thập lục phân tương tự như đã làm với thập phân và nhị phân:

1. Mỗi chữ số trong một số ở cơ số 16 lưu trữ một trong 16 giá trị duy nhất. Việc có hơn 10 giá trị tạo ra một thách thức mới cho hexadecimal — các chữ số truyền thống của cơ số 10 dừng ở giá trị tối đa là 9. Theo quy ước, hexadecimal dùng các chữ cái để biểu diễn giá trị lớn hơn 9: A cho 10, B cho 11, …, F cho 15. Giống như các hệ khác, để lưu trữ giá trị lớn hơn 15, số phải **carry** sang một chữ số bổ sung bên trái. Ví dụ, nếu một chữ số đang ở giá trị tối đa (F) và ta cộng thêm 1, kết quả sẽ cần hai chữ số:  
   **0xF + 0x1 = 0x10** (lưu ý: tiền tố **0x** được dùng để chỉ số hexadecimal).

2. Vị trí của mỗi chữ số trong số quyết định tầm quan trọng của nó đối với giá trị tổng thể. Đánh số các chữ số từ _phải sang trái_ là d0, d1, d2, …, mỗi chữ số kế tiếp đóng góp một hệ số gấp 16 lần chữ số liền kề bên phải.

Không có gì ngạc nhiên, công thức quen thuộc để diễn giải một con số cũng áp dụng cho hexadecimal, chỉ khác là cơ số là 16:

$$(d_{N-1} \times 16^{N-1}) + (d_{N-2} \times 16^{N-2}) + \dots + (d_2 \times 16^2) + (d_1 \times 16^1) + (d_0 \times 16^0)$$

Ví dụ, để xác định giá trị thập phân của **0x23C8**:

$$(2 \times 16^3) + (3 \times 16^2) + (C \times 16^1) + (8 \times 16^0)$$  
$$(2 \times 16^3) + (3 \times 16^2) + (12 \times 16^1) + (8 \times 16^0)$$  
$$(2 \times 4096) + (3 \times 256) + (12 \times 16) + (8 \times 1)$$  
$$= 8192 + 768 + 192 + 8 = 9160$$

**Hiểu lầm thường gặp về hexadecimal**

Khi mới học lập trình hệ thống (systems programming), bạn có thể không thường xuyên gặp số hexadecimal. Thực tế, ngữ cảnh phổ biến nhất là khi biểu diễn địa chỉ bộ nhớ (memory address). Ví dụ, nếu bạn in địa chỉ của một biến bằng **`%p`** (pointer) trong hàm `printf`, bạn sẽ nhận được kết quả ở dạng hexadecimal.

Nhiều sinh viên thường bắt đầu đồng nhất địa chỉ bộ nhớ (ví dụ: biến con trỏ trong C) với hexadecimal. Mặc dù bạn sẽ quen với việc thấy địa chỉ được biểu diễn theo cách này, hãy nhớ rằng **chúng vẫn được lưu trữ ở dạng nhị phân trong phần cứng**, giống như mọi dữ liệu khác.

### 4.1.4. Giới hạn lưu trữ (Storage Limitations)

Về mặt khái niệm, tồn tại vô hạn các **unsigned integers** (số nguyên không dấu). Tuy nhiên, trong thực tế, lập trình viên phải chọn số lượng **bits** dành cho một biến _trước khi_ lưu trữ nó, vì nhiều lý do:

- Trước khi lưu trữ một giá trị, chương trình phải cấp phát không gian lưu trữ cho giá trị đó. Trong C, việc khai báo một biến sẽ cho **compiler** (trình biên dịch) biết [lượng bộ nhớ](getting_started.html#_c_numeric_types) cần thiết dựa trên kiểu dữ liệu của biến.

- Các thiết bị lưu trữ phần cứng có dung lượng hữu hạn. Trong khi **main memory** (bộ nhớ chính) của hệ thống thường lớn và hiếm khi là yếu tố giới hạn, thì các vị trí lưu trữ bên trong CPU được dùng làm vùng "scratch space" (bộ nhớ tạm thời), tức là [registers](storagecircs.html#_cpu_register), lại bị hạn chế hơn nhiều. CPU sử dụng các register bị giới hạn bởi **word size** (độ dài từ) của nó — thường là 32 hoặc 64 bits, tùy thuộc vào **CPU architecture** (kiến trúc CPU).

- Các chương trình thường di chuyển dữ liệu từ một thiết bị lưu trữ sang thiết bị khác (ví dụ: giữa CPU registers và main memory). Khi giá trị dữ liệu lớn hơn, các thiết bị lưu trữ cần nhiều dây dẫn hơn để truyền tín hiệu giữa chúng. Do đó, việc mở rộng dung lượng lưu trữ sẽ làm tăng độ phức tạp của phần cứng và giảm không gian vật lý dành cho các thành phần khác.

Số lượng bits được dùng để lưu trữ một số nguyên quyết định phạm vi giá trị mà nó có thể biểu diễn. [Hình 2](#FigUnsignedLine) minh họa cách chúng ta có thể hình dung không gian lưu trữ số nguyên không dấu vô hạn và hữu hạn.

![The infinite unsigned number line starts at zero and increases infinitely.  The finite unsigned number line starts at 0 and ends at a maximum value.  Attempting to move off one end wraps around to the other.](_images/UnsignedLine.png)

**Hình 2.** Minh họa (a) trục số nguyên không dấu vô hạn và (b) trục số nguyên không dấu hữu hạn. Trường hợp hữu hạn sẽ "quay vòng" (wrap around) tại hai đầu mút (hiện tượng **overflow**).

Cố gắng lưu trữ một giá trị lớn hơn khả năng chứa của biến được gọi là **integer overflow** (tràn số nguyên). Chương này sẽ để phần chi tiết về overflow ở [mục sau](overflow.html#_integer_overflow). Hiện tại, bạn có thể hình dung nó giống như đồng hồ đo quãng đường (odometer) của ô tô: khi đạt giá trị tối đa và tăng thêm, nó sẽ "quay vòng" trở lại 0. Tương tự, nếu trừ 1 từ 0, ta sẽ nhận được giá trị lớn nhất.

Tại thời điểm này, một câu hỏi tự nhiên về **unsigned binary** (nhị phân không dấu) là: "_Giá trị dương lớn nhất mà N bits có thể lưu trữ là bao nhiêu?_"  
Nói cách khác, nếu có một dãy _N_ bits đều bằng 1, thì dãy đó biểu diễn giá trị nào?  
Lý luận một cách không hình thức, phân tích ở [mục trước](index.html#_binary_and_data_representation) cho thấy _N_ bits tạo ra $2^N$ dãy bit khác nhau. Vì một trong số đó phải biểu diễn số 0, nên còn lại $2^N - 1$ giá trị dương, từ 1 đến $2^N - 1$. Do đó, giá trị lớn nhất của một số nhị phân không dấu _N_ bits là $2^N - 1$.

Ví dụ: 8 bits tạo ra $2^8 = 256$ dãy bit khác nhau. Một trong số đó, **0b00000000**, được dành cho giá trị 0, để lại 255 dãy cho các giá trị dương. Vì vậy, một biến 8-bit có thể biểu diễn các giá trị dương từ 1 đến 255, trong đó giá trị lớn nhất là **255**.
