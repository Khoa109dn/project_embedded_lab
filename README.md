🎯 Mục tiêu:
Hệ thống sử dụng hai board STM32 giao tiếp qua mạng CAN Bus để giám sát môi trường và điều khiển cơ cấu chấp hành:

-Đọc dữ liệu độ ẩm từ cảm biến AHT20. Nếu độ ẩm vượt ngưỡng an toàn (Độ ẩm cao), board sẽ gửi lệnh dừng qua CAN Bus.

-Nhận lệnh từ máy tính (1,f,space) <-> (tăng 10', reset, lùi 10') để điều khiển Servo. Khi độ ẩm bình thường, Servo quay; khi nhận tín hiệu "Độ ẩm cao", Servo sẽ lập tức dừng quay.

Hiển thị: góc quay của servo hiện tại so với góc ban đầu.

🔌 Sơ đồ đấu chân :
* Linh kiện: 2 stm32f103c8t6, 2 mcp2551, 1 màn oled ssd1306 giao tiếp spi, 1 nguồn nuôi (adapter), 1 servo, 1 aht20, 1 dc2551 đầu cái kèm domino.

1. Kết nối CAN Bus (Qua MCP2551)
Kết nối tương tự trên cả 2 board STM32:
STM32 (CAN_TX/RX) ↔ MCP2551 (TXD/RXD)
MCP2551 (CANH/CANL) ↔ (CANH/CANL) MCP2551 của module còn lại.
Giữa 2 chân CANH/ CANL có 1 điện trở 120 ohm.

3. Cảm biến AHT20 (Giao tiếp I2C)
VCC: 3.3V
GND: GND
SCL / SDA: PB6 / PB7.

4. Màn hình LCD/OLED SPI (7 chân)
VCC / GND: 3.3V / GND
SCL (Clock) / SDA (Data): Kết nối chân SPI (PA5 / PA7).
RES / DC / CS: Các chân điều khiển Digital (PB0, PB1, PA4).
BLK (Backlight): Nối 3.3V hoặc chân PWM nếu muốn chỉnh độ sáng.

5. Servo & Nguồn Adapter
   Adapter đầu DC <-> DC2551 Cái Kèm Domino với 2 đầu dương/ âm.
Dây Cam (Signal): Kết nối chân PWM trên STM32 ( PA0).
Dây Đỏ (VCC): Nối vào cực (+) của Adapter 5V.
Dây Nâu (GND): Nối vào cực (-) của Adapter và nối chung với chân GND của STM32.

Video sản phẩm:
https://drive.google.com/drive/folders/1v5a0IQ5o6gKGarEnl4JRJTmHVa0CXFZk?usp=drive_link
