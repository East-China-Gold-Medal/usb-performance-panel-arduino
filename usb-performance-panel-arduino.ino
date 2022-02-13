#include <PluggableUSB.h>
#define DEBUG
typedef enum {
    DATA_CPU_USAGE=0x01,
    DATA_RAM_USAGE=0x02,
//...
    DATA_MAX_OUT  =0x80
}AvailableDataChannels;
typedef enum {
    COMMAND_QUERY_CAP = 0xFF,
    COMMAND_SET_USAGE = 0xFE
}HostOperationCommand;
typedef struct {
    InterfaceDescriptor id;
} PerfPanelDescriptor;
//interface for CBC to implement!
//tietie cbc
void set_pwm(uint8_t channel,uint8_t data)
{
    #ifdef DEBUG
    Serial1.println("Channel:");
    Serial1.println(channel);
    Serial1.println("Data:");
    Serial1.println(data);
    #endif
}
class UsbPerformancePanel:public PluggableUSBModule {
    private:
        uint8_t epType[1] = {EP_TYPE_CONTROL};
        uint8_t availChn[1] = {(DATA_CPU_USAGE|DATA_RAM_USAGE)};
    public:
        UsbPerformancePanel():PluggableUSBModule(0, 1, epType) {
            #ifdef DEBUG
            Serial1.begin(9600);
            Serial1.println("Init......");
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
    #ifdef DEBUG
    Serial1.println(request);
    Serial1.println(" ");
    Serial1.println(value);
    Serial1.println(" ");
    Serial1.println(request);
    #endif
    switch (request) {
        case COMMAND_QUERY_CAP: {
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
    // put your setup code here, to run once:
    UsbPerformancePanel panel;
    (void)panel;
}

void loop()
{
    // put your main code here, to run repeatedly:

}
