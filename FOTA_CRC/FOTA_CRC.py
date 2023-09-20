import binascii

def calculate_crc32(data):
    crc = 0xFFFFFFFF

    for byte in data:
        crc ^= byte

        for _ in range(8):
            if crc & 0x00000001:
                crc = (crc >> 1) ^ 0xEDB88320
            else:
                crc = crc >> 1

    return crc & 0xFFFFFFFF

def create_file_with_crc32(input_file_path, output_file_path):
    try:
        # Read the binary file
        with open(input_file_path, 'rb') as input_file:
            binary_data = input_file.read()

        # Calculate CRC32 checksum
        crc = calculate_crc32(binary_data)

        # Convert CRC32 to bytes (4 bytes, little-endian)
        crc_bytes = crc.to_bytes(4, byteorder='little')

        # Create a new file with the CRC32 value appended
        with open(output_file_path, 'wb') as output_file:
            output_file.write(binary_data)
            output_file.write(crc_bytes)

        print("CRC32 appended to new file:", output_file_path)
    except Exception as e:
        print("Error:", str(e))

if __name__ == "__main__":
    input_file_path = "E:\STM_M66\STM32_M66\Debug\STM32_M66.bin"  # Replace with the path to your binary file
    output_file_path = "E:\STM_M66\FOTA_CRC\ApplicationCRC.bin"  # Replace with the desired output file path
    create_file_with_crc32(input_file_path, output_file_path)
