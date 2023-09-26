import binascii

# Constants
CRC_SIZE = 4  # 32-bit CRC
COMPLETION_MESSAGE = b"\r\nFIRMWARE COMPLETE\r\n"

def calculate_crc32(data):
    """Calculate CRC-32 of binary data."""
    return binascii.crc32(data) & 0xFFFFFFFF

def create_new_bin_with_crc_and_message(filename, new_filename, data, crc, completion_message):
    """Write data plus CRC and completion message to a new file."""
    with open(new_filename, 'wb') as f:  # 'wb' mode for write-binary
        f.write(data)  # Write original data
        f.write(crc.to_bytes(CRC_SIZE, 'little'))  # Convert CRC to bytes and write
        f.write(completion_message)

def main():
    firmware_file = "E:\STM_M66\STM32_M66\Debug\STM32_M66.bin"  # Update with your firmware file path
    new_firmware_file = "E:\STM_M66\FOTA_CRC\ApplicationCRC.bin"   # Update with desired path for new .bin file

    with open(firmware_file, 'rb') as f:
        data = f.read()

    crc = calculate_crc32(data)
    create_new_bin_with_crc_and_message(firmware_file, new_firmware_file, data, crc, COMPLETION_MESSAGE)

    print(f"CRC {crc:#010x} and completion message added to create {new_firmware_file}")

if __name__ == "__main__":
    main()
