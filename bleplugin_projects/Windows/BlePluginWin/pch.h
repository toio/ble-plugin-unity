#pragma once
//Å@https://blog.okazuki.jp/entry/2018/10/16/144556

/* workaround */
#define USE_WORKAROUND 1

#if defined(USE_WORKAROUND)
#include "winrt/base.h"
namespace winrt::impl
{
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}
#endif

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"
#include <winrt/Windows.Storage.Streams.h>

#include <iostream>

// namespaceí∑Ç¢ÇÃÇ≈è»ó™å^Çópà”ÇµÇ‹Ç∑

namespace BlePlugin {
    using WinRtGuid = winrt::guid;
    using WinRtBleAdvertiseWatcher = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
    using WinRtBleDevice = winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
    using WinRtBleAdvertiseRecieveEventArg = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
    using WinRtBleAdvertiseFilter = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementFilter;
    using WinRtBleGattService = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService;
    using WinRtBleGattServiceResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;

    using WinRtBleCharacteristicsResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult;
    using WinRtBleCharacteristic = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
    using WinRtBleValueChangedEventArgs = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs;

    using WinRtGattReadResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattReadResult;
    using WinRtGattWriteResult = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattWriteResult;
    using WinRtGattCommunicateState = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus;

    template<class Value>
    using WinRtAsyncOperation = winrt::Windows::Foundation::IAsyncOperation<Value>;

    using WinRtIBuffer = winrt::Windows::Storage::Streams::IBuffer;
    using WinRtBleConnectStatus = winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;

    using WinRtCharacteristicConfigValue = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue;
}

