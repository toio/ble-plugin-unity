#include "BluetoothAdapterChecker.h"

using namespace BlePlugin;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices::Radios;

BluetoothAdapterChecker BluetoothAdapterChecker::s_instance;


BluetoothAdapterChecker::BluetoothAdapterChecker():
bluetoothStatus(EBluetoothStatus::None),
status(ESearchStatus::None),
radio(nullptr), adapter(nullptr),
asyncBluetoothAdapter(), asyncRadio()
{
}

BluetoothAdapterChecker& BluetoothAdapterChecker::GetInstance() {
    return s_instance;
}
void BluetoothAdapterChecker::Request() {
    asyncBluetoothAdapter = winrt::Windows::Devices::Bluetooth::BluetoothAdapter::GetDefaultAsync();
    status = ESearchStatus::WaitingAdapter;
    bluetoothStatus = EBluetoothStatus::None;
}
void BluetoothAdapterChecker::Update() {
    switch (status) {
    case ESearchStatus::WaitingAdapter:
        UpdateWaitingAdapter();
        break;
    case ESearchStatus::WaitingRadio:
        UpdateWaitingRadio();
        break;
    }
}

void BluetoothAdapterChecker::UpdateWaitingAdapter() {
    if (asyncBluetoothAdapter.Status() == AsyncStatus::Error) {
        SetBluetoothStatus(EBluetoothStatus::UnknownError);
    }
    else if (asyncBluetoothAdapter.Status() == AsyncStatus::Completed) {
        this->adapter = asyncBluetoothAdapter.get();
        if (this->adapter == nullptr) {
            SetBluetoothStatus(EBluetoothStatus::UnknownError);
            return;
        }
        if (!this->adapter.IsLowEnergySupported() ||
            !this->adapter.IsCentralRoleSupported()) {
            SetBluetoothStatus(EBluetoothStatus::NotSupportBle);
            return;
        }
        this->asyncRadio = this->adapter.GetRadioAsync();
        status = ESearchStatus::WaitingRadio;
    }
}
void BluetoothAdapterChecker::UpdateWaitingRadio() {
    if (asyncRadio.Status() == AsyncStatus::Error) {
        SetBluetoothStatus(EBluetoothStatus::UnknownError);
    }
    else if (asyncRadio.Status() == AsyncStatus::Completed) {
        this->radio = asyncRadio.get();
        if (this->radio == nullptr) {
            SetBluetoothStatus(EBluetoothStatus::UnknownError);
            return;
        }
        auto radioState = this->radio.State();
        if (radioState  == RadioState::On) {
            SetBluetoothStatus(EBluetoothStatus::Fine);
            return;
        }
        else {
            SetBluetoothStatus(EBluetoothStatus::BluetoothDisable);
        }
    }
}

bool supportBle(){

    
    WinRtAsyncOperation<WinRtBluetoothAdapter> res = winrt::Windows::Devices::Bluetooth::BluetoothAdapter::GetDefaultAsync();
    while (!res.Completed()) {
        break;
    }
    winrt::Windows::Devices::Bluetooth::BluetoothAdapter adapter = res.get();
    bool enableBle = adapter.IsLowEnergySupported();
    auto radioSync = adapter.GetRadioAsync();
    winrt::Windows::Devices::Radios::Radio radio = radioSync.get();
    auto stat = radioSync.get().State();
    return true;
}
