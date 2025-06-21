#include <gui/screen1_screen/Screen1View.hpp>
#include "usb_device.h"
#include "usbd_hid.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

// Đối tượng USB HID từ middleware
extern USBD_HandleTypeDef hUsbDeviceHS;

// Vị trí hiện tại của chuột
float currentX = 0;
float currentY = 0;

// Cấu trúc báo cáo HID chuột
typedef struct {
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
} MouseHID_Report_t;

// Gửi báo cáo HID với delta tương đối
void SendMouseHIDUSB(float deltaX, float deltaY)
{
    MouseHID_Report_t mouseReport;

    // Giới hạn giá trị delta trong phạm vi -127 đến 127
    if (deltaX > 127) deltaX = 127;
    if (deltaX < -127) deltaX = -127;
    if (deltaY > 127) deltaY = 127;
    if (deltaY < -127) deltaY = -127;

    mouseReport.x = (int8_t)deltaX;
    mouseReport.y = (int8_t)deltaY;
    mouseReport.buttons = 0;
    mouseReport.wheel = 0;

    USBD_HID_SendReport(&hUsbDeviceHS, (uint8_t*)&mouseReport, sizeof(mouseReport));

    HAL_Delay(1); // Đảm bảo host nhận kịp gói tin
}

Screen1View::Screen1View() {}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

// Xử lý sự kiện chạm
//void Screen1View::handleClickEvent(const ClickEvent& evt)
//{
//    if (evt.getType() == ClickEvent::PRESSED)
//    {
//        // Tỷ lệ từ màn hình cảm ứng 320x240 → PC 1920x1080
//        float scaleX = 1920.0f * 2 / 320.0f; // vì setup tần số là 12,5 mhz
//        float scaleY = 1080.0f / 240.0f;
//
//        int16_t rawX = evt.getX();
//        int16_t rawY = evt.getY();
//
//        float targetX = rawX * scaleX;
//        float targetY = rawY * scaleY;
//
//        float deltaX = targetX - currentX;
//        float deltaY = targetY - currentY;
//
//        // Gửi từng bước ±127
//        while (deltaX != 0 || deltaY != 0)
//        {
//            float stepX = (deltaX > 127) ? 127 : (deltaX < -127) ? -127 : deltaX;
//            float stepY = (deltaY > 127) ? 127 : (deltaY < -127) ? -127 : deltaY;
//
//            SendMouseHIDUSB(stepX, stepY);
//
//            currentX += stepX;
//            currentY += stepY;
//            deltaX -= stepX;
//            deltaY -= stepY;
//        }
//        currentCircle = 0;
//		circle4.moveTo(evt.getX() - 20, evt.getY() - 20);
//		circle4.invalidate();
//		circle3.moveTo(evt.getX() - 20, evt.getY() - 20);
//		circle3.invalidate();
//		circle2.moveTo(evt.getX() - 20, evt.getY() - 20);
//		circle2.invalidate();
//		circle1.moveTo(evt.getX() - 20, evt.getY() - 20);
//		circle1.invalidate();
//        // Toggle đèn debug
//        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
//        invalidate();
//    }
void Screen1View::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        // Bật đèn khi chạm
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

        // Tính toán tỷ lệ từ màn hình cảm ứng 320x240 sang độ phân giải PC 1920x1080
        float scaleX = 1920.0f * 2 / 320.0f;  // do setup tần số là 12.5 MHz
        float scaleY = 1080.0f / 240.0f;

        int16_t rawX = evt.getX();
        int16_t rawY = evt.getY();

        float targetX = rawX * scaleX;
        float targetY = rawY * scaleY;

        float deltaX = targetX - currentX;
        float deltaY = targetY - currentY;

        // Chia nhỏ bước di chuyển (tối đa ±127) để gửi nhiều gói HID
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

        // Hiển thị vòng tròn tại vị trí chạm
        currentCircle = 0;
        circle4.moveTo(rawX - 20, rawY - 20);
        circle4.invalidate();
        circle3.moveTo(rawX - 20, rawY - 20);
        circle3.invalidate();
        circle2.moveTo(rawX - 20, rawY - 20);
        circle2.invalidate();
        circle1.moveTo(rawX - 20, rawY - 20);
        circle1.invalidate();

        invalidate();
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        // Tắt đèn khi thả tay ra khỏi màn hình
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
    }
}


void Screen1View::handleTickEvent()
{

	    if (currentCircle <= 5) // Mỗi giây
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
	        			osDelay(1000/4);
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
	        			osDelay(1000/4);
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
	        			osDelay(1000/4);
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
	        			osDelay(1000/4);
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
						osDelay(1000/4);
	        	}
	        currentCircle++;

	    }
}
