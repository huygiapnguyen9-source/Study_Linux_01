#include <iostream>
#include <fcntl.h>    // Thư viện chứa các cờ flag như O_RDONLY, O_WRONLY, O_CREAT
#include <unistd.h>   // Thư viện chứa các system call như: read, write, close
#include <cstring>    // Thư viện chứa strerror
#include <cerrno>     // Thư viện chứa errno để bắt lỗi hệ thống
#include <sys/stat.h> // Thư viện chứa S_IRUSR và S_IWUSR là phân quyền cho phép User có quyền...

int main()
{
    // 1. Mở file input.txt để ở chế độ chỉ đọc (Read Only)
    //  Hàm open() trả về một "File Descriptor" (fd) - là một số nguyên quản lý bởi Kernel
    int fd_in = open("input.txt", O_RDONLY);
    if (fd_in < 0)
    {
        std ::cerr << "Lỗi mở file input.txt: " << std ::strerror(errno) << std ::endl;
        return 1;
    }

    // 2. Mở hoặc tạo mới file output.txt ở chế độ chỉ ghi (Write Only)
    // S_IRUSR | S_IWUSR là phân quyền cho phếp User có quyền Read/Write file này
    int fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out < 0)
    {
        std ::cerr << "Lỗi mở/tạo file output.txt" << std ::strerror(errno) << std::endl;
        close(fd_in); // Đóng file đã mở trước
        return 1;
    }

    // 3. Đọc dữ liệu từ fd_in và ghi sang fd_out sử dụng 1 vùng đệm (Buffer)
    char buffer[1024];
    ssize_t bytes_read;

    // Hàm read() trả về số byte thực tế đọc được. Trả về 0 tức là đã hết file (EOF).
    while ((bytes_read = read(fd_in, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytes_writeen = write(fd_out, buffer, bytes_read);
        if (bytes_writeen < 0)
        {
            std ::cerr << "Lỗi ghi dữ liệu: " << std ::strerror(errno) << std ::endl;
            break;
        }
    }
    if (bytes_read < 0)
    {
        std ::cerr << "Lỗi đọc dữ liệu: " << std::strerror(errno) << std ::endl;
    }

    // 4. Đóng các File Descriptor để giải phóng tài nguyên cho hệ điều hành
    close(fd_in);
    close(fd_out);

    std ::cout << "Sao chép file hoàn tất thành công! " << std ::endl;
    return 0;
}