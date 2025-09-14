
## 14.6. Thread Safety (An toàn luồng)

Cho đến nay, chúng ta đã tìm hiểu các cấu trúc đồng bộ hóa mà lập trình viên có thể sử dụng để đảm bảo chương trình đa luồng của mình hoạt động **nhất quán** và **đúng đắn** bất kể số lượng thread được dùng.  
Tuy nhiên, **không phải lúc nào** cũng an toàn khi giả định rằng các hàm trong thư viện C chuẩn có thể được dùng “nguyên trạng” trong mọi ứng dụng đa luồng.  
Không phải tất cả các hàm trong thư viện C đều **thread safe** (an toàn luồng) — tức là có thể được nhiều thread chạy đồng thời mà vẫn đảm bảo kết quả đúng và không gây ra tác dụng phụ ngoài ý muốn.  

Để đảm bảo các chương trình *chúng ta* viết là thread safe, điều quan trọng là phải sử dụng [synchronization primitives](synchronization.html#_synchronizing_threads) như **mutex** và **barrier** để buộc chương trình đa luồng duy trì tính nhất quán và đúng đắn, bất kể số lượng thread thay đổi ra sao.

Một khái niệm liên quan chặt chẽ đến thread safety là **re-entrancy** (tái nhập).  
Tất cả code thread safe đều là re-entrant; tuy nhiên, **không phải** tất cả code re-entrant đều thread safe.  
Một hàm được gọi là **re-entrant** nếu nó có thể được thực thi lại hoặc thực thi một phần bởi chính nó mà không gây ra vấn đề.  
Theo định nghĩa, code re-entrant đảm bảo rằng mọi truy cập vào trạng thái toàn cục (global state) của chương trình đều giữ cho trạng thái đó nhất quán.  
Mặc dù re-entrancy thường (một cách sai lầm) được dùng như từ đồng nghĩa với thread safety, vẫn có những trường hợp đặc biệt mà code re-entrant **không** thread safe.

Khi viết code đa luồng, hãy xác minh rằng các hàm thư viện C bạn sử dụng thực sự là thread safe.  
May mắn thay, danh sách các hàm **không** thread safe trong thư viện C khá nhỏ.  
**The Open Group** duy trì [danh sách các hàm không thread safe](http://pubs.opengroup.org/onlinepubs/009695399/functions/xsh_chap02_09.html).

### 14.6.1. Khắc phục các vấn đề về Thread Safety

[Synchronization primitives](synchronization.html#_synchronizing_threads) là cách phổ biến nhất để khắc phục các vấn đề liên quan đến thread safety.  
Tuy nhiên, việc vô tình sử dụng các hàm thư viện C **không** thread safe có thể gây ra những lỗi tinh vi, khó phát hiện.

Hãy xem một phiên bản được chỉnh sửa nhẹ của hàm `countsElem` có tên `countElemsStr`, với mục tiêu đếm tần suất xuất hiện của các chữ số trong một chuỗi, trong đó mỗi chữ số được phân tách bằng dấu cách.  
Chương trình dưới đây đã được rút gọn; mã nguồn đầy đủ có tại: [countElemsStr.c](_attachments/countElemsStr.c).


```c
/* computes the frequency of all the elements in the input string and stores
 * the associated counts of each element in the array called counts. */
void countElemsStr(int *counts, char *input_str) {
    int val, i;
    char *token;
    token = strtok(input_str, " ");
    while (token != NULL) {
        val = atoi(token);
        counts[val] = counts[val] + 1;
        token = strtok(NULL, " ");
    }
}

/* main function:
 * calls countElemsStr on a static string and counts up all the digits in
 * that string. */
int main( int argc, char **argv ) {
    //lines omitted for brevity, but gets user defined length of string

    //fill string with n digits
    char *inputString = calloc(length * 2, sizeof(char));
    fillString(inputString, length * 2);

    countElemsStr(counts, inputString);

    return 0;
}
```

Hàm `countElemsStr` sử dụng hàm `strtok` (đã được phân tích trong [phần thảo luận về strings](../C2-C_depth/strings.html#_strtok_strtok_r)) để tách từng chữ số (lưu trong `token`) từ chuỗi, sau đó chuyển đổi sang số nguyên và cập nhật vào mảng `counts`.

Biên dịch và chạy chương trình này với 100.000 phần tử cho ra kết quả:


```
$ gcc -o countElemsStr countElemsStr.c

$ ./countElemsStr 100000 1
contents of counts array:
9963 9975 9953 10121 10058 10017 10053 9905 9915 10040

```
Bây giờ, hãy xem phiên bản đa luồng của `countElemsStr` (mã nguồn đầy đủ xem [tại đây](_attachments/countElemsStr_p.c)):

```c
/* parallel version of countElemsStr (First cut):
 * computes the frequency of all the elements in the input string and stores
 * the associated counts of each element in the array called counts
*/
void *countElemsStr(void *args) {
    //parse args
    struct t_arg *myargs = (struct t_arg *)args;
    //omitted for brevity

    //local variables
    int val, i;
    char *token;
    int local_counts[MAX] = {0};

    //compute local start and end values and chunk size:
    //omitted for brevity

    //tokenize values
    token = strtok(input_str + start, " ");
    while (token != NULL) {
        val = atoi(token); //convert to an int
        local_counts[val] = local_counts[val] + 1; //update associated counts
        token = strtok(NULL, " ");
    }

    pthread_mutex_lock(&mutex);
    for (i = 0; i < MAX; i++) {
        counts[i] += local_counts[i];
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}
```

Trong phiên bản này, mỗi thread xử lý một phần riêng của chuỗi `input_str`.  
Mảng `local_counts` đảm bảo phần lớn các thao tác ghi diễn ra trên bộ nhớ cục bộ.  
Một **mutex** được sử dụng để đảm bảo không có hai thread nào ghi vào biến chia sẻ `counts` cùng lúc.

Tuy nhiên, biên dịch và chạy chương trình này cho ra kết quả:

```

$ gcc -o countElemsStr_p countElemsStr_p.c -pthread

$ ./countElemsStr_p 100000 1 1
contents of counts array:
9963 9975 9953 10121 10058 10017 10053 9905 9915 10040

$ ./countElemsStr_p 100000 1 2
contents of counts array:
498 459 456 450 456 471 446 462 450 463

$ ./countElemsStr_p 100000 1 4
contents of counts array:
5038 4988 4985 5042 5056 5013 5025 5035 4968 5065

```

Mặc dù đã dùng mutex để khóa khi truy cập mảng `counts`, kết quả giữa các lần chạy lại **khác nhau hoàn toàn**.  
Nguyên nhân là vì hàm `countsElemsStr` **không** thread safe, do hàm thư viện xử lý chuỗi `strtok` **không** thread safe!  
Truy cập trang [OpenGroup](http://pubs.opengroup.org/onlinepubs/009695399/functions/xsh_chap02_09.html) xác nhận rằng `strtok` nằm trong danh sách các hàm không thread safe.

Để khắc phục, chỉ cần thay `strtok` bằng phiên bản thread safe của nó là `strtok_r`.  
Trong `strtok_r`, một con trỏ được truyền làm tham số cuối để giúp thread theo dõi vị trí hiện tại trong chuỗi khi phân tách.  
Dưới đây là hàm đã sửa với `strtok_r` (mã nguồn đầy đủ tại [countsElemsStr_p_v2.c](_attachments/countElemsStr_p_v2.c)):



```c
/* parallel version of countElemsStr (First cut):
 * computes the frequency of all the elements in the input string and stores
 * the associated counts of each element in the array called counts */
void* countElemsStr(void* args) {
    //parse arguments
    //omitted for brevity

    //local variables
    int val, i;
    char * token;
    int local_counts[MAX] = {0};
    char * saveptr; //for saving state of strtok_r

    //compute local start and end values and chunk size:
    //omitted for brevity

    //tokenize values
    token = strtok_r(input_str+start, " ", &saveptr);
    while (token != NULL) {
        val = atoi(token); //convert to an int
        local_counts[val] = local_counts[val]+1; //update associated counts
        token = strtok_r(NULL, " ", &saveptr);
    }

    pthread_mutex_lock(&mutex);
    for (i = 0; i < MAX; i++) {
        counts[i]+=local_counts[i];
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}
```


Thay đổi duy nhất trong phiên bản này là khai báo thêm con trỏ ký tự `saveptr` và thay tất cả các lần gọi `strtok` bằng `strtok_r`.  
Chạy lại chương trình sau khi thay đổi cho ra kết quả:


```
$ gcc -o countElemsStr_p_v2 countElemsStr_p_v2.c -pthread

$ ./countElemsStr_p_v2 100000 1 1
contents of counts array:
9963 9975 9953 10121 10058 10017 10053 9905 9915 10040

$ ./countElemsStr_p_v2 100000 1 2
contents of counts array:
9963 9975 9953 10121 10058 10017 10053 9905 9915 10040

$ ./countElemsStr_p_v2 100000 1 4
contents of counts array:
9963 9975 9953 10121 10058 10017 10053 9905 9915 10040


```

Giờ đây, chương trình cho ra **kết quả giống nhau ở mọi lần chạy**.  
Việc sử dụng `saveptr` kết hợp với `strtok_r` đảm bảo mỗi thread có thể **độc lập** theo dõi vị trí của mình khi phân tách chuỗi.

**Kết luận**: Khi viết ứng dụng đa luồng, luôn kiểm tra [danh sách các hàm C không thread safe](http://pubs.opengroup.org/onlinepubs/009695399/functions/xsh_chap02_09.html).  
Điều này có thể giúp lập trình viên tránh được nhiều rắc rối và tiết kiệm thời gian khi viết và gỡ lỗi chương trình đa luồng.

--- 
