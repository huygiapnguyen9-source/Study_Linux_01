vi# Giải thích đoạn code `main.cpp` (Sao chép file input.txt → output.txt bằng POSIX)

## Mục tiêu
Đoạn code C++ dưới đây dùng **POSIX system calls** để **sao chép nội dung từ `input.txt` sang `output.txt`** bằng cách:
1) Mở file input ở chế độ đọc  
2) Mở/tạo file output ở chế độ ghi (xóa nội dung cũ)  
3) Đọc theo từng khối bằng `read()` rồi ghi bằng `write()`  
4) Đóng file descriptor

---

## 1) Include các thư viện cần thiết
```cpp
#include <iostream>
#include <fcntl.h>    // O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC...
#include <unistd.h>   // read, write, close
#include <cstring>    // strerror
#include <cerrno>     // errno

#include <sys/stat.h> // S_IRUSR và S_IWUSR (quyền truy cập file)
```

- `fcntl.h`: định nghĩa **các cờ (flags)** cho `open()`
  - `O_RDONLY`: mở để đọc
  - `O_WRONLY`: mở để ghi
  - `O_CREAT`: nếu file chưa tồn tại thì tạo mới
  - `O_TRUNC`: nếu file tồn tại thì cắt/xóa nội dung về rỗng
- `unistd.h`: định nghĩa **các system call POSIX**:
  - `read()`, `write()`, `close()`
- `cerrno` + `cstring`:
  - `errno` lưu mã lỗi hệ thống nếu syscalls thất bại
  - `std::strerror(errno)` chuyển mã lỗi thành chuỗi mô tả
- `sys/stat.h`:
  - `S_IRUSR`: quyền **đọc** cho user
  - `S_IWUSR`: quyền **ghi** cho user

---

## 2) Khai báo hàm `main`
```cpp
int main()
{
```
Chương trình bắt đầu.

---

## 3) Mở `input.txt` để đọc (Read Only)
```cpp
int fd_in = open("input.txt", O_RDONLY);
if (fd_in < 0)
{
    std ::cerr << "Lỗi mở file input.txt: " << std ::strerror(errno) << std ::endl;
    return 1;
}
```

- `open()` mở file và trả về một **file descriptor** (`fd_in`).
- File descriptor là một **số nguyên** do OS (kernel) quản lý để tham chiếu đến file.
- Nếu `open()` thất bại:
  - trả về giá trị âm (thường là `-1`)
  - in lỗi chi tiết bằng `errno`
  - kết thúc chương trình với `return 1`

---

## 4) Mở/tạo `output.txt` để ghi (Write Only)
```cpp
int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
if (fd_out < 0)
{
    std ::cerr << "Lỗi mở/tạo file output.txt" << std ::strerror(errno) << std ::endl;
    close(fd_in); // Đóng file đã mở trước
    return 1;
}
```

- Các cờ trong `open()`:
  - `O_WRONLY`: chỉ ghi
  - `O_CREAT`: tạo nếu chưa tồn tại
  - `O_TRUNC`: cắt nội dung cũ về rỗng
- Tham số thứ 3 `S_IRUSR | S_IWUSR` chỉ có ý nghĩa khi có `O_CREAT`:
  - cấp quyền đọc/ghi cho user đối với file mới tạo.
- Nếu thất bại:
  - in lỗi
  - `close(fd_in)` để giải phóng tài nguyên đã mở
  - `return 1`

---

## 5) Sao chép dữ liệu bằng buffer (1024 bytes)
```cpp
char buffer[1024];
ssize_t bytes_read;
```

- `buffer[1024]`: vùng đệm lưu tạm dữ liệu đọc được.
- `ssize_t bytes_read`:
  - kiểu trả về của `read()`
  - `> 0`: số byte đọc được
  - `0`: EOF (hết file)
  - `< 0`: lỗi

---

## 6) Vòng lặp `read()` → `write()` cho tới EOF
```cpp
while ((bytes_read = read(fd_in, buffer, sizeof(buffer))) > 0)
{
    ssize_t bytes_writeen = write(fd_out, buffer, bytes_read);
    if (bytes_writeen < 0)
    {
        std ::cerr << "Lỗi ghi dữ liệu: " << std ::strerror(errno) << std ::endl;
        break;
    }
}
```

- `read(fd_in, buffer, sizeof(buffer))` đọc tối đa 1024 bytes từ `input.txt` vào `buffer`.
- Điều kiện `> 0` đảm bảo chỉ lặp khi còn dữ liệu.
- `write(fd_out, buffer, bytes_read)` ghi chính xác `bytes_read` byte từ `buffer` sang `output.txt`.
- Nếu `write()` thất bại (`< 0`) thì in lỗi và `break` thoát vòng lặp.

> Lưu ý: biến `bytes_writeen` có typo (đúng ra thường là `bytes_written`) nhưng không ảnh hưởng logic.

---

## 7) Kiểm tra lỗi nếu `read()` thất bại
```cpp
if (bytes_read < 0)
{
    std ::cerr << "Lỗi đọc dữ liệu: " << std ::strerror(errno) << std ::endl;
}
```

- Sau khi thoát vòng lặp, có 2 khả năng:
  - `bytes_read == 0`: đã tới EOF (đọc hết)
  - `bytes_read < 0`: `read()` gặp lỗi

---

## 8) Đóng file descriptor
```cpp
close(fd_in);
close(fd_out);
```

Đóng để giải phóng tài nguyên hệ điều hành.

---

## 9) Thông báo hoàn tất
```cpp
std ::cout << "Sao chép file hoàn tất thành công! " << std ::endl;
return 0;
```

---

## Tóm tắt luồng chương trình
1. `open("input.txt", O_RDONLY)` lấy `fd_in`
2. `open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, permissions)` lấy `fd_out`
3. Lặp:
   - `read(fd_in, buffer, 1024)`
   - `write(fd_out, buffer, bytes_read)`
   cho tới khi EOF
4. `close(fd_in)`, `close(fd_out)`

---

Nếu bạn muốn, có thể mở file markdown này để xem lại nội dung giải thích: 
- `Study_Linux_01/main_code_explanation.md`

