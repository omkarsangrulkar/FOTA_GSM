import sys
import os

CRC_POLYNOMIAL = 0xEDB88320

def calculate_crc32(data):
    crc = 0xFFFFFFFF

    for byte in data:
        crc ^= byte

        for _ in range(8):
            if crc & 0x00000001:
                crc = (crc >> 1) ^ CRC_POLYNOMIAL
            else:
                crc = crc >> 1

    return ~crc & 0xFFFFFFFF

def verify_firmware_update(filename):
    try:
        with open(filename, 'rb') as f:
            content = f.read()
            
            if len(content) % 4 != 0:
                print("Error: The file size is not a multiple of 4 bytes.")
                return False

            # Calculate the CRC32 for the entire content including the appended CRC
            calculated_crc = calculate_crc32(content)

            # Verification: If the entire content's CRC is 0xFFFFFFFF, the verification is successful
            if calculated_crc == 0xFFFFFFFF:
                print(f"Verification Successful for {filename}")
                return True
            else:
                print(f"Verification Failed for {filename}")
                return False
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        return False
    except Exception as e:
        print(f"Unexpected error: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python verifier.py <path_to_file_with_crc>")
        sys.exit(1)

    filename = sys.argv[1]
    verify_firmware_update(filename)
