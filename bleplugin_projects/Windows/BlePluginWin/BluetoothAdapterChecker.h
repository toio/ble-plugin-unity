#pragma once

#include "pch.h"
#include <time.h>
#include <mutex>

namespace BlePlugin {
    class BluetoothAdapterChecker {
    public:
        enum class EBluetoothStatus :int{
            None = -1,
            Fine = 0,
            NotSupportBle = 1,
            BluetoothDisable = 2,
            UnknownError = 99
        };
    private:
        enum class ESearchStatus {
            None,
            WaitingAdapter,
            WaitingRadio,
            Complete
        };
        static BluetoothAdapterChecker s_instance;
        EBluetoothStatus bluetoothStatus;
        ESearchStatus status;

        WinRtBluetoothAdapter adapter;
        WinRtRadio radio;

        WinRtAsyncOperation<WinRtBluetoothAdapter> asyncBluetoothAdapter;
        WinRtAsyncOperation<WinRtRadio> asyncRadio;

        BluetoothAdapterChecker();
    public:
        static BluetoothAdapterChecker& GetInstance();
        void Request();
        void Update();
        inline bool IsRequestComplete()const {
            return (this->bluetoothStatus != EBluetoothStatus::None);
        }
        EBluetoothStatus GetStatus()const {
            return this->bluetoothStatus;
        }
    private:

        void UpdateWaitingAdapter();
        void UpdateWaitingRadio();

        inline void SetBluetoothStatus(EBluetoothStatus stat) {
            this->bluetoothStatus = stat;
            this->status = ESearchStatus::Complete;
        }
    };
}
