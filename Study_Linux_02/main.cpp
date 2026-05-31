#include <iostream>
#include <unistd.h>     // Chứa fork(), getpid(), getppid(), sleep()
#include <sys/types.h>  // Chứa kiểu dữ liệu pid_t
#include <sys/wait.h>   // Chứa hàm wait()

int main() {
    std::cout << "--- HỆ THỐNG BẮT ĐẦU ---" << std::endl;
    std::cout << "Tiến trình gốc (Main) có PID: " << getpid() << std::endl;

    // Gọi hàm nhân bản tiến trình
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Lỗi: Không thể tạo tiến trình con!" << std::endl;
        return 1;
    } 
    else if (pid == 0) {
        // ========== KHÔNG GIAN CỦA TIẾN TRÌNH CON ==========
        // Thường dùng để chạy các tác vụ nặng hoặc rủi ro
        std::cout << "[Child] Tôi là tiến trình con. PID của tôi: " << getpid() 
                  << " | PID của cha tôi: " << getppid() << std::endl;
        
        std::cout << "[Child] Đang xử lý thuật toán Computer Vision..." << std::endl;
        sleep(3); // Giả lập việc xử lý mất 3 giây
        
        std::cout << "[Child] Xử lý xong. Tiến trình con kết thúc!" << std::endl;
        return 0; // Tiến trình con tự hủy tại đây
    } 
    else {
        // ========== KHÔNG GIAN CỦA TIẾN TRÌNH CHA ==========
        std::cout << "[Parent] Tôi là tiến trình cha. Tôi vừa tạo ra đứa con có PID: " << pid << std::endl;
        
        std::cout << "[Parent] Đang làm việc khác (ví dụ: đọc cảm biến)..." << std::endl;
        sleep(1);
        
        std::cout << "[Parent] Chờ tiến trình con xử lý xong để tránh tạo ra Zombie Process..." << std::endl;
        
        // Hàm wait() chặn tiến trình cha lại cho đến khi một tiến trình con kết thúc
        int status;
        wait(&status); 

        std::cout << "[Parent] Con đã chạy xong. Hệ thống kết thúc an toàn." << std::endl;
    }

    return 0;
}