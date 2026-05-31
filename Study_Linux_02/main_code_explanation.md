## Mục tiêu
Đoạn code C++ dưới đây dùng **POSIX `fork()`** để:
1) Tạo một tiến trình con từ tiến trình cha
2) In ra PID của cha và con
3) Tiến trình con mô phỏng công việc bằng `sleep(3)` rồi kết thúc
4) Tiến trình cha dùng `wait()` để chờ con kết thúc, tránh Zombie Process

---

## 1) Include các thư viện cần thiết
```cpp
#include <iostream>
#include <unistd.h>     // fork(), getpid(), getppid(), sleep()
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait()
```

- `unistd.h`:
  - `fork()`: tạo tiến trình mới
  - `getpid()`: lấy PID tiến trình hiện tại
  - `getppid()`: lấy PID của cha
  - `sleep()`: tạm dừng
- `sys/types.h`: định nghĩa kiểu dữ liệu `pid_t`.
- `sys/wait.h`: khai báo `wait()`.

---

## 2) In PID của tiến trình gốc
Trong đầu `main()`:
- `getpid()` lấy PID của tiến trình đang chạy (tức process hiện tại trước khi fork).

---

## 3) Tạo tiến trình con với `fork()`
```cpp
pid_t pid = fork();
```
`fork()` trả về:
- `< 0`: lỗi tạo tiến trình
- `== 0`: đang ở **tiến trình con**
- `> 0`: đang ở **tiến trình cha**, và giá trị trả về chính là PID của tiến trình con

---

## 4) Nhánh xử lý khi fork lỗi
```cpp
if (pid < 0) {
    std::cerr << "Lỗi: Không thể tạo tiến trình con!" << std::endl;
    return 1;
}
```
Nếu `fork()` thất bại thì chương trình kết thúc với mã lỗi.

---

## 5) Không gian của tiến trình con (Child)
```cpp
else if (pid == 0) {
    std::cout << "[Child] ... PID của tôi: " << getpid()
              << " | PID của cha tôi: " << getppid() << std::endl;

    std::cout << "[Child] Đang xử lý ..." << std::endl;
    sleep(3);

    std::cout << "[Child] Xử lý xong. ..." << std::endl;
    return 0;
}
```
- In PID của chính nó (`getpid()`)
- In PID của cha (`getppid()`)
- Mô phỏng xử lý mất 3 giây rồi thoát.

---

## 6) Không gian của tiến trình cha (Parent)
```cpp
else {
    std::cout << "[Parent] ... PID của con: " << pid << std::endl;
    sleep(1);

    int status;
    wait(&status);

    std::cout << "[Parent] Con đã chạy xong. ..." << std::endl;
}
```
- In PID con mà `fork()` trả về
- `sleep(1)` để mô phỏng cha làm việc khác trong thời gian con chạy
- `wait(&status)` chặn cha cho tới khi con kết thúc (tránh Zombie Process)

---

## 7) Tại sao cần `wait()`?
Nếu cha không gọi `wait()` thì tiến trình con kết thúc xong nhưng vẫn giữ trạng thái (Zombie), gây rác trong bảng tiến trình.

---

## Tóm tắt luồng chương trình
1) In PID của main
2) `fork()` tạo tiến trình con
3) Child: in thông tin, sleep 3 giây, kết thúc
4) Parent: in thông tin, sleep 1 giây, `wait()` cho đến khi child kết thúc


