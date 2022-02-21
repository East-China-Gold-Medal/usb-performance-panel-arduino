#include <PluggableUSB.h>
#include <EEPROM.h>
#define CALIBRATION_EEPROM_ADDRESS 0x00
uint8_t calibration;
typedef enum {
    DATA_CPU_USAGE=0x01,
    DATA_RAM_USAGE=0x02,
//...
    DATA_MAX_OUT  =0x80
}AvailableDataChannels;
typedef enum {
    PWM_PIN_CPU=3,
    PWM_PIN_RAM=5
}AvailablePwmPins;
typedef enum {
    COMMAND_QUERY_CAP = 0xFF,
    COMMAND_SET_USAGE = 0xFE
}HostOperationCommand;
typedef struct {
    InterfaceDescriptor id;
} PerfPanelDescriptor;
void set_pwm(uint8_t channel,uint8_t data)
{
    uint32_t tmp;
    tmp = data*calibration;
    tmp /= 256;
    switch(channel){
        case DATA_CPU_USAGE: {
            analogWrite(PWM_PIN_CPU, (uint8_t)tmp);
            break;
        }
        case DATA_RAM_USAGE: {
            analogWrite(PWM_PIN_RAM, (uint8_t)tmp);
            break;
        }
    }

}
class UsbPerformancePanel:public PluggableUSBModule {
    private:
        uint8_t epType[1] = {EP_TYPE_CONTROL};
        uint8_t availChn[1] = {(DATA_CPU_USAGE|DATA_RAM_USAGE)};
    public:
        UsbPerformancePanel():PluggableUSBModule(0, 1, epType) {
            #ifdef DEBUG
            Serial.begin(9600);
            #endif
            PluggableUSB().plug(this);
        }
        bool setup(USBSetup& setup);
        int getInterface(uint8_t* interfaceCount);
        int getDescriptor(USBSetup& setup);
};
bool UsbPerformancePanel::setup(USBSetup& st)
{
    if (pluggedInterface != st.wIndex) {
        return false;
    }
    uint8_t channel = st.wValueH;
    uint8_t value = st.wValueL;
    uint8_t request = st.bRequest;
    uint8_t requestType = st.bmRequestType;
    switch (request) {
        case COMMAND_QUERY_CAP: {
            USB_SendControl(0, &availChn, 1);
            return true;
        }
        case COMMAND_SET_USAGE: {
            set_pwm(channel,value);
            return true;
        }
    }
    return false;
}
int UsbPerformancePanel::getInterface(uint8_t* interfaceCount)
{
    *interfaceCount += 1;
    PerfPanelDescriptor desc = {
        D_INTERFACE(pluggedInterface,0, USB_DEVICE_CLASS_VENDOR_SPECIFIC, 0x0c, 0xbc),  
    };
    return USB_SendControl(0, &desc, sizeof(desc));
}
int UsbPerformancePanel::getDescriptor(USBSetup& st)
{
    //We do not have any sub desc...
    return 0;
}

void setup()
{
    RXLED1;
    TXLED1;
    calibration = EEPROM.read(CALIBRATION_EEPROM_ADDRESS);
    if(calibration==0xFF) //Not calibrated......
        EEPROM.write(CALIBRATION_EEPROM_ADDRESS, 182);
    UsbPerformancePanel panel;
    (void)panel;
}

void loop()
{
  
}
