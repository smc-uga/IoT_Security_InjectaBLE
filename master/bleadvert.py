#pip install bleak for Windows system
#sudo apt install python3-bleak for Linux

import asyncio
from bleak import BleakClient

nRF52840="e2:39:cb:94:bc:32"
on_off="0000ffab-0000-1000-8000-00805f9b34fb"

####################################################################
########## Basic BLE Communication to turn on/off an LED ###########
####################################################################
async def main():
    async with BleakClient(nRF52840) as client:

        print("Connected.")
        while True:
            # Prompt user input in a non-blocking way
            user_input = await asyncio.to_thread(input, "Enter command (on/off/quit): ")
            user_input = user_input.lower()

            if user_input == "on":
                await client.write_gatt_char(on_off, bytearray([1]))
                print("Sent ON")
            elif user_input == "off":
                await client.write_gatt_char(on_off, bytearray([0]))
                print("Sent OFF")
            elif user_input == "quit":
                print("Exiting...")
                break
            else:
                print("Invalid input. Try 'on', 'off', or 'quit'.")

asyncio.run(main())


############## USED TO FIND THE "on_off" variable ##############
# async def main():
#     async with BleakClient(nRF52840) as client:
#         if(user input == 'on'):
#             await client.write_gatt_char(on_off, 1)
#         elif(user input == 'off'):
#             await client.write_gatt_char(on_off, 0)
#
#
# asyncio.run(main())


############## USED TO FIND THE "nRF52840" variable ##############
# import asyncio
# from bleak import BleakScanner
#
# async def run():
#     print("Scanning for BLE devices...\n")
#     devices = await BleakScanner.discover()
#     for device in devices:
#         print(f"{device.name} - {device.address}")
#
# asyncio.run(run())
