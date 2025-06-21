 - Dự án React Mouse: sử dụng STM32F429 và TouchGFX, thư viện mouse HID.
 - Phát hiện vị trí user chạm trên màn hình và thực hiện 2 thao tác bên dưới
 - Vẽ circle ở vị trí  touch trên màn hình Touch, và tạo hiệu ứng vòng tròn thu nhỏ về tâm trong 1 giây.
 - Gửi sự kiện mouse HID về máy tính, để chuột di chuột tương ứng trên máy tính. 


ĐẶC TẢ HÀM
Giải thích một số hàm quan trọng: ý nghĩa của hàm, tham số vào, ra
// Xử lý sự kiện chạm
void Screen1View::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        // Tỷ lệ từ màn hình cảm ứng 320x240 → PC 1920x1080
        float scaleX = 1920.0f * 2 / 320.0f; // vì setup tần số là 12,5 mhz
        float scaleY = 1080.0f / 240.0f;

        int16_t rawX = evt.getX();
        int16_t rawY = evt.getY();

        float targetX = rawX * scaleX;
        float targetY = rawY * scaleY;

        float deltaX = targetX - currentX;
        float deltaY = targetY - currentY;

        // Gửi từng bước ±127
        while (deltaX != 0 || deltaY != 0)
        {
            float stepX = (deltaX > 127) ? 127 : (deltaX < -127) ? -127 : deltaX;
            float stepY = (deltaY > 127) ? 127 : (deltaY < -127) ? -127 : deltaY;

            SendMouseHIDUSB(stepX, stepY);

            currentX += stepX;
            currentY += stepY;
            deltaX -= stepX;
            deltaY -= stepY;
        }
        currentCircle = 0;
		circle4.moveTo(evt.getX() - 20, evt.getY() - 20);
		circle4.invalidate();
		circle3.moveTo(evt.getX() - 20, evt.getY() - 20);
		circle3.invalidate();
		circle2.moveTo(evt.getX() - 20, evt.getY() - 20);
		circle2.invalidate();
		circle1.moveTo(evt.getX() - 20, evt.getY() - 20);
		circle1.invalidate();
        // Toggle đèn debug
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
        invalidate();
    }
}
// Hiển thị hình tròn thu nhỏ
	    if (currentCircle <= 5) 
	    {

	        	switch(currentCircle)
	        	{
	        		case 0:
	        			circle1.setVisible(false);
						circle1.invalidate();
						circle2.setVisible(false);
						circle2.invalidate();
						circle3.setVisible(false);
						circle3.invalidate();
						circle4.setVisible(false);
						circle4.invalidate();
	        			circle4.setVisible(true);
	        			circle4.invalidate();
	        			osDelay(1000/4); // Mỗi hình hiển thị trong vòng 1 giây chia 4
	        			break;
	        		case 1:
	        			circle1.setVisible(false);
						circle1.invalidate();
						circle2.setVisible(false);
						circle2.invalidate();
						circle3.setVisible(false);
						circle3.invalidate();
						circle4.setVisible(false);
						circle4.invalidate();
	        			circle3.setVisible(true);
	        			circle3.invalidate();
	        			osDelay(1000/4); // Mỗi hình hiển thị trong vòng 1 giây chia 4
	        			break;
	        		case 2:
	        			circle1.setVisible(false);
						circle1.invalidate();
						circle2.setVisible(false);
						circle2.invalidate();
						circle3.setVisible(false);
						circle3.invalidate();
						circle4.setVisible(false);
						circle4.invalidate();
	        			circle2.setVisible(true);
	        			circle2.invalidate();
	        			osDelay(1000/4);// Mỗi hình hiển thị trong vòng 1 giây chia 4
	        			break;
	        		case 3:
	        			circle1.setVisible(false);
						circle1.invalidate();
						circle2.setVisible(false);
						circle2.invalidate();
						circle3.setVisible(false);
						circle3.invalidate();
						circle4.setVisible(false);
						circle4.invalidate();
	        			circle1.setVisible(true);
	        			circle1.invalidate();
	        			osDelay(1000/4);// Mỗi hình hiển thị trong vòng 1 giây chia 4
	        			break;
	        		case 4:
	        			circle1.setVisible(false);
						circle1.invalidate();
						circle2.setVisible(false);
						circle2.invalidate();
						circle3.setVisible(false);
						circle3.invalidate();
						circle4.setVisible(false);
						circle4.invalidate();
						osDelay(1000/4);// Mỗi hình hiển thị trong vòng 1 giây chia 4
	        	}
	        currentCircle++;

	    }

# STM32F429I_DISCO_REV_D01 TBS

The default IDE is set to STM32CubeIDE, to change IDE open the STM32F429I_DISCO_REV_D01.ioc with STM32CubeMX and select from the supported IDEs (EWARM from version 8.50.9, MDK-ARM, and STM32CubeIDE). Supports flashing of the STM32F429I_DISCO_DEV_D01 board directly from TouchGFX Designer using GCC and STM32CubeProgrammer. Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage. 

This TBS is configured for 320 x 240 pixels 16bpp screen resolution.  

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ  - Pin PE2, RENDER_TIME - Pin PE3, FRAME_RATE  - Pin PE4, MCU_ACTIVE  - Pin PE5
 