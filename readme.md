ĐỒ ÁN MÔN HỌC HỆ NHÚNG (IT4210)
REACT MOUSE TRÊN STM32F429I VỚI TOUCHGFX
---
GIỚI THIỆU
---
- Mô tả đề bài: Dự án sử dụng vi điều khiển STM32F429 kết hợp với thư viện TouchGFX và giao thức mouse HID để xây dựng một hệ thống tương tác chuột. Khi người dùng chạm vào màn hình cảm ứng, hệ thống thực hiện hai thao tác chính: vẽ một vòng tròn tại vị trí chạm và tạo hiệu ứng thu nhỏ dần về tâm trong vòng 1 giây; đồng thời, hệ thống tính toán và gửi sự kiện mouse HID tương ứng về máy tính để mô phỏng hành vi di chuyển chuột theo vị trí chạm.
- Tính năng chính của sản phẩm
    - Vẽ vòng tròn tại vị trí điểm chạm khi người dùng chạm vào màn hình cảm ứng:
    - Tạo hiệu ứng thu vòng tròn dần về tâm trong vòng 1 giây
    - Gửi lệnh di chuyển chuột tương ứng tới máy tính qua giao diện USB HID để di chuyển theo chuột theo vị trí tương đối trên màn hình máy tính
- Ảnh chụp mình họa:
    - Ảnh vẽ hiệu ứng hình tròn thu nhỏ trên màn hình: 
