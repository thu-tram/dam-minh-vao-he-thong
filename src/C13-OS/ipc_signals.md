### 13.4.1. Tín hiệu (Signals)

**Signal** là một **software interrupt** (ngắt phần mềm) được gửi từ một **process** (tiến trình) này tới một process khác thông qua **OS** (hệ điều hành).  
Khi một process nhận được signal, điểm thực thi hiện tại của nó sẽ bị OS ngắt để chạy **signal handler** (mã xử lý tín hiệu).  
Nếu signal handler trả về, quá trình thực thi của process sẽ tiếp tục từ vị trí mà nó bị ngắt để xử lý signal.  
Trong một số trường hợp, signal handler sẽ khiến process thoát (exit), và khi đó nó sẽ không tiếp tục thực thi từ vị trí trước khi bị ngắt.

Signal tương tự như **hardware interrupt** (ngắt phần cứng) và **trap**, nhưng khác ở một số điểm.  
Trong khi trap là một **synchronous software interrupt** (ngắt phần mềm đồng bộ) xảy ra khi một process gọi trực tiếp một **system call**, thì signal là **asynchronous** (bất đồng bộ) — một process có thể bị ngắt bởi việc nhận signal tại bất kỳ thời điểm nào trong quá trình thực thi.  
Signal cũng khác với asynchronous hardware interrupt ở chỗ chúng được kích hoạt bởi phần mềm chứ không phải thiết bị phần cứng.

Một process có thể gửi signal tới process khác bằng cách thực hiện system call `kill`, yêu cầu OS gửi (post) một signal tới process kia.  
OS sẽ xử lý việc gửi signal tới process đích và thiết lập trạng thái thực thi của nó để chạy signal handler tương ứng với signal được gửi.

> Tên của system call `kill` có thể gây hiểu nhầm và nghe có phần “bạo lực”.  
> Mặc dù nó có thể (và thường) được dùng để gửi signal kết thúc (termination signal), nhưng nó cũng được dùng để gửi **bất kỳ loại signal nào khác** tới một process.

OS cũng tự sử dụng signal để thông báo cho process về một số sự kiện nhất định.  
Ví dụ: OS sẽ gửi signal `SIGCHLD` tới một process khi một trong các **child process** (tiến trình con) của nó kết thúc.

Mỗi hệ thống định nghĩa một số lượng signal cố định (ví dụ: Linux định nghĩa 32 loại signal khác nhau).  
Do đó, signal cung cấp một cách thức giao tiếp hạn chế giữa các process, so với các phương pháp **interprocess communication** (IPC) khác như **message passing** hoặc **shared memory**.

**Bảng 1** liệt kê một số signal đã được định nghĩa.  
Xem thêm trong **man page** (`man 7 signal`) để biết thêm ví dụ.

| Signal Name | Mô tả |
|-------------|-------|
| `SIGSEGV`   | Lỗi phân đoạn (ví dụ: dereference một con trỏ null) |
| `SIGINT`    | Ngắt process (ví dụ: nhấn Ctrl-C trong cửa sổ terminal để dừng process) |
| `SIGCHLD`   | Child process đã kết thúc (ví dụ: một child trở thành zombie sau khi chạy `exit`) |
| `SIGALRM`   | Thông báo cho process khi bộ đếm thời gian hết (ví dụ: `alarm(2)` mỗi 2 giây) |
| `SIGKILL`   | Kết thúc process (ví dụ: `pkill -9 a.out`) |
| `SIGBUS`    | Lỗi bus (ví dụ: truy cập một địa chỉ bộ nhớ không căn chỉnh để đọc giá trị `int`) |
| `SIGSTOP`   | Tạm dừng process, chuyển sang trạng thái **Blocked** (ví dụ: Ctrl-Z) |
| `SIGCONT`   | Tiếp tục một process bị chặn (chuyển sang trạng thái **Ready**; ví dụ: `bg` hoặc `fg`) |

**Bảng 1.** Ví dụ về các signal dùng cho giao tiếp liên tiến trình.

Khi một process nhận được signal, một trong số các hành động mặc định sau có thể xảy ra:

- Process có thể bị kết thúc (**terminate**).
- Signal có thể bị bỏ qua (**ignore**).
- Process có thể bị chặn (**blocked**).
- Process có thể được bỏ chặn (**unblocked**).


