#include <PluggableUSB.h>
typedef enum {
    DATA_CPU_USAGE=0x01,
    DATA_RAM_USAGE=0x02,
//...
    DATA_MAX_OUT  =0x80
}AvailableDataChannels;
class UsbPerformancePanel:public PluggableUSBModule {
    private:
        uint8_t epType[1] = {EP_TYPE_CONTROL};
        uint8_t availChn[1] = {(DATA_CPU_USAGE|DATA_RAM_USAGE)};
    public:
        UsbPerformancePanel():PluggableUSBModule(1, 1, epType) {
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
    uint8_t command = st.wValueH;
    uint8_t value = st.wValueL;
    uint8_t request = st.bRequest;
    uint8_t requestType = st.bmRequestType;
}
int UsbPerformancePanel::getInterface(uint8_t* interfaceCount)
{
    
}
int UsbPerformancePanel::getDescriptor(USBSetup& st)
{
    
}
void setup()
{
    // put your setup code here, to run once:

}

void loop()
{
    // put your main code here, to run repeatedly:

}
