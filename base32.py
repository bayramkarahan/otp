import base64

def encode_to_base32(text):
  encoded_bytes = text.encode('utf-8')
  base32_encoded = base64.b32encode(encoded_bytes)
  return base32_encoded.decode('utf-8')

# Example usage:
text = "Hello, world!"
encoded_text = encode_to_base32(text)
print(encoded_text)