OS định nghĩa một **hành động mặc định** và cung cấp mã **default signal handler** (trình xử lý tín hiệu mặc định) cho mỗi số hiệu signal.  
Tuy nhiên, lập trình viên ứng dụng có thể thay đổi hành động mặc định của hầu hết các signal và có thể viết mã signal handler của riêng mình.  
Nếu một chương trình ứng dụng không đăng ký hàm signal handler riêng cho một signal cụ thể, thì handler mặc định của OS sẽ được thực thi khi process nhận signal đó.  

Với một số signal, hành động mặc định do OS định nghĩa **không thể** bị ghi đè bởi mã signal handler của ứng dụng.  
Ví dụ: nếu một process nhận signal `SIGKILL`, OS **luôn** buộc process phải thoát; và khi nhận signal `SIGSTOP`, process sẽ **luôn** bị chặn cho đến khi nhận signal để tiếp tục (`SIGCONT`) hoặc để thoát (`SIGKILL`).

Linux hỗ trợ hai system call khác nhau có thể được dùng để thay đổi hành vi mặc định của một signal hoặc để đăng ký signal handler cho một signal cụ thể: `sigaction` và `signal`.  
Vì `sigaction` tuân thủ chuẩn POSIX và có nhiều tính năng hơn, nó nên được dùng trong phần mềm triển khai thực tế.  
Tuy nhiên, trong ví dụ mã của chúng ta, ta dùng `signal` vì nó dễ hiểu hơn.

Dưới đây là [chương trình ví dụ](_attachments/signals.c) đăng ký signal handler cho các signal `SIGALRM`, `SIGINT` và `SIGCONT` bằng system call `signal` (đã bỏ phần xử lý lỗi để dễ đọc):

#### Ví dụ đăng ký signal handler

```c
/*
 * Example of signal handlers for SIGALRM, SIGINT, and SIGCONT
 *
 * A signal handler function prototype must match:
 *   void handler_function_name(int signum);
 *
 * Compile and run this program, then send this process signals by executing:
 *  kill -INT  pid  (or Ctrl-C) will send a SIGINT
 *  kill -CONT pid  (or Ctrl-Z fg) will send a SIGCONT
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* signal handler for SIGALRM */
void sigalarm_handler(int sig) {
    printf("BEEP, signal number %d\n.", sig);
    fflush(stdout);
    alarm(5);  /* sends another SIGALRM in 5 seconds */
}

/* signal handler for SIGCONT */
void sigcont_handler(int sig) {
    printf("in sigcont handler function, signal number %d\n.", sig);
    fflush(stdout);
}

/* signal handler for SIGINT */
void sigint_handler(int sig) {
    printf("in sigint handler function, signal number %d...exiting\n.", sig);
    fflush(stdout);
    exit(0);
}

/* main: register signal handlers and repeatedly block until receive signal */
int main(void) {

    /* Register signal handlers. */
    if (signal(SIGCONT, sigcont_handler) == SIG_ERR) {
        printf("Error call to signal, SIGCONT\n");
        exit(1);
    }

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        printf("Error call to signal, SIGINT\n");
        exit(1);
    }

    if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) {
        printf("Error call to signal, SIGALRM\n");
        exit(1);
    }

    printf("kill -CONT %d to send SIGCONT\n", getpid());

    alarm(5);  /* sends a SIGALRM in 5 seconds */

    while(1) {
        pause(); /* wait for a signal to happen */
    }
}
```


Khi chạy, process sẽ nhận `SIGALRM` mỗi 5 giây (do lời gọi `alarm` trong `main` và `sigalarm_handler`).  
Các signal `SIGINT` và `SIGCONT` có thể được kích hoạt bằng cách chạy lệnh `kill` hoặc `pkill` trong một shell khác.  
Ví dụ: nếu PID của process là 1234 và file thực thi là `a.out`, thì các lệnh sau sẽ gửi signal `SIGINT` và `SIGCONT` tới process, kích hoạt các hàm signal handler tương ứng:

```bash
pkill -INT a.out
kill  -INT 1234

pkill -CONT a.out
kill  -CONT 1234
```

#### Viết signal handler cho SIGCHLD

Hãy nhớ rằng khi một process kết thúc, OS sẽ gửi signal `SIGCHLD` tới **parent process** của nó.  
Trong các chương trình tạo ra **child process**, parent process không phải lúc nào cũng muốn bị chặn bởi lời gọi `wait` cho đến khi tất cả child process kết thúc.  