![z6728915376862_593ddcb1f3458b23185c070664ce9c37](https://github.com/user-attachments/assets/3df02fb6-ce10-4c2f-92ef-b1d7d9d7b56e)

TÁC GIẢ
---
- Tên nhóm: CLOUD
- Thành viên trong nhóm:
  
  <table>
  <tr>
    <th>STT</th>
    <th>Họ tên</th>
    <th>MSSV</th>
    <th>Công việc</th>
  </tr>
  <tr>
    <td>1</td>
    <td>Lê Minh Mạnh</td>
    <td>20225360</td>
    <td>
      Xử lý sự kiện gửi vị trí điểm chạm và di chuyển tương đối vị trí điểm chạm đó trên màn hình máy tính
    </td>
  </tr>
  <tr>
    <td>2</td>
    <td>Dương Quang Khải</td>
    <td>20225340</td>
    <td>
      Vẽ hình tròn và tạo hiệu ứng thu nhỏ dần về tâm tại vị trí điểm chạm trên màn hình
    </td>
  </tr>
</table>

MÔI TRƯỜNG HOẠT ĐỘNG
---
- Module CPU/Dev kit: STM32F429I
- Các kit, module phần cứng được sử dụng:
     - STM32F429I Discovery kit: Bao gồm vi điều khiển chính, màn hình LCD-TFT 2.4 inch (240x320 pixels) với bộ điều khiển cảm ứng (không sử dụng cảm ứng trong dự án này), và các ngoại vi cần thiết.
- Phần mềm và Công cụ phát triển:
     - STM32CubeIDE: Môi trường phát triển tích hợp (IDE) chính để viết mã, biên dịch, và gỡ lỗi firmware.
     - TouchGFX Designer: Công cụ thiết kế giao diện người dùng đồ họa, cho phép tạo các màn hình, widget và luồng tương tác. Code giao diện được tạo tự động và tích hợp vào dự án STM32CubeIDE.
     - STM32CubeMX: (Tích hợp trong STM32CubeIDE) Công cụ cấu hình vi điều khiển, dùng để thiết lập các chân GPIO, ngoại vi ADC, clock hệ thống, và middleware TouchGFX.
     - Trình biên dịch ARM GCC: (Đi kèm với STM32CubeIDE).

TÍCH HỢP HỆ THỐNG
---
- Thành phần phần cứng và vai trò của chúng
    - STM32F429I Discovery Kit (MCU & Display): Là "bộ não" của hệ thống. Vi điều khiển STM32F429ZIT6U đảm nhận toàn bộ việc xử lý vị trí chạm từ người dùng, thực hiện chuyển đổi tọa độ từ màn hình cảm ứng sang hệ tọa độ máy tính, gửi lệnh điều khiển chuột thông qua giao thức HID USB, và cập nhật giao diện hiển thị trên màn hình LCD-TFT tích hợp.
    - Màn hình cảm ứng 2.4 inch (có sẵn trên kit): Đây là thiết bị đầu vào chính. Người dùng tương tác bằng cách chạm trực tiếp lên màn hình. Mỗi thao tác chạm sẽ được phát hiện và xử lý bởi TouchGFX thông qua sự kiện ClickEvent, từ đó xác định tọa độ và truyền về máy tính dưới dạng chuột di chuyển.
     - Cổng USB OTG HS : Là kênh giao tiếp giữa STM32F429 và máy tính. Cổng này được cấu hình để hoạt động như một thiết bị HID (Human Interface Device), mô phỏng chuột USB. Các lệnh di chuyển chuột được gửi từ vi điều khiển đến máy tính thông qua giao tiếp này.
     - LED PG13 (User LED) Được sử dụng làm đèn thông báo trạng thái chạm. Đèn sáng khi người dùng đang giữ tay trên màn hình, và tắt khi người dùng thả tay ra.
- Thành phần phần mềm và vai trò của chúng (chạy trên STM32F429)
     - Firmware STM32 (Viết bằng C++)
          - Screen1View / Screen1Presenter:
              - Là màn hình chính điều khiển bàn di chuột.
              - Nhận sự kiện chạm (ClickEvent) để xác định tọa độ điểm chạm.
              - Tính toán tọa độ tương ứng trên màn hình máy tính (dựa vào tỉ lệ scale giữa màn hình cảm ứng 320x240 và độ phân giải máy tính 1920x1080).
              - Gửi lệnh di chuột thông qua hàm SendMouseHIDUSB(dx, dy) bằng giao thức HID USB.
              - Gọi các hàm circleX.setVisible(true) và invalidate() để tạo hiệu ứng vẽ vòng tròn thu nhỏ tại điểm chạm.
              - Điều khiển LED PG13 để phản ánh trạng thái chạm: sáng khi nhấn, tắt khi thả.
          - SendMouseHIDUSB(deltaX, deltaY)
              - Tạo và gửi gói tin HID kiểu chuột, gồm thông tin dịch chuyển x, y.
              - Đảm bảo mỗi bước di chuyển không vượt quá ±127 (giới hạn của chuẩn HID).
              - Gửi từng bước nhỏ đến máy tính để mô phỏng di chuyển chuột mượt mà.
    - TouchGFX Framework / Engine
          - Là middleware đồ họa hoạt động trên STM32, quản lý giao diện người dùng.
          - Nhận và xử lý sự kiện ClickEvent từ màn hình cảm ứng.
          - Vẽ các vòng tròn circle1, circle2, circle3, circle4 lần lượt thu nhỏ về tâm tạo hiệu ứng khi chạm.
    - STM32Cube HAL Drivers (Hardware Abstraction Layer)
          - Là thư viện phần cứng cấp thấp do ST cung cấp.
          - Cung cấp API để điều khiển chân LED PG13 (GPIO).
          - Quản lý thời gian thực (dùng HAL_Delay() để ngắt giữa các gói HID hoặc hiệu ứng).
          - Tương tác với USB thông qua middleware usbd_hid.c để gửi gói HID chuẩn.
      
ĐẶC TẢ HÀM
---

```cpp
/**
 * @brief Gửi lệnh di chuyển chuột qua USB HID.
 *
 * Tính toán delta di chuyển tương đối (giới hạn ±127) và gửi gói tin HID chuột
 * về máy tính qua cổng USB. Mỗi gói gồm thông tin dịch chuyển x, y và được 
 * gửi cách nhau 1ms để đảm bảo host nhận kịp.
 *
 * @param deltaX Khoảng cách di chuyển theo trục X (float, -127 đến 127)
 * @param deltaY Khoảng cách di chuyển theo trục Y (float, -127 đến 127)
 */
void SendMouseHIDUSB(float deltaX, float deltaY);
```

```cpp
/**
 * @brief Xử lý sự kiện chạm từ màn hình cảm ứng.
 *
 * Hàm xử lý khi người dùng nhấn xuống (PRESSED) hoặc thả ra (RELEASED) trên màn hình cảm ứng:
 *
 * - PRESSED:
 *   + Bật LED PG13 làm đèn báo.
 *   + Tính tọa độ điểm chạm trên màn hình cảm ứng (320x240).
 *   + Chuyển đổi sang tọa độ tương ứng trên màn hình máy tính (1920x1080).
 *   + Gửi gói HID để di chuyển chuột tương ứng.
 *   + Hiển thị hiệu ứng vòng tròn thu nhỏ tại vị trí chạm.
 *
 * - RELEASED:
 *   + Tắt LED PG13.
 *
 * @param evt Sự kiện chạm từ TouchGFX (ClickEvent)
 */
void Screen1View::handleClickEvent(const ClickEvent& evt);
```

```cpp
/**
 * @brief Tạo hiệu ứng hình tròn thu nhỏ dần về tâm khi chạm.
 *
 * Hàm được gọi định kỳ theo tick của TouchGFX. Dựa vào biến `currentCircle`, lần lượt hiển thị
 * các vòng tròn từ lớn đến nhỏ để tạo hiệu ứng thu nhỏ dần, mỗi hình giữ trong 250ms.
 * Sau khi hoàn tất, tất cả các vòng tròn được ẩn đi.
 */
void Screen1View::handleTickEvent();
```

KẾT QUẢ
---
 
