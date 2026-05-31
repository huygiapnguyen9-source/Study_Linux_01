# Study Linux Project (Study_Linux_02)

## Nội dung
Ví dụ về **Linux Process Management** sử dụng hàm POSIX `fork()` để tạo tiến trình con.

Chương trình in ra PID của tiến trình cha (Parent) và con (Child), mô phỏng xử lý (child sleep 3 giây) và cha chờ kết thúc bằng `wait()` để tránh Zombie Process.

