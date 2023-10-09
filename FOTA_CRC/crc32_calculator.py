import sys
import os

CRC_POLYNOMIAL = 0x04C11DB7

def calculate_crc32(data):
    crc = 0xFFFFFFFF

    for byte in data:
        crc ^= byte << 24
        for _ in range(8):
            if crc & 0x80000000:
                crc = (crc << 1) ^ CRC_POLYNOMIAL
            else:
                crc = crc << 1

    return crc & 0xFFFFFFFF

def compute_and_append_crc32(filename):
    try:
        with open(filename, 'rb') as f:
            content = f.read()

            crc32_val = calculate_crc32(content)
            print(f"CRC for data: {hex(crc32_val)}")

            base, ext = os.path.splitext(filename)
            new_filename = base + "_with_crc" + ext

            with open(new_filename, 'wb') as new_file:
                new_file.write(content)
                new_file.write(crc32_val.to_bytes(4, byteorder='big'))

            print(f"File with appended CRC saved as {new_filename}")
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
    except Exception as e:
        print(f"Unexpected error: {e}")

def verify_crc(filename):
    try:
        with open(filename, 'rb') as f:
            content = f.read()

            crc32_val = calculate_crc32(content)

            if crc32_val == 0x0:
                print(f"Verification Successful for {filename}")
                return True
            else:
                print(f"Verification Failed for {filename}. Calculated CRC: {hex(crc32_val)}")
                return False
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        return False
    except Exception as e:
        print(f"Unexpected error: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: CRC32 <path_to_file>")
        sys.exit(1)

    filename = sys.argv[1]
    compute_and_append_crc32(filename)

    base, ext = os.path.splitext(filename)
    new_filename = base + "_with_crc" + ext
    verify_crc(new_filename)
