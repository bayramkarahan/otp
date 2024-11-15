import hmac, base64, struct, hashlib, time

def base32_encode(data):
    """Encodes binary data using base32 encoding."""
    encoded_data = base64.b32encode(data)
    return encoded_data.decode('utf-8').rstrip('=')  # Remove padding

def get_hotp_token(secret, intervals_no):
    key = base64.b32decode(secret, True)
    #decoding our key
    msg = struct.pack(">Q", intervals_no)
    #conversions between Python values and C structs represente
    h = hmac.new(key, msg, hashlib.sha1).digest()
    o = o = h[19] & 15
    #Generate a hash using both of these. Hashing algorithm is HMAC
    h = (struct.unpack(">I", h[o:o+4])[0] & 0x7fffffff) % 1000000
    #unpacking
    return h

def get_totp_token(secret):
    #ensuring to give the same otp for 30 seconds
    x =str(get_hotp_token(secret,intervals_no=int(time.time())//60))
    #adding 0 in the beginning till OTP has 6 digits
    while len(x)!=6:
        x+='0'
    return x#base64 encoded key

kurumid="0000001"
binary_data = bytes("qrkilit-"+str(kurumid), 'utf-8')
secret_key = base32_encode(binary_data)
print(secret_key)
print(get_totp_token(secret_key))
