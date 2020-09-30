//
// Copyright (c) 2020-present, Sony Interactive Entertainment Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using toio;

public class SampleScene : MonoBehaviour
{
    const string SERVICE_UUID = "10b20100-5b3b-4571-9508-cf3efcd7bbae";
    const string MOTOR_UUID = "10b20102-5b3b-4571-9508-cf3efcd7bbae";
    const string ID_UUID = "10b20101-5b3b-4571-9508-cf3efcd7bbae";

    string targetIdentifier = null;

    // Start is called before the first frame update
    void Start()
    {
        Ble.Initialize(() =>
        {
            Debug.Log("initialized");

            Ble.StartScan(new string[] { SERVICE_UUID }, (identifier, name, rssi, manufactureData) =>
            {
                targetIdentifier = identifier;
                Ble.ConnectToPeripheral(identifier, null, null, (deviceIdentifier, serviceUUID, characteristicUUID) =>
                {
                    if (characteristicUUID == ID_UUID)
                    {
                        Ble.SubscribeCharacteristic(targetIdentifier, serviceUUID, characteristicUUID, (name2, name3, bytedata) =>
                        {
                            Debug.Log(BitConverter.ToString(bytedata));
                        });
                    }
                }, null);
            });
        }, (error) =>
        {
            Debug.LogErrorFormat("error = {0}", error);
        });
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0) && targetIdentifier != null)
        {
            byte[] data = buildMoveData(30, 30, 500);
            Ble.WriteCharacteristic(targetIdentifier, SERVICE_UUID, MOTOR_UUID, data, data.Length, false, null);
        }
    }

    byte[] buildMoveData(int left, int right, int durationMs)
    {
        var lSign = left > 0 ? 1 : -1;
        var rSign = right > 0 ? 1 : -1;
        var lDirection = left > 0 ? 1 : 2;
        var rDirection = right > 0 ? 1 : 2;
        var lPower = Mathf.Min(Mathf.Abs(left), 100);
        var rPower = Mathf.Min(Mathf.Abs(right), 100);
        var duration = Mathf.Clamp(durationMs / 10, 0, 255);

        byte[] buff = new byte[8];
        buff[0] = 2;
        buff[1] = 1;
        buff[2] = BitConverter.GetBytes(lDirection)[0];
        buff[3] = BitConverter.GetBytes(lPower)[0];
        buff[4] = 2;
        buff[5] = BitConverter.GetBytes(rDirection)[0];
        buff[6] = BitConverter.GetBytes(rPower)[0];
        buff[7] = BitConverter.GetBytes(duration)[0];

        return buff;
    }
}
