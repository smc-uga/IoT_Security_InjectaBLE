#pip install bleak for Windows

import asyncio
from bleak import BleakScanner

async def run():
    print("Scanning for BLE devices...\n")
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"{device.name} - {device.address}")

asyncio.run(run())