Ví dụ: khi một chương trình shell chạy một lệnh ở chế độ nền, nó vẫn tiếp tục chạy song song với child process, xử lý các lệnh shell khác ở chế độ nền trong khi child process chạy ở chế độ nền.  
Tuy nhiên, parent process cần gọi `wait` để **reap** (thu hồi) các **zombie process** sau khi chúng kết thúc.  
Nếu không, các zombie process sẽ không bao giờ biến mất và tiếp tục chiếm giữ một số tài nguyên hệ thống.  

Trong những trường hợp này, parent process có thể đăng ký signal handler cho signal `SIGCHLD`.  
Khi parent nhận `SIGCHLD` từ một child process đã kết thúc, mã handler của nó sẽ chạy và gọi `wait` để thu hồi các zombie process.

Dưới đây là đoạn mã minh họa việc triển khai hàm signal handler cho signal `SIGCHLD`.  
Đoạn mã này cũng cho thấy một phần của hàm `main` đăng ký signal handler cho `SIGCHLD` (lưu ý: việc này nên được thực hiện **trước** bất kỳ lời gọi `fork` nào):

#### Ví dụ signal handler cho SIGCHLD


```c
/*
 * signal handler for SIGCHLD: reaps zombie children
 *  signum: the number of the signal (will be 20 for SIGCHLD)
 */
void sigchld_handler(int signum) {
    int status;
    pid_t pid;

    /*
     * reap any and all exited child processes
     * (loop because there could be more than one)
     */
    while( (pid = waitpid(-1, &status, WNOHANG)) > 0) {
        /* uncomment debug print stmt to see what is being handled
        printf("signal %d me:%d child: %d\n", signum, getpid(), pid);
         */
    }
}

int main(void) {

    /* register SIGCHLD handler: */
    if ( signal(SIGCHLD, sigchild_handler) == SIG_ERR) {
        printf("ERROR signal failed\n");
    exit(1);
    }

    ...

    /* create a child process */
    pid = fork();
    if(pid == 0) {
        /* child code...maybe call execvp */
        ...
    }
    /* the parent continues executing concurrently with child */
    ...
}
```

Trong ví dụ trên, `waitpid` được gọi với PID là `-1`, nghĩa là “thu hồi **bất kỳ** zombie child process nào”.  
Nó cũng truyền cờ `WNOHANG`, nghĩa là lời gọi `waitpid` sẽ **không bị chặn** nếu không có zombie child process nào để thu hồi.  

Ngoài ra, `waitpid` được gọi bên trong vòng lặp `while` và tiếp tục chạy miễn là nó trả về một PID hợp lệ (tức là vẫn còn zombie child process để thu hồi).  
Điều quan trọng là signal handler phải gọi `waitpid` trong vòng lặp, vì trong khi nó đang chạy, process có thể nhận thêm các signal `SIGCHLD` từ các child process khác vừa kết thúc.  

OS **không** theo dõi số lượng signal `SIGCHLD` mà một process nhận được; nó chỉ ghi nhận rằng process đã nhận một `SIGCHLD` và ngắt thực thi của nó để chạy mã handler.  
Do đó, nếu không có vòng lặp, signal handler có thể bỏ sót một số zombie process chưa được thu hồi.

Signal handler sẽ được thực thi bất cứ khi nào parent nhận signal `SIGCHLD`, bất kể parent đang bị chặn bởi lời gọi `wait` hay `waitpid`.  

- Nếu parent đang bị chặn bởi `wait` khi nhận `SIGCHLD`, nó sẽ thức dậy và chạy mã handler để thu hồi một hoặc nhiều zombie process, sau đó tiếp tục thực thi tại vị trí ngay sau lời gọi `wait`.  
- Nếu parent đang bị chặn bởi `waitpid` cho một child cụ thể, thì sau khi handler chạy, parent có thể tiếp tục hoặc vẫn bị chặn:  
  - Nếu handler đã thu hồi đúng child mà `waitpid` đang chờ, parent sẽ tiếp tục thực thi.  
  - Nếu không, parent sẽ tiếp tục bị chặn trong `waitpid` cho đến khi child được chỉ định kết thúc.

Một lời gọi `waitpid` với PID của một child process **không tồn tại** (có thể đã được thu hồi trước đó trong vòng lặp handler) sẽ **không** chặn process gọi.
