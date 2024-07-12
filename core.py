import serial
import time

ports = ['COM4', 'COM5', 'COM6']
serials = [serial.Serial(port, 9600, timeout=1) for port in ports]
time.sleep(2)

ADDRESS_OFFSET = 4
COMMAND_OFFSET = 2
POSITION_OFFSET = 0

CMD_BIND_ADDRESS = 0b11 << COMMAND_OFFSET
CMD_LED_SINGLE = 0b10 << COMMAND_OFFSET
CMD_LED_MULTI = 0b01 << COMMAND_OFFSET

def assign_address():
    for i, serial in enumerate(serials):
        address_command = i << ADDRESS_OFFSET | CMD_BIND_ADDRESS
        serial.write(address_command.to_bytes(2, 'little'))
        time.sleep(0.1)

def send_command(board_address, command_type, led_index):
    command = (board_address << ADDRESS_OFFSET) | command_type | led_index
    for serial in serials:
        serial.write(command.to_bytes(2, 'little'))

try:
    assign_address()
    while True:
        try:
            board_address = int(input("Board Address: ").strip())
            action  = input("Command (sg, mt): ").strip().lower()
            led_index = int(input("Choose LED (0, 1, 2): ").strip())
        except Exception as e:
            print(f"Error Occured: {e}")
            continue

        if action == "sg":
            send_command(board_address, CMD_LED_SINGLE, led_index)
        elif action == "mt":
            send_command(board_address, CMD_LED_MULTI, led_index)
        else:
            print(f"Wrong Command >> {board_address}, {action}, {led_index}")

except KeyboardInterrupt:
    print("Detect Keyboard Interrupt")
finally:
    for serial in serials:
        serial.close()