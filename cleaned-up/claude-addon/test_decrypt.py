import os

bin_path = r'C:\TalesOfPirateDX9\Client\scripts\table\TerrainInfo.bin'
with open(bin_path, 'rb') as f:
    data = f.read()

print(f'TerrainInfo.bin size: {len(data)} bytes')
print(f'First 32 bytes (hex): {data[:32].hex()}')
print(f'Last 16 bytes (hex): {data[-16:].hex()}')

# Keys from TableData.h
key = bytes([0x32,0x72,0x35,0x75,0x38,0x78,0x2f,0x41,0x3f,0x44,0x28,0x47,0x2b,0x4b,0x62,0x50])
iv  = bytes([0x43,0x2a,0x46,0x29,0x4a,0x40,0x4e,0x63,0x52,0x66,0x55,0x6a,0x58,0x6e,0x32,0x72])
print(f'\nKey (ASCII): {key}')
print(f'IV  (ASCII): {iv}')

# Try decryption with Python cryptography library
try:
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM
    # GCM tag is typically the last 16 bytes, but CryptoPP default is 16
    # Try with tag at end (last 16 bytes)
    tag_size = 16
    ciphertext = data[:-tag_size]
    tag = data[-tag_size:]
    
    aesgcm = AESGCM(key)
    try:
        # AESGCM decrypt expects ciphertext+tag combined
        plaintext = aesgcm.decrypt(iv, data, None)  # data = ciphertext+tag
        print(f'\nDecrypt SUCCESS! Plaintext size: {len(plaintext)}')
        print(f'First 64 bytes of plaintext: {plaintext[:64]}')
    except Exception as e:
        print(f'\nDecrypt FAILED: {e}')
        # Try without AAD, different tag sizes
        for ts in [12, 16]:
            try:
                ct = data[:-ts]
                t = data[-ts:]
                from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
                from cryptography.hazmat.backends import default_backend
                decryptor = Cipher(algorithms.AES(key), modes.GCM(iv, t, min_tag_length=ts), backend=default_backend()).decryptor()
                pt = decryptor.update(ct) + decryptor.finalize()
                print(f'  tag_size={ts}: SUCCESS! First 32 bytes: {pt[:32]}')
            except Exception as e2:
                print(f'  tag_size={ts}: FAILED: {e2}')
except ImportError:
    print('cryptography not installed, trying pycryptodome...')
    try:
        from Crypto.Cipher import AES
        tag_size = 16
        ct = data[:-tag_size]
        tag = data[-tag_size:]
        cipher = AES.new(key, AES.MODE_GCM, nonce=iv)
        try:
            pt = cipher.decrypt_and_verify(ct, tag)
            print(f'Decrypt SUCCESS! Size: {len(pt)}, first 64: {pt[:64]}')
        except Exception as e:
            print(f'Decrypt FAILED: {e}')
    except ImportError:
        print('Neither cryptography nor pycryptodome available')
        print('Install with: pip install cryptography --break-system-packages')
